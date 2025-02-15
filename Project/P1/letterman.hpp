// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include <stack>
using namespace std;

struct Word
{
    string wd;
    size_t prev;
    bool dis_ed;
};

class letterman
{
private:
    vector<Word> dictionary;
    deque<size_t> container;
    string begin_w;
    string end_w;
    size_t b_pos;
    size_t dict_size;
    bool stque; // false for queue, true for stack
    bool op; // false for word format, true for modification format
    bool change;
    bool length;
    bool swap;
    string reversal(string &word);
    bool check_dis(const string &inspect, const string &dict);
    
public:
    void print_dict();
    void read_dict();
    void search();
    void mod_output(const string &prev, const string &later);
    void swap_let(string &word, size_t pos);
    void change_let(string &word, size_t pos, char ch);
    void insert_let(string &word, size_t pos, char ch);
    void delete_let(string &word, size_t pos);
    bool compare_equal(const string &dict, const string &word);
    letterman(string &b_w, string &e_w, bool sq, bool output, bool change, bool length, bool swap);
};
