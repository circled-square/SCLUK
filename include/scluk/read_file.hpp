#ifndef SCLUK_READFILE_HPP
#define SCLUK_READFILE_HPP

#include <fstream>
#include <vector>
#include <format>

namespace scluk {
    inline std::string read_file(const char* fname) {
        std::ifstream fi(fname);
        if (fi.fail())
            throw std::runtime_error(std::format("scluk::read_file could not read file at path '{}'", fname));
        std::string contents((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        return contents;
    }
	inline std::string read_file(const std::string& fname) { return read_file(fname.c_str()); }

    inline std::vector<unsigned char> read_binary_file(const char* fname) {
        using uchar = unsigned char;
        std::basic_ifstream<uchar> fi(fname, std::ios_base::binary);
        std::vector<uchar> contents((std::istreambuf_iterator<uchar>(fi)), std::istreambuf_iterator<uchar>());
        return contents;
    }
    inline std::vector<unsigned char> read_binary_file(const std::string& fname) { return read_binary_file(fname.c_str()); }
}
#endif
