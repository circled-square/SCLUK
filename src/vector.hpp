#ifndef SCLUK_VECTOR_HPP
#define SCLUK_VECTOR_HPP

namespace scluk {
    template<class vector_t, typename...Ts>
    void populate_from_pack(vector_t& vec, Ts&...pack);
}
#include "template_definition/vector.tpp"

#endif
