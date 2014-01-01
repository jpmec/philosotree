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




struct get_response
{
	get_response(HttpGetter::Response& r)
	: response(r)
	{
	}

	void operator() (const HttpGetter::Response& r)
	{
		response = r;		
	}

	HttpGetter::Response& response;
};




BOOST_AUTO_TEST_CASE( HttpGetter_get_noop )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	getter.get("httpbin.org", "/ip", HttpGetter::noop);

	io_service.run();
}




BOOST_AUTO_TEST_CASE( HttpGetter_get )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	HttpGetter::Response response;
	get_response f(response);

	getter.get("httpbin.org", "/ip", f);

	io_service.run();

	BOOST_CHECK(response.status_code = 200);
	BOOST_CHECK(response.status_message == "OK");
	BOOST_CHECK(response.http_version == "HTTP/1.1");
	BOOST_CHECK(response.header.size() > 0);
	BOOST_CHECK(response.body.size() > 0);
}




BOOST_AUTO_TEST_CASE( HttpGetter_get_404 )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	HttpGetter::Response response;
	get_response f(response);

	getter.get("httpbin.org", "/status/404", f);

	io_service.run();

	BOOST_CHECK(response.status_code = 404);
	BOOST_CHECK(response.status_message == "NOT FOUND");
	BOOST_CHECK(response.http_version == "HTTP/1.1");
	BOOST_CHECK(response.header.size() > 0);
	BOOST_CHECK(response.body.size() == 0);
}




BOOST_AUTO_TEST_CASE( HttpGetter_get_500 )
{
	boost::asio::io_service io_service;
	HttpGetter getter(io_service);

	HttpGetter::Response response;
	get_response f(response);

	getter.get("httpbin.org", "/status/500", f);

	io_service.run();

	BOOST_CHECK(response.status_code = 404);
	BOOST_CHECK(response.status_message == "INTERNAL SERVER ERROR");
	BOOST_CHECK(response.http_version == "HTTP/1.1");
	BOOST_CHECK(response.header.size() > 0);
	BOOST_CHECK(response.body.size() == 0);
}




// BOOST_AUTO_TEST_CASE( HttpGetter_get_2 )
// {
// 	boost::asio::io_service io_service;
// 	HttpGetter<> getter1(io_service);
// 	HttpGetter<> getter2(io_service);

// 	getter1.get("www.google.com", "/");
// 	getter2.get("www.yahoo.com", "/");

// 	io_service.run();

// 	{
// 		HttpGetterResponse response = getter1.getResponse();

// 		BOOST_CHECK(response.status_code = 200);
// 		BOOST_CHECK(response.status_message == "OK");
// 		BOOST_CHECK(response.http_version == "HTTP/1.0");
// 		BOOST_CHECK(response.header.size() > 0);
// 		BOOST_CHECK(response.body.size() > 0);
// 	}

// 	{
// 		HttpGetterResponse response = getter2.getResponse();

// 		BOOST_CHECK(response.status_code = 200);
// 		BOOST_CHECK(response.status_message == "OK");
// 		BOOST_CHECK(response.http_version == "HTTP/1.0");
// 		BOOST_CHECK(response.header.size() > 0);
// 		BOOST_CHECK(response.body.size() > 0);
// 	}	
// }




// BOOST_AUTO_TEST_CASE( HttpGetter_get_2_302 )
// {
// 	boost::asio::io_service io_service;
// 	HttpGetter<> getter(io_service);

// 	getter.get("en.wikipedia.org", "/wiki/Special:Random");

// 	io_service.run();

// 	HttpGetterResponse response = getter.getResponse();

// 	BOOST_CHECK(response.status_code = 302);
// }




// struct PrintAfterGet
// {
// 	PrintAfterGet(HttpGetter& getter)
// 	: getter(getter)
// 	{

// 	}

// 	void operator()()
// 	{
// 		const HttpGetterResponse& response = getter->getResponse();
// 		cout << getter->getResponse() << endl;
// 	}

// 	HttpGetter& getter;
// };


// BOOST_AUTO_TEST_CASE( HttpGetter_get_3_callback_functor )
// {
// 	boost::asio::io_service io_service;

// 	HttpGetter<PrintAfterGet> getter(io_service);

// 	PrintAfterGet callback(getter);

// 	getter.get("www.google.com", "/", callback);

// 	io_service.run();


// 	HttpGetterResponse response = getter.getResponse();

// 	BOOST_CHECK(response.status_code = 200);
// }

