#include <string>
#include <iostream>
#include <assert.h>

#include "variant.h"

using namespace std;
using namespace variant;

int main(int argc, char *argv[]) {
	Variant<int, bool, double, string> test(4);
	assert(test.tag() == 0);
	assert(test.get<int>() == 4);
	
	test = string("a");
	assert(test.tag() == 3);
	assert(test.get<string>() == "a");
	
	cout << "All tests passed." << endl;
	return 0;
}

