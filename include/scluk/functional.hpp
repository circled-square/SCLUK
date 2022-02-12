#ifndef SCLUK_FUNCTIONAL_HPP
#define SCLUK_FUNCTIONAL_HPP

#include <cassert>
#include <type_traits>
#include "language_extension.hpp"
#include "metaprogramming.hpp"

namespace scluk {
    template<typename signature_t> using fnptr_t = typename meta::signature_helper<signature_t>::fptr_type;

    //takes ownership of a callable object (moving it) and returns a function pointer calling it with minimum overhead.
    //if the type used is not unique (std::function as opposed to lambdas) a unique id needs to be used for each callable with the same type
    //uses assert to check if the id is reused with the same type
    template<typename signature_t, class lambda_t, u64 id = 0>
    static fnptr_t<signature_t> lambda_to_fnptr(lambda_t&& fn) {
        alignas(lambda_t)
        static char lambda_buf[sizeof(lambda_t)] = {};
        static bool is_empty = true;

        assert(is_empty && "every lambda with the same type converted with lambda_to_ptr needs a different id");

        fnptr_t<signature_t> func = [](auto...args) {
            return reinterpret_cast<lambda_t&>(lambda_buf)(args...); 
        };

        //copy the lambda to get ownership
        new (lambda_buf) lambda_t(std::move(fn));
        is_empty = false;

        return func;
    }

    auto compose_fns(const auto& f, const auto& g) {
        return [=](auto...args){ return f(g(args...)); };
    }

    template<typename T>
    T identity(T a) { return a; }
}

#endif
