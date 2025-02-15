// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "sillyql.hpp"
#include <getopt.h>
#include <iostream>
#include <algorithm>
using namespace std;

void Table::insert(){
    string word;
    double db;
    int val;
    size_t n;
    size_t start = data.size();
    bool bl;
    cin >> n >> word;
    data.resize(start + n);
    for (size_t i = start; i < start + n; i++){
        data[i].reserve(coltype.size());
        for (size_t j = 0; j < coltype.size(); j++){
            switch (coltype[j])
            {
            case EntryType::String:
                cin >> word;
                data[i].emplace_back(TableEntry(word));
                break;
            case EntryType::Double:
                cin >> db;
                data[i].emplace_back(TableEntry(db));
                break;
            case EntryType::Int:
                cin >> val;
                data[i].emplace_back(TableEntry(val));
                break;
            case EntryType::Bool:
                cin >> bl;
                data[i].emplace_back(TableEntry(bl));
                break;
            default:
                break;
            }
        }
    }
    if (!idxtype.empty()){
        idx_update(idxcol, idxtype, idxcol_num);
    }
    cout << "Added " << n << " rows to " << tablename << " from position " << start << " to " << start + n - 1 << "\n";
}

bool Table::colname_err(const string &col, unordered_map<string, size_t>::iterator &pos){
    pos = colname.find(col);
    return pos == colname.end();
}

void Table::print(bool quiet){
    vector<pair<string, size_t>> outcol;
    string coname;
    string option;
    size_t n;
    cin >> n;
    outcol.reserve(n);
    for (size_t i = 0; i < n; i++){
        cin >> coname;
        if (!colname_err(coname, pos)){
            outcol.emplace_back(pos->first, pos->second);
        }
        else{
            cout << "Error during PRINT: " << coname << " does not name a column in " << tablename << "\n";
            getline(cin, coname);
            return;
        }
    }
    cin >> option;
    if (option[0] == 'A'){
        if (!quiet){
            for (size_t i = 0; i < outcol.size(); i++){
                cout << outcol[i].first << ' ';
            }
            cout << "\n";
            for (size_t i = 0; i < data.size(); i++){
                for (size_t j = 0; j < outcol.size(); j++){
                    cout << data[i][outcol[j].second] << ' ';
                }
                cout << "\n";
            }
        }
        cout << "Printed " << data.size() << " matching rows from " << tablename << "\n";
    }
    else{ //where
        string value;
        size_t p_times = 0;
        char op;
        cin >> coname >> op >> value;
        if (!colname_err(coname, pos)){
            if (!quiet){
                for (size_t i = 0; i < outcol.size(); i++){
                    cout << outcol[i].first << ' ';
                }
                cout << "\n";
            }
            size_t idx = pos->second;
            switch (op)
            {
            case '>':
                if (idxcol == coname && idxtype == "bst"){
                    TableEntry bst_en = make_entry(value, coltype[idxcol_num]);
                    p_times = print_bst(bst.upper_bound(bst_en), bst.end(), outcol, quiet);
                }
                else{
                    p_times = print_where(data.begin(), data.end(), Greater(idx, make_entry(value, coltype[idx])), quiet, outcol);
                }
                break;
            case '=':
                if (idxcol == coname && idxtype == "bst"){
                    TableEntry bst_en = make_entry(value, coltype[idxcol_num]);
                    auto bstit = bst.find(bst_en);
                    if (bstit != bst.end()){
                        p_times = bstit->second.size();
                        if (!quiet){
                            for (auto i : bstit->second){
                                for (auto j : outcol){
                                    cout << data[i][j.second] << ' ';
                                }
                                cout << '\n';
                            }
                        }
                    }
                }
                else if (idxcol == coname && idxtype == "hash"){
                    TableEntry hash_en = make_entry(value, coltype[idxcol_num]);
                    auto hashit = hash.find(hash_en);
                    if (hashit != hash.end()){
                        p_times = hashit->second.size();
                        if (!quiet){
                            for (auto i : hashit->second){
                                for (auto j : outcol){
                                    cout << data[i][j.second] << ' ';
                                }
                                cout << '\n';
                            }
                        }
                    }
                }
                else{
                    p_times = print_where(data.begin(), data.end(), Equal(idx, make_entry(value, coltype[idx])), quiet, outcol);
                }
                break;
            case '<':
                if (idxcol == coname && idxtype == "bst"){
                    TableEntry bst_en = make_entry(value, coltype[idxcol_num]);
                    p_times = print_bst(bst.begin(), bst.lower_bound(bst_en), outcol, quiet);
                }
                else{
                    p_times = print_where(data.begin(), data.end(), Less(idx, make_entry(value, coltype[idx])), quiet, outcol);
                }
                break;
            default:
                break;
            }
            cout << "Printed " << p_times << " matching rows from " << tablename << "\n";
        }
        else{
            cout << "Error during PRINT: " << coname << " does not name a column in " << tablename << "\n";
            return;
        }
    }
}

