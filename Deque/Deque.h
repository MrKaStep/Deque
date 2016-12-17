//https://github.com/MrKaStep/Deque
#pragma once

#include <functional>
#include <iterator>
#include <exception>
#include <assert.h>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <iostream>

#ifndef UI32
#define UI32
typedef unsigned int ui32;
#endif ///UI32

template<class T>
class Deque;

template<bool isConst, class T>
class DequeIterator : 
    public std::iterator<std::random_access_iterator_tag,
                         T,
                         std::ptrdiff_t,
                         typename std::conditional<isConst, const T*, T*>::type,
                         typename std::conditional<isConst, const T&, T&>::type> {
public:
    typedef typename std::conditional<isConst, const T*, T*>::type pointer;
    typedef typename std::conditional<isConst, const T&, T&>::type reference;

    friend class Deque<T>;
    friend class DequeIterator<true, T>;
private:
    pointer current;
    pointer first;
    pointer last;
    pointer bufferFirst;
    pointer bufferLast;
    DequeIterator(pointer current, pointer first, pointer last, pointer bufferFirst, pointer bufferLast) :
            current(current),
            first(first),
            last(last),
            bufferFirst(bufferFirst),
            bufferLast(bufferLast) {}
public:
    DequeIterator() :
            current(nullptr),
            first(nullptr),
            last(nullptr),
            bufferFirst(nullptr),
            bufferLast(nullptr) {}
    DequeIterator(const DequeIterator<isConst, T>& another) = default;
    DequeIterator(const DequeIterator<isConst ^ true, T>& another) :
            current(another.current),
            first(another.first),
            last(another.last),
            bufferFirst(another.bufferFirst),
            bufferLast(another.bufferLast){}
    pointer operator->() const {
        if (current == last)
            throw std::runtime_error("Deque iterator is not dereferencalbe");
        return current;
    }
    reference operator*() const {
        if (current == last)
            throw std::runtime_error("Deque iterator is not dereferencalbe");
        return *current;
    }
    DequeIterator<isConst, T>& operator++() {
        if (current == last)
            throw std::runtime_error("Deque iterator is not incrementable");
        if (++current == bufferLast)
            current = bufferFirst;
        return *this;
    }
    DequeIterator<isConst, T> operator++(int) {
        DequeIterator<isConst, T> ans(*this);
        operator++();
        return ans;
    }
    DequeIterator<isConst, T>& operator--() {
        if (current == first)
            throw std::runtime_error("Deque iterator is not decrementable");
        if (--current < bufferFirst)
            current = bufferLast - 1;
        return *this;
    }
    DequeIterator<isConst, T> operator--(int) {
        DequeIterator<isConst, T> ans(*this);
        operator--();
        return ans;
    }
    DequeIterator<isConst, T>& operator+=(std::ptrdiff_t offset) {
        if (offset > 0) {
            if (offset > (current < last ? last - current : bufferLast - current + last - bufferFirst))
                throw std::runtime_error("Deque iterator out of range");
            current += offset - (offset < bufferLast - current ? 0 : bufferLast - bufferFirst);
        } else {
            if(offset < (current > first ? first - current : bufferFirst - current + first - bufferLast))
                throw std::runtime_error("Deque iterator out of range");
            current += offset + (offset < bufferFirst - current ? bufferLast - bufferFirst : 0);
        }
        return *this;
    }
    DequeIterator<isConst, T>& operator-=(std::ptrdiff_t offset) {
        return operator+=(-offset);
    }
    const DequeIterator<isConst, T> operator+(std::ptrdiff_t offset) const {
        DequeIterator<isConst, T> ans(*this);
        ans += offset;
        return ans;
    }
    const DequeIterator<isConst, T> operator-(std::ptrdiff_t offset) const {
        DequeIterator<isConst, T> ans(*this);
        ans -= offset;
        return ans;
    }
    std::ptrdiff_t operator-(const DequeIterator<isConst, T>& another) const {
        return (current >= first 
                ? current - first 
                : (current - bufferFirst) + (bufferLast - first))
            -  (another.current >= first 
                ? another.current - first 
                : (another.current - bufferFirst) + (bufferLast - first));
    }
    reference operator[](int offset) {
        return *(*this + offset);
    }
    const reference operator[](int offset) const {
        return *(*this + offset);
    }
    bool operator<(const DequeIterator<isConst, T>& another) const {
        return operator-(another) < 0;
    }
    bool operator>(const DequeIterator<isConst, T>& another) const {
        return another < (*this);
    }
    bool operator<=(const DequeIterator<isConst, T>& another) const {
        return !(another < *this);
    }
    bool operator>=(const DequeIterator<isConst, T>& another) const {
        return !(*this < another);
    }
    bool operator==(const DequeIterator<isConst, T>& another) const {
        return !(*this < another) && !(another < *this);
    }
    bool operator!=(const DequeIterator<isConst, T>& another) const {
        return *this < another || another < *this;
    }
};

