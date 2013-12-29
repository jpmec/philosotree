#ifndef WIKI_GETTER_HPP
#define WIKI_GETTER_HPP

#include "HttpGetter.hpp"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;



class WikiGetter : public HttpGetter
{
private:
  typedef HttpGetter super;

public:

  WikiGetter(boost::asio::io_service& io_service)
    : HttpGetter(io_service)
    , server("en.wikipedia.org")
  {
  }


  virtual void get(const char* path)
  {
    get(server.c_str(), path);
  }

  virtual void get(const std::string& server, const std::string& path)
  {
    super::get(server, path);
  }  


private:
  std::string server;
};




#endif // WIKI_GETTER_HPP
