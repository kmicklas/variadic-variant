#include <stdexcept>

namespace variant {

// Implementation details, the user should not import this:
namespace impl {

template<int N, typename... Ts>
struct storage;

template<typename X, typename... Ts>
struct position;

template<typename... Ts>
struct type_info;

template<int N, typename T, typename... Ts>
struct storage<N, T&, Ts...> {
	void del(int n) {}
};
template<int N, typename T, typename... Ts>
struct storage<N, T, Ts...> {
	union {
		char first[sizeof(T)];
		storage<N + 1, Ts...> rest;
	};
	void del(int n) {
		if(n == N) reinterpret_cast<T*>(first)->~T();
		else rest.del(n);
	}
};
template<int N>
struct storage<N> {
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

template<typename T, typename... Ts>
struct type_info<T&, Ts...> {
	static const bool no_reference_types = false;
	static const bool no_duplicates = position<T, Ts...>::pos == -1 && type_info<Ts...>::no_duplicates;
	static const size_t size = type_info<Ts...>::size > sizeof(T&) ? type_info<Ts...>::size : sizeof(T&);
};
template<typename T, typename... Ts>
struct type_info<T, Ts...> {
	static const bool no_reference_types = type_info<Ts...>::no_reference_types;
	static const bool no_duplicates = position<T, Ts...>::pos == -1 && type_info<Ts...>::no_duplicates;
	static const size_t size = type_info<Ts...>::size > sizeof(T) ? type_info<Ts...>::size : sizeof(T&);
};
template<>
struct type_info<> {
	static const bool no_reference_types = true;
	static const bool no_duplicates = true;
	static const size_t size = 0;
};

} // namespace impl

template<typename... Types>
class Variant {
	static_assert(impl::type_info<Types...>::no_reference_types, "Reference types are not permitted in variant.");
	static_assert(impl::type_info<Types...>::no_duplicates, "duplicates in types");
	
	int t;
	impl::storage<0, Types...> s;
	
	Variant() = delete;
	
	template<typename X>
	void init(const X& x) {
		new(&s) X(x);
	}
	
public:
	template<typename X>
	Variant(const X& v) : t(impl::position<X, Types...>::pos) {
		static_assert(impl::position<X, Types...>::pos != -1, "not in variant");
		init(v);
	}
	template<typename X>
	void operator=(const X& v) {
		static_assert(impl::position<X, Types...>::pos != -1, "not in variant");
		s.del(t);
		t = impl::position<X, Types...>::pos;
		init(v);
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

