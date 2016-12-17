#pragma once

#include <functional>
#include <iterator>
#include <exception>
#include <assert.h>
#include <algorithm>

#ifndef UI32
#define UI32
typedef unsigned int ui32;
#endif ///UI32

template<class T>
class Deque;

template<bool isConst, class T>
class DequeIterator : std::iterator<std::random_access_iterator_tag, T> {
    friend class Deque<T>;
private:
    typedef typename std::conditional<isConst, const T*, T*>::type Ptr;
    typedef typename std::conditional<isConst, const T&, T&>::type Ref;
    Ptr current;
    Ptr first;
    Ptr last;
    Ptr bufferFirst;
    Ptr bufferLast;
    DequeIterator(Ptr current, Ptr first, Ptr last, Ptr bufferFirst, Ptr bufferLast) :
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
        
    Ptr operator->() const {
        if (current == last)
            throw std::exception("Deque iterator is not dereferencalbe");
        return current;
    }
    Ref operator*() const {
        return *current;
    }
    DequeIterator<isConst, T>& operator++() {
        if (current == last)
            throw std::exception("Deque iterator is not dereferencalbe");
        if (current == last)
            throw std::exception("Deque iterator is not incrementable");
        current = (current == bufferLast ? bufferFirst : current + 1);
        return *this;
    }
    DequeIterator<isConst, T> operator++(int) {
        if (current == last)
            throw std::exception("Deque iterator is not incrementable");
        DequeIterator<isConst, T> ans(*this);
        current = (current == bufferLast ? bufferFirst : current + 1);
        return ans;
    }
    DequeIterator<isConst, T>& operator--() {
        if (current == first)
            throw std::exception("Deque iterator is not decrementable");
        current = (current == bufferFirst ? bufferLast : current - 1);
        return *this;
    }
    DequeIterator<isConst, T> operator--(int) {
        if (current == first)
            throw std::exception("Deque iterator is not decrementable");
        DequeIterator<isConst, T> ans(*this);
        current = (current == bufferFirst ? bufferLast : current - 1);
        return ans;
    }
    DequeIterator<isConst, T>& operator+=(ptrdiff_t offset) {
        if (offset > 0) {
            if (offset > (current < last ? last - current : bufferLast - current + last - bufferFirst))
                throw std::exception("Deque iterator out of range");
            current += offset - (offset < bufferLast - current ? 0 : bufferLast - bufferFirst);
        } else {
            if(offset < (current > first ? first - current : bufferFirst - current + first - bufferLast))
                throw std::exception("Deque iterator out of range");
            current += offset + (offset < bufferFirst - current ? bufferLast - bufferFirst : 0);
        }
        return *this;
    }
    DequeIterator<isConst, T>& operator-=(ptrdiff_t offset) {
        return operator+=(-offset);
    }
    const DequeIterator<isConst, T> operator+(ptrdiff_t offset) const {
        DequeIterator<isConst, T> ans(*this);
        ans += offset;
        return ans;
    }
    const DequeIterator<isConst, T> operator-(ptrdiff_t offset) const {
        DequeIterator<isConst, T> ans(*this);
        ans -= offset;
        return ans;
    }
    ptrdiff_t operator-(const DequeIterator<isConst, T>& another) const {
        return (current + (current >= first ? 0 : bufferFirst - bufferLast))
            - (another.current + (another.current >= first ? 0 : bufferFirst - bufferLast));
    }
    Ref operator[](int offset) {
        return *(*this + offset);
    }
    const Ref operator[](int offset) const {
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
};

template<bool isConst, class T>
const DequeIterator<isConst, T> operator+(ptrdiff_t offset, const DequeIterator<isConst, T>& iter) {
    DequeIterator<isConst, T> ans(iter);
    ans += offset;
    return ans;
}
template<bool isConst, class T>
const DequeIterator<isConst, T> operator-(ptrdiff_t offset, const DequeIterator<isConst, T>& iter) {
    DequeIterator<isConst, T> ans(iter);
    ans -= offset;
    return ans;
}

template<class T>
class Deque {
private:
    T* a_;
    T* first;
    T* last;
    size_t capacity_;
    size_t size_;
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

};
