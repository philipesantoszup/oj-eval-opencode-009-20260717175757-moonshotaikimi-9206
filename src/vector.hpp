#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
    /**
     * TODO
     * a type for actions of the elements of a vector, and you should write
     *   a class named const_iterator with same interfaces.
     */
    /**
     * you can see RandomAccessIterator at CppReference for help.
     */
    class const_iterator;
    class iterator
    {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the following
    // typedef) to work properly. In particular, without the following code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
    // About iterator_category: https://en.cppreference.com/w/cpp/iterator
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

    private:
        T* ptr;
        const vector* container;
        
    public:
        iterator(T* p = nullptr, const vector* c = nullptr) : ptr(p), container(c) {}
        
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        iterator operator+(const int &n) const
        {
            return iterator(ptr + n, container);
        }
        iterator operator-(const int &n) const
        {
            return iterator(ptr - n, container);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const
        {
            if (container != rhs.container)
                throw invalid_iterator();
            return static_cast<int>(ptr - rhs.ptr);
        }
        iterator& operator+=(const int &n)
        {
            ptr += n;
            return *this;
        }
        iterator& operator-=(const int &n)
        {
            ptr -= n;
            return *this;
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            iterator tmp(*this);
            ++ptr;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        iterator& operator++() {
            ++ptr;
            return *this;
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            iterator tmp(*this);
            --ptr;
            return tmp;
        }
        /**
         * TODO --iter
         */
        iterator& operator--() {
            --ptr;
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const {
            return *ptr;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.get_ptr();
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.get_ptr();
        }
        
        T* get_ptr() const { return ptr; }
        const vector* get_container() const { return container; }
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     */
    class const_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

    private:
        T* ptr;
        const vector* container;
        
    public:
        const_iterator(T* p = nullptr, const vector* c = nullptr) : ptr(p), container(c) {}
        const_iterator(const iterator& it) : ptr(it.get_ptr()), container(it.get_container()) {}
        
        const_iterator operator+(const int &n) const
        {
            return const_iterator(ptr + n, container);
        }
        const_iterator operator-(const int &n) const
        {
            return const_iterator(ptr - n, container);
        }
        int operator-(const const_iterator &rhs) const
        {
            if (container != rhs.container)
                throw invalid_iterator();
            return static_cast<int>(ptr - rhs.ptr);
        }
        const_iterator& operator+=(const int &n)
        {
            ptr += n;
            return *this;
        }
        const_iterator& operator-=(const int &n)
        {
            ptr -= n;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++ptr;
            return tmp;
        }
        const_iterator& operator++() {
            ++ptr;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp(*this);
            --ptr;
            return tmp;
        }
        const_iterator& operator--() {
            --ptr;
            return *this;
        }
        const T& operator*() const {
            return *ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.get_ptr();
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.get_ptr();
        }
        
        T* get_ptr() const { return ptr; }
        const vector* get_container() const { return container; }
    };
    
private:
    T* data;
    size_t sz;
    size_t cap;
    
    void expand() {
        if (sz >= cap) {
            size_t new_cap = cap == 0 ? 2 : cap * 2;
            reallocate(new_cap);
        }
    }
    
    void reallocate(size_t new_cap) {
        if (new_cap <= cap) return;
        
        T* new_data = (T*)malloc(sizeof(T) * new_cap);
        
        for (size_t i = 0; i < sz; ++i) {
            new (new_data + i) T(data[i]);
            data[i].~T();
        }
        
        ::free(data);
        data = new_data;
        cap = new_cap;
    }
    
public:
    /**
     * TODO Constructs
     * At least two: default constructor, copy constructor
     */
    vector() : data(nullptr), sz(0), cap(0) {}
    
    vector(const vector &other) : sz(other.sz), cap(other.cap) {
        if (cap > 0) {
            data = (T*)malloc(sizeof(T) * cap);
            for (size_t i = 0; i < sz; ++i) {
                new (data + i) T(other.data[i]);
            }
        } else {
            data = nullptr;
        }
    }
    
    /**
     * TODO Destructor
     */
    ~vector() {
        for (size_t i = 0; i < sz; ++i) {
            data[i].~T();
        }
        ::free(data);
    }
    
    /**
     * TODO Assignment operator
     */
    vector &operator=(const vector &other) {
        if (this == &other) return *this;
        
        // Destroy existing elements
        for (size_t i = 0; i < sz; ++i) {
            data[i].~T();
        }
        
        // Reallocate if needed
        if (other.sz > cap) {
            ::free(data);
            cap = other.cap;
            data = (T*)malloc(sizeof(T) * cap);
        }
        
        sz = other.sz;
        for (size_t i = 0; i < sz; ++i) {
            new (data + i) T(other.data[i]);
        }
        
        return *this;
    }
    
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
    T & at(const size_t &pos) {
        if (pos >= sz) {
            throw index_out_of_bound();
        }
        return data[pos];
    }
    
    const T & at(const size_t &pos) const {
        if (pos >= sz) {
            throw index_out_of_bound();
        }
        return data[pos];
    }
    
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary but I want you to do.
     */
    T & operator[](const size_t &pos) {
        if (pos >= sz) {
            throw index_out_of_bound();
        }
        return data[pos];
    }
    
    const T & operator[](const size_t &pos) const {
        if (pos >= sz) {
            throw index_out_of_bound();
        }
        return data[pos];
    }
    
    /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
    const T & front() const {
        if (sz == 0) {
            throw container_is_empty();
        }
        return data[0];
    }
    
    /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
    const T & back() const {
        if (sz == 0) {
            throw container_is_empty();
        }
        return data[sz - 1];
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(data, this);
    }
    
    const_iterator begin() const {
        return const_iterator(data, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(data, this);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(data + sz, this);
    }
    
    const_iterator end() const {
        return const_iterator(data + sz, this);
    }
    
    const_iterator cend() const {
        return const_iterator(data + sz, this);
    }
    
    /**
     * checks whether the container is empty
     */
    bool empty() const {
        return sz == 0;
    }
    
    /**
     * returns the number of elements
     */
    size_t size() const {
        return sz;
    }
    
    /**
     * clears the contents
     */
    void clear() {
        for (size_t i = 0; i < sz; ++i) {
            data[i].~T();
        }
        sz = 0;
    }
    
    /**
     * inserts value before pos
     * returns an iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) {
        return insert(pos - begin(), value);
    }
    
    /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
     */
    iterator insert(const size_t &ind, const T &value) {
        if (ind > sz) {
            throw index_out_of_bound();
        }
        
        if (sz >= cap) {
            size_t offset = ind;
            size_t new_cap = cap == 0 ? 2 : cap * 2;
            T* new_data = (T*)malloc(sizeof(T) * new_cap);
            
            // Copy elements before ind
            for (size_t i = 0; i < ind; ++i) {
                new (new_data + i) T(data[i]);
                data[i].~T();
            }
            
            // Construct at ind
            new (new_data + ind) T(value);
            
            // Copy elements after ind
            for (size_t i = ind; i < sz; ++i) {
                new (new_data + i + 1) T(data[i]);
                data[i].~T();
            }
            
            ::free(data);
            data = new_data;
            cap = new_cap;
        } else {
            // Shift elements
            for (size_t i = sz; i > ind; --i) {
                new (data + i) T(data[i - 1]);
                data[i - 1].~T();
            }
            // Construct at ind
            new (data + ind) T(value);
        }
        
        ++sz;
        return iterator(data + ind, this);
    }
    
    /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is returned.
     */
    iterator erase(iterator pos) {
        return erase(pos - begin());
    }
    
    /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
    iterator erase(const size_t &ind) {
        if (ind >= sz) {
            throw index_out_of_bound();
        }
        
        data[ind].~T();
        
        // Shift elements
        for (size_t i = ind; i < sz - 1; ++i) {
            new (data + i) T(data[i + 1]);
            data[i + 1].~T();
        }
        
        --sz;
        
        if (ind < sz) {
            return iterator(data + ind, this);
        } else {
            return end();
        }
    }
    
    /**
     * adds an element to the end.
     */
    void push_back(const T &value) {
        expand();
        new (data + sz) T(value);
        ++sz;
    }
    
    /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
    void pop_back() {
        if (sz == 0) {
            throw container_is_empty();
        }
        data[sz - 1].~T();
        --sz;
    }
};


}

#endif
