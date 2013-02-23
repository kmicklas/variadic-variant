CXX:=g++-4.8
CXXFLAGS:=-Wall -std=c++0x

bin/test: test.cpp variant.h
	$(CXX) $(CXXFLAGS) -o bin/test test.cpp
	./bin/test

