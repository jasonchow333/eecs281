
#include "TableEntry.h"
#include "compare.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
struct Table {
  string Tablename;
  vector<EntryType> Col_type;
  unordered_map<string, int> Col_name;
  vector<vector<TableEntry>> Table_content;
  unordered_map<TableEntry, vector<int>> hash_index;
  map<TableEntry, vector<int>> bst_index;
  string index_col;
  char index_type;
};
class SillyQL {
private:
  bool quiet = false;
  unordered_map<string, Table> Main_Table;

public:
  void getOptions(int argc, char *argv[]);
  void read_input();
  void create_table();
  void remove_table();
  void insert_table();
  void print_table();
  void print_helper(TableEntry &entry, char op,
                    unordered_map<string, Table>::iterator it,
                    vector<int> &Col_print, int col_comp);
  template <typename Comp>
  void print_helper2(unordered_map<string, Table>::iterator it,
                     vector<int> &Col_print, int col_comp, TableEntry entry);
  void print_helper_index(unordered_map<string, Table>::iterator it,
                          vector<int> &Col_print, int col_comp,
                          TableEntry entry, char op);
  void delete_table();
  void delete_helper(TableEntry &entry, char op,
                     unordered_map<string, Table>::iterator it, int col_comp);
  void join_table();
  template <typename T>
  void join_index_helper(unordered_map<string, Table>::iterator it1,
                         unordered_map<string, Table>::iterator it2,
                         int colname1, vector<int> &col_print,
                         vector<int> &col_table, T index);
  void generate_table();
};

void SillyQL::getOptions(int argc, char *argv[]) {
  opterr = false;
  int choice;
  int index = 0;
  option long_options[] = {
      {"help", no_argument, nullptr, 'h'},
      {"quiet", no_argument, nullptr, 'q'},
      {nullptr, 0, nullptr, '\0'},
  };

  while ((choice = getopt_long(argc, argv, "hq", long_options, &index)) != -1) {
    switch (choice) {
    case 'h':
      cout << "This program is a SillyQL implementation \n";
      exit(0);
      break;

    case 'q':
      quiet = true;
      break;

    default:
      cerr << "Unknown command line option" << endl;
      exit(1);
    }
  }
}

void SillyQL::read_input() {
  string command;
  do {
    cout << "% ";
    if (!(cin >> command)) { // Check if input operation failed
      // Clear the fail state and ignore any invalid input
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      continue; // Continue to the next iteration of the loop
    }
    if (command[0] == 'C') {
      create_table();
    } else if (command[0] == '#') {
      getline(cin, command);
    } else if (command[0] == 'R') {
      remove_table();
    } else if (command[0] == 'I') {
      insert_table();
    } else if (command[0] == 'P') {
      print_table();
    } else if (command[0] == 'D') {
      delete_table();
    } else if (command[0] == 'J') {
      join_table();
    } else if (command[0] == 'G') {
      generate_table();
    } else if (command[0] == 'Q') {
      cout << "Thanks for being silly!\n";
    } else {
      cout << "Error: unrecognized command\n";
      string command;
      getline(cin, command);
    }
  } while (command[0] != 'Q');
}

void SillyQL::create_table() {
  string command;
  cin >> command;
  if (Main_Table.find(command) != Main_Table.end()) {
    cout << "Error during <COMMAND>: Cannot create already existing table "
            "<tablename> \n";
    getline(cin, command);
  } else {
    cout << "New table " << command << " with column(s) ";
    auto &it = Main_Table[command];
    it.Tablename = command;
    int n;
    cin >> n;
    it.Col_name.reserve(n);
    it.Col_type.reserve(n);
    for (auto i = 0; i < n; i++) {
      cin >> command;
      if (command == "string") {
        it.Col_type.emplace_back(EntryType::String);
      } else if (command == "int") {
        it.Col_type.emplace_back(EntryType::Int);
      } else if (command == "double") {
        it.Col_type.emplace_back(EntryType::Double);
      } else if (command == "bool") {
        it.Col_type.emplace_back(EntryType::Bool);
      }
    }
    for (auto i = 0; i < n; i++) {
      cin >> command;
      it.Col_name.emplace(command, i);
      cout << command << ' ';
    }
    cout << "created\n";
  }
}

