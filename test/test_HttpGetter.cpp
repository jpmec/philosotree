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
}



BOOST_AUTO_TEST_CASE( HttpGetter_get_2 )
{
	boost::asio::io_service io_service;
	HttpGetter getter1(io_service);
	HttpGetter getter2(io_service);

	getter1.get("www.google.com", "/");
	getter2.get("www.yahoo.com", "/");

	io_service.run();

	{
		HttpGetter::Response response = getter1.getResponse();

		BOOST_CHECK(response.status_code = 200);
		BOOST_CHECK(response.status_message == "OK");
		BOOST_CHECK(response.http_version == "HTTP/1.0");
		BOOST_CHECK(response.header.size() > 0);
		BOOST_CHECK(response.body.size() > 0);
	}

	{
		HttpGetter::Response response = getter2.getResponse();

		BOOST_CHECK(response.status_code = 200);
		BOOST_CHECK(response.status_message == "OK");
		BOOST_CHECK(response.http_version == "HTTP/1.0");
		BOOST_CHECK(response.header.size() > 0);
		BOOST_CHECK(response.body.size() > 0);
	}	
}


BOOST_AUTO_TEST_CASE( HttpGetter_get_2_302 )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	getter.get("en.wikipedia.org", "/wiki/Special:Random");

	io_service.run();


	HttpGetter::Response response = getter.getResponse();

	BOOST_CHECK(response.status_code = 302);

//	cout << response << endl;
}




struct PrintFunctorAfterGet : public HttpGetter::AfterGetFunctor
{
	void operator()(HttpGetter::Response&) const
	{
		cout << "PrintFunctorAfterGet" << endl;
	}
};


BOOST_AUTO_TEST_CASE( HttpGetter_get_3_callback_functor )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	PrintFunctorAfterGet after_get;
	getter.get("www.google.com", "/", after_get);

	io_service.run();


	HttpGetter::Response response = getter.getResponse();

	BOOST_CHECK(response.status_code = 200);

//	cout << response << endl;
}