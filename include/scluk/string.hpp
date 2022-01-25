#ifndef SCLUK_STRING_HPP
#define SCLUK_STRING_HPP

#include <vector>
#include <string>
#include <concepts>
#include "range.hpp"
#include "format.hpp"

namespace scluk {
    inline namespace string {
        using namespace std::string_literals;
        template<typename c>
        using str = std::basic_string<c>;
        template<typename c>
        using str_view = std::basic_string_view<c>;
        using std::size_t;


        template<std::integral char_t>
        str<char_t> repeat(char_t c, size_t n) { return str<char_t>(n, c); }

        template<std::integral char_t>
        str<char_t> repeat(const str<char_t>& s, size_t n) {
            const size_t sz = s.length();
            str<char_t> ret(sz*n, char_t('a'));
            for(int i : range(n)) 
                std::copy(s.begin(), s.end(), ret.begin()+i*sz);
            
            return ret;
        }

        template<std::integral char_t>
        str<char_t> operator*(const str<char_t>& s, size_t n) {
            return repeat(s, n);
        }

        template<std::integral char_t>
        int levenshtein_dist(str_view<char_t> a, str_view<char_t> b) {
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

        template<std::integral char_t>
        std::vector<size_t> get_all_positions_of(str_view<char_t> s, char_t c) {
            std::vector<size_t> positions;
            for(size_t i : index(s))
                if(s[i] == c) positions.push_back(i);
            
            return positions;
        }
    }
}

#endif //SCLUK_STRING_HPP
