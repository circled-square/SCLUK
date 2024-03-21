#ifndef LOG_HPP
#define LOG_HPP

#include "format.hpp"

namespace scluk {

    /*
     * //for example to create a log in append mode:
     * std::ofstream ofs("log/file/path", std::ios::app);
     * scluk::log log(ofs);
     */
    enum class log_level : int8_t {
        OFF = -1,
        FATAL = 0,
        ERROR = 1,
        WARN = 2,
        INFO = 3,
        DEBUG = 4,
        TRACE = 5,
    };

    class log {
        std::ostream& m_stream;
        std::string m_timestamp;
        std::size_t m_last_line_hash;
        std::size_t m_repeated_line_count;
        log_level m_log_level;
    public:
        log(std::ostream&, log_level, bool timestamp = false);
        void set_log_level(log_level l);

        void operator()(log_level l, const std::string& line);

        //shorthand for .trace()
        inline void operator()(const char* fmt, auto... args) { this->trace(fmt, args...); }

        inline void trace(const char* fmt, auto... args) { this->operator()(log_level::TRACE, scluk::sout(fmt, args...)); }
        inline void debug(const char* fmt, auto... args) { this->operator()(log_level::DEBUG, scluk::sout(fmt, args...)); }
        inline void info (const char* fmt, auto... args) { this->operator()(log_level::INFO,  scluk::sout(fmt, args...)); }
        inline void warn (const char* fmt, auto... args) { this->operator()(log_level::WARN,  scluk::sout(fmt, args...)); }
        inline void error(const char* fmt, auto... args) { this->operator()(log_level::ERROR, scluk::sout(fmt, args...)); }
        inline void fatal(const char* fmt, auto... args) { this->operator()(log_level::FATAL, scluk::sout(fmt, args...)); }

        ~log();
    };

    extern log stdout_log;
}


#endif // LOG_HPP