size_t Table::print_bst(map<TableEntry, vector<size_t>>::iterator begin, map<TableEntry, vector<size_t>>::iterator end, const vector<pair<string, size_t>> &outcol, bool quiet){
    size_t print_num = 0;
    while (begin != end)
    {
        print_num += begin->second.size();
        if (!quiet){
            for (auto i : begin->second){
                for (auto j : outcol){
                    cout << data[i][j.second] << ' ';
                }
                cout << "\n";
            }
        }
        begin++;
    }
    return print_num;
}

template <class It, class Compare> size_t print_where(It begin, It end, Compare comp, bool quiet, const vector<pair<string, size_t>> &outcol){
    size_t valid_r = 0;
    while (begin != end){
        if (comp(*begin)){
            if (!quiet){
                for (auto col_it: outcol){
                    cout << begin->at(col_it.second) << ' ';
                }
                cout << "\n";
            }
            valid_r++;
        }
        begin++;
    }
    return valid_r;
}

void Table::deltab(){
    string coname;
    cin >> coname;
    if (!colname_err(coname, pos)){
        auto new_end = data.end();
        string value;
        size_t idx = pos->second;
        size_t ori_sz = data.size();
        char op;
        cin >> op >> value;
        switch (op)
        {
        case '>':
            new_end = remove_if(data.begin(), data.end(), Greater(idx, make_entry(value, coltype[idx])));
            break;
        case '=':
            new_end = remove_if(data.begin(), data.end(), Equal(idx, make_entry(value, coltype[idx])));
            break;
        case '<':
            new_end = remove_if(data.begin(), data.end(), Less(idx, make_entry(value, coltype[idx])));
            break;
        default:
            break;
        }
        data.erase(new_end, data.end());
        if (!idxtype.empty()){
            idx_update(idxcol, idxtype, idxcol_num);
        }
        cout << "Deleted " << ori_sz - data.size() << " rows from " << tablename << "\n";
    }
    else{
        cout << "Error during DELETE: " << coname << " does not name a column in " << tablename << "\n";
        getline(cin, coname);
    }
}

void Table::generate(){
    string coname;
    cin >> idxtype >> coname >> coname >> coname;
    if (!colname_err(coname, pos)){
        size_t sz = idx_update(coname, idxtype, pos->second);
        cout << "Created " << idxtype << " index for table " << tablename << " on column " << coname << ", with " << sz << " distinct keys\n";
        return;
    }
    else{
        cout << "Error during GENERATE: " << coname << " does not name a column in " << tablename << "\n";
    }
}

size_t Table::idx_update(const string &colname, const string &idxtype, const size_t &colnum){
    if (!hash.empty()){
        hash.clear();
    }
    if (!bst.empty()){
        bst.clear();
    }
    if (idxtype[0] == 'h'){
        for (size_t i = 0; i < data.size(); i++){
            hash[data[i][colnum]].emplace_back(i);
        }
        idxcol = colname;
        this->idxtype = idxtype;
        idxcol_num = colnum;
        return hash.size();
    }
    else{
        for (size_t i = 0; i < data.size(); i++){
            bst[data[i][colnum]].emplace_back(i);
        }
        idxcol = colname;
        this->idxtype = idxtype;
        idxcol_num = colnum;
        return bst.size();
    }
}

void Sillyql::option_r(int argc, char *argv[]){
    int option;
    const char* short_options = "hq";
    const struct option long_options[] = {
        {"help",    no_argument,       nullptr, 'h'},
        {"quiet", no_argument,       nullptr, 'q'},
        {nullptr,   0,                 nullptr, 0}  // Terminate the array
    };

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                cout << "Help: This program accepts one optional flags. -q/--quiet\n";
                exit(0);
            case 'q':
                quiet = true;
                break;
            case '?':
                cerr << "Unknown command line option" << endl;
                exit(1);
            default:
                // Should not reach here
                exit(1);
        }
    }
}

bool Sillyql::tabname_err(const string &tabname, unordered_map<string, Table*>::iterator &pos){
    pos = database.find(tabname);
    return pos == database.end();
}

