#include "WikiLinkWeb.hpp"

#define BOOST_TEST_MODULE WikiLinkWeb
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;



BOOST_AUTO_TEST_CASE( WikiLinkWeb_constructor )
{
	WikiLinkWeb web();
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_getNodes )
{
	WikiLinkWeb web;

	const WikiLinkWeb::NodeSet& nodes = web.getNodes();
	BOOST_CHECK(0 == nodes.size());
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_insert)
{
	WikiLinkWeb web;

	// add the first node
	{
		string expected1("hello");

		WikiLinkWeb::NodeSharedPtr result = web.insert(expected1);
		BOOST_CHECK(*result == expected1);

		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		BOOST_CHECK(1 == nodes.size());

		const WikiLinkWeb::DirectedGraph& graph = web.getGraph();
		BOOST_CHECK(1 == graph.size());
	}

	// add second unique node
	{
		string expected2("world");

		WikiLinkWeb::NodeSharedPtr result = web.insert(expected2);
		BOOST_CHECK(*result == expected2);

		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		BOOST_CHECK(2 == nodes.size());

		const WikiLinkWeb::DirectedGraph& graph = web.getGraph();
		BOOST_CHECK(2 == graph.size());		
	}

	// add third non-unique node
	{
		string expected3("hello");

		WikiLinkWeb::NodeSharedPtr result = web.insert(expected3);
		BOOST_CHECK(*result == expected3);

		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		BOOST_CHECK(2 == nodes.size());

		const WikiLinkWeb::DirectedGraph& graph = web.getGraph();
		BOOST_CHECK(2 == graph.size());		
	}					
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_find_1)
{
	WikiLinkWeb web;

	string expected1("hello");
	web.insert(expected1);

	string expected2("world");
	web.insert(expected2);

	{
		WikiLinkWeb::NodeSharedPtr ptr = web.find(expected1);
		BOOST_CHECK(*ptr == expected1);	
	}

	{
		WikiLinkWeb::NodeSharedPtr ptr = web.find(expected2);
		BOOST_CHECK(*ptr == expected2);	
	}	

	{
		WikiLinkWeb::NodeSharedPtr ptr = web.find(string("not here"));
		BOOST_CHECK(ptr.get() == 0);
	}
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_erase)
{
	WikiLinkWeb web;

	string expected1("hello");
	web.insert(expected1);

	string expected2("world");
	web.insert(expected2);

	const WikiLinkWeb::NodeSet& nodes = web.getNodes();
	const WikiLinkWeb::DirectedGraph& graph = web.getGraph();

	BOOST_CHECK(2 == nodes.size());
	BOOST_CHECK(2 == graph.size());

	web.erase(expected1);
	BOOST_CHECK(1 == nodes.size());	
	BOOST_CHECK(1 == graph.size());

	web.erase(expected2);
	BOOST_CHECK(0 == nodes.size());	
	BOOST_CHECK(0 == graph.size());

	web.erase(expected2);
	BOOST_CHECK(0 == nodes.size());
	BOOST_CHECK(0 == graph.size());		
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_connect_1 )
{
	WikiLinkWeb web;

	{
		const string expected1("hello");
		const string expected2("world");
		const string expected3("peace");		

		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		const WikiLinkWeb::DirectedGraph& graph = web.getGraph();

		web.connect(expected1, expected2);

		BOOST_CHECK(2 == nodes.size());
		BOOST_CHECK(2 == graph.size());

		web.connect(expected2, expected3);

		BOOST_CHECK(3 == nodes.size());
		BOOST_CHECK(3 == graph.size());								
	}


}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_connect_2 )
{
	WikiLinkWeb web;

	{	
		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		const WikiLinkWeb::DirectedGraph& graph = web.getGraph();

		web.connect("hello", "world");

		BOOST_CHECK(2 == nodes.size());
		BOOST_CHECK(2 == graph.size());

		web.connect("world", "peace");

		BOOST_CHECK(3 == nodes.size());
		BOOST_CHECK(3 == graph.size());								
	}				
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_find_2 )
{
	WikiLinkWeb web;

	{
		web.connect("hello", "world");
		web.connect("world", "peace");
		web.connect("peace", "frog");
		web.connect("hello", "dolly");
		web.connect("dolly", "pardon");
		web.connect("pardon", "me");
		web.connect("me", "too");
		web.connect("too", "you");

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find(string("not"), string("here"));
			BOOST_CHECK(0 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("hello", string("not here"));
			BOOST_CHECK(0 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find(string("not here"), "hello");
			BOOST_CHECK(0 == result.size());
		}				

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("hello", "world");
			BOOST_CHECK(2 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("world", "peace");
			BOOST_CHECK(2 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("peace", "frog");
			BOOST_CHECK(2 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("hello", "dolly");
			BOOST_CHECK(2 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("dolly", "pardon");
			BOOST_CHECK(2 == result.size());
		}							

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("pardon", "me");
			BOOST_CHECK(2 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("hello", "peace");
			BOOST_CHECK(3 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("hello", "frog");
			BOOST_CHECK(4 == result.size());
		}

		{
			WikiLinkWeb::NodeSharedPtrList result = web.find("hello", "you");
			BOOST_CHECK(6 == result.size());

			BOOST_CHECK(*result.front() == "hello");
			result.pop_front();

			BOOST_CHECK(*result.front() == "dolly");
			result.pop_front();

			BOOST_CHECK(*result.front() == "pardon");
			result.pop_front();

			BOOST_CHECK(*result.front() == "me");
			result.pop_front();

			BOOST_CHECK(*result.front() == "too");
			result.pop_front();

			BOOST_CHECK(*result.front() == "you");
		}								
	}				
}

