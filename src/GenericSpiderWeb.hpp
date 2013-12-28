#ifndef GENERIC_SPIDER_WEB_HPP
#define GENERIC_SPIDER_WEB_HPP


#include <functional>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <boost/shared_ptr.hpp>




template< typename NodeType >
class GenericSpiderWeb
{
protected:
  struct NodeSharedPtrLessThanNodeSharedPtr;
  struct NodeSharedPtrEqualsNodeSharedPtr;

  struct NodeSharedPtrLessThanNode;
  struct NodeSharedPtrEqualsNode;

public:
  typedef NodeType Node;
  typedef boost::shared_ptr< NodeType > NodeSharedPtr;
  typedef std::list< NodeSharedPtr > NodeList;
  typedef std::set< NodeSharedPtr, NodeSharedPtrLessThanNodeSharedPtr > NodeSet;
  typedef std::pair< NodeSet, NodeSet > NodeSetPair;
  typedef boost::shared_ptr< NodeSetPair > NodeSetPairSharedPtr;

  // key is node,
  // pair first value is set of nodes leading to key node
  // pair second value is set of nodes lead to by key node  
  typedef std::map< NodeSharedPtr, NodeSetPairSharedPtr > DirectedGraph;
  typedef std::pair< NodeSharedPtr, NodeSetPairSharedPtr > DirectedGraphPair;

  GenericSpiderWeb()
  {

  }

  virtual ~GenericSpiderWeb()
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


  // Node must support copy constructor.
  NodeSharedPtr insert(const Node& node)
  {
    NodeSharedPtr found_node = find(node);
    if (found_node.get())
    {
      return found_node;
    }

    NodeSharedPtr new_node(new Node(node));

    this->nodes.insert(new_node);

    NodeSetPairSharedPtr new_node_set_pair(new NodeSetPair());

    this->graph.insert(DirectedGraphPair(new_node, new_node_set_pair));

    return new_node;    
  }


  NodeSharedPtr find(const Node& node)
  {
    typename NodeSet::iterator i = std::find_if(
      nodes.begin(), 
      nodes.end(),
      std::bind2nd(NodeSharedPtrEqualsNode(), node));

    if (i != nodes.end())
    {
      return *i;
    }
    else
    {
      return NodeSharedPtr();      
    }    
  }


  void erase(const Node& node)
  {
    typename NodeSet::iterator i = std::find_if(
      nodes.begin(), 
      nodes.end(),
      std::bind2nd(NodeSharedPtrEqualsNode(), node));

    if (i != nodes.end())
    {
      graph.erase(*i);
      nodes.erase(i);
    }
  }


  void connectNodes(const Node& from_node, const Node& to_node)
  {
    NodeSharedPtr from_node_ptr = insert(from_node);
    NodeSharedPtr to_node_ptr = insert(to_node);

    // DirectedGraph::iterator i = web.find(from_node);
    // if (i == web.end())
    // {

    // }
  }


  // Return a list of nodes from the given from_node to the given to_node.
  NodeList findPathFromTo(const Node& from_node, const Node& to_node)
  {
    // TODO IMPLEMENT
    NodeList result;

    return result;
  }


protected:
  GenericSpiderWeb(const GenericSpiderWeb&);
  GenericSpiderWeb& operator=(const GenericSpiderWeb&);

  struct NodeSharedPtrLessThanNodeSharedPtr 
  : public std::binary_function<NodeSharedPtr, NodeSharedPtr, bool > {
    bool operator() (const NodeSharedPtr& lhs, const NodeSharedPtr& rhs) const
    {
      return *lhs < *rhs;
    }
  };

  struct NodeSharedPtrLessThanNode 
  : public std::binary_function<NodeSharedPtr, Node, bool > {
    bool operator() (const NodeSharedPtr& lhs, const Node& rhs) const
    {
      return *lhs < *rhs;
    }
  };  

  struct NodeSharedPtrEqualsNodeSharedPtr 
  : public std::binary_function<NodeSharedPtr, NodeSharedPtr, bool > {
    bool operator() (const NodeSharedPtr& lhs, const NodeSharedPtr& rhs) const
    {
      return !(*lhs < *rhs) && !(*rhs < *lhs);
    }
  };

  struct NodeSharedPtrEqualsNode
  : public std::binary_function<NodeSharedPtr, Node, bool > {
    bool operator() (const NodeSharedPtr& lhs, const Node& rhs) const
    {
      return !(*lhs < rhs) && !(rhs < *lhs);
    }
  };  

private:
  NodeSet nodes;
  DirectedGraph graph;
};




#endif//GENERIC_SPIDER_WEB_HPP
