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
	WikiLinkGetter getter(io_service);
	WikiLinkSpider spider(web, getter);
}



BOOST_AUTO_TEST_CASE( WikiLinkSpider_crawl )
{
	boost::asio::io_service io_service;

	WikiLinkWeb web;
	WikiLinkGetter getter(io_service);
	WikiLinkSpider spider(web, getter);

	spider.crawl("Kevin_Bacon", "Philosopy");

	io_service.run();


	WikiLinkSpider::Web::NodeSharedPtrList result = spider.getWeb().find("Kevin_Bacon", "Philosopy");
//	HttpGetter::Response response = getter.getResponse();
}
