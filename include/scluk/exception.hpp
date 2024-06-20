#ifndef SCLUK_EXCEPTION_HPP
#define SCLUK_EXCEPTION_HPP

#include <stdexcept>
#include <format>

namespace scluk {
    //if need be define other functions for other exceptions or transform this into a macro to be used for any exception
    template<typename... Ts>
    std::runtime_error runtime_error(std::format_string<const Ts&...> fmt, const Ts&...args) {
        return std::runtime_error(std::format(fmt, args...));
    }

    void throw_exception(const std::exception& e);
}

#endif //SCLUK_EXCEPTION_HPP
