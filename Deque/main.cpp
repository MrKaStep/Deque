#include <stdio.h>
#include <iostream>
#include <deque>

#include "Deque.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    Deque<int> d(4);
    Deque<int>::iterator it = d.begin();
    const Deque<int>::const_iterator cit = d.cbegin();
    *it = 1;
    cout << *cit;
    system("pause");
    return 0;
}