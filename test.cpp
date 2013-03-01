#include <string>
#include <iostream>
#include <assert.h>

#include "variant.h"

using namespace std;
using namespace variant;

struct has_destructor {
	bool& flag;
	
	has_destructor(bool& flag) : flag(flag) {}
	
	~has_destructor() {
		flag = true;
	}
};

void test_assignment() {
	Variant<int, bool, double, string> v(4);
	assert(v.get<int>() == 4);
	
	v = string("a");
	assert(v.get<string>() == "a");
}
void test_tag() {
	Variant<int, bool, double, string> v(4);
	assert(v.tag() == 0);
	
	v = string("a");
	assert(v.tag() == 3);
}
void test_variant_destructor() {
	bool destructed = false; // has our object been destructed?
	{
		Variant<int, has_destructor> v = has_destructor(destructed);
	}
	assert(destructed);
}
void test_assignment_destruction() {
	bool destructed = false; // has our object been destructed?
	Variant<int, has_destructor> v = has_destructor(destructed);
	
	v = 5;
	
	assert(destructed);
}


int main(int argc, char *argv[]) {
	test_assignment();
	test_tag();
	test_variant_destructor();
	test_assignment_destruction();
	
	cout << "All tests passed." << endl;
	return 0;
}

