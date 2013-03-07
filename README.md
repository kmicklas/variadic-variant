variadic-variant
================

A C++ variant library (like Boost.Variant) using C++11 variadic templates and other C++11 features.

This library implements a stack based "discriminated union" data structure. Using variadic templates rather than a fixed large number of arguments with defaults (as in Boost) keeps compile times down and provides more readable error messages.

This library is not meant to be source compatible with Boost, but it provides similar features.

