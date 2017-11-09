//#pragma once
//
//#define REM(...) __VA_ARGS__
//#define EAT(...)
//
//Retrieve the Type
//#define TYPEOF(x) DETAIL_TYPEOF(DETAIL_TYPEOF_PROBE x,)
//#define DETAIL_TYPEOF(...) DETAIL_TYPEOF_HEAD(__VA_ARGS__)
//#define DETAIL_TYPEOF_HEAD(x, ...) REM x
//#define DETAIL_TYPEOF_PROBE(...) (__VA__ARGS__)
//Strip off the type
//#define STRIP(x) EAT x
//Show the type without parenthises
//#define PAIR(x) REM x
//
//
//------------------------------------------------------
//template<class M, class T>
//struct make_const
//{
//	typedef T type;
//};
//
//template<class M, class T>
//struct make_const<const M, T>
//{
//	typedef typename boost::add_const<T>::type type
//};
//
//#define REFLECTABLE(...) \
//static const int fields_n = BOOST_PP_VARIDIC_SIZE(__VA_ARGS__) \
//friend struct reflector; \
//template <int N, class Self> \
//struct field_data{}; \
//BOOST_PP_SEQ_FOR_EACH_I(REFLECT_EACH, data, BOOST_PP_VARIDIC_TO_SEQ(__VA_ARGS__))
//
//#define REFLECT_EACH(r, data, i, x) \
//PAIR(x); \
//template<class Self> \
//struct field_data<i, Self> \
//{ \
//	Self &self \
//	field_data(Self& self) : self(self){} \
//    \
//    typename make_const<Self, TYPEOF(x)>::type & get() \
//    { \
//        return self.STRIP(x); \
//    }\
//    typename boost::add_const<TYPEOF(x)>::type & get() const \
//    { \
//        return self.STRIP(x); \
//    }\
//    const char * name() const \
//    {\
//        return BOOST_PP_STRINGIZE(STRIP(x)); \
//    } \
//};