void SillyQL::remove_table() {
  string tablename;
  cin >> tablename;
  auto it = Main_Table.find(tablename);
  if (it == Main_Table.end()) {
    cout << "Error during REMOVE: " << tablename
         << " does not name a table in the database\n";
    getline(cin, tablename);
  } else {
    Main_Table.erase(it);
    cout << "Table " << tablename << " removed\n";
  }
}

void SillyQL::insert_table() {
  string command;
  cin >> command; // Read in INTO
  cin >> command; // Read in tablename
  auto it = Main_Table.find(command);
  if (it == Main_Table.end()) {
    cout << "Error during INSERT: " << command
         << " does not name a table in the database\n";
    getline(cin, command);
  } else {
    int n;
    cin >> n;
    int m = static_cast<int>(it->second.Col_type.size());
    int curr_row = static_cast<int>(it->second.Table_content.size());
    it->second.Table_content.reserve(curr_row + n);
    cin >> command; // read row
    int col_index;
    for (auto i = 0; i < n; i++) {
      vector<TableEntry> row;
      row.reserve(m);
      for (auto j = 0; j < m; j++) {
        switch (it->second.Col_type[j]) {
        case EntryType::String: {
          string input_s;
          cin >> input_s;
          row.emplace_back(input_s);
          break;
        }
        case EntryType::Bool: {
          string input_b;
          cin >> input_b;
          row.emplace_back(input_b);
          break;
        }
        case EntryType::Double: {
          double input_d;
          cin >> input_d;
          row.emplace_back(input_d);
          break;
        }
        case EntryType::Int: {
          int input_i;
          cin >> input_i;
          row.emplace_back(input_i);
          break;
        }
        }
      }
      it->second.Table_content.emplace_back(row);
      if (it->second.index_type == 'h') {
        col_index = it->second.Col_name[it->second.index_col];
        it->second.hash_index[row[col_index]].push_back(curr_row);
      } else if (it->second.index_type == 'b') {
        col_index = it->second.Col_name[it->second.index_col];
        it->second.bst_index[row[col_index]].push_back(curr_row);
      }
      curr_row++;
    }
    cout << "Added " << n << " rows to " << it->second.Tablename
         << " from position " << curr_row - n << " to " << curr_row - 1 << "\n";
  }
}

