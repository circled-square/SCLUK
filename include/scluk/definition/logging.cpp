#include "../logging.hpp"
#include <ctime>
#include <ios>

scluk::log_file::log_file(const char* filename, std::ios_base::openmode openmode) 
        : file_name(filename), mode(openmode) {
    std::time_t t = std::time(nullptr);
    std::tm& p = *std::localtime(&t);
    timestamp = scluk::sout("[%/%/% %:%:%]", p.tm_mday, p.tm_mon+1, p.tm_year%100, p.tm_hour, p.tm_min, p.tm_sec);
}
