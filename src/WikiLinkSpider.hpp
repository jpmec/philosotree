#ifndef WIKI_LINK_SPIDER_HPP
#define WIKI_LINK_SPIDER_HPP


#include <set>
#include <list>
#include <string>

#include "GenericSpider.hpp"
#include "WikiLinkGetter.hpp"
#include "WikiLinkWeb.hpp"




class WikiLinkSpider
{
public:
  WikiLinkSpider(boost::asio::io_service& io_service, WikiLinkWeb& web)
  : unexplored_()
  , io_service_(io_service)
  , web_(web)
  , getter_(io_service)
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
      getter_.get(from, LinksToWeb(getter_, from_str, web_, unexplored_, explore_next_));
    }
  }

  void verbose(bool v)
  {
    verbose_ = v;
    getter_.verbose(v);
  }

protected:
  WikiLinkSpider();  
  WikiLinkSpider(const WikiLinkSpider&);
  WikiLinkSpider& operator=(const WikiLinkSpider&);


  struct LinksToWeb
  {
    LinksToWeb( WikiLinkGetter& getter
              , const std::string& link_title
              , WikiLinkWeb& web
              , std::set<WikiLinkWeb::NodeSharedPtr>& unexplored
              , std::list<WikiLinkWeb::NodeSharedPtr>& explore_next              
              , int get_limit = 2
              , bool verbose = false)
    : getter_(getter)
    , link_title_(link_title)
    , web_(web)
    , unexplored_(unexplored)
    , explore_next_(explore_next)
    , get_limit_(get_limit)
    , verbose_(verbose)
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
          WikiLinkWeb::NodeSharedPtr ptr = web_.insert(*i);
          web_.connect(link_title_, *i);
          unexplored_.insert(ptr);
          explore_next_.push_back(ptr);
        }
      }


      if ((0 < get_limit_) && explore_next_.size())
      {
        --get_limit_;
        
//        if (verbose_)
          std::cout << "get_limit: " << get_limit_ << std::endl;

        std::list<WikiLinkWeb::NodeSharedPtr>::iterator first = explore_next_.begin();
        if (first != explore_next_.end())
        {
          std::string link = (*first)->c_str();
          unexplored_.erase(*first);
          explore_next_.pop_front();
          getter_.get(link.c_str(), LinksToWeb(getter_, link, web_, unexplored_, explore_next_, get_limit_, verbose_));
        }
      }
    }

    void verbose(bool v)
    {
      verbose_ = v;
    }

    WikiLinkGetter& getter_;
    std::string link_title_;
    WikiLinkWeb& web_;
    std::set<WikiLinkWeb::NodeSharedPtr>& unexplored_;
    std::list<WikiLinkWeb::NodeSharedPtr>& explore_next_;    
    int get_limit_;
    bool verbose_;    
  };


private:
  std::set<WikiLinkWeb::NodeSharedPtr> unexplored_;
  std::list<WikiLinkWeb::NodeSharedPtr> explore_next_;

  boost::asio::io_service& io_service_;
  WikiLinkWeb& web_;
  WikiLinkGetter getter_;

  std::set<std::string> links_;
  bool verbose_;

};




#endif
