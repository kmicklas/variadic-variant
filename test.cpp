#include <string>
#include <iostream>
#include <assert.h>

#include "variant.h"

using namespace std;

int main(int argc, char *argv[]) {
	variant<int, bool, double, string> test(4);
	assert(test.tag() == 0);
	assert(test.get<int>() == 4);
	
	test = string("a");
	assert(test.tag() == 3);
	assert(test.get<string>() == "a");
	
	cout << "All tests passed." << endl;
	return 0;
}

