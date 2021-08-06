#ifndef SCLUK_METAPROGRAMMING_HPP
#define SCLUK_METAPROGRAMMING_HPP

#include <concepts>
#include <iterator>
#include <cstdint>

namespace scluk::meta {
    template <template<typename> typename unary_type_template, typename T, bool condition>
    struct apply_template_if_true_struct {
        using type = T;
    };
    template <template<typename> typename unary_type_template, typename T>
    struct apply_template_if_true_struct<unary_type_template, T, true>{
        using type = unary_type_template<T>;
    };
    template <template<typename> typename unary_type_template, typename T, bool condition>
    using apply_template_if_true = typename apply_template_if_true_struct<unary_type_template, T, condition>::type;

    template <template<typename> typename unary_type_template_a, template<typename> typename unary_type_template_b, typename T, bool condition>
    struct apply_template_if_true_else_apply_other_template_struct {
        using type = unary_type_template_b<T>;
    };
    template <template<typename> typename unary_type_template_a, template<typename> typename unary_type_template_b, typename T>
    struct apply_template_if_true_else_apply_other_template_struct<unary_type_template_a, unary_type_template_b, T, true> {
        using type = unary_type_template_a<T>;
    };
    template <template<typename> typename unary_type_template_a, template<typename> typename unary_type_template_b, typename T, bool condition>
    using apply_template_if_true_else_apply_other_template = 
        typename apply_template_if_true_else_apply_other_template_struct<unary_type_template_a, unary_type_template_b, T, condition>::type;

    template<typename T, typename...Ts>
    struct signature_helper;

    template<typename T, typename...Ts>
    struct signature_helper<T(Ts...)> {
        using fun_type = T(Ts...);
        using fptr_type = T(*)(Ts...);
        using ret_type = T;
    };

    template<typename T, class class_t, typename...Ts>
    struct signature_helper<T(class_t::*)(Ts...)> {
        using fun_type = T(Ts...);
        using fptr_type = T(*)(Ts...);
        using ret_type = T;
        using class_type = class_t;
        using method_type = T(class_t::*)(Ts...);
        using const_method_type = T(class_t::*)(Ts...)const;
    };
    
    template<typename T, class class_t, typename...Ts>
    struct signature_helper<T(class_t::*)(Ts...)const> {
        using fun_type = T(Ts...);
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