void SillyQL::print_table() {
  string command;
  cin >> command; // FROM
  cin >> command;
  auto it = Main_Table.find(command);
  if (it == Main_Table.end()) {
    cout << "Error during PRINT: " << command
         << " does not name a table in the database\n";
    getline(cin, command);
    return;
  }
  int n;
  cin >> n;
  vector<string> col_print_name;
  vector<int> col_print;
  col_print_name.resize(n);
  col_print.resize(n);
  for (auto i = 0; i < n; i++) {
    cin >> command;
    auto col = it->second.Col_name.find(command);
    if (col == it->second.Col_name.end()) {
      cout << "Error during PRINT: " << command << " does not name a column in "
           << it->second.Tablename << "\n";
      getline(cin, command);
      return;
    }
    col_print_name[i] = command;
    col_print[i] = col->second;
  }
  cin >> command;

  if (command[0] == 'A') {
    if (!quiet) {
      for (auto j = 0; j < n; j++) {
        cout << col_print_name[j] << " ";
      }
      cout << '\n';
      for (size_t i = 0; i < it->second.Table_content.size(); i++) {
        for (auto col : col_print) {
          cout << it->second.Table_content[i][col] << ' ';
        }
        cout << '\n';
      }
    }
    cout << "Printed " << it->second.Table_content.size()
         << " matching rows from " << it->second.Tablename << "\n";

  } else {
    cin >> command;
    auto col_comp = it->second.Col_name.find(command);
    if (col_comp == it->second.Col_name.end()) {
      cout << "Error during PRINT: " << command << " does not name a column in "
           << it->second.Tablename << "\n";
      getline(cin, command);
      return;
    }
    char op;
    cin >> op;
    EntryType type = it->second.Col_type[col_comp->second];
    bool index = false;
    if (it->second.index_col == command) {
      index = true;
    }
    if (!quiet) {
      for (auto j = 0; j < n; j++) {
        cout << col_print_name[j] << " ";
      }
      cout << '\n';
    }

    // four way split of the type
    // read the type and call helper witha table entry conatining the type
    // like Tableentry(fusion helper function 3 way split on the
    // comparison type
    switch (type) {
    case EntryType::String: {
      string input_s;
      cin >> input_s;
      TableEntry entry_s(input_s);
      if (index) {
        print_helper_index(it, col_print, col_comp->second, entry_s, op);
      } else {
        print_helper(entry_s, op, it, col_print, col_comp->second);
      }

      break;
    }
    case EntryType::Bool: {
      string input_b;
      cin >> input_b;
      TableEntry entry_b(input_b);
      if (index) {
        print_helper_index(it, col_print, col_comp->second, entry_b, op);
      } else {
        print_helper(entry_b, op, it, col_print, col_comp->second);
      }
      break;
    }
    case EntryType::Double: {
      double input_d;
      cin >> input_d;
      TableEntry entry_d(input_d);
      if (index) {
        print_helper_index(it, col_print, col_comp->second, entry_d, op);
      } else {
        print_helper(entry_d, op, it, col_print, col_comp->second);
      }
      break;
    }
    case EntryType::Int: {
      int input_i;
      cin >> input_i;
      TableEntry entry_i(input_i);
      if (index) {
        print_helper_index(it, col_print, col_comp->second, entry_i, op);
      } else {
        print_helper(entry_i, op, it, col_print, col_comp->second);
      }
      break;
    }
    }
  }
}

template <typename Comp>
void SillyQL::print_helper2(unordered_map<string, Table>::iterator it,
                            vector<int> &Col_print, int col_comp,
                            TableEntry entry) {
  int number_row = 0;
  Comp comp(entry, col_comp);
  for (size_t i = 0; i < it->second.Table_content.size(); i++) {
    if (comp(it->second.Table_content[i])) {
      if (!quiet) {
        for (auto col : Col_print) {
          cout << it->second.Table_content[i][col] << ' ';
        }
        cout << '\n';
      }
      number_row++;
    }
  }
  cout << "Printed " << number_row << " matching rows from "
       << it->second.Tablename << "\n";
}

void SillyQL::print_helper(TableEntry &entry, char op,
                           unordered_map<string, Table>::iterator it,
                           vector<int> &Col_print, int col_comp) {
  switch (op) {
  case '>':
    print_helper2<GreaterComparator>(it, Col_print, col_comp, entry);
    break;
  case '=':
    print_helper2<EqualComparator>(it, Col_print, col_comp, entry);
    break;
  case '<':
    print_helper2<LessComparator>(it, Col_print, col_comp, entry);
    break;
  }
}

