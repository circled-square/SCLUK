#include <sstream>
#include <cstring>
#include <mutex>
#include <iostream>

namespace scluk {
    inline namespace fmt {
        template<typename...Ts> inline
        void fmt_to_stream(std::ostream& os, const char* fmt, Ts...args) {
            [[maybe_unused]] auto f = [&os, &fmt] (auto arg) -> void { 
                for(; *fmt; fmt++) {
                    if(*fmt == '\\' && (fmt[1] == '%' || fmt[1] == '\\'))
                        fmt++;
                    else if(*fmt == '%') {
                        os << arg;
                        fmt++;
                        return;
                    }
                    os << *fmt;
                }
            };
            (f(args), ...);
            os << fmt;
        }

        template<typename...Ts> inline
        void out_no_ln(const char* fmt, Ts...args) {
            fmt_to_stream(std::cout, fmt, args...);
        }

        template<typename...Ts> inline
        void out(const char* fmt, Ts...args) {
            out_no_ln(fmt, args...);
            std::cout << std::endl;
        }
         inline
        void out() { std::cout << std::endl; }

        //mutex regulated output to stdout
        static std::mutex stdout_mutex;

        template<typename...Ts> inline
        void pout(const char* fmt, Ts...args) {
            std::lock_guard<std::mutex> lock(stdout_mutex);
            out(fmt, args...);
        }
         inline
        void pout() {
            std::lock_guard<std::mutex> lock(stdout_mutex);
            out();
        }
        template<typename...Ts> inline
        void pout_no_ln(const char* fmt, Ts...args) {
            std::lock_guard<std::mutex> lock(stdout_mutex);
            out_no_ln(fmt, args...);
        }

        template<typename...Ts> inline
        std::string sout(const char* fmt, Ts...args) {
            std::stringstream ss;
            fmt_to_stream(ss, fmt, args...);
            return ss.str();
        }
    }
}