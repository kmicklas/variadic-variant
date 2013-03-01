#include <stdexcept>

namespace variant {

// Implementation details, the user should not import this:
namespace impl {

template<int N, typename... Ts>
struct storage;

template<typename X, typename... Ts>
struct position;

template<int N, typename T, typename... Ts>
struct storage<N, T&, Ts...> {
	static const bool no_reference_types = false;
	static const bool no_duplicates = storage<N + 1, Ts...>::no_duplicates && position<T, Ts...>::pos == -1;
	
	void init(const T& v) {}
	
	template<typename X>
	void init(const X& v) {}
	
	void del(int n) {}
};
template<int N, typename T, typename... Ts>
struct storage<N, T, Ts...> {
	static const bool no_reference_types = storage<N + 1, Ts...>::no_reference_types;
	static const bool no_duplicates = storage<N + 1, Ts...>::no_duplicates && position<T, Ts...>::pos == -1;
	
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

template<typename X>
struct position<X> {
	static const int pos = -1;
};
template<typename X, typename... Ts>
struct position<X, X, Ts...> {
	static const int pos = 0;
};
template<typename X, typename T, typename... Ts>
struct position<X, T, Ts...> {
	static const int pos = position<X, Ts...>::pos != -1 ? position<X, Ts...>::pos + 1 : -1;
};

} // namespace impl

template<typename... Types>
struct Variant {
	static_assert(impl::storage<0, Types...>::no_reference_types, "Reference types are not permitted in variant.");
	static_assert(impl::storage<0, Types...>::no_duplicates, "duplicates in types");
	
	int t;
	impl::storage<0, Types...> s;
	
	Variant() = delete;
	
	template<typename X>
	Variant(const X& v) : t(impl::position<X, Types...>::pos) {
		static_assert(impl::position<X, Types...>::pos != -1, "not in variant");
		s.init(v);
	}
	template<typename X>
	void operator=(const X& v) {
		static_assert(impl::position<X, Types...>::pos != -1, "not in variant");
		s.del(t);
		t = impl::position<X, Types...>::pos;
		s.init(v);
	}
	template<typename X>
	X& get() /* const */ {
		static_assert(impl::position<X, Types...>::pos != -1, "not in variant");
		if(t == impl::position<X, Types...>::pos) {
			return *reinterpret_cast<X*>(&s);
		} else {
			throw std::runtime_error(
				"asghasgh"
			);
		}
	}
	int tag() {return t;}
};

} // namespace variant

