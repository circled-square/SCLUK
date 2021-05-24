namespace scluk {
    inline namespace helpers { //helpers for populate_from_pack
        template<class vector_t, typename> [[gnu::always_inline]] inline
        void populate_from_pack_helper(vector_t&) { }
        template<class vector_t, typename T, typename...Ts> [[gnu::always_inline]] inline
        void populate_from_pack_helper(vector_t& vec, T& arg, Ts&...pack) {
            vec.push_back(arg);
            populate_from_pack_helper(vec, pack...);
        }
    }
    template<class vector_t, typename...Ts>
    void populate_from_pack(vector_t& vec, Ts&...pack) {
        vec.reserve(vec.size() + sizeof...(pack));
        populate_from_pack_helper(vec, pack...);
    }
}
