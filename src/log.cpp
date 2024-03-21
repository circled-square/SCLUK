#include "../include/scluk/log.hpp"
#include <iostream>


namespace scluk {
    log stdout_log(std::cout, log_level::WARN); //match the extern declaration in the header

    static std::string get_timestamp_string() {
        std::time_t t = std::time(nullptr);
        std::tm& p = *std::localtime(&t);
        return std::format(" {}/{}/{} {}:{}:{}", p.tm_mday, p.tm_mon+1, p.tm_year%100, p.tm_hour, p.tm_min, p.tm_sec);
    }

    static const char* log_level_names[] = {
        "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE"
    };

    log::log(std::ostream& stream, log_level l, bool timestamp)
        : m_stream(stream), 
          m_last_line_hash(0), 
          m_repeated_line_count(0),
          m_timestamp(timestamp ? get_timestamp_string() : std::string())
    {
        set_log_level(l);
    }

    void log::set_log_level(log_level l) {
        m_log_level = l;
        info("log level set to {}", log_level_names[int(l)]);
    }

    void log::operator()(log_level l, std::string_view fmt, std::format_args args) {
        if (l <= m_log_level) {
            //only perform formatting if necessary
            std::string line = std::vformat(fmt, args);

            //simply ignore the possibility of hash collision
            std::size_t line_hash = std::hash<std::string>{}(line) + (std::size_t)l;

            if (line_hash == m_last_line_hash && m_repeated_line_count != 0) {
                //repeated the last line
                m_repeated_line_count++;
            } else {
                //a new, different line is being printed

                //print the count of how many times the last line was printed and terminate it
                if (m_repeated_line_count > 1)
                    m_stream << " (repeated x" << m_repeated_line_count << ")";
                m_stream << std::endl;

                //print the new line without the \n
                m_stream << "[" << log_level_names[int(l)] << m_timestamp << "] " << line << std::flush;
                m_repeated_line_count = 1;
                m_last_line_hash = line_hash;
            }
        }
    }

    log::~log() {
        //print the count of how many times it was printed and terminate the last line
        if(m_repeated_line_count > 1)
            m_stream << "(repeated x" << m_repeated_line_count << ")";
        m_stream << std::endl;
    }
}
