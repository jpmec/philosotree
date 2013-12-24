

#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::asio::ip::tcp;
using boost::property_tree::ptree;


class client
{
public:
  client(boost::asio::io_service& io_service,
      const std::string& server, const std::string& path)
    : resolver_(io_service),
      socket_(io_service),
      verbose_(false)
  {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, "http");
    resolver_.async_resolve(query,
        boost::bind(&client::handle_resolve, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator));
  }

private:
  void handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // Attempt a connection to each endpoint in the list until we
      // successfully establish a connection.
      boost::asio::async_connect(socket_, endpoint_iterator,
          boost::bind(&client::handle_connect, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cerr << "Error: " << err.message() << "\n";
    }
  }

  void handle_connect(const boost::system::error_code& err)
  {
    if (!err)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_, request_,
          boost::bind(&client::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void handle_write_request(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Read the response status line. The response_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_, "\r\n",
          boost::bind(&client::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cerr << "Error: " << err.message() << "\n";
    }
  }

  void handle_read_status_line(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&response_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        std::cerr << "Invalid response\n";
        return;
      }
      if (status_code != 200)
      {
        std::cout << "Response returned with status code ";
        std::cout << status_code << "\n";
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
          boost::bind(&client::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

  void handle_read_headers(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&response_);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
      {
        if (this->verbose_)
        {
          std::cout << header << "\n";
        }
      }

      if (this->verbose_)
      {
        std::cout << "\n";
      }

      // Write whatever content we already have to output.
      if (response_.size() > 0)
        result_ << &response_;

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cerr << "Error: " << err << "\n";
    }
  }

  void handle_read_content(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Write all of the data that has been read so far.
      result_ << &response_;

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      std::cerr << "Error: " << err << "\n";
    }
  }

  public: string result()
  {
    return result_.str();
  }

  tcp::resolver resolver_;
  tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;
  ostringstream result_;
  bool verbose_;
};


int main(int argc, char* argv[])
{


  // key is url, value is set of urls contained by key url
  map< string, set<string> > forward_links;

  // key is url, value is set of urls containing key url
  map< string, set<string> > reverse_links;

  try
  {
    if (argc != 3)
    {
      std::cout << "Usage: async_client <server> <path>\n";
      std::cout << "Example:\n";
      std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
      return 1;
    }


    boost::asio::io_service io_service;
    client c(io_service, argv[1], argv[2]);
    io_service.run();

    ptree pt;

    istringstream json_stream(c.result());
    read_json(json_stream, pt);

    pt = pt.get_child("query");
    pt = pt.get_child("pages");

    for (ptree::iterator i = pt.begin(); i != pt.end(); ++i)
    {
      ptree links =  i->second.get_child("links");

      for (ptree::iterator l = links.begin(); l != links.end(); ++l)
      {
        cout << l->second.get_child("title").get_value<string>() << endl;
      }
    }


  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
