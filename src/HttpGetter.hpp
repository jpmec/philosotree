#ifndef HTTP_GETTER_HPP
#define HTTP_GETTER_HPP

#include <map>
#include <fstream>
#include <iostream>
#include <string>

#include "boost/algorithm/string.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "HttpResponse.hpp"

using boost::asio::ip::tcp;
using boost::algorithm::trim;



class HttpGetter
{
private:
  tcp::resolver resolver_;
  tcp::socket socket_;
  boost::asio::streambuf request_buffer_;
  boost::asio::streambuf response_buffer_;
  std::string server_;
  std::string path_;
  std::ostringstream result_;
  bool verbose_;
  HttpResponse response_;

  boost::function<void(const HttpResponse&)> after_get_response_;
  boost::function<void(const boost::system::error_code&)> on_error_;


public:

  HttpGetter(boost::asio::io_service& io_service, bool verbose = false)
    : resolver_(io_service)
    , socket_(io_service)
    , verbose_(verbose)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::HttpGetter" << std::endl;
    }
  }


  void get( const std::string& server
          , const std::string& path
          , boost::function<void(const HttpResponse&)> after_get = HttpGetter::noop
          , boost::function<void(const boost::system::error_code&)> on_error = HttpGetter::error_noop)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::get" << std::endl;
      std::cout << server << path << std::endl;
    }

    result_.str("");
    response_.clear();
    after_get_response_ = after_get;
    on_error_ = on_error;
    server_ = server;
    path_ = path;
    request_get(server_, path_);
  }


  void verbose(bool v)
  {
    verbose_ = v;
  }


  static void noop(const HttpResponse&)
  {
  }


  static void error_noop(const boost::system::error_code&)
  {
  }


protected:


  virtual std::string url_encode(const std::string& path)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::url_encode" << std::endl;
    }

    std::string url_encoded_path(path);

    boost::algorithm::replace_all(url_encoded_path, " ", "\x25""20");

    return url_encoded_path;
  }


  virtual void request_get(const std::string& server, const std::string& path)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::request_get" << std::endl;
    }

    std::string url_encoded_path = url_encode(path);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_buffer_);
    request_stream << "GET " << url_encoded_path << " HTTP/1.0\r\n";
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

  virtual void handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::handle_resolve" << std::endl;
    }

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
      on_error_(err);
    }
  }

  virtual void handle_connect(const boost::system::error_code& err)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::handle_connect" << std::endl;
    }

    if (!err)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_, request_buffer_,
          boost::bind(&HttpGetter::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
      on_error_(err);
    }
  }

  virtual void handle_write_request(const boost::system::error_code& err)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::handle_write_request" << std::endl;
    }

    if (!err)
    {
      // Read the response status line. The response_buffer_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_buffer_, "\r\n",
          boost::bind(&HttpGetter::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cerr << "Error: " << err.message() << "\n";
      on_error_(err);
    }
  }

  virtual void handle_read_status_line(const boost::system::error_code& err)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::handle_read_status_line" << std::endl;
    }

    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&response_buffer_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);

      trim(http_version);
      trim(status_message);
      response_.status_code = status_code;
      response_.http_version = http_version;
      response_.status_message = status_message;

      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        std::cerr << "Invalid response\n";
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_, response_buffer_, "\r\n\r\n",
          boost::bind(&HttpGetter::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else if (err == boost::asio::error::eof)
    {
      std::cout << "Error eof: " << err << "\n";

      std::istream response_stream(&response_buffer_);
      std::string str;
      std::cout << "error str: " << str << std::endl;
      on_error_(err);
    }
    else
    {
      std::cout << "Error: " << err << "\n";
      on_error_(err);
    }
  }

  virtual void handle_read_headers(const boost::system::error_code& err)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::handle_read_headers" << std::endl;
    }

    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&response_buffer_);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
      {
        const size_t pos = header.find(":");

        if (pos == std::string::npos)
        {

        }
        else
        {
          std::string header_key = header.substr(0, pos);
          std::string header_value = header.substr(pos + 2, header.size());
          response_.header[header_key] = header_value;
        }
      }

      // Write whatever content we already have to output.
      if (response_buffer_.size() > 0)
        result_ << &response_buffer_;

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_, response_buffer_,
          boost::asio::transfer_at_least(1),
          boost::bind(&HttpGetter::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cerr << "Error: " << err << "\n";
      on_error_(err);
    }
  }

  virtual void handle_read_content(const boost::system::error_code& err)
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::handle_read_content" << std::endl;
    }

    if (!err)
    {
      // Write all of the data that has been read so far.
      result_ << &response_buffer_;

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_buffer_,
          boost::asio::transfer_at_least(1),
          boost::bind(&HttpGetter::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else if (err == boost::asio::error::eof)
    {
      onEof();
      after_get_response_(response_);
    }
    else if (err != boost::asio::error::eof)
    {
      std::cerr << "Error: " << err << "\n";
      on_error_(err);
    }
  }


  virtual void onEof()
  {
    if (verbose_)
    {
      std::cout << this << " HttpGetter::onEof" << std::endl;
    }

    response_.body = result_.str();
  }


  class FileLogger
  {
  public:
    FileLogger(const std::string& filename)
    : log_(filename.c_str(), std::fstream::out | std::fstream::app)
    {
    }

    FileLogger& operator<<(const std::string& str)
    {
      log_ << str << std::endl;
      return *this;
    }

  protected:
    FileLogger();
    FileLogger(const FileLogger&);
    FileLogger& operator=(const FileLogger&);

  private:
    std::ofstream log_;
  };

  void log(const std::string& str)
  {
    std::string filename(__FILE__);
    filename += ".log.txt";
    FileLogger log(filename);
    log << str;
  }
};







#endif
