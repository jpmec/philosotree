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
  const WikiLinkWeb::DirectedGraph& graph = web.getGraph();

  WikiLinkWeb::NodeSet::iterator i = nodes.begin();

  for (; i != nodes.end(); ++i)
  {
    os << (*i)->c_str() << ": { ";

    WikiLinkWeb::DirectedGraph::const_iterator g = graph.find(*i);

    if (g != graph.end())
    {
      const WikiLinkWeb::NodeSet& from(g->second->first);

      os << "from : { ";

      WikiLinkWeb::NodeSet::const_iterator f = from.begin();

      if (f != from.end())
      {
        os << (*f)->c_str();

        for (++f; f != from.end(); ++f)
        {
          os << ", " << (*f)->c_str();
        }
      }   

      os << " }, ";


      const WikiLinkWeb::NodeSet& to(g->second->second);

      os << "to : { ";

      WikiLinkWeb::NodeSet::const_iterator t = to.begin();

      if (t != to.end())
      {
        os << (*t)->c_str();

        for (++t; t != to.end(); ++t)
        {
          os << ", " << (*t)->c_str();
        }
      }    

      os << " } ";      
    }

    os << " }" << std::endl;
  }

  return os;
}




#endif//WIKI_LINK_WEB_HPP
