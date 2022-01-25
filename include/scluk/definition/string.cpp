#include "../string.hpp"
#include "../aliases.hpp"
namespace scluk::string {
    #define EXPLICIT_TEMPLATE_INSTANTIATION(T) \
        template std::basic_string<T> repeat(T c, std::size_t n); \
        template std::basic_string<T> repeat(const std::basic_string<T>& s, std::size_t n); \
        template std::basic_string<T> operator*(const std::basic_string<T>& s, std::size_t n); \
        template int levenshtein_dist(std::basic_string_view<T> a, std::basic_string_view<T> b); \
        template std::vector<size_t> get_all_positions_of(str_view<T> s, T c);

    EXPLICIT_TEMPLATE_INSTANTIATION(char)
    EXPLICIT_TEMPLATE_INSTANTIATION(uchar)
    EXPLICIT_TEMPLATE_INSTANTIATION(schar)
    EXPLICIT_TEMPLATE_INSTANTIATION(char8_t)
    EXPLICIT_TEMPLATE_INSTANTIATION(char16_t)
    EXPLICIT_TEMPLATE_INSTANTIATION(char32_t)
    EXPLICIT_TEMPLATE_INSTANTIATION(wchar_t)

    #undef EXPLICIT_TEMPLATE_INSTANTIATION
}
