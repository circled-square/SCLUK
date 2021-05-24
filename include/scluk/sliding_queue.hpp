#ifndef SCLUK_SLIDING_QUEUE_HPP
#define SCLUK_SLIDING_QUEUE_HPP

#include <memory>
#include <array>
#include <cstddef>
#include <cassert>
#include <initializer_list>

namespace scluk {
    template<typename T, std::size_t sz>
    class sliding_queue {
    public://iterator class definition
        template<bool is_internal_cursor>
        friend class circular_iterator;
        template<bool is_internal_cursor = false>
        class circular_iterator {
            friend class sliding_queue<T, sz>;
            static constexpr std::size_t end = ~0ull;

            sliding_queue<T,sz>& q;
            std::size_t i;

            void advance() {
                if constexpr(!is_internal_cursor) {
                    //calculate before assigning to i so if (*this) 
                    //is q.cursor you know it didn't reach end
                    size_t future_value = (i + 1) % sz;
                    if(future_value == q.cursor.i)
                        i = end;
                    else 
                        i = future_value;
                } else {
                    i = (i + 1) % sz;
                }
            }
            
            circular_iterator(sliding_queue<T,sz>& q, size_t i) : q(q), i(i) { }

            operator circular_iterator<false>() {
                return circular_iterator<false>(q, i);
            }
        public:
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

            T& operator*() { 
                return q.array[i];
            }

            bool operator!=(const circular_iterator& o) {
                assert(&q == &o.q);
                return i != o.i;
            }
        };
    private:
        std::unique_ptr<T[]> array;
        circular_iterator<true> cursor;
    public:
        static constexpr std::size_t size = sz;
        using input_t = T;

        sliding_queue() : array(new T[sz]), cursor(*this, 0) { }
        sliding_queue(const T& default_value) : sliding_queue() {
            for(T& el : *this)
                el = default_value;
        }
        sliding_queue(std::initializer_list<T>& elems) : sliding_queue() {
            array = elems;
        }

        T push(const T& el) {
            T ret = *cursor;
            *cursor++ = el;
            return ret;
        }

        circular_iterator<false> begin() { return cursor; }
        circular_iterator<false> end() { return circular_iterator<false>(*this, circular_iterator<false>::end); }
    };
}

#endif //SCLUK_SLIDING_QUEUE_HPP
