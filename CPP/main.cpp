// C++ 23
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    auto vec{vector{1, 2, 3, 4, 5}};
    for_each(vec.begin(), vec.end(), [](auto i) { cout << i << " "; });
    cout << endl;
    return 0;
}