#ifndef SCLUK_RESULT_HPP
#define SCLUK_RESULT_HPP

#include <variant>
#include <optional>
#include <string>
#include <stdexcept>

namespace scluk {
    template<class ret_t, class warning_t = std::string>
    class result {
    public:
        using exception_ptr_t = std::unique_ptr<std::exception>;
        using variant_t = std::variant<ret_t, exception_ptr_t>;
        variant_t res_or_except;
        std::optional<warning_t> warning;

        result(result&& o) : res_or_except(std::move(o.res_or_except)), warning(std::move(o.warning)) {}
        result(ret_t res, std::optional<warning_t> warning = {}) : res_or_except(res), warning(warning) {}
        result(const std::exception& e) : res_or_except(std::make_unique<std::exception>(std::move(e))) {}
        template<std::derived_from<std::exception> except_t>
        result(const except_t& e) : res_or_except(std::make_unique<std::exception>(std::move(e))) {}
        result(exception_ptr_t&& e) : res_or_except(std::move(e)) {}

        result set(ret_t v) { res_or_except = v; return *this; }
        result warn(warning_t w) { warning = w; }
        void fail(std::exception e) { res_or_except = e; }

        //throw exceptions, ignore warnings
        ret_t unwrap() {
            return unwrap(scluk::throw_exception);
        }

        //throw exceptions, cb for warnings
        template<scluk::concepts::func<void(warning_t)> warning_cb_t>
        ret_t unwrap(warning_cb_t warning_cb) {
            return unwrap(scluk::throw_exception, warning_cb);
        }
        
        //cb for exceptions, ignore warnings
        template<scluk::concepts::func<ret_t(std::exception)> except_cb_t>
        ret_t unwrap(except_cb_t except_cb) {
            if(std::holds_alternative<ret_t>(res_or_except))
                return std::move(std::get<ret_t>(res_or_except));
            else return except_cb(std::move(std::get<exception_ptr_t>(res_or_except)));
        }

        //cb for exceptions and for warnings
        template<
            scluk::concepts::func<ret_t(std::exception)> except_cb_t, 
            scluk::concepts::func<void(warning_t)> warning_cb_t>
        ret_t unwrap(except_cb_t except_cb, warning_cb_t warning_cb) {
            if(std::holds_alternative<ret_t>(res_or_except)) {
                if(warning) warning_cb(*warning);
                return std::move(std::get<ret_t>(res_or_except));
            } else return except_cb(std::move(std::get<exception_ptr_t>(res_or_except)));
        }
        
        //rethrowing cb for exception, ignore warnings
        template<scluk::concepts::func<void(std::exception)> except_cb_t>
        ret_t unwrap(except_cb_t except_cb) {
            if(std::holds_alternative<ret_t>(res_or_except))
                return std::get<ret_t>(res_or_except);
            else {
                except_cb(std::move(*std::get<exception_ptr_t>(res_or_except)));
                throw std::runtime_error("func except_cb passed to unwrap does neither return the correct type nor throw");
            }
        }

        //rethrowing cb for exception, cb for warnings
        template<
            scluk::concepts::func<void(std::exception)> except_cb_t, 
            scluk::concepts::func<void(warning_t)> warning_cb_t>
        ret_t unwrap(except_cb_t exception_cb, warning_cb_t warning_cb) {
            if(std::holds_alternative<ret_t>(res_or_except)) {
                if(warning) warning_cb(*warning);
                return std::get<ret_t>(res_or_except);
            } else {
                exception_cb(std::move(*std::get<exception_ptr_t>(res_or_except)));
                throw std::runtime_error("func except_cb passed to unwrap does neither return the correct type nor throw");
            }
        }
    };
}

#endif