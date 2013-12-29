#include "WikiGetter.hpp"

#define BOOST_TEST_MODULE WikiGetter
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>
#include <boost/asio.hpp>


using std::string;
using std::cout;
using std::endl;



BOOST_AUTO_TEST_CASE( WikiGetter_constructor )
{
	boost::asio::io_service io_service;

	WikiGetter getter(io_service);
}


BOOST_AUTO_TEST_CASE( WikiGetter_get )
{
	{
		boost::asio::io_service io_service;
		WikiGetter getter(io_service);

		getter.get("/wiki/Special:Random");

		io_service.run();


		WikiGetter::Response response = getter.getResponse();

		BOOST_CHECK(response.status_code == 302);
		BOOST_CHECK(response.status_message == "Found");
		BOOST_CHECK(response.http_version == "HTTP/1.1");
		BOOST_CHECK(response.header.size() > 0);
		BOOST_CHECK(response.body.size() == 0);
	}


	{
		boost::asio::io_service io_service;
		WikiGetter getter(io_service);

		getter.get("/wiki/Philosophy");

		io_service.run();


		WikiGetter::Response response = getter.getResponse();

		BOOST_CHECK(response.status_code == 200);
		BOOST_CHECK(response.status_message == "OK");
		BOOST_CHECK(response.http_version == "HTTP/1.1");
		BOOST_CHECK(response.header.size() > 0);
		BOOST_CHECK(response.body.size() > 0);		
	}


	{
		boost::asio::io_service io_service;
		WikiGetter getter(io_service);

		getter.get("/w/api.php?action=query&format=json&prop=links&pllimit=max&titles=Philosophy");

		io_service.run();


		WikiGetter::Response response = getter.getResponse();

		BOOST_CHECK(response.status_code == 200);
		BOOST_CHECK(response.status_message == "OK");
		BOOST_CHECK(response.http_version == "HTTP/1.1");
		BOOST_CHECK(response.header.size() > 0);
		BOOST_CHECK(response.body.size() > 0);		
	}
}