template<bool isConst, class T>
const DequeIterator<isConst, T> operator+(std::ptrdiff_t offset, const DequeIterator<isConst, T>& iter) {
    DequeIterator<isConst, T> ans(iter);
    ans += offset;
    return ans;
}
template<bool isConst, class T>
const DequeIterator<isConst, T> operator-(std::ptrdiff_t offset, const DequeIterator<isConst, T>& iter) {
    DequeIterator<isConst, T> ans(iter);
    ans -= offset;
    return ans;
}

template<class T>
class Deque {
private:
    typedef T* pointer;
    typedef T& reference;

    pointer a_;
    pointer first;
    pointer last;
    size_t capacity_;
    size_t size_;

    void move_(pointer dest) {
        if (first <= last)
            memcpy(dest, first, (last - first) * sizeof(T));
        else {
            memcpy(dest, first, (a_ + capacity_ - first) * sizeof(T));
            memcpy(dest + (a_ + capacity_ - first), a_, (last - a_) * sizeof(T));
        }
        delete[] a_;
        a_ = dest;
        first = a_;
        last = a_ + size_;
    }

    void grow_() {
        pointer buffer = new T[std::max(static_cast<size_t>(1), capacity_ << 1)];
        move_(buffer);
        capacity_ = std::max(static_cast<size_t>(1), capacity_ << 1);
    }
    void shrink_() {
        if (capacity_ <= 4)
            return;
        pointer buffer = new T[capacity_ >> 1];
        move_(buffer);
        capacity_ >>= 1;
    }


public:
    typedef DequeIterator<false, T> iterator;
    typedef DequeIterator<true, T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    Deque() : 
            a_(new T[4]),
            first(a_),
            last(a_),
            capacity_(4),
            size_(0) {}
    Deque(const Deque<T>& another) :
            a_(new T[another.capacity_]),
            first(a_ + (another.first - another.a_)),
            last(a_ + (another.last - another.a_)),
            capacity_(another.capacity_),
            size_(another.size_) {
        memcpy(a_, another.a_, capacity_*sizeof(T));
    }
    iterator begin() {
        return iterator(first, first, last, a_, a_ + capacity_);
    }
    const_iterator begin() const {
        return const_iterator(first, first, last, a_, a_ + capacity_);
    }
    const_iterator cbegin() const {
        return const_iterator(first, first, last, a_, a_ + capacity_);
    }
    iterator end() {
        return iterator(last, first, last, a_, a_ + capacity_);
    }
    const_iterator end() const {
        return const_iterator(last, first, last, a_, a_ + capacity_);
    }
    const_iterator cend() const {
        return const_iterator(last, first, last, a_, a_ + capacity_);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }
    reference operator[](ui32 position) {
        return begin()[position];
    }
    const T& operator[](ui32 position) const {
        return cbegin()[position];
    }
    reference back() {
        return *rbegin();
    }
    const T& back() const {
        return *crbegin();
    }
    reference front() {
        return *begin();
    }
    const T& front() const {
        return *cbegin();
    }
    bool empty() const {
        return size_ == 0;
    }
    size_t size() const {
        return size_;
    }
    size_t capacity() const {
        return capacity_;
    }
    void push_back(const T& value) {
        if (size_ + 1 == capacity_)
            grow_();
        *(last++) = value;
        if (last == a_ + capacity_)
            last = a_;
        ++size_;
    }
    void push_front(const T& value) {
        if (size_ + 1 == capacity_)
            grow_();
        --first;
        if (first < a_)
            first = a_ + capacity_ - 1;
        *first = value;
        ++size_;
    }
    void pop_back() {
        if (--last < a_)
            last = a_ + capacity_ - 1;
        --size_;
        if (size_ * 4 <= capacity_)
            shrink_();
    }
    void pop_front() {
        if (++first == a_ + capacity_)
            first = a_;
        --size_;
        if (size_ * 4 <= capacity_)
            shrink_();
    }
};
