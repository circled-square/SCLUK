#include "../include/scluk/exception.hpp"
namespace scluk {
    void throw_exception(const std::exception& e) { throw e; }
}
