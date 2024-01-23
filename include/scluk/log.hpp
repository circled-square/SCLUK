#ifndef LOG_HPP
#define LOG_HPP

#include "format.hpp"

namespace scluk {

    /*
     * //for example to create a log in append mode:
     * std::ofstream ofs("log/file/path", std::ios::app);
     * scluk::log log(ofs);
     */
    class log {
        std::ostream& m_stream;
        std::string m_timestamp;
        std::size_t m_last_line_hash;
        std::size_t m_repeated_line_count;

    public:
        log(std::ostream&, bool timestamp = false);

        void operator()(const std::string&);
        inline void operator()(const char* fmt, auto... args) {
            std::string line = sout(fmt, args...);
            this->operator()(line);
        }

        ~log();
    };

    extern log stdout_log;
}


#endif // LOG_HPP
