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
    : io_service_(io_service)
    , http_getter_(io_service)
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


  void get(const char* link_title, boost::function<void(const std::set<std::string>&)> callback)
  {
    std::string link_path(path_);
    link_path += link_title;

    links_set_.clear();
    retry_ = 1;
    http_getter_.get( server_
                    , link_path
                    , ResponseToLinks( http_getter_
                                     , server_
                                     , link_path
                                     , link_title
                                     , links_set_
                                     , callback
                                     , verbose_)
                    , ErrorHandler(retry_));
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
    ResponseToLinks( HttpGetter& http_getter
                   , const std::string& server
                   , const std::string& link_path
                   , const std::string& link_title
                   , std::set<std::string>& links_set
                   , boost::function<void(const std::set<std::string>&)> after_response
                   , bool verbose = false)
    : http_getter_(http_getter)
    , server_(server)
    , link_path_(link_path)
    , link_title_(link_title)
    , links_set_(links_set)
    , after_response_(after_response)
    , verbose_(verbose)
    {
      if (verbose_)
      {
        std::cout << this << " WikiLinkGetter::ResponseToLinks::ResponseToLinks()" << std::endl;
      }
    }


    virtual ~ResponseToLinks()
    {
      if (verbose_)
      {
        std::cout << this << " WikiLinkGetter::ResponseToLinks::~ResponseToLinks()" << std::endl;
      }
    }


    void operator()(const HttpResponse& r)
    {
      if (verbose_)
        std::cout << this << " WikiLinkGetter::ResponseToLinks::operator()" << std::endl;

      if (verbose_)
      {
        std::cout << r << std::endl;
      }

      std::istringstream json_stream(r.body);
      std::string plcontinue;

      try
      {
        boost::property_tree::ptree pt;
        read_json(json_stream, pt);

        plcontinue = pt.get<std::string>("query-continue.links.plcontinue", "");

        if (verbose_)
        {
          std::cout << "plcontinue: '";

          for (std::string::iterator c = plcontinue.begin(); c != plcontinue.end(); ++c)
          {
            std::cout << (*c);
          }

          std::cout <<  "'" << std::endl;
        }
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
        http_getter_.get( server_
                        , link_path_ + "&plcontinue=" + plcontinue
                        , ResponseToLinks( http_getter_
                                         , server_
                                         , link_path_
                                         , link_title_
                                         , links_set_
                                         , after_response_)
                        , ErrorHandler());
      }
      else
      {
        after_response_(links_set_);
      }
    }

    HttpGetter& http_getter_;
    std::string server_;
    std::string link_path_;
    std::string link_title_;
    std::set<std::string>& links_set_;
    boost::function<void(const std::set<std::string>&)> after_response_;
    bool verbose_;
  };


  struct ErrorHandler
  {
    ErrorHandler(unsigned retry = 0)
    : retry_(retry)
    {

    }

    void operator()(const boost::system::error_code& err)
    {
      std::cerr << "ErrorHandler::operator(): " << err << "\n";
    }

    unsigned retry_;
  };


private:
  boost::asio::io_service& io_service_;
  HttpGetter http_getter_;
  std::string server_;
  std::string path_;
  bool verbose_;
  unsigned retry_;
  std::set<std::string> links_set_;
};




#endif // WIKI_LINK_GETTER_HPP
