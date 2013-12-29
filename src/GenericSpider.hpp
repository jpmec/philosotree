#ifndef GENERIC_SPIDER_HPP
#define GENERIC_SPIDER_HPP

#include <set>


template < typename WebType, typename GetterType >
class GenericSpider
{
public:
	typedef WebType Web;
	typedef GetterType Getter;

	GenericSpider(WebType& web, GetterType& getter)
	: web(web), getter(getter)
	{

	}

	virtual ~GenericSpider()
	{

	}

	const Web& getWeb()
	{
		return web;
	}


	void crawl(const typename Web::Node& from, const typename Web::Node& to)
	{
		if (web.contains(from))
		{
			if (web.contains(to))
			{
				return;
			}
		}
		else
		{
			unexplored.insert(web.insert(from));
		}
	}	

protected:

	GenericSpider();	
	GenericSpider(const GenericSpider&);
	GenericSpider& operator=(const GenericSpider&);



private:
	Web& web;
	Getter& getter;

	std::set<typename Web::NodeSharedPtr> unexplored;
};




#endif//GENERIC_SPIDER_HPP
