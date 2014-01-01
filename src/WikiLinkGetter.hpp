#ifndef WIKI_LINK_GETTER_HPP
#define WIKI_LINK_GETTER_HPP


#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "HttpGetter.hpp"




class WikiLinkGetter
{
public:
  WikiLinkGetter(boost::asio::io_service& io_service)
    : http_getter_(io_service)
    , server_("en.wikipedia.org")
    , path_("/w/api.php?action=query&format=json&prop=links&pllimit=max&titles=")
    , after_response_(WikiLinkGetter::noop)
    , verbose_(false)
  {
    if (verbose_)
      std::cout << this << " WikiLinkGetter::WikiLinkGetter()" << std::endl;    
  }


  virtual ~WikiLinkGetter()
  {
    if (verbose_)    
      std::cout << this << " WikiLinkGetter::~WikiLinkGetter()" << std::endl;      
  }


  void get(const char* link, boost::function<void(const std::set<std::string>&)> callback)
  {
    std::string link_path(path_);
    link_path += link;

    after_response_ = ResponseToLinks(callback);
    http_getter_.get(server_, link_path, after_response_);
  }


  void verbose(bool v)
  {
    verbose_ = v;
    http_getter_.verbose(v);
  }


  static void noop(const std::set<std::string>&)
  {
  }

protected:
  WikiLinkGetter();
  WikiLinkGetter(const WikiLinkGetter&);
  WikiLinkGetter& operator=(const WikiLinkGetter&);  

  struct ResponseToLinks
  {
    ResponseToLinks(boost::function<void(const std::set<std::string>&)> after_response)
    : after_response_(after_response)
    , verbose_(false)
    {
      if (verbose_)
        std::cout << this << " WikiLinkGetter::ResponseToLinks::ResponseToLinks()" << std::endl;
    }

    virtual ~ResponseToLinks()
    {
      if (verbose_)
        std::cout << this << " WikiLinkGetter::ResponseToLinks::~ResponseToLinks()" << std::endl;      
    }

    void operator()(const HttpGetter::Response& r)
    {
      if (verbose_)
        std::cout << this << " WikiLinkGetter::ResponseToLinks::operator()" << std::endl;   

      boost::property_tree::ptree pt;
      std::set<std::string> links_set;

      std::istringstream json_stream(r.body);

      //std::cout << "body" << std::endl;
      //std::cout << r.body << std::endl;


      try
      {
        read_json(json_stream, pt);
        
        // //pt.get_child("query-continue").get_child("links").get_child("plcontinue");

        pt = pt.get_child("query");
        pt = pt.get_child("pages");
        
        for (boost::property_tree::ptree::iterator i = pt.begin(); i != pt.end(); ++i)
        {
          boost::property_tree::ptree links = i->second.get_child("links");

          for (boost::property_tree::ptree::iterator l = links.begin(); l != links.end(); ++l)
          {
            links_set.insert(l->second.get_child("title").get_value<std::string>());
          }
        }
      }
      catch (std::runtime_error& e)
      {
        
      }

      after_response_(links_set);
    }

    boost::function<void(const std::set<std::string>&)> after_response_;
    bool verbose_;    
  };


private:
  HttpGetter http_getter_;
  std::string server_;
  std::string path_;
  ResponseToLinks after_response_;
  bool verbose_;
};




#endif // WIKI_LINK_GETTER_HPP
