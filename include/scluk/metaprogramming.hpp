#ifndef SCLUK_METAPROGRAMMING_HPP
#define SCLUK_METAPROGRAMMING_HPP

#include <concepts>
#include <iterator>
#include <cstdint>
#include <type_traits>

namespace scluk::meta {
    template<bool c, template<class> class templ_t, class T>
    using apply_if = std::conditional_t<c, templ_t<T>, T>;

    template<bool c, template<class> class templ_a, template<class> class templ_b, class T>
    using apply_if_else = std::conditional_t<c, templ_a<T>, templ_b<T>>;

    template<typename T, typename U>
    using biggest_t = std::conditional_t<sizeof(T) >= sizeof(U), T, U>;

    template<typename T, typename...Ts>
    struct signature_helper;

    template<typename T, typename...Ts>
    struct signature_helper<T(Ts...)> {
        using fn_type = T(Ts...);
        using fptr_type = T(*)(Ts...);
        using ret_type = T;
    };

    template<typename T, class class_t, typename...Ts>
    struct signature_helper<T(class_t::*)(Ts...)> {
        using fn_type = T(Ts...);
        using fptr_type = T(*)(Ts...);
        using ret_type = T;
        using class_type = class_t;
        using method_type = T(class_t::*)(Ts...);
        using const_method_type = T(class_t::*)(Ts...)const;
    };
    
    template<typename T, class class_t, typename...Ts>
    struct signature_helper<T(class_t::*)(Ts...)const> {
        using fn_type = T(Ts...);
        using fptr_type = T(*)(Ts...);
        using ret_type = T;
        using class_type = class_t;
        using method_type = T(class_t::*)(Ts...);
        using const_method_type = T(class_t::*)(Ts...)const;
    };    
}

namespace scluk::concepts {
    template<typename T>
    concept iterable = requires(T a) { 
    	{ std::begin(a), std::end(a) }; 
    };

    template <typename T>
    concept indexable = requires(T a) { 
        { a.size() } -> std::convertible_to<std::size_t>;
        { a[0] };
        { std::declval<typename T::value_type>() };
    };
    
    template<typename T>
    concept ratio = requires(T) {
        { T::num } -> std::convertible_to<std::intmax_t>;
        { T::den } -> std::convertible_to<std::intmax_t>;
    };
}

#endif