void Sillyql::create(){
    string name; // table name and column name
    string coltype;
    size_t n;
    cin >> name >> n;
    if (database.find(name) == database.end()){
        Table *table = new Table(name);
        cout << "New table " << table->tablename << " with column(s) ";
        table->coltype.reserve(n);
        for (size_t i = 0; i < n; i++){
            cin >> coltype;
            table->coltype.emplace_back(convert(coltype));
        }
        table->colname.reserve(2*n);
        for (size_t i = 0; i < n; i++){
            cin >> name;
            table->colname.emplace(name, i);
            cout << name << ' ';
        }
        database.emplace(table->tablename, table);
        cout << "created\n";
    }
    else{
        getline(cin, coltype);
        cout << "Error during CREATE: Cannot create already existing table " << name << "\n";
    }
}

void Sillyql::insert(){
    string name;
    cin >> name;
    if (tabname_err(name, pos)){
        cout << "Error during INSERT: " << name << " does not name a table in the database\n";
        size_t n;
        cin >> n;
        getline(cin, name);
        return;
    }
    else{
        pos->second->insert();
    }
}

void Sillyql::print(){
    string name;
    cin >> name;
    if (tabname_err(name, pos)){
        cout << "Error during PRINT: " << name << " does not name a table in the database\n";
        getline(cin, name);
        return;
    }
    else{
        pos->second->print(quiet);
    }
}

void Sillyql::remove(){
    string name;
    cin >> name;
    if (tabname_err(name, pos)){
        cout << "Error during REMOVE: " << name << " does not name a table in the database\n";
    }
    else{
        delete pos->second;
        database.erase(pos);
        cout << "Table " << name << " removed\n";
    }
}

void Sillyql::deletable(){
    string name;
    cin >> name;
    if (tabname_err(name, pos)){
        cout << "Error during DELETE: " << name << " does not name a table in the database\n";
        getline(cin, name);
        return;
    }
    else{
        cin >> name;
        pos->second->deltab();
    }
}

void Sillyql::join(){
    unordered_map<string, Table*>::iterator it;
    string tabname1;
    string tabname2;
    string useless;
    cin >> tabname1 >> useless >> tabname2;
    if (tabname_err(tabname1, pos)){
        cout << "Error during JOIN: " << tabname1 << " does not name a table in the database\n";
        getline(cin, useless);
        return;
    }
    else if (tabname_err(tabname2, it)){
        cout << "Error during JOIN: " << tabname2 << " does not name a table in the database\n";
        getline(cin, useless);
        return;
    }
    else{
        string colname1;
        string colname2;
        cin >> useless >> colname1 >> useless >> colname2;
        if (pos->second->colname_err(colname1, pos->second->pos)){
            cout << "Error during JOIN: " << colname1 << " does not name a column in " << tabname1 << "\n";
            getline(cin, useless);
            return;
        }
        else if (it->second->colname_err(colname2, it->second->pos)){
            cout << "Error during JOIN: " << colname2 << " does not name a column in " << tabname2 << "\n";
            getline(cin, useless);
            return;
        }
        else{
            vector<pair<string, size_t>> p_colnames;
            vector<bool> col_location;
            size_t n;
            cin >> useless >> useless >> n;
            p_colnames.resize(n);
            col_location.resize(n);
            for (size_t i = 0; i < n; i++){
                cin >> p_colnames[i].first >> p_colnames[i].second;
                if (p_colnames[i].second == 1){
                    if (pos->second->colname_err(p_colnames[i].first, pos->second->pos)){
                        cout << "Error during JOIN: " << p_colnames[i].first << " does not name a column in " << tabname1 << "\n";
                        getline(cin, useless);
                        return;
                    }
                    else{
                        p_colnames[i].second = pos->second->pos->second;
                        col_location[i] = false;
                    }
                }
                else{
                    if (it->second->colname_err(p_colnames[i].first, it->second->pos)){
                        cout << "Error during JOIN: " << p_colnames[i].first << " does not name a column in " << tabname2 << "\n";
                        getline(cin, useless);
                        return;
                    }
                    else{
                        p_colnames[i].second = it->second->pos->second;
                        col_location[i] = true;
                    }
                }
            }
            if (!quiet){
                for (size_t i = 0; i < n; i++){
                    cout << p_colnames[i].first << ' ';
                }
                cout << "\n";
            }
            unordered_map<string, size_t>::iterator first_tab;
            pos->second->colname_err(colname1, first_tab);
            it->second->colname_err(colname2, it->second->pos);
            if (it->second->idxtype.empty()){
                // no fast search index exist
                it->second->idx_update(colname2, "hash", it->second->pos->second);
            }
            else if (it->second->idxcol != colname2){
                it->second->idx_update(colname2, it->second->idxtype, it->second->pos->second);
            }
            size_t print_num = print_join(pos->second, it->second, quiet, p_colnames, col_location, first_tab);
            cout << "Printed " << print_num << " rows from joining " << tabname1 << " to " << tabname2 << "\n";
        }
    }
}

