#include<iostream>
using namespace std;

int main() {
    int n = 2;
    int m = -1*(-3);
    cin >> n;
    for (int i = 0; i < 1; ++i) {
        cout << i +(+ ++n);
    }

    return 1;
}