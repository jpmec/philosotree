#include "WikiLinkGetter.hpp"

#define BOOST_TEST_MODULE WikiLinkGetter
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>
#include <boost/asio.hpp>


using std::string;
using std::cout;
using std::endl;



BOOST_AUTO_TEST_CASE( WikiLinkGetter_constructor )
{
	boost::asio::io_service io_service;

	WikiLinkGetter getter(io_service);
}




struct get_links
{
	get_links(std::set<std::string>& links)
	: links(links)
	{
	}

	void operator() (const std::set<std::string>& l)
	{
		links = l;		
	}

	std::set<std::string>& links;
};


BOOST_AUTO_TEST_CASE( WikiLinkGetter_get )
{
	boost::asio::io_service io_service;

    std::set<std::string> links;
	get_links f(links);
	WikiLinkGetter getter(io_service);

	getter.get("Philosophy", f);

	io_service.run();

	BOOST_CHECK(0 < links.size());

	//cout << response << endl;

	for (std::set<std::string>::iterator i = links.begin(); i != links.end(); ++i)
	{
		cout << *i << endl;
	}
}




// struct PrintAfterGet
// {
// 	template <typename T>
// 	void operator()(T* getter)
// 	{
// 		const std::set<std::string>& links = getter->getLinks();
// 		for (std::set<std::string>::iterator i = links.begin(); i != links.end(); ++i)
// 		{
// 			cout << *i << endl;
// 		}
// 	}
// };


// BOOST_AUTO_TEST_CASE( WikiLinkGetter_get_1 )
// {
// 	boost::asio::io_service io_service;
// 	WikiLinkGetter<PrintAfterGet> getter(io_service);

// 	getter.get("Philosophy");

// 	io_service.run();

// 	std::set<std::string> links = getter.getLinks();

// 	HttpGetterResponse response = getter.getResponse();

// 	BOOST_CHECK(0 < links.size());

// 	//cout << response << endl;

// 	// for (std::set<std::string>::iterator i = links.begin(); i != links.end(); ++i)
// 	// {
// 	// 	cout << *i << endl;
// 	// }
// }
