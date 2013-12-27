#ifndef GENERIC_SPIDER_WEB_HPP
#define GENERIC_SPIDER_WEB_HPP


#include <map>
#include <set>
#include <utility>
#include <boost/shared_ptr.hpp>




template< typename NodeType >
class GenericSpiderWeb
{
public:
  typedef NodeType Node;
  typedef boost::shared_ptr< NodeType > NodeSharedPtr;

  struct NodeSharedPtrCompare {
    bool operator() (const NodeSharedPtr& lhs, const NodeSharedPtr& rhs) const
    {
      return *lhs < *rhs;
    }
  };

  typedef std::set< NodeSharedPtr, NodeSharedPtrCompare > NodeSet;
  typedef std::pair< NodeSet, NodeSet > NodeSetPair;
  typedef boost::shared_ptr< NodeSetPair > NodeSetPairSharedPtr;

  // key is node,
  // pair first value is set of nodes containing key node
  // pair second value is set of nodes contained by key node  
  typedef std::map< NodeSharedPtr, NodeSetPairSharedPtr > DirectedGraph;
  typedef std::pair< NodeSharedPtr, NodeSetPairSharedPtr > DirectedGraphPair;

  ~GenericSpiderWeb()
  {

  }


  const NodeSet& getNodes()
  {
    return nodes;
  }


  const DirectedGraph& getGraph()
  {
    return graph;
  }


  // NodeType must support copy constructor.
  NodeSharedPtr insert(const NodeType& node)
  {
    NodeSharedPtr new_node(new NodeType(node));

    this->nodes.insert(new_node);

    NodeSetPairSharedPtr new_node_set_pair(new NodeSetPair());

    this->graph.insert(DirectedGraphPair(new_node, new_node_set_pair));

    return new_node;    
  }


  NodeSharedPtr find(const NodeType& node)
  {
    typename NodeSet::iterator i = nodes.begin();

    for (; i != nodes.end(); ++i)
    {
      if (**i == node)
      {
        return *i;
      }
    }

    return NodeSharedPtr();    
  }


  void erase(const NodeType& node)
  {
    typename NodeSet::iterator i = nodes.begin();

    for (; i != nodes.end(); ++i)
    {
      if (**i == node)
      {
        graph.erase(*i);
        nodes.erase(i);
        return;
      }
    }
  }


  void connectNodes(const NodeType& from_node, const NodeType& to_node)
  {
    // NodeSharedPtr from_node_ptr = addNode(from_node);
    // NodeSharedPtr to_node_ptr = addNode(to_node);

    // DirectedGraph::iterator i = web.find(from_node);
    // if (i == web.end())
    // {

    // }
  }


private:

  NodeSet nodes;
  DirectedGraph graph;
};




#endif//GENERIC_SPIDER_WEB_HPP
