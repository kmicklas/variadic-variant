#include <stdexcept>

template<int N, typename... Ts>
struct storage;

template<typename X, int N, typename... Ts>
struct position;

template<int N, typename T, typename... Ts>
struct storage<N, T&, Ts...> {
	static const bool no_reference_types = false;
	static const bool no_duplicates = storage<N + 1, Ts...>::no_duplicates && position<T, 0, Ts...>::pos == -1;
	
	void init(const T& v) {}
	
	template<typename X>
	void init(const X& v) {}
	
	void del(int n) {}
};
template<int N, typename T, typename... Ts>
struct storage<N, T, Ts...> {
	static const bool no_reference_types = storage<N + 1, Ts...>::no_reference_types;
	static const bool no_duplicates = storage<N + 1, Ts...>::no_duplicates && position<T, 0, Ts...>::pos == -1;
	
	union {
		char first[sizeof(T)];
		storage<N + 1, Ts...> rest;
	};
	
	void init(const T& v) {
		new(first) T(v);
	}
	
	template<typename X>
	void init(const X& v) {rest.init(v);}
	
	void del(int n) {
		if(n == N) reinterpret_cast<T*>(first)->~T();
		else rest.del(n);
	}
};
template<int N>
struct storage<N> {
	static const bool no_reference_types = true;
	static const bool no_duplicates = true;
	
	template<typename X>
	void init(const X& v) {}
	
	void del(int n) {
		throw std::runtime_error(
			"asghasgh"
		);
	}
};

template<typename X, int N>
struct position<X, N> {
	static const int pos = -1;
};

template<typename X, int N, typename... Ts>
struct position<X, N, X, Ts...> {
	static const int pos = N;
};

template<typename X, int N, typename T, typename... Ts>
struct position<X, N, T, Ts...> {
	static const int pos = position<X, N + 1, Ts...>::pos;
};

template<typename... Types>
struct variant {
	static_assert(storage<0, Types...>::no_reference_types, "Reference types are not permitted in variant.");
	static_assert(storage<0, Types...>::no_duplicates, "duplicates in types");
	
	int t;
	storage<0, Types...> s;
	
	variant() = delete;
	
	template<typename X>
	variant(const X& v) : t(position<X, 0, Types...>::pos) {
		static_assert(position<X, 0, Types...>::pos != -1, "not in variant");
		s.init(v);
	}
	template<typename X>
	void operator=(const X& v) {
		static_assert(position<X, 0, Types...>::pos != -1, "not in variant");
		s.del(t);
		t = position<X, 0, Types...>::pos;
		s.init(v);
	}
	template<typename X>
	X& get() /* const */ {
		static_assert(position<X, 0, Types...>::pos != -1, "not in variant");
		if(t == position<X, 0, Types...>::pos) {
			return *reinterpret_cast<X*>(&s);
		} else {
			throw std::runtime_error(
				"asghasgh"
			);
		}
	}
	int tag() {return t;}
};

