#include "../range.hpp"

namespace scluk::ranges {
    //range constructors
    range::range(int64_t upper) : range(0, upper) {}
    range::range(int64_t lower, int64_t upper)
        : lower_bound(lower), upper_bound(upper) {}
    range::range(size_t upper) : range(int64_t(upper)) {}
    range::range(size_t lower, size_t upper)
        : range(int64_t(lower), int64_t(upper)) {}
    //range iteration methods
    range::iterator range::begin() { 
        return range::iterator { lower_bound }; 
    }
    i64 range::end() { return upper_bound; }
    //range::iterator methods
    bool range::iterator::operator!=(i64 o) { return el < 0; }
    range::iterator range::iterator::operator++() { 
        return range::iterator{ el++ };
    }
    i64& range::iterator::operator*() {
        return el;
    }
    //range_shorthand_helper 
    range range_shorthand_helper::operator>(i64 upper_bound) {
        return range(lower_bound, upper_bound);
    }
    range_shorthand_helper range_shorthand_helper::operator-() { 
        return range_shorthand_helper { -lower_bound };
    }
    range_shorthand_helper operator""_(unsigned long long lower_bound) {
        return range_shorthand_helper { i64(lower_bound) };
    }
}
