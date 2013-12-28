#ifndef WIKI_LINK_GETTER_HPP
#define WIKI_LINK_GETTER_HPP


#include <set>
#include <string>

#include "WikiGetter.hpp"




class WikiLinkGetter : public WikiGetter
{
private:
  typedef WikiGetter super;

public:

  WikiLinkGetter(boost::asio::io_service& io_service)
    : WikiGetter(io_service)
    , path("/w/api.php?action=query&format=json&prop=links&pllimit=max&titles=")
  {
  }

  virtual void get(const char* link)
  {
    std::string link_path(path);
    link_path += link;

    super::get(link_path.c_str());
  }

  const std::set<std::string>& getLinks() const
  {
    return links;
  }

protected:
  WikiLinkGetter();
  WikiLinkGetter(const WikiLinkGetter&);
  WikiLinkGetter& operator=(const WikiLinkGetter&);  

private:
  std::string path;
  std::set<std::string> links;

};




#endif // WIKI_GETTER_HPP
