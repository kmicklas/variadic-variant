#include <string>
#include <iostream>

#include "variant.h"

#define BOOST_TEST_MODULE VariantTest

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace variant;

struct has_destructor {
	bool& flag;
	
	has_destructor(bool& flag) : flag(flag) {}
	
	~has_destructor() {
		flag = true;
	}
};

BOOST_AUTO_TEST_CASE(assignment) {
	Variant<int, bool, double, string> v(4);
	BOOST_CHECK(v.get<int>() == 4);
	
	v = string("a");
	BOOST_CHECK(v.get<string>() == "a");
}
BOOST_AUTO_TEST_CASE(which) {
	Variant<int, bool, double, string> v(4);
	BOOST_CHECK(v.which() == 0);
	
	v = string("a");
	BOOST_CHECK(v.which() == 3);
}
BOOST_AUTO_TEST_CASE(variant_destructor) {
	bool destructed = false; // has our object been destructed?
	{
		Variant<int, has_destructor> v = has_destructor(destructed);
	}
	BOOST_CHECK(destructed);
}
BOOST_AUTO_TEST_CASE(assignment_destruction) {
	bool destructed = false; // has our object been destructed?
	Variant<int, has_destructor> v = has_destructor(destructed);
	
	v = 5;
	
	BOOST_CHECK(destructed);
}