size_t Sillyql::print_join(Table *table1, Table *table2, bool quiet, const vector<pair<string, size_t>> &outcol, const vector<bool> &oneortwo, unordered_map<string, size_t>::iterator first_tab){
    size_t valid_r = 0;
    auto it = table1->data.begin();
    while (it != table1->data.end()){
        if (table2->idxtype[0] == 'h'){
            auto hash_pos = table2->hash.find(it->at(first_tab->second));
            if (hash_pos != table2->hash.end()){
                valid_r += hash_pos->second.size();
                if (!quiet){
                    for (size_t i = 0; i < hash_pos->second.size(); i++){
                        for (size_t j = 0; j < outcol.size(); j++){
                            if (!oneortwo[j]){
                                cout << it->at(outcol[j].second) << ' ';
                            }
                            else{
                                cout << table2->data[hash_pos->second[i]][outcol[j].second] << ' ';
                            }
                        }
                        cout << '\n';
                    }
                }
            }
        }
        else{
            auto bst_pos = table2->bst.find(it->at(first_tab->second));
            if (bst_pos != table2->bst.end()){
                valid_r += bst_pos->second.size();
                if (!quiet){
                    for (size_t i = 0; i < bst_pos->second.size(); i++){
                        for (size_t j = 0; j < outcol.size(); j++){
                            if (!oneortwo[j]){
                                cout << it->at(outcol[j].second) << ' ';
                            }
                            else{
                                cout << table2->data[bst_pos->second[i]][outcol[j].second] << ' ';
                            }
                        }
                        cout << '\n';
                    }
                }
            }
        }
        it++;
    }
    return valid_r;
}

void Sillyql::generate(){
    string tabname;
    cin >> tabname;
    if (!tabname_err(tabname, pos)){
        pos->second->generate();
    }
    else{
        cout << "Error during GENERATE: " << tabname << " does not name a table in the database\n";
        getline(cin, tabname);
        return;
    }
}

void Sillyql::quit(){
    pos = database.begin();
    while (pos != database.end()){
        delete pos->second;
        pos->second = nullptr;
        pos++;
    }
}

int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    cin >> std::boolalpha;
    cout << std::boolalpha;
    Sillyql sql;
    sql.option_r(argc, argv);
    string cmd = "";
    string useless;
    string name;
    while (cmd[0] != 'Q')
    {
        cout << "% ";
        cin >> cmd;
        if (cin.fail()){
            cerr << " Error: Reading from cin has failed\n";
            exit(1);
        }
        switch (cmd[0])
        {
        case 'C': // create
            sql.create();
            break;
        case '#': // comment
            getline(cin, useless);
            break;
        case 'R': // remove
            sql.remove();
            break;
        case 'I': // insert
            cin >> useless;
            sql.insert();
            break;
        case 'P': // print
            cin >> useless; // "FROM"
            sql.print();
            break;
        case 'D': // delete
            cin >> useless;
            sql.deletable();
            break;
        case 'J': // join
            sql.join();
            break;
        case 'G': // generate
            cin >> useless;
            sql.generate();
            break;
        case 'Q': // quit
            break;
        // case '?':
        //     cout << "Error: unrecognized command\n";
        //     break;
        default:
            getline(cin, useless);
            cout << "Error: unrecognized command\n";
            break;
        }
    }
    sql.quit();
    cout << "Thanks for being silly!\n";
    return 0;
}

EntryType convert(const string &type){
    switch (type[0])
    {
    case 's':
        return EntryType::String;
    case 'd':
        return EntryType::Double;
    case 'i':
        return EntryType::Int;
    case 'b':
        return EntryType::Bool;
    default:
        cout << "Error during create: Wrong column type\n";
        return EntryType::String;
    }
}

bool convert_bl(const string &bl){
    if (bl[0] == 't'){
        return true;
    }
    else{
        return false;
    }
}

TableEntry make_entry(const string &value, EntryType etype){
    switch (etype)
    {
    case EntryType::String:
        return TableEntry(value);
    case EntryType::Double:
        {
            double db = stod(value);
            return TableEntry(db);
        }
    case EntryType::Int:
        {
            int it = stoi(value);
            return TableEntry(it);
        }
    case EntryType::Bool:
        {
            bool bl = convert_bl(value);
            return TableEntry(bl);
        }
    default:
        exit(1);
        break;
    }
}
