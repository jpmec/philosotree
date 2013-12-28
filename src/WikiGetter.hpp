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


    //pt = pt.get_child("query");
    //pt = pt.get_child("pages");
    //
    //for (ptree::iterator i = pt.begin(); i != pt.end(); ++i)
    //{
    //  ptree links =  i->second.get_child("links");
    //
    //  for (ptree::iterator l = links.begin(); l != links.end(); ++l)
    //  {
    //    cout << l->second.get_child("title").get_value<string>() << endl;
    //  }
    //}

private:
  std::string server;
};




#endif // WIKI_GETTER_HPP
