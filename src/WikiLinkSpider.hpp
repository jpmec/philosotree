#ifndef WIKI_LINK_SPIDER_HPP
#define WIKI_LINK_SPIDER_HPP


// #include <map>
// #include <set>
// #include <string>
// #include <utility>

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>

#include "GenericSpider.hpp"
#include "WikiLinkGetter.hpp"
#include "WikiLinkWeb.hpp"

// #include "WikiLinkGetter.hpp"


class WikiLinkSpider// : public GenericSpider< WikiLinkWeb, WikiLinkGetter >
{
//  typedef GenericSpider< WikiLinkWeb, WikiLinkGetter > super;

public:
  WikiLinkSpider(boost::asio::io_service& io_service, WikiLinkWeb& web)
  : unexplored_()
  , io_service_(io_service)
  , web_(web)
  , getter_(io_service)
  , links_to_web_(getter_, web, unexplored_)
  , verbose_(false)
  {
    if (verbose_)
      std::cout << this << " WikiLinkSpider::WikiLinkSpider" << std::endl;
  }

  virtual ~WikiLinkSpider()
  {
    if (verbose_)
      std::cout << this << " WikiLinkSpider::~WikiLinkSpider" << std::endl;    
  }

  void crawl(const char* from, const char* to)
  {
    if (verbose_)
      std::cout << this << " WikiLinkSpider::crawl" << std::endl;
    
    const std::string from_str(from);
    const std::string to_str(to);

    if (web_.contains(from_str))
    {
      if (web_.contains(to_str))
      {
       return;
      }
    }
    else
    {
      web_.insert(from_str);
      getter_.get(from, links_to_web_);
    }
  }

  void verbose(bool v)
  {
    verbose_ = v;
    getter_.verbose(v);
    links_to_web_.verbose(v);
  }

protected:
  WikiLinkSpider();  
  WikiLinkSpider(const WikiLinkSpider&);
  WikiLinkSpider& operator=(const WikiLinkSpider&);


  struct LinksToWeb
  {
    LinksToWeb(WikiLinkGetter& getter, WikiLinkWeb& web, std::set<WikiLinkWeb::NodeSharedPtr>& unexplored)
    : getter_(getter)
    , web_(web)
    , unexplored_(unexplored)
    , get_limit_(100)
    , verbose_(false)
    {
      if (verbose_)
        std::cout << this << " WikiLinkSpider::LinksToWeb::LinksToWeb" << std::endl;
    }
    
    virtual ~LinksToWeb()
    {
      if (verbose_)
        std::cout << this << " WikiLinkSpider::LinksToWeb::~LinksToWeb" << std::endl;
    }

    void operator()(const std::set<std::string>& links)
    {
      if (verbose_)
        std::cout << this <<  " WikiLinkSpider::LinksToWeb::operator()" << std::endl;
      
      std::set<std::string>::const_iterator i = links.begin();
      for (; i != links.end(); ++i)
      {
        if (!web_.contains(*i))
        {
          unexplored_.insert(web_.insert(*i));
        }
      }


      if ((0 < get_limit_) && unexplored_.size())
      {
        --get_limit_;
        
        if (verbose_)
          std::cout << "get_limit: " << get_limit_ << std::endl;

        std::set<WikiLinkWeb::NodeSharedPtr>::iterator first = unexplored_.begin();
        if (first != unexplored_.end())
        {
          std::string link = (*first)->c_str();
          unexplored_.erase(first);
          getter_.get(link.c_str(), *this);
        }
      }
    }

    void verbose(bool v)
    {
      verbose_ = v;
    }

    WikiLinkGetter& getter_;
    WikiLinkWeb& web_;
    std::set<WikiLinkWeb::NodeSharedPtr>& unexplored_;
    int get_limit_;
    bool verbose_;    
  };

//   std::list<std::string> findFromTo(const char* from, const char* to)
//   {
//     std::list<std::string> result;

//     return result;
//   }


private:
  std::set<WikiLinkWeb::NodeSharedPtr> unexplored_;

  boost::asio::io_service& io_service_;
  WikiLinkWeb& web_;
  WikiLinkGetter getter_;
  LinksToWeb links_to_web_;

  std::set<std::string> links_;
  bool verbose_;

};




#endif
