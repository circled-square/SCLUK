#ifndef SCLUK_STRING_HPP
#define SCLUK_STRING_HPP

#include <string>
#include <concepts>
#include "range.hpp"
#include "format.hpp"
namespace scluk {
    inline namespace str {
        using namespace std::string_literals;

        template<std::integral char_t>
        static std::basic_string<char_t> repeat(char_t c, std::size_t n) { return std::basic_string(n, c); }

        template<std::integral char_t>
        static std::basic_string<char_t> repeat(const std::basic_string<char_t>& s, std::size_t n) {
            const std::size_t sz = s.length();
            std::string ret(sz*n, 'a');
            for(int i : range(n)) 
                std::copy(s.begin(), s.end(), ret.begin()+i*sz);
            
            return ret;
        }

        template<std::integral char_t>
        static std::basic_string<char_t> operator*(const std::basic_string<char_t>& s, std::size_t n) {
            return repeat(s, n);
        }
    }
}

#endif //SCLUK_STRING_HPP
