#ifndef WIKI_LINK_SPIDER_HPP
#define WIKI_LINK_SPIDER_HPP


// #include <map>
// #include <set>
// #include <string>
// #include <utility>

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>

#include "GenericSpider.hpp"
#include "WikiLinkWeb.hpp"

// #include "WikiLinkGetter.hpp"


class WikiLinkSpider : public GenericSpider< WikiLinkWeb >
{
// public:
//   WikiLinkSpider()
//   {
//   }

//   virtual ~WikiLinkSpider()
//   {
//   }

//   std::list<std::string> crawlFromTo(const char* from, const char* to)
//   {
//     std::string link(from);

//     WikiLinkGetter getter(io_service);

//     getter.get(link.c_str());

//     io_service.run();

//     return findFromTo(from, to);
//   }

// protected:
//   WikiLinkSpider(const WikiLinkSpider&);
//   WikiLinkSpider& operator=(const WikiLinkSpider&);

//   std::list<std::string> findFromTo(const char* from, const char* to)
//   {
//     std::list<std::string> result;

//     return result;
//   }

// private:
//   boost::asio::io_service io_service;


};




#endif
