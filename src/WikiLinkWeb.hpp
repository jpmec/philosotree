#ifndef WIKI_LINK_WEB_HPP
#define WIKI_LINK_WEB_HPP


#include <iostream>
#include <string>
#include "GenericSpiderWeb.hpp"


class WikiLinkWeb : public GenericSpiderWeb<std::string>
{
  typedef GenericSpiderWeb<std::string> super;

public:
  void connect(const char* from, const char* to)
  {
    const Node from_node(from);
    const Node to_node(to);

    super::connect(from_node, to_node);
  }


  void connect(const std::string& from, const std::string& to)
  {
    super::connect(from, to);
  }


  NodeSharedPtr find(const std::string& value) const
  {
    return super::find(value);
  }


  NodeSharedPtrList find(const char* from, const char* to) const
  {
    const Node from_node(from);
    const Node to_node(to);

    return super::find(from_node, to_node);
  }


  NodeSharedPtrList find(const std::string& from, const std::string& to) const
  {
    return super::find(from, to);
  }   
};




std::ostream& operator<<(std::ostream& os, const WikiLinkWeb& web)
{
  const WikiLinkWeb::NodeSet& nodes = web.getNodes();
  WikiLinkWeb::NodeSet::iterator i = nodes.begin();

  for (; i != nodes.end(); ++i)
  {
    os << (*i)->c_str() << std::endl;
  }

  return os;
}




#endif//WIKI_LINK_WEB_HPP
