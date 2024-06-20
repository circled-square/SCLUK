#ifndef SCLUK_SLIDING_QUEUE_HPP
#define SCLUK_SLIDING_QUEUE_HPP

#include <memory>
#include <array>
#include <cstddef>
#include <cassert>
#include <initializer_list>

#include "aliases.hpp"

namespace scluk {
    template<typename T, size_t sz>
    class sliding_queue;

    namespace detail {
        template<typename T, size_t sz, bool is_internal_cursor = false>
        class circular_iterator {
            template<typename, size_t>
            friend class scluk::sliding_queue;

            static constexpr size_t end = ~size_t(0);

            sliding_queue<T,sz>& q;
            size_t i;

            void advance() {
                if constexpr(!is_internal_cursor) {
                    //calculate before assigning to i so if (*this) 
                    //is q.cursor you know it didn't reach end
                    size_t future_value = (i + 1) % sz;
                    if(future_value == q.begin().i)
                        i = end;
                    else 
                        i = future_value;
                } else {
                    i = (i + 1) % sz;
                }
            }
            operator circular_iterator<T, sz, false>() {
                return circular_iterator<T, sz, false>(q, i);
            }
        public:
            circular_iterator(sliding_queue<T,sz>& queue, size_t index) : q(queue), i(index) { }

            size_t pos() { return i; }
            //prefix
            circular_iterator& operator++() {
                advance();
                return *this;
            }
            //postfix
            circular_iterator operator++(int) {
                size_t ret = i;
                advance();
                return circular_iterator(q, ret);
            }

            T& operator*() { return q.array[i]; }

            bool operator!=(const circular_iterator& o) {
                assert(&q == &o.q);
                return i != o.i;
            }
        };
    }
    template<typename T, size_t sz>
    class sliding_queue {
        template<typename, size_t, bool>
        friend class detail::circular_iterator;
        std::unique_ptr<T[]> array;
        detail::circular_iterator<T, sz, true> cursor;
    public:
        using input_t = T;
        using iterator_type = detail::circular_iterator<T, sz, false>;

        sliding_queue() : array(new T[sz]), cursor(*this, 0) {}
        sliding_queue(std::initializer_list<T> elems) : array(new T[sz]{ elems }), cursor(*this, 0) {}
        sliding_queue(const T& v) : sliding_queue() { for(T& el : *this) el = v; }

        //pushes a new element into the queue in place of the oldest; the oldest is returned
        T push(T el) {
            T ret = *cursor;
            std::swap(*cursor, el);
            cursor++;
            return ret;
        }
        //equivalent to push, but returns *this.
        sliding_queue& operator<<(T el) {
            push(std::move(el));
            return *this;
        }

        iterator_type begin() { return cursor; }
        iterator_type end() { return iterator_type(*this, iterator_type::end); }

        T& operator[](size_t i) { return array[(i + cursor.i) % sz]; }
        const T& operator[](size_t i) const { return array[(i + cursor.i) % sz]; }

        static constexpr size_t size() { return sz; }
    };
}
namespace std {
    template<class T, size_t sz>
    struct iterator_traits<scluk::detail::circular_iterator<T, sz, false>> {
        using iterator_category = std::forward_iterator_tag;
    };
}

#endif //SCLUK_SLIDING_QUEUE_HPP
