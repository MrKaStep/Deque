#include <stdio.h>
#include <iostream>
#include <deque>

#include "Deque.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    Deque<int> d;
    d.push_back(2);
    d.push_front(1);
    auto it = d.crbegin();
    cout << *it;
    system("pause");
    return 0;
}