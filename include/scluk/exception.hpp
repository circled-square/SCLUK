#ifndef SCLUK_EXCEPTION_HPP
#define SCLUK_EXCEPTION_HPP

#include <stdexcept>
#include "format.hpp"

namespace scluk {
    //if need be, transform this into a macro to be used for any exception
    template<typename...Ts>
    std::runtime_error runtime_error(const char* fmt, Ts...args) {
        return std::runtime_error(sout(fmt, args...));
    }
}

#endif //SCLUK_EXCEPTION_HPP