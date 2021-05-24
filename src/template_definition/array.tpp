namespace scluk {
    namespace detail {//helpers for make_array_from_single_value
        template<typename T, std::size_t> inline constexpr
        T ignore_index(T value){ return value; }

        template<typename T, typename arr_t, std::size_t...indexes> inline constexpr
        arr_t make_array_from_single_value_helper(T& value, std::index_sequence<indexes...>) {
            return arr_t{ ignore_index<T&, indexes>(value)... };
        }
    }

    template<std::size_t sz, typename T> inline constexpr
    std::array<T,sz> make_array_from_single_value(T& value) {
        return detail::make_array_from_single_value_helper<std::array<T,sz>,T>(value, std::make_index_sequence<sz>());
    }

    template<std::size_t sz, typename T> inline constexpr
    circular_array<T,sz> make_circ_array_from_single_value(T& value) {
        return detail::make_array_from_single_value_helper<circular_array<T,sz>,T>(value, std::make_index_sequence<sz>());
    }

    template<typename T>
    static std::unique_ptr<T[]> make_unique_ptr_array(unsigned size, const auto& func) {
        std::unique_ptr<T[]> ret = reinterpret_cast<T*>(new char[size*sizeof(T)]);
        for(int i = 0; i < size; i++)
            new (&ret[i]) T(func());        

        return ret;
    }
}
