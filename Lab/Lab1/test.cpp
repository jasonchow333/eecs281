// int main(){
//     int some_ints[5] = {1,2,3,4,0};
//     for (int *p = some_ints; p; ++p)
//         *p=0;
//     return 0;
// }
// #include <vector>
// int* get_ints(){
//     std::vector<int> ints = {1,2,3,4,5};
//     return ints.data();
// }
// int main(){
//     int *some_ints = get_ints();
//     delete[] some_ints;
//     return 0;
// }
// #include<iostream>
// void take_int(int x){}
// int main(){
//     size_t x;
//     std::cin >> x;
//     take_int(x);
// }
// #include <iostream>
// #include <string>
// using namespace std;
// int main(){
//     string s1;
//     string s2;
//     //getline(cin, s2);
//     cin >> s2;
//     cin >> s1;
//     cout << s1<< s2 <<s1.size()<<s2.size() << endl;
//     return 0;
// }
#include <iostream>
int w(){
    int x,y = 281;
    return x+=y;
}
int main(){
    std::cout << w() << std::endl;
}
