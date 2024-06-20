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
    struct with_warning {
        ret_t r; 
        std::optional<warning_t> w;

        with_warning(ret_t r, std::optional<warning_t> w = {}) : r(r), w(w) {}

        [[nodiscard]]
        ret_t&& unwrap() { return std::move(r); }

        [[nodiscard]]
        ret_t&& unwrap(concepts::fn<void(warning_t)> auto warning_cb) {
            if(w) warning_cb(*w);
            return std::move(r);
        }

        [[nodiscard]]
        ret_t&& unwrap(std::optional<warning_t>& warning) {
            warning = std::move(this->w);
            return std::move(r);
        }

        std::tuple<ret_t, std::optional<warning_t>> to_tuple(){ return { std::move(r), std::move(w) }; }
    };

    // at any point the result has either state 'ok' (contains a value of type T) or state 'err' (contains an exception)
    // call is_ok/is_err to check the state
    // call set_ok/set_err to set the state
    // call unwrap/unwrap_err to obtain ownership of the ok/err value
    // call inspect/inspect_err to obtain a reference to the ok/err value
    template<typename T>
    class result {
    public:
        using value_type = T;
    private:
        using exception_ptr_t = std::unique_ptr<std::exception>;
        using variant_t = std::variant<exception_ptr_t, value_type>;
        variant_t m_contents;

        exception_ptr_t& get_exception_ptr(){ return std::get<exception_ptr_t>(m_contents); }
    public:
        // sets the state to 'ok'
        result(value_type r) : m_contents(std::move(r)) {}
        // sets the state to 'err'
        template<std::derived_from<std::exception> exception_t>
        result(const exception_t& e) : m_contents(new exception_t(std::move(e))) {}
        result(std::unique_ptr<std::exception> e) : m_contents(std::move(e)) {}
        // mimics the moved object's state
        result(result&& o) : m_contents(std::move(o.m_contents)) {}


        // returns true if the state is 'err'
        [[nodiscard]]
        bool is_err() { return std::holds_alternative<exception_ptr_t>(m_contents); }
        // returns true if the state is 'ok'
        [[nodiscard]]
        bool is_ok(){ return std::holds_alternative<value_type>(m_contents); }

        
        /* sets the state to 'ok' */
        void set_ok(value_type v) { m_contents = std::move(v); }
        /* sets the state to 'err' */
        void set_err(std::unique_ptr<std::exception> e) { m_contents = std::move(e); }

        /* assumes state is 'err' */
        [[nodiscard]]
        std::exception& inspect_err() { return *get_exception_ptr(); }
        /* assumes state is 'ok' */
        [[nodiscard]]
        value_type& inspect() { return std::get<value_type>(m_contents); }

        /*
         *  unwrap will return the contained value, coping with any errors the way the user specifies.
         */
        //throw exceptions
        [[nodiscard]]
        value_type&& unwrap() { return unwrap(throw_exception); }
        
        //cb for exceptions
        [[nodiscard]]
        value_type&& unwrap(concepts::fn<value_type(std::exception&)> auto except_cb) {
            if(is_err()) return except_cb(inspect_err());
            return std::move(inspect());
        }


        //cb for exception, rethrows them anyway afterwards if the cb itself does not throw
        [[nodiscard]]
        value_type&& unwrap(concepts::fn<void(std::exception&)> auto except_cb) {
            if(is_err()) {
                except_cb(inspect_err());
                throw unwrap_err();
            }
            return std::move(inspect());
        }

        //unwrap_err moves the error out of the result, throws if the state is 'ok'
        [[nodiscard]]
        std::unique_ptr<std::exception>&& unwrap_err() {
            if(is_ok()) {
                throw std::runtime_error("unwrap_err called on a result with state 'ok'");
            }
            return std::move(get_exception_ptr());
        }
    };
}

#endif