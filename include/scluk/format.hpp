#ifndef SCLUK_MODERN_PRINT_HPP
#define SCLUK_MODERN_PRINT_HPP

#include <string>

namespace scluk {
    //the declarations and definitions (but not specialisations) are static because the functions must not
    //be exported to other translation units that might also define them
    inline namespace fmt {
        template<typename...Ts> [[gnu::always_inline]] inline
        void fmt_to_stream(std::ostream&, const char*, Ts...);

        template <typename...Ts> inline 
        void out(const char*, Ts...);
        inline void out();
        template <typename...Ts> inline
        void out_no_ln(const char*, Ts...);

        template<typename...Ts> inline 
        void pout(const char*, Ts...);
        inline void pout();
        template<typename...Ts> inline
        void pout_no_ln(const char*, Ts...);

        template <typename...Ts> inline
        std::string sout(const char*, Ts...);
    }
}

#include "template_definition/modern_print.tpp"

#endif