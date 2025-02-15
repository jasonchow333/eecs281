#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(){
    vector<vector<int>> a;
    a.resize(1);
    for (size_t i = 0; i < 10; i++){
        a[0].push_back(i);
    }
    vector<vector<int>>::iterator b = a.begin();
    cout << b->at(0) << endl;
}
