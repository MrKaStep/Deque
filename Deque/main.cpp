#include <stdio.h>
#include <iostream>

#include "Deque.h"

template<bool T>
class G;

typedef int G<true>;

int main() {
    Deque<int> d(4);
    Deque<int>::iterator it = d.begin();
    Deque<int>::const_iterator cit = d.cbegin();
    *it = 2;
    std::cout << *(d.begin()) << std::endl;
    *cit = 3;
    std::cout << *(d.begin()) << std::endl;
    system("pause");
    return 0;
}