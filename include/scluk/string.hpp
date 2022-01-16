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
        std::basic_string<char_t> repeat(char_t c, std::size_t n) { return std::basic_string(n, c); }

        template<std::integral char_t>
        std::basic_string<char_t> repeat(const std::basic_string<char_t>& s, std::size_t n) {
            const std::size_t sz = s.length();
            std::basic_string<char_t> ret(sz*n, char_t('a'));
            for(int i : range(n)) 
                std::copy(s.begin(), s.end(), ret.begin()+i*sz);
            
            return ret;
        }

        template<std::integral char_t>
        std::basic_string<char_t> operator*(const std::basic_string<char_t>& s, std::size_t n) {
            return repeat(s, n);
        }

        template<std::integral char_t>
        int levenshtein_dist(std::basic_string_view<char_t> a, std::basic_string_view<char_t> b) {
            //this is NOT a fast implementation of the levenshtein distance, only a correct one.
            //it's basically a translation into c++ of the definition from wikipedia.
            //this is for convenience, not speed.
            using namespace scluk;

            if (a.empty()) return b.size();
            if (b.empty()) return a.size();

            auto tail = [](auto sw) { return sw.substr(1); };

            if (a[0] == b[0]) return levenshtein_dist(tail(a), tail(b));

            return 1 + std::min({
                levenshtein_dist(tail(a), b),
                levenshtein_dist(a, tail(b)),
                levenshtein_dist(tail(a), tail(b)),
            });
        }
    }
}

#endif //SCLUK_STRING_HPP
