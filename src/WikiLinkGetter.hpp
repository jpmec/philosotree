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

    links_set_.clear();
    http_getter_.get(server_, link_path, ResponseToLinks(http_getter_, server_, link_path, links_set_, callback));
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
    ResponseToLinks(HttpGetter& http_getter, const std::string& server, const std::string& link_path, std::set<std::string>& links_set, boost::function<void(const std::set<std::string>&)> after_response)
    : http_getter_(http_getter)
    , server_(server)
    , link_path_(link_path)
    , links_set_(links_set)
    , after_response_(after_response)
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

      std::istringstream json_stream(r.body);

      //std::cout << "body" << std::endl;
      //std::cout << r.body << std::endl;


      std::string plcontinue;

      try
      {
        read_json(json_stream, pt);
        
        plcontinue = pt.get<std::string>("query-continue.links.plcontinue", "");        

        pt = pt.get_child("query");
        pt = pt.get_child("pages");
        
        for (boost::property_tree::ptree::iterator i = pt.begin(); i != pt.end(); ++i)
        {
          boost::property_tree::ptree links = i->second.get_child("links");

          for (boost::property_tree::ptree::iterator l = links.begin(); l != links.end(); ++l)
          {
            links_set_.insert(l->second.get_child("title").get_value<std::string>());
          }
        }
      }
      catch (std::runtime_error& e)
      {
        
      }

      if (plcontinue.size())
      {
        http_getter_.get(server_, link_path_ + "&plcontinue=" + plcontinue, ResponseToLinks(http_getter_, server_, link_path_, links_set_, after_response_));
      }
      else
      {
        after_response_(links_set_);
      }
    }

    HttpGetter& http_getter_;
    std::string server_;
    std::string link_path_;
    std::set<std::string>& links_set_;
    boost::function<void(const std::set<std::string>&)> after_response_;
    bool verbose_;    
  };


private:
  HttpGetter http_getter_;
  std::string server_;
  std::string path_;
  bool verbose_;
  std::set<std::string> links_set_;
};




#endif // WIKI_LINK_GETTER_HPP
