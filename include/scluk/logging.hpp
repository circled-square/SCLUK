#ifndef LOG_AND_ERRORS_HPP
#define LOG_AND_ERRORS_HPP

#include <fstream>

#include "format.hpp"

namespace scluk {
    class log_file {
        std::string timestamp;
        const char* const file_name;
        const std::ios_base::openmode mode;
    public:
        log_file(const char* filename, std::ios_base::openmode openmode = std::ios::app);
    
        void operator()(const char* fmt, auto...x) {
            std::ofstream(file_name, mode) << timestamp << scluk::sout(fmt, x...) << std::endl;
        }
    };
}

#endif
