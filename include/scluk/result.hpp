#ifndef SCLUK_RESULT_HPP
#define SCLUK_RESULT_HPP

#include <variant>
#include <optional>
#include <string>
#include <memory>
#include "exception.hpp"
#include "metaprogramming.hpp"

namespace scluk {
    template<std::default_initializable ret_t, class warning_t = std::string> requires(!std::derived_from<warning_t, std::exception>)
    struct successful_result {
        ret_t r; 
        std::optional<warning_t> w;

        successful_result(ret_t r, std::optional<warning_t> w = {}) : r(r), w(w) {}

        ret_t unwrap() { return r; }

        template<concepts::fn<void(warning_t)> warning_cb_t>
        ret_t unwrap(warning_cb_t warning_cb) {
            if(w) warning_cb(*w);
            return r;
        }

        ret_t unwrap(std::optional<warning_t>& w) {
            w = this->w;
            return r;
        }

        std::tuple<ret_t, std::optional<warning_t>> to_tuple(){ return {r,w}; }
    };

    template<std::default_initializable ret_t, class warning_t = std::string> requires(!std::derived_from<warning_t, std::exception>)
    class result {
        using exception_ptr_t = std::unique_ptr<std::exception>;
        using success = successful_result<ret_t, warning_t>;
        using variant_t = std::variant<exception_ptr_t, success>;
        variant_t m_contents;

        exception_ptr_t& get_except_ptr(){ return std::get<exception_ptr_t>(m_contents); }
        success& get_success_struct() { return std::get<success>(m_contents); }
    public:
        /* assumes state is 'exception' */
        std::exception& get_except() { return *get_except_ptr(); }
        /* assumes state is 'success' */
        ret_t& get_result(){ return get_success_struct().r; }
        std::optional<warning_t>& get_opt_warning(){ return get_success_struct().w; }
        warning_t& get_warning(){ return *get_success_struct().w; }

        bool has_except(){ return std::holds_alternative<exception_ptr_t>(m_contents); }
        bool has_result(){ return std::holds_alternative<success>(m_contents); }
        bool has_warning(){ return has_result() && std::get<success>(m_contents).w; }

        /* sets the state to 'success' */
        result() : m_contents(success{ret_t(),{}}) {}
        result(ret_t r) : m_contents(success{std::move(r), {}}) {}
        result(ret_t r, warning_t w) : m_contents(success{std::move(r), std::move(w)}) {}
        result(ret_t r, std::optional<warning_t> w) : m_contents(success{std::move(r), std::move(w)}) {}
        /* sets the state to 'exception' */
        template<std::derived_from<std::exception> except_t>
        result(const except_t& e) : m_contents(std::make_unique<std::exception>(std::move(e))) {}
        result(exception_ptr_t&& e) : m_contents(std::move(e)) {}
        /* mimics the moved object's state*/
        result(result&& o) : m_contents(std::move(o.m_contents)) {}
        
        /* sets the state to 'success' */
        result set(ret_t v, std::optional<warning_t> w) {
            m_contents = {v, w};
            return *this; 
        }
        /* assumes state is 'success' */
        result warn(warning_t w) { get_warning() = w; return *this; }
        /* sets the state to 'exception' */
        void fail(std::exception e) { m_contents = std::make_unique<std::exception>(std::move(e)); }
        void fail(std::exception_ptr e) { m_contents = std::move(e); }

        /*
         *  unwrap will return the contained value, coping with any errors or warnings the way the user specifies. 
         *  assumes state isn't 'nothing'.
         */
        //throw exceptions, ignore warnings
        ret_t unwrap() { return unwrap(throw_exception); }

        //throw exceptions, cb for warnings
        template<concepts::fn<void(warning_t)> warning_cb_t>
        ret_t unwrap(warning_cb_t warning_cb) { return unwrap(throw_exception, warning_cb); }
        
        //cb for exceptions, ignore warnings
        template<concepts::fn<ret_t(std::exception)> except_cb_t>
        ret_t unwrap(except_cb_t except_cb) {
            if(has_except()) return except_cb(get_except());
            return std::move(get_result());
        }

        //cb for exceptions and for warnings
        template<
            concepts::fn<ret_t(std::exception)> except_cb_t, 
            concepts::fn<void(warning_t)> warning_cb_t> 
        ret_t unwrap(except_cb_t except_cb, warning_cb_t warning_cb) {
            if(has_except()) return except_cb(get_except());
            if(has_warning()) warning_cb(get_warning());
            return std::move(get_result());
        }
        
        //rethrowing cb for exception, ignore warnings
        template<concepts::fn<void(std::exception)> except_cb_t>
        ret_t unwrap(except_cb_t except_cb) {
            if(has_except()) {
                except_cb(get_except());
                throw get_except();
            }
            return std::move(get_result());
        }

        //rethrowing cb for exception, cb for warnings
        template<
            concepts::fn<void(std::exception)> except_cb_t, 
            concepts::fn<void(warning_t)> warning_cb_t>
        ret_t unwrap(except_cb_t except_cb, warning_cb_t warning_cb) {
            if(has_except()) {
                except_cb(get_except());
                throw get_except();
            }
            if(has_warning()) warning_cb(get_warning());
            return std::move(get_result());
        }
    };
}

#endif