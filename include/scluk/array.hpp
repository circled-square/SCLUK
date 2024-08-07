#ifndef SCLUK_ARRAY_HPP
#define SCLUK_ARRAY_HPP

#include <cstddef>
#include <array>
#include <memory>
#include <cstdint>
#include <stdexcept>
#include <cassert>
#include "metaprogramming.hpp"

namespace scluk {
    template<typename T, size_t sz>
    struct circular_array : public std::array<T, sz> {
        template<typename...Ts>
        circular_array(Ts...args) : std::array<T,sz>(args...) { }

        inline T& operator[](size_t index) { return this->data()[index % sz]; }
        static circular_array make_from_value(const T& v) {
            return circular_array(make_array_from_single_value(v));
        }
    };

    template<size_t sz, typename T> constexpr
    std::array<T,sz> make_array_from_single_value(const T& value);

    template<typename T>
    static std::unique_ptr<T[]> make_unique_ptr_array(unsigned size, const auto& func);

    //heap_array father class definition
    namespace detail {
        template<typename T>
        class heap_array_father {
        protected:
            std::unique_ptr<T[]> arr_ptr;
            inline void check() const { assert(arr_ptr && "tried accessing heap_array<...> with empty pointer (e.g. after move)"); }

            heap_array_father(std::unique_ptr<T[]>&& o) : arr_ptr(std::move(o)) {}
            heap_array_father(heap_array_father<T>&& o) : arr_ptr(std::move(o.arr_ptr)) {}
            virtual ~heap_array_father() {}
        public:
            using value_type = T;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator = pointer;
            using const_iterator = const_pointer;
            using reverse_iterator = iterator;
            using const_reverse_iterator = const_pointer;

            virtual size_t size() const = 0;
            virtual size_t max_size() const = 0;

            T& at(size_t i) {
                check();
                if(i >= size()) throw std::out_of_range(sout("heap_array::at(i): i > size(); i = %, size() = %", i, size()));
                return arr_ptr[i];
            }

            T&       operator[](size_t i)       { check(); return arr_ptr[i]; }
            const T& operator[](size_t i) const { check(); return arr_ptr[i]; }

            T&       front()       { check(); return arr_ptr[0]; }
            const T& front() const { check(); return arr_ptr[0]; }

            T&       back()       { check(); return arr_ptr[size()-1]; }
            const T& back() const { check(); return arr_ptr[size()-1]; }

            T*       data()       { check(); return arr_ptr.get(); }
            const T* data() const { check(); return arr_ptr.get(); }

            T*       begin()        { check(); return arr_ptr.get(); }
            const T* begin()  const { check(); return arr_ptr.get(); }
            const T* cbegin() const { check(); return arr_ptr.get(); }

            T*       end()           { return begin() + size(); }
            const T* end()  const    { return begin() + size(); }
            const T* cend() const    { return begin() + size(); }

            T*       rbegin()        { return end() - 1; }
            const T* rbegin()  const { return end() - 1; }
            const T* crbegin() const { return end() - 1; }

            T*       rend()        { return begin() - 1; }
            const T* rend()  const { return begin() - 1; }
            const T* crend() const { return begin() - 1; }

            constexpr bool empty() const { return !arr_ptr; }

            void fill(const T& v) { check(); for(T& e : *this) e = v; }
            void swap(heap_array_father& o) {
                assert(size() == o.size() && "tried to swap two scluk::heap_array<...> with different sizes");
                std::swap(arr_ptr, o.arr_ptr);
            }

            operator bool() { return bool(arr_ptr); }
            bool operator!() { return !arr_ptr; }

            heap_array_father& operator=(const concepts::iterable<value_type> auto& o) {
                assert(o.size() == size() && "tried assigning to scluk::heap_array<...> a container with a different size");
                if(!arr_ptr)
                    arr_ptr.reset(new T[size()]);
                std::copy(o.begin(), o.end(), begin());
                return *this;
            }
        };
    }

