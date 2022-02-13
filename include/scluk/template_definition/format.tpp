#include <sstream>
#include <cstring>
#include <iostream>

#ifndef SCLUK_NO_FMT_POUT
#include <mutex>
#endif

namespace scluk {
    inline namespace fmt {
        template<typename...Ts> inline
        void fmt_to_stream(std::ostream& os, std::string_view fmt, Ts...args) {
            auto it = fmt.begin(), end = fmt.end();
            [[maybe_unused]] auto f = [&os, &it, &end] (auto arg) -> void {
                for(; it != end; it++) {
                    auto next = it; next++;
                    if(*it == '\\' && (*next == '%' || *next == '\\'))
                        it++;
                    else if(*it == '%') {
                        os << arg;
                        it++;
                        return;
                    }
                    os << *it;
                }
            };
            (f(args), ...);
            for(;it != end; it++) os << *it;
        }

        template<typename...Ts> inline
        void out_no_ln(std::string_view fmt, Ts...args) {
            fmt_to_stream(std::cout, fmt, args...);
        }

        template<typename...Ts> inline
        void out(std::string_view fmt, Ts...args) {
            out_no_ln(fmt, args...);
            std::cout << std::endl;
        }
         inline
        void out() { std::cout << std::endl; }

#ifndef SCLUK_NO_FMT_POUT
        //mutex regulated output to stdout
        static std::mutex stdout_mutex;

        template<typename...Ts> inline
        void pout(std::string_view fmt, Ts...args) {
            std::lock_guard<std::mutex> lock(stdout_mutex);
            out(fmt, args...);
        }
         inline
        void pout() {
            std::lock_guard<std::mutex> lock(stdout_mutex);
            out();
        }
        template<typename...Ts> inline
        void pout_no_ln(std::string_view fmt, Ts...args) {
            std::lock_guard<std::mutex> lock(stdout_mutex);
            out_no_ln(fmt, args...);
        }
#endif //SCLUK_NO_FMT_POUT

        template<typename...Ts> inline
        std::string sout(std::string_view fmt, Ts...args) {
            std::stringstream ss;
            fmt_to_stream(ss, fmt, args...);
            return ss.str();
        }
    }
}