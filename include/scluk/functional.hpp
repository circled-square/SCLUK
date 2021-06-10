#ifndef SCLUK_FUNCTIONAL_HPP
#define SCLUK_FUNCTIONAL_HPP

#include <cassert>
#include <type_traits>
#include "language_extension.hpp"
#include "metaprogramming.hpp"

namespace scluk {
    template<typename signature_t> using fnptr_t = typename meta::signature_helper<signature_t>::fptr_type;

    // this isn't a singleton and it doesn't need to be a class anymore but conceptually it is a singleton
    // template so I won't rewrite it as a template function with static stuff
    template<class lambda_t, typename signature_t, u64 id = 0>
    class lambda_to_ptr_singleton {
        alignas(lambda_t)
        static char lambda_buf[sizeof(lambda_t)];
        static bool is_null;
        
        template<typename...Ts>
        static typename meta::signature_helper<signature_t>::ret_type func(Ts...args) {
            return reinterpret_cast<lambda_t&>(lambda_buf)(args...); 
        }
    public:
        lambda_to_ptr_singleton() = delete;

        static fnptr_t<signature_t> get(lambda_t&& fn) {
            assert(is_null && "every lambda with the same type converted with lambda_to_ptr needs a different id");
            //copy the lambda to get ownership
            new (lambda_buf) lambda_t(std::move(fn));
            return func;
        }
    };
    
    template<class lambda_t, typename signature_t, u64 id> alignas(lambda_t)
    char lambda_to_ptr_singleton<lambda_t, signature_t, id>::lambda_buf[sizeof(lambda_t)] = {};

    template<class lambda_t, typename signature_t, u64 id>
    bool lambda_to_ptr_singleton<lambda_t, signature_t, id>::is_null = true;

    //takes ownership of a callable object (moving it) and returns a function pointer calling it with minimum overhead.
    //if the type used is not unique (std::function as opposed to lambdas) a unique id needs to be used for each callable with the same type
    //uses assert to check if the id is reused with the same type
    template<typename signature_t, class lambda_t, u64 id = 0>
    fnptr_t<signature_t> lambda_to_fnptr(lambda_t&& lambda) {
        return lambda_to_ptr_singleton<lambda_t, signature_t, id>::get(std::move(lambda));
    }
}

#endif
