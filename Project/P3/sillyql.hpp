// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "TableEntry.h"
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
using namespace std;

EntryType convert(const string &type);
bool convert_bl(const string &bl);
TableEntry make_entry(const string &value, EntryType etype);
template <class It, class Compare> size_t print_where(It begin, It end, Compare comp, bool quiet, const vector<pair<string, size_t>> &outcol);

class Less
{
    private:
    TableEntry standard;
    size_t colnum;

    public:
    Less(size_t col, const TableEntry &stand): standard{stand}, colnum{col}{}
    bool operator()(const TableEntry &tabentry){
        return tabentry < standard;
    }
    bool operator()(const vector<TableEntry> &row){
        return row[colnum] < standard;
    }
};

class Equal
{
    private:
    TableEntry standard;
    size_t colnum;

    public:
    Equal(size_t col, const TableEntry &stand): standard{stand}, colnum{col}{}
    bool operator()(const TableEntry &tabentry){
        return tabentry == standard;
    }
    bool operator()(const vector<TableEntry> &row){
        return row[colnum] == standard;
    }
};

class Greater
{
    private:
    TableEntry standard;
    size_t colnum;

    public:
    Greater(size_t col, const TableEntry &stand): standard{stand}, colnum{col}{}
    bool operator()(const TableEntry &tabentry){
        return tabentry > standard;
    }
    bool operator()(const vector<TableEntry> &row){
        return row[colnum] > standard;
    }
};

class Table
{
private:
    vector<vector<TableEntry>> data;
    unordered_map<string, size_t> colname;
    unordered_map<TableEntry, vector<size_t>> hash;
    map<TableEntry, vector<size_t>> bst;
    vector<EntryType> coltype;
    unordered_map<string, size_t>::iterator pos;
    string tablename;
    string idxcol;
    string idxtype = "";
    size_t idxcol_num;
public:
    void insert();
    bool colname_err(const string &col, unordered_map<string, size_t>::iterator &pos);
    void print(bool quiet);
    size_t print_bst(map<TableEntry, vector<size_t>>::iterator begin, map<TableEntry, vector<size_t>>::iterator end, const vector<pair<string, size_t>> &outcol, bool quiet);
    void deltab();
    void generate();
    size_t idx_update(const string &colname, const string &idxtype, const size_t &colnum);
    Table(const string &tabname): tablename(tabname){};
    Table(){};
    ~Table(){};
    friend class Sillyql;
};

class Sillyql
{
private:
    unordered_map<string, Table*> database;
    unordered_map<string, Table*>::iterator pos;
    bool quiet = false;
public:
    void option_r(int argc, char *argv[]);
    bool tabname_err(const string &tabname, unordered_map<string, Table*>::iterator &pos);
    void create();
    void insert();
    void print();
    void remove();
    void deletable();
    void join();
    size_t print_join(Table *table1, Table *t2_begin, bool quiet, const vector<pair<string, size_t>> &outcol, const vector<bool> &oneortwo, unordered_map<string, size_t>::iterator first_tab);
    void generate();
    void quit();
    Sillyql(){};
    ~Sillyql(){};
};
