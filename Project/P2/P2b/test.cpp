#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main(){
    vector<int> a;
    for (int i=0;i<10;i++){
        a.push_back(10-i);
    }
    sort(a.begin(),a.end());
    a.push_back(4);
    for (int i=0; i<11; i++){
        cout << a[i]<<' ';
    }
    cout << endl;
    vector<int>::iterator p;
    cout << *lower_bound(a.begin(), a.end(), 4, greater<int>())<<endl;
    for (p = a.end()-1; p>lower_bound(a.begin(), a.end(), 4, greater<int>());){
        swap(*p,*p--);
        for (int i=0; i<11; i++){
            cout << a[i]<<' ';
        }
        cout << endl;
    }
    
}