void SillyQL::print_helper_index(unordered_map<string, Table>::iterator it,
                                 vector<int> &Col_print, int col_comp,
                                 TableEntry entry, char op) {

  int number_row = 0;
  if (it->second.index_type == 'b') {
    switch (op) {
    case '>': {
      auto upper_bound = it->second.bst_index.upper_bound(entry);
      for (auto iter = upper_bound; iter != it->second.bst_index.end();
           ++iter) {
        for (auto num : iter->second) {
          if (!quiet) {
            for (auto col : Col_print) {
              cout << it->second.Table_content[num][col] << ' ';
            }
            cout << '\n';
          }
          number_row++;
        }
      }
      break;
    }

    case '=': {
      auto iter = it->second.bst_index.find(entry);
      if (iter != it->second.bst_index.end()) {
        if (!quiet) {
          for (auto num : iter->second) {
            for (auto col : Col_print) {
              cout << it->second.Table_content[num][col] << ' ';
            }
            cout << '\n';
          }
        }
        number_row = static_cast<int>(iter->second.size());
      }
      break;
    }

    case '<': {
      auto lower_bound = it->second.bst_index.lower_bound(entry);
      for (auto iter = it->second.bst_index.begin(); iter != lower_bound;
           ++iter) {

        for (auto num : iter->second) {
          if (!quiet) {
            for (auto col : Col_print) {
              cout << it->second.Table_content[num][col] << ' ';
            }
            cout << '\n';
          }
          number_row++;
        }
      }
      break;
    }
    }
    cout << "Printed " << number_row << " matching rows from "
         << it->second.Tablename << "\n";
  } else {
    switch (op) {
    case '>':
      print_helper2<GreaterComparator>(it, Col_print, col_comp, entry);
      break;
    case '=': {
      auto iter = it->second.hash_index.find(entry);
      if (iter != it->second.hash_index.end()) {
        if (!quiet) {
          for (auto num : iter->second) {
            for (auto col : Col_print) {
              cout << it->second.Table_content[num][col] << ' ';
            }
            cout << '\n';
          }
        }
        number_row = static_cast<int>(iter->second.size());
      }
      cout << "Printed " << number_row << " matching rows from "
           << it->second.Tablename << "\n";
      break;
    }

    case '<':
      print_helper2<LessComparator>(it, Col_print, col_comp, entry);
      break;
    }
  }
}

void SillyQL::delete_table() {
  string command;
  cin >> command; // read in from
  cin >> command;
  auto it = Main_Table.find(command);
  if (it == Main_Table.end()) {
    cout << "Error during DELETE: " << command
         << "does not name a table in the database\n";
    getline(cin, command);
    return;
  }
  cin >> command; // read in where
  cin >> command;
  auto it2 = it->second.Col_name.find(command);
  if (it2 == it->second.Col_name.end()) {
    cout << "Error during DELETE: " << command
         << " does not name a table in the database\n";
    getline(cin, command);
    return;
  }
  EntryType type = it->second.Col_type[it2->second];
  char op;
  cin >> op;
  switch (type) {
  case EntryType::String: {
    string input_s;
    cin >> input_s;
    TableEntry entry_s(input_s);
    delete_helper(entry_s, op, it, it2->second);
    break;
  }
  case EntryType::Bool: {
    string input_b;
    cin >> input_b;
    TableEntry entry_b(input_b);
    delete_helper(entry_b, op, it, it2->second);
    break;
  }
  case EntryType::Double: {
    double input_d;
    cin >> input_d;
    TableEntry entry_d(input_d);
    delete_helper(entry_d, op, it, it2->second);
    break;
  }
  case EntryType::Int: {
    int input_i;
    cin >> input_i;
    TableEntry entry_i(input_i);
    delete_helper(entry_i, op, it, it2->second);
    break;
  }
  }
  if (it->second.index_type == 'b') {
    it->second.bst_index.clear();
    int index_col = it->second.Col_name[it->second.index_col];
    for (size_t i = 0; i < it->second.Table_content.size(); i++) {
      it->second.bst_index[it->second.Table_content[i][index_col]].push_back(
          static_cast<int>(i));
    }
  } else if (it->second.index_type == 'h') {
    it->second.hash_index.clear();
    int index_col = it->second.Col_name[it->second.index_col];
    for (size_t i = 0; i < it->second.Table_content.size(); i++) {
      it->second.hash_index[it->second.Table_content[i][index_col]].push_back(
          static_cast<int>(i));
    }
  }
}

