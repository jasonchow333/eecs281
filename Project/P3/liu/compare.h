
#include "TableEntry.h"
#include <vector>

using namespace std;

class GreaterComparator {
public:
  TableEntry value;
  int col_num;
  GreaterComparator(TableEntry val, int col) : value(val), col_num(col) {}
  bool operator()(const vector<TableEntry> &other) const {
    return value < other[col_num];
  }
};

class EqualComparator {
public:
  TableEntry value;
  int col_num;
  EqualComparator(TableEntry val, int col) : value(val), col_num(col) {}
  bool operator()(const vector<TableEntry> &other) const {
    return value == other[col_num];
  }
};

class LessComparator {
public:
  TableEntry value;
  int col_num;
  LessComparator(TableEntry val, int col) : value(val), col_num(col) {}
  bool operator()(const vector<TableEntry> &other) const {
    return value > other[col_num];
  }
};
