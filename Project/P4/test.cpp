#include <iostream>
using namespace std;

struct node
{
    int val;
    node *next = nullptr;
    node(int v){
        val = v;
    }
};

int main(){
    node *n1 = new node(1);
    node *n2 = new node(2);
    n1->next = n2;
    node *n3 = new node(3);
    node *n4 = n2;
    n2->next = n2 = n3;
    cout << n2->val << endl;
    delete n4;
    delete n3;
    delete n1;
}