void SillyQL::delete_helper(TableEntry &entry, char op,
                            unordered_map<string, Table>::iterator it,
                            int col_comp) {
  switch (op) {
  case '>': {
    GreaterComparator comp(entry, col_comp);
    auto new_end = remove_if(it->second.Table_content.begin(),
                             it->second.Table_content.end(), comp);
    cout << "Deleted " << it->second.Table_content.end() - new_end
         << " rows from " << it->second.Tablename << '\n';
    it->second.Table_content.erase(new_end, it->second.Table_content.end());

    break;
  }

  case '=': {
    EqualComparator comp(entry, col_comp);
    auto new_end = remove_if(it->second.Table_content.begin(),
                             it->second.Table_content.end(), comp);
    cout << "Deleted " << it->second.Table_content.end() - new_end
         << " rows from " << it->second.Tablename << '\n';
    it->second.Table_content.erase(new_end, it->second.Table_content.end());

    break;
  }

  case '<': {
    LessComparator comp(entry, col_comp);
    auto new_end = remove_if(it->second.Table_content.begin(),
                             it->second.Table_content.end(), comp);
    cout << "Deleted " << it->second.Table_content.end() - new_end
         << " rows from " << it->second.Tablename << '\n';
    it->second.Table_content.erase(new_end, it->second.Table_content.end());

    break;
  }
  }
}

void SillyQL::join_table() {
  string command;
  cin >> command;
  auto it1 = Main_Table.find(command);
  if (it1 == Main_Table.end()) {
    cout << "Error during JOIN: " << command
         << "does not name a table in the database\n";
    getline(cin, command);
    return;
  }
  cin >> command; // read and
  cin >> command;
  auto it2 = Main_Table.find(command);
  if (it2 == Main_Table.end()) {
    cout << "Error during JOIN: " << command
         << " does not name a table in the database\n";
    getline(cin, command);
    return;
  }
  cin >> command; // read where

  cin >> command;
  auto col1 = it1->second.Col_name.find(command);
  if (col1 == it1->second.Col_name.end()) {
    cout << "Error during JOIN: " << command << " does not name a column in "
         << it1->second.Tablename << "\n";
    getline(cin, command);
    return;
  }

  cin >> command; // read =

  cin >> command;
  auto col2 = it2->second.Col_name.find(command);
  if (col2 == it2->second.Col_name.end()) {
    cout << "Error during JOIN: " << command << " does not name a column in "
         << it2->second.Tablename << "\n";
    getline(cin, command);
    return;
  }
  string index_col = command;

  cin >> command; // read and
  cin >> command; // read print
  int n;
  cin >> n;
  vector<string> col_print_name;
  col_print_name.reserve(n);
  vector<int> col_print;
  col_print.reserve(n);
  vector<int> col_table;
  col_table.reserve(n);
  for (auto i = 0; i < n; i++) {
    int table_num;
    cin >> command;
    cin >> table_num;
    if (table_num == 1) {
      auto coln = it1->second.Col_name.find(command);
      if (coln == it2->second.Col_name.end()) {
        cout << "Error during JOIN: " << command
             << " does not name a column in " << it2->second.Tablename << "\n";
        getline(cin, command);
        return;
      }
      col_print_name.push_back(command);
      col_print.push_back(coln->second);
      col_table.push_back(table_num);

    } else {
      auto coln = it2->second.Col_name.find(command);
      if (coln == it2->second.Col_name.end()) {
        cout << "Error during JOIN: " << command
             << " does not name a column in " << it2->second.Tablename << "\n";
        getline(cin, command);
        return;
      }
      col_print_name.push_back(command);
      col_print.push_back(coln->second);
      col_table.push_back(table_num);
    }
  }
  if (!quiet) {
    for (auto colname_print : col_print_name) {
      cout << colname_print << ' ';
    }
    cout << '\n';
  }

  int colname1 = col1->second;

  if (it2->second.index_col != command) {
    unordered_map<TableEntry, vector<int>> temp_join_table;
    for (size_t i = 0; i < it2->second.Table_content.size(); i++) {
      temp_join_table[it2->second.Table_content[i][col2->second]].emplace_back(
          i);
    }
    join_index_helper(it1, it2, colname1, col_print, col_table,
                      temp_join_table);
  } else {
    if (it2->second.index_type == 'b') {
      join_index_helper(it1, it2, colname1, col_print, col_table,
                        it2->second.bst_index);
    } else if (it2->second.index_type == 'h') {
      join_index_helper(it1, it2, colname1, col_print, col_table,
                        it2->second.hash_index);
    }
  }

  // for (size_t i = 0; i < it1->second.Table_content.size(); i++) {
  //   for (size_t j = 0; j < it2->second.Table_content.size(); j++) {
  //     if (it1->second.Table_content[i][colname1] ==
  //         it2->second.Table_content[j][colname2]) {
  //       if (!quiet) {
  //         for (size_t k = 0; k < col_print.size(); k++) {
  //           if (col_table[k] == 1) {
  //             cout << it1->second.Table_content[i][col_print[k]] << ' ';
  //           } else {
  //             cout << it2->second.Table_content[j][col_print[k]] << ' ';
  //           }
  //         }
  //         cout << '\n';
  //       }
  //       number_row++;
  //     }
  //   }
  // }
  // cout << "Printed " << number_row << " rows from joining "
  //      << it1->second.Tablename << " to " << it2->second.Tablename << "\n";

  // Iterate through the first table <tablename1> from beginning to end.
  // For each row’s respective <colname1> value in <tablename1>, find
  // matching <colname2> values in <tablename2>, if any exist. For each
  // match found, print the column values in the matching rows in the order
  // specified by the JOIN command. The matching rows from the second table
  // must be selected in the order of insertion into that table. If no rows
  // in the second table match a row in the first table, that row is ignored
  // from the join.
}
template <typename T>
void SillyQL::join_index_helper(unordered_map<string, Table>::iterator it1,
                                unordered_map<string, Table>::iterator it2,
                                int colname1, vector<int> &col_print,
                                vector<int> &col_table, T index) {
  int number_row = 0;
  for (size_t i = 0; i < it1->second.Table_content.size(); i++) {
    auto iter = index.find(it1->second.Table_content[i][colname1]);
    if (iter != index.end()) {
      for (auto col2 : iter->second) {
        if (!quiet) {
          for (size_t k = 0; k < col_print.size(); k++) {
            if (col_table[k] == 1) {
              cout << it1->second.Table_content[i][col_print[k]] << " ";
            } else {
              cout << it2->second.Table_content[col2][col_print[k]] << ' ';
            }
          }
          cout << '\n';
        }
        number_row++;
      }
    }
  }
  cout << "Printed " << number_row << " rows from joining "
       << it1->second.Tablename << " to " << it2->second.Tablename << "\n";
}

