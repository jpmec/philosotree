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



BOOST_AUTO_TEST_CASE( WikiLinkGetter_get )
{
	boost::asio::io_service io_service;
	WikiLinkGetter getter(io_service);

	getter.get("Philosophy");

	io_service.run();

	std::set<std::string> links = getter.getLinks();

	WikiGetter::Response response = getter.getResponse();

	BOOST_CHECK(0 < links.size());

	//cout << response << endl;

	// for (std::set<std::string>::iterator i = links.begin(); i != links.end(); ++i)
	// {
	// 	cout << *i << endl;
	// }
}
