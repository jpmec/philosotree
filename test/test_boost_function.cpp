#include <boost/function.hpp>



#define BOOST_TEST_MODULE boost_function
#include <boost/test/included/unit_test.hpp>


int add_one_function(int i)
{
	return i + 1;
}




struct add_one_functor
{
	int operator()(int i)
	{
		return i + 1;
	}
};


struct add_one_to_n_functor
{
	int n;

	add_one_to_n_functor(int n)
	: n(n)
	{

	}

	int operator()(int i)
	{
		return i + n;
	}
};




struct test_class_void
{
	test_class_void(boost::function<int(void)> f)
	: f(f)
	{

	}

	int foo()
	{
		return f();
	}

	boost::function<int(void)> f;
};




struct test_class_int
{
	test_class_int(boost::function<int(int)> f)
	: f(f)
	{

	}

	int foo(int i)
	{
		return f(i);
	}

	boost::function<int(int)> f;
};



struct test_class_static_member
{
	test_class_static_member(int i)
	: i_(i)
	{
		f_ = test_class_static_member::foo;
	}

	static int foo(int i, int j)
	{
		return i + j;
	}

	int operator()(int j)
	{
		return f_(i_, j);
	}

	int i_;
	boost::function<int(int, int)> f_;
};



BOOST_AUTO_TEST_CASE( boost_function_function )
{
	boost::function<int(int)> f = add_one_function;

	int result = f(1);
	
	BOOST_CHECK(result == 2);
}




BOOST_AUTO_TEST_CASE( boost_function_class_int_1 )
{
	{
		boost::function<int(int)> f = add_one_function;

		test_class_int c(f);

		int result = c.foo(1);
		
		BOOST_CHECK(result == 2);
	}

	{
		test_class_int c(add_one_function);

		int result = c.foo(1);
		
		BOOST_CHECK(result == 2);
	}

	{
        add_one_functor f;

		test_class_int c(f);

		int result = c.foo(1);
		
		BOOST_CHECK(result == 2);
	}

	{
        add_one_to_n_functor f(2);

		test_class_int c(f);

		int result = c.foo(1);
		
		BOOST_CHECK(result == 3);
	}		
}

