#include "WikiLinkWeb.hpp"

#define BOOST_TEST_MODULE WikiLinkWeb
#include <boost/test/included/unit_test.hpp>

#include <string>

using std::string;




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
	}

	// add second unique node
	{
		string expected2("world");

		WikiLinkWeb::NodeSharedPtr result = web.insert(expected2);
		BOOST_CHECK(*result == expected2);

		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		BOOST_CHECK(2 == nodes.size());
	}

	// add third non-unique node
	{
		string expected3("hello");

		WikiLinkWeb::NodeSharedPtr result = web.insert(expected3);
		BOOST_CHECK(*result == expected3);

		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
		BOOST_CHECK(2 == nodes.size());
	}					
}


BOOST_AUTO_TEST_CASE( WikiLinkWeb_find)
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
	BOOST_CHECK(2 == nodes.size());

	web.erase(expected1);
	BOOST_CHECK(1 == nodes.size());	

	web.erase(expected2);
	BOOST_CHECK(0 == nodes.size());	

	web.erase(expected2);
	BOOST_CHECK(0 == nodes.size());	
}


// BOOST_AUTO_TEST_CASE( WikiLinkWeb_connectNodes )
// {
// 	WikiLinkWeb web;

// 	// add the first node
// 	{
// 		string expected1("hello");
// 		string expected2("world");

// 		web.connectNodes(expected1, expected2);

// 		const WikiLinkWeb::NodeSet& nodes = web.getNodes();
// 		BOOST_CHECK(2 == nodes.size());		
// 	}				
// }


