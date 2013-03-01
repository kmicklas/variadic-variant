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

int main(int argc, char *argv[]) {
	bool destructed = false; // has our object been destructed?
	
	Variant<int, bool, double, string, has_destructor> test(4);
	assert(test.tag() == 0);
	assert(test.get<int>() == 4);
	
	test = has_destructor(destructed);
	test = string("a");
	assert(destructed); // assignment should call destructor of previous object
	assert(test.tag() == 3);
	assert(test.get<string>() == "a");
	
	cout << "All tests passed." << endl;
	return 0;
}

