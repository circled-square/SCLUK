#include "../range.hpp"

namespace scluk {
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
    bool range::iterator::operator!=(i64 o) { return el < o; }
    range::iterator range::iterator::operator++() { 
        return range::iterator{ el++ };
    }
    i64& range::iterator::operator*() {
        return el;
    }
}
