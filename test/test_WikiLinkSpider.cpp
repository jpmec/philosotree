#include "WikiLinkSpider.hpp"

#define BOOST_TEST_MODULE WikiLinkSpider
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;



BOOST_AUTO_TEST_CASE( WikiLinkSpider_constructor )
{
	boost::asio::io_service io_service;

	WikiLinkWeb web;
	WikiLinkSpider spider(io_service, web);
}



BOOST_AUTO_TEST_CASE( WikiLinkSpider_crawl )
{
	boost::asio::io_service io_service;

	WikiLinkWeb web;
	WikiLinkSpider spider(io_service, web);

	spider.verbose(true);

	spider.crawl("Kevin Bacon", "Philosopy");

	io_service.run();

	BOOST_CHECK(web.size() > 0);

	cout << "web.size(): " << web.size() << endl;
	cout << web << endl;

	// WikiLinkSpider::Web::NodeSharedPtrList result = web.find("Kevin_Bacon", "Philosopy");

	// BOOST_CHECK(result.size() > 0);
//	HttpGetter::Response response = getter.getResponse();
}
