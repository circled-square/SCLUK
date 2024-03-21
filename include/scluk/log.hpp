#ifndef LOG_HPP
#define LOG_HPP

#include <format>

namespace scluk {

    /*
     * //for example to create a log in append mode:
     * std::ofstream ofs("log/file/path", std::ios::app);
     * scluk::log log(ofs);
     *
     * this is obviously not thread safe.
     */
    enum class log_level : int {
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

        void operator()(log_level l, std::string_view fmt, std::format_args args);

        //shorthand for .trace()
        inline void operator()(std::string_view s, auto...args) { this->trace(s, args...); }

        inline void trace(std::string_view s, auto...args) { operator()(log_level::TRACE, s, std::make_format_args(args...)); }
        inline void debug(std::string_view s, auto...args) { operator()(log_level::DEBUG, s, std::make_format_args(args...)); }
        inline void info (std::string_view s, auto...args) { operator()(log_level::INFO,  s, std::make_format_args(args...)); }
        inline void warn (std::string_view s, auto...args) { operator()(log_level::WARN,  s, std::make_format_args(args...)); }
        inline void error(std::string_view s, auto...args) { operator()(log_level::ERROR, s, std::make_format_args(args...)); }
        inline void fatal(std::string_view s, auto...args) { operator()(log_level::FATAL, s, std::make_format_args(args...)); }

        ~log();
    };

    extern log stdout_log;
}


#endif // LOG_HPP
