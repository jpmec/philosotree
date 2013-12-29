#ifndef WIKI_LINK_GETTER_HPP
#define WIKI_LINK_GETTER_HPP


#include <iostream>
#include <set>
#include <string>

#include "WikiGetter.hpp"


using std::cout;
using std::endl;


class WikiLinkGetter : public WikiGetter
{
private:
  typedef WikiGetter super;

public:

  WikiLinkGetter(boost::asio::io_service& io_service)
    : WikiGetter(io_service)
    , path_("/w/api.php?action=query&format=json&prop=links&pllimit=max&titles=")
  {
  }

  virtual void get(const char* link)
  {
    std::string link_path(path_);
    link_path += link;

    super::get(link_path.c_str());
  }

  const std::set<std::string>& getLinks() const
  {
    return links_;
  }

protected:
  WikiLinkGetter();
  WikiLinkGetter(const WikiLinkGetter&);
  WikiLinkGetter& operator=(const WikiLinkGetter&);  

  virtual void onEof(void)
  {
    super::onEof();

    ptree pt;

    istringstream json_stream(getResponse().body);
    read_json(json_stream, pt);
    
    //pt.get_child("query-continue").get_child("links").get_child("plcontinue");

    pt = pt.get_child("query");
    pt = pt.get_child("pages");
    
    for (ptree::iterator i = pt.begin(); i != pt.end(); ++i)
    {
      ptree links =  i->second.get_child("links");

      for (ptree::iterator l = links.begin(); l != links.end(); ++l)
      {
        links_.insert(l->second.get_child("title").get_value<string>());
      }
    }

    //cout << getResponse().body << endl;    
  }

private:
  std::string path_;
  std::set<std::string> links_;

};




#endif // WIKI_GETTER_HPP
