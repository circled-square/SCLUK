#include "../log.hpp"


namespace scluk {
    log stdout_log(std::cout); //match the extern declaration in the header

    static std::string get_timestamp_string() {
        std::time_t t = std::time(nullptr);
        std::tm& p = *std::localtime(&t);
        return scluk::sout("[%/%/% %:%:%] ", p.tm_mday, p.tm_mon+1, p.tm_year%100, p.tm_hour, p.tm_min, p.tm_sec);
    }

    log::log(std::ostream& stream, bool timestamp) 
        : m_stream(stream), 
          m_last_line_hash(0), 
          m_repeated_line_count(0),
          m_timestamp(timestamp ? get_timestamp_string() : std::string())
    {}

    void log::print_line(std::string line) {
        std::size_t line_hash = std::hash<std::string>{}(line);

        if(line_hash == m_last_line_hash && m_repeated_line_count != 0) {
            m_repeated_line_count++;
            //repeated the last line
        } else {
            //a new, different line is being printed

            //print the count of how many times it was printed and terminate the line
            if(m_repeated_line_count > 1)
                m_stream << "(repeated x" << m_repeated_line_count << ")";
            m_stream << std::endl;


            m_repeated_line_count = 1;

            m_stream << m_timestamp << line << std::flush;
        }

    }

    log::~log() {
        //terminate the last line
        m_stream << std::endl;
    }

}
