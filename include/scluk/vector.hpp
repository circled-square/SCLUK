#ifndef SCLUK_VECTOR_HPP
#define SCLUK_VECTOR_HPP

namespace scluk {
    inline void populate_from_pack(auto& vec, const auto&... pack) {
        vec.reserve(vec.size() + sizeof...(pack));
        (vec.push_back(pack), ...);
    }
}

#endif
