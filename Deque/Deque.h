#pragma once

#include <functional>
#include <iterator>
#include <assert.h>

#ifndef UI32
#define UI32
typedef unsigned int ui32;
#endif ///UI32

template<class T>
class Deque {
private:
    T* a_;
    ui32 capacity_;
    ui32 size_;

    template<bool isConst>
    class DequeIterator : std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&> {
        friend class Deque;
    private:
        T* current;
        T* first;
        T* last;
        T* bufferFirst;
        T* bufferLast;
        DequeIterator(T* current, T* first, T* last, T* bufferFirst, T* bufferLast) :
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
        DequeIterator(const DequeIterator<isConst>& another) = default;

        T& operator*() {
            return isConst ? const_cast<const T&>(*current) : *current;
        }
        T* operator->() {
            return isConst ? const_cast<const T*>(current) : current;
        }
        DequeIterator<isConst>& operator++() {
            current = (current == bufferLast ? bufferFirst : current + 1);
            return *this;
        }
        DequeIterator<isConst> operator++(int) {
            DequeIterator<isConst> ans(*this);
            current = (current == bufferLast ? bufferFirst : current + 1);
            return ans;
        }
        DequeIterator<isConst>& operator--() {
            current = (current == bufferFirst ? bufferLast : current - 1);
            return *this;
        }
        DequeIterator<isConst> operator--(int) {
            DequeIterator<isConst> ans(*this);
            current = (current == bufferFirst ? bufferLast : current - 1);
            return ans;
        }
    };
public:
    typedef DequeIterator<false> iterator;
    typedef DequeIterator<true> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    Deque(int sz) {
        capacity_ = sz;
        size_ = sz;
        a_ = new T(sz);
    }
    iterator begin() {
        return iterator(a_, a_, a_ + size_, a_, a_ + size_);
    }
    const_iterator cbegin() const {
        return const_iterator(a_, a_, a_ + size_, a_, a_ + size_);
    }
};
