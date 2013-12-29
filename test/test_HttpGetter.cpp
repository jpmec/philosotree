#include "HttpGetter.hpp"

#define BOOST_TEST_MODULE HttpGetter
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>
#include <boost/asio.hpp>


using std::string;
using std::cout;
using std::endl;



BOOST_AUTO_TEST_CASE( HttpGetter_constructor )
{
	boost::asio::io_service io_service;

	HttpGetter getter(io_service);
}



BOOST_AUTO_TEST_CASE( HttpGetter_get )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	getter.get("www.google.com", "/");

	io_service.run();


	HttpGetter::Response response = getter.getResponse();

	BOOST_CHECK(response.status_code = 200);
	BOOST_CHECK(response.status_message == "OK");
	BOOST_CHECK(response.http_version == "HTTP/1.0");
	BOOST_CHECK(response.header.size() > 0);
	BOOST_CHECK(response.body.size() > 0);

	// cout << response.status_code << endl;
	// cout << response.status_message << endl;
	// cout << response.http_version << endl;
	// cout << response.header << endl;
	// cout << response.body << endl;
}
