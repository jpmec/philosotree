#ifndef HTTP_GETTER_HPP
#define HTTP_GETTER_HPP

#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "boost/algorithm/string.hpp"

using namespace std;
using boost::asio::ip::tcp;
using boost::algorithm::trim;


class HttpGetter
{
public:
  struct Response
  {
    unsigned int status_code;
    std::string status_message;
    std::string http_version;
    std::string header;
    std::string body;
  };


  HttpGetter(boost::asio::io_service& io_service)
    : resolver_(io_service),
      socket_(io_service),
      verbose_(false)
  {
  }

  virtual void get(const std::string& server, const std::string& path)
  {
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_buffer);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, "http");
    resolver_.async_resolve(query,
        boost::bind(&HttpGetter::handle_resolve, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator));
  }

  Response getResponse()
  {
    response.body = result_.str();
    return response;
  }

protected:
  void handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // Attempt a connection to each endpoint in the list until we
      // successfully establish a connection.
      boost::asio::async_connect(socket_, endpoint_iterator,
          boost::bind(&HttpGetter::handle_connect, this,
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
      boost::asio::async_write(socket_, request_buffer,
          boost::bind(&HttpGetter::handle_write_request, this,
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
      // Read the response status line. The response_buffer streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_buffer, "\r\n",
          boost::bind(&HttpGetter::handle_read_status_line, this,
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
      std::istream response_stream(&response_buffer);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);

      trim(http_version);
      trim(status_message);
      response.status_code = status_code;
      response.http_version = http_version;
      response.status_message = status_message;

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
      boost::asio::async_read_until(socket_, response_buffer, "\r\n\r\n",
          boost::bind(&HttpGetter::handle_read_headers, this,
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
      std::istream response_stream(&response_buffer);
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

      response.header = header;

      // Write whatever content we already have to output.
      if (response_buffer.size() > 0)
        result_ << &response_buffer;

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_, response_buffer,
          boost::asio::transfer_at_least(1),
          boost::bind(&HttpGetter::handle_read_content, this,
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
      result_ << &response_buffer;

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_buffer,
          boost::asio::transfer_at_least(1),
          boost::bind(&HttpGetter::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      std::cerr << "Error: " << err << "\n";
    }
  }



private:
  tcp::resolver resolver_;
  tcp::socket socket_;
  boost::asio::streambuf request_buffer;
  boost::asio::streambuf response_buffer;
  ostringstream result_;
  bool verbose_;
  Response response;
};


#endif
