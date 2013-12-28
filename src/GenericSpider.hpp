#ifndef GENERIC_SPIDER_HPP
#define GENERIC_SPIDER_HPP




template < typename WebType >
class GenericSpider
{
public:
	typedef WebType Web;

	virtual ~GenericSpider()
	{

	}

	const Web& getWeb()
	{
		return web;
	}


protected:
	GenericSpider(const GenericSpider&);
	GenericSpider& operator=(const GenericSpider&);



private:
	Web web;
};




#endif//GENERIC_SPIDER_HPP
