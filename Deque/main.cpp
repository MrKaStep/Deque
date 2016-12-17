#include <stdio.h>
#include <iostream>
#include <deque>
#include <vector>

#include "Deque.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    Deque<int> d;
    for (int i = 0; i < 32; ++i) {
        if (rand() % 2)
            d.push_back(1);
        else
            d.push_front(1);
        cout << d.begin() - d.end() << endl;
    }
    for (int i = 0; i < 32; ++i) {
        if (rand() % 2)
            d.pop_back();
        else
            d.pop_front();
        cout << d.begin() - d.end() << endl;
    }
    system("pause");
    return 0;
}