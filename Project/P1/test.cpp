#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main(){
    string la = "lol";
    la.erase(la.size()-1);
    cout << la << endl;
    vector<string> l;
    l.reserve(1);
    l.push_back(la);
    l.push_back(la);
    l.push_back(la);
}
