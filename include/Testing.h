using namespace std;

#ifndef TESTING
#define TESTING

#include <iostream>
#include <string>

template<typename T>
void assert(T actual, T expected, string test_name) {
    if (actual != expected) {
        cout << "\033[31m" << "FAILED" << "\033[0m" << ":  " << test_name << "\n";
    } else{
        cout << "\033[32m" << "PASSED" << "\033[0m" << ":  " << test_name << "\n";
    }
}

#endif