void SillyQL::generate_table() {
  string command;
  cin >> command; // read for
  cin >> command;
  auto it = Main_Table.find(command);
  if (it == Main_Table.end()) {
    cout << "Error during GENERATE: " << command
         << "does not name a table in the database\n";
  }
  cin >> command;
  char mode;
  mode = command[0];
  cin >> command; // read index
  cin >> command; // read on
  cin >> command; // read colname
  auto col = it->second.Col_name.find(command);
  if (col == it->second.Col_name.end()) {
    cout << "Error during GENERATE: " << command
         << " does not name a column in " << it->second.Tablename << "\n";
  } else {
    it->second.index_col = command;
    int col_num = col->second;
    it->second.bst_index.clear();
    it->second.hash_index.clear();
    if (mode == 'h') {
      it->second.index_type = 'h';
      for (size_t i = 0; i < it->second.Table_content.size(); i++) {
        it->second.hash_index[it->second.Table_content[i][col_num]]
            .emplace_back(i);
      }
      cout << "Created hash index for table " << it->second.Tablename
           << " on column " << command << ", with "
           << it->second.hash_index.size() << " distinct keys\n";

    } else {
      it->second.index_type = 'b';
      for (size_t i = 0; i < it->second.Table_content.size(); i++) {
        it->second.bst_index[it->second.Table_content[i][col_num]].push_back(
            static_cast<int>(i));
      }
      cout << "Created bst index for table " << it->second.Tablename
           << " on column " << command << ", with "
           << it->second.bst_index.size() << " distinct keys\n";
    }
  }
}

int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false); // you should already have this
  cin >> std::boolalpha;            // add these two lines
  cout << std::boolalpha;           // add these two lines
  SillyQL sql;
  sql.getOptions(argc, argv);
  sql.read_input();
  return 0;
}
