#ifndef WIKI_LINK_WEB_HPP
#define WIKI_LINK_WEB_HPP


#include <iostream>
#include <string>
#include <vector>

#include "GenericSpiderWeb.hpp"
#include "iojson.hpp"




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

  os << "{" << std::endl;

  for (; i != nodes.end();)
  {
    os << "\""<< (*i)->c_str() << "\"" << " : { ";

    WikiLinkWeb::DirectedGraph::const_iterator g = graph.find(*i);

    if (g != graph.end())
    {
      const WikiLinkWeb::NodeSet& from(g->second->first);
      WikiLinkWeb::NodeSet::const_iterator f = from.begin();

      const WikiLinkWeb::NodeSet& to(g->second->second);
      WikiLinkWeb::NodeSet::const_iterator t = to.begin();

      if (f != from.end())
      {
        os << "\"from\" : [ ";

        os << "\"" << (*f)->c_str() << "\"";

        for (++f; f != from.end(); ++f)
        {
          os << " , " << "\"" << (*f)->c_str() << "\"";
        }

        os << " ]";

        if (t != to.end())
        {
          os << " , ";
        }
      }

      if (t != to.end())
      {
        os << "\"to\" : [ ";

        os << "\"" << (*t)->c_str() << "\"";

        for (++t; t != to.end(); ++t)
        {
          os << " , " << "\"" << (*t)->c_str() << "\"";
        }

        os << " ]";
      }
    }

    os << " }";

    ++i;
    if (i != nodes.end())
    {
      os << "," << std::endl;
    }
    else
    {
      os << std::endl;
    }
  }

  os << "}" << std::endl;

  return os;
}




std::ostream& operator<<(std::ostream& os, const std::vector< std::string >& v)
{

  std::vector< std::string >::const_iterator i = v.begin();

  for(; i != v.end(); ++i)
  {
    os << *i << std::endl;
  }

  return os;
}




std::istream& operator>>(std::istream& is, std::vector< std::string >& v)
{
  iojson::left_bracket left_bracket;
  iojson::right_bracket right_bracket;

  iojson::string s;
  iojson::comma comma;

  is >> left_bracket;

  try
  {
    while(1)
    {
      is >> s;
      v.push_back(s.value);
      is >> comma;
    }
  }
  catch (std::exception& e)
  {

  }

  is >> right_bracket;

  return is;
}




std::istream& operator>>(std::istream& is, WikiLinkWeb& web)
{
  iojson::colon colon;
  iojson::comma comma;
  iojson::left_brace left_brace;
  iojson::right_brace right_brace;

  try
  {
    is >> left_brace;

    while(1)
    {
      iojson::key link_key;
      is >> link_key;
      is >> colon;
      is >> left_brace;

      iojson::key key;
      is >> key;
      is >> colon;

      std::vector< std::string > string_vector;
      is >> string_vector;

      if (key.value == "from")
      {
        std::vector< std::string >::iterator i = string_vector.begin();
        for (; i != string_vector.end(); ++i)
        {
          web.connect(*i, link_key.value.c_str());
        }
      }

      is >> right_brace;
      is >> comma;
    }
  }
  catch (std::exception& e)
  {
    // do nothing
  }

  return is;
}




#endif//WIKI_LINK_WEB_HPP