    template<typename T, size_t sz = 0>
    class heap_array : public detail::heap_array_father<T> {
    protected:
        heap_array(std::unique_ptr<T[]>&& o) : detail::heap_array_father<T>(std::move(o)) {}
    public:
        heap_array(std::nullptr_t)      : detail::heap_array_father<T>(nullptr) {}
        heap_array()                    : heap_array(std::unique_ptr<T[]>(new T[sz])) {}
        heap_array(heap_array&& o)      : detail::heap_array_father<T>(std::move(o)) {}
        heap_array(const heap_array& o)       : heap_array(o.clone()) {}
        heap_array(const T& value)      : heap_array() { this->fill(value); }

        static constexpr size_t array_size = sz;
        static constexpr size_t array_bytes = sz * sizeof(T);

        constexpr size_t size() const override { return array_size; }
        constexpr size_t max_size() const override { return array_size; }

        heap_array& operator=(heap_array<T, sz> o) {
            this->swap(o);
            return *this;
        }

        heap_array clone() const {
            this->check();
            heap_array n;
            std::copy(this->begin(), this->end(), n.begin());
            return n;
        }
    };

    template<typename T>
    class heap_array<T, 0> : public detail::heap_array_father<T> {
    protected:
        heap_array(std::unique_ptr<T[]>&& o, size_t sz) : detail::heap_array_father<T>(std::move(o)), array_size(sz), array_bytes(sz * sizeof(T)) {}
    public:
        heap_array(std::nullptr_t, size_t sz) : heap_array(std::unique_ptr<T[]>(nullptr), sz) {}
        heap_array(size_t sz)                 : heap_array(std::unique_ptr<T[]>(new T[sz]), sz) { }
        template<size_t sz>
        heap_array(heap_array<T, sz>&& o)     : heap_array(std::move(o.arr_ptr), o.size()) { }
        template <size_t sz>
        heap_array(const heap_array<T, sz>& o)      : heap_array(o.clone()) {}
        heap_array(size_t sz, const T& value)            : heap_array(sz) { this->fill(value); }

        const size_t array_size;
        const size_t array_bytes;

        constexpr size_t size() const override { return array_size; }
        constexpr size_t max_size() const override { return array_size; }

        heap_array& operator=(heap_array o) {
            this->swap(o);
            return *this;
        }

        heap_array clone() const {
            this->check();
            heap_array n;
            std::copy(this->begin(), this->end(), n.begin());
            return n;
        }
    };

    //multi-dimensional static array implementation
    namespace detail {
        template<typename T, int s, int...sizes> struct array_helper {
            using type = typename array_helper<std::array<T, s>, sizes...>::type;
        };
        template<typename T, int s> struct array_helper<T, s> {
            using type = std::array<T, s>;
        };
    }
    //multi-dimensional static array
    template<typename T, int...sizes> using array = typename detail::array_helper<T, sizes...>::type;


    //template definitions follow:

    namespace detail {//helpers for make_array_from_single_value
        template<typename T, std::size_t> inline constexpr
        T ignore_index(T value){ return value; }

        template<typename T, typename arr_t, std::size_t...indexes> inline constexpr
        arr_t make_array_from_single_value_helper(const T& value, std::index_sequence<indexes...>) {
            return arr_t{ ignore_index<const T&, indexes>(value)... };
        }
    }

    template<std::size_t sz, typename T> inline constexpr
    std::array<T,sz> make_array_from_single_value(T& value) {
        return detail::make_array_from_single_value_helper<std::array<T,sz>,T>(value, std::make_index_sequence<sz>());
    }

    template<typename T>
    static std::unique_ptr<T[]> make_unique_ptr_array(unsigned size, const auto& func) {
        std::unique_ptr<T[]> ret = reinterpret_cast<T*>(new char[size*sizeof(T)]);
        for(int i = 0; i < size; i++)
            new (&ret[i]) T(func());

        return ret;
    }
}

#endif
