// IDENTIFIER  = 1761414855B69983BD8035097EFBD312EB0527F0

#include <cmath>
#include <cstddef>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

enum area { medical_c, main_c, border };
// Part A
// MST distance functor
// cross campus return infinity
// determine if a point read x,y store category(ennum)
// struct xy seperate prem d k p
// math distance calculation double (convert from int) overflow cast to double
// first
struct position {
  int x;
  int y;
  area category;
  position(int x, int y, area c) : x(x), y(y), category(c) {}
};

struct edge {
  int start;
  int end;
  double length;
  edge(int s, int e, double l) : start(s), end(e), length(l) {}
  edge() : start(0), end(0), length(-1) {}
};

struct pos_b {
  int x;
  int y;
  pos_b(int x, int y) : x(x), y(y) {}
};

struct prim {
  bool k;
  double d; // squared distance
  int p;
  prim(bool k, double d, int p) : k(k), d(d), p(p) {}
  // Default constructor
  prim() : k(false), d(numeric_limits<double>::infinity()), p(-1) {}
};

template <typename T> double edge_calc(vector<T> &vertex_list, int i, int j) {
  return sqrt(pow(static_cast<double>(vertex_list[i].x) -
                      static_cast<double>(vertex_list[j].x),
                  2) +
              pow(static_cast<double>(vertex_list[i].y) -
                      static_cast<double>(vertex_list[j].y),
                  2));
}

template <typename T>
double path_calc(vector<T> &vertex_list, vector<edge> &path, int i, int j) {
  return edge_calc<T>(vertex_list, path[j].start, i) +
         edge_calc<T>(vertex_list, path[j].end, i);
}

int read_input_bc(vector<edge> &path, vector<pos_b> &vertex_list) {
  int n;
  cin >> n;
  vertex_list.reserve(n);
  path.reserve(n);
  for (auto i = 0; i < n; i++) {
    int x;
    int y;
    cin >> x >> y;
    vertex_list.emplace_back(x, y);
  }
  return n;
}
double MST() {
  double weight = 0;
  vector<prim> prim_table;
  vector<position> vertex_list;
  int n;
  cin >> n;
  vertex_list.reserve(n);
  prim_table.resize(n);
  for (auto i = 0; i < n; i++) {
    int x;
    int y;
    cin >> x >> y;
    if (x > 0 || y > 0) {
      vertex_list.emplace_back(x, y, area::main_c);
    } else if (x < 0 && y < 0) {
      vertex_list.emplace_back(x, y, area::medical_c);
    } else {
      vertex_list.emplace_back(x, y, area::border);
    }
  }
  prim_table[0].d = 0;
  prim_table[0].k = true;
  for (auto i = 1; i < n; i++) {
    if (vertex_list[0].category == vertex_list[i].category ||
        vertex_list[0].category == area::border ||
        vertex_list[i].category == area::border) {

      prim_table[i].d = edge_calc<position>(vertex_list, 0, i);
      prim_table[i].p = 0;
    }
  }

  for (auto i = 1; i < n; i++) {
    double min_d = numeric_limits<double>::infinity();
    int min_index = 0;
    for (auto i = 1; i < n; i++) {
      if (!prim_table[i].k) {
        if (min_d > prim_table[i].d) {
          min_d = prim_table[i].d;
          min_index = i;
        }
      }
    }                               // find smallest d to update k
    prim_table[min_index].k = true; // set k as true

    for (auto i = 0; i < n; i++) {
      if (!prim_table[i].k) {
        double temp_d = numeric_limits<double>::infinity();
        if (vertex_list[min_index].category == vertex_list[i].category ||
            vertex_list[min_index].category == area::border ||
            vertex_list[i].category == area::border) {
          temp_d = edge_calc<position>(vertex_list, min_index, i);
        }
        if (prim_table[i].d > temp_d) {
          prim_table[i].d = temp_d;
          prim_table[i].p = min_index;
        }
      }
    }
  }
  for (auto prim : prim_table) {
    weight += prim.d;
  }
  cout << std::fixed << std::setprecision(2) << weight << '\n';
  for (auto i = 1; i < n; i++) {
    if (prim_table[i].p > i) {
      cout << i << ' ' << prim_table[i].p << '\n';
    } else {
      cout << prim_table[i].p << ' ' << i << '\n';
    }
  }
  return weight;
}

double MST_helper(vector<prim> &prim_table, vector<pos_b> vertix_list,
                  int permlength, vector<int> &path) {
  size_t n = path.size() - permlength;
  double weight = 0;
  prim_table[0].d = 0;
  prim_table[0].k = true;
  for (size_t i = 1; i < n; i++) {
    prim_table[i].d =
        pow(static_cast<double>(vertix_list[path[i + permlength]].x) -
                static_cast<double>(vertix_list[path[permlength]].x),
            2) +
        pow(static_cast<double>(vertix_list[path[i + permlength]].y) -
                static_cast<double>(vertix_list[path[permlength]].y),
            2);
    prim_table[i].p = 0;
  }

  for (size_t i = 1; i < n; i++) {
    double min_d = numeric_limits<double>::infinity();
    size_t min_index = 0;
    for (size_t j = 1; j < n; j++) {
      if (!prim_table[j].k) {
        if (min_d > prim_table[j].d) {
          min_d = prim_table[j].d;
          min_index = j;
        }
      }
    }                               // find smallest d to update k
    prim_table[min_index].k = true; // set k as true
    for (size_t j = 0; j < n; j++) {
      if (!prim_table[j].k) {
        double temp_d = numeric_limits<double>::infinity();
        temp_d = pow(static_cast<double>(vertix_list[path[j + permlength]].x) -
                         static_cast<double>(
                             vertix_list[path[min_index + permlength]].x),
                     2) +
                 pow(static_cast<double>(vertix_list[path[j + permlength]].y) -
                         static_cast<double>(
                             vertix_list[path[min_index + permlength]].y),
                     2);
        if (prim_table[j].d > temp_d) {
          prim_table[j].d = temp_d;
          prim_table[j].p = static_cast<int>(min_index);
        }
      }
    }
  }
  for (auto prim : prim_table) {
    weight += sqrt(prim.d);
  }
  return weight;
}

// Part B
// search for TSP heuristic
// O(n^2)
// arbitrary insertion
double Arbitrary_Insertion_helper(vector<edge> &path,
                                  vector<pos_b> &vertex_list, int n,
                                  vector<int> &optimal) {
  path.emplace_back(0, 1, edge_calc<pos_b>(vertex_list, 0, 1));
  path.emplace_back(1, 2, edge_calc<pos_b>(vertex_list, 1, 2));
  path.emplace_back(2, 0, edge_calc<pos_b>(vertex_list, 2, 0));

  // int curr_path = 3;
  for (auto i = 3; i < n; i++) {
    double min_d = path_calc(vertex_list, path, i, 0) - path[0].length;
    int min_index = 0;
    for (auto j = 1; j < i; j++) {
      double temp_d = path_calc(vertex_list, path, i, j) - path[j].length;
      if (temp_d < min_d) {
        min_index = j;
        min_d = temp_d;
      }
    }
    path.emplace_back(i, path[min_index].end,
                      edge_calc<pos_b>(vertex_list, i, path[min_index].end));
    path[min_index].end = i;
    path[min_index].length =
        edge_calc<pos_b>(vertex_list, path[min_index].start, i);
  }
  int i = 0;
  double weight = 0;
  vector<int> sequence;
  while (true) {
    weight += path[i].length;
    optimal.push_back(i);
    i = path[i].end;
    if (i == 0) {
      break;
    }
  }
  return weight;
}

void Arbitrary_Insertion() {
  vector<edge> path;
  vector<pos_b> vertex_list;
  vector<int> sequence;
  int n = read_input_bc(path, vertex_list);
  double weight = Arbitrary_Insertion_helper(path, vertex_list, n, sequence);
  cout << std::fixed << std::setprecision(2) << weight << '\n';
  for (auto vertex : sequence) {
    cout << vertex << ' ';
  }
  cout << '\n';
}

//

void Optimal() {

  class OPT {
  private:
    vector<int> path;
    vector<int> optimal;
    vector<pos_b> vertex_list;
    int n;
    double estimate;
    double weight;

  public:
    void read_input() {
      cin >> n;
      vector<edge> sequence;
      vertex_list.reserve(n);
      sequence.reserve(n);
      for (auto i = 0; i < n; i++) {
        int x;
        int y;
        cin >> x >> y;
        vertex_list.emplace_back(x, y);
      }
      estimate = Arbitrary_Insertion_helper(sequence, vertex_list, n, optimal);
      path = optimal;
      weight = 0;
    }
    bool promising(int permlength) {
      if (path.size() - permlength < 5) {
        return true;
      }
      vector<prim> prim_table;
      prim_table.resize(path.size() - permlength);
      double MST_weight = MST_helper(prim_table, vertex_list, permlength, path);
      double min_1 = numeric_limits<double>::max();
      double min_2 = numeric_limits<double>::max();
      for (size_t j = permlength; j < path.size(); j++) {
        double temp_1 = edge_calc<pos_b>(vertex_list, static_cast<int>(path[0]),
                                         static_cast<int>(path[j]));
        double temp_2 = edge_calc<pos_b>(vertex_list,
                                         static_cast<int>(path[permlength - 1]),
                                         static_cast<int>(path[j]));
        if (temp_1 < min_1) {
          min_1 = temp_1;
        }
        if (temp_2 < min_2) {
          min_2 = temp_2;
        }
      }
      double promise = weight + MST_weight + min_1 + min_2;
      // for (size_t i = 0; i < path.size(); ++i)
      //   cerr << setw(2) << path[i] << ' ';
      // cerr << setw(4) << permlength << setw(10) << weight;
      // cerr << setw(10) << min_1 << setw(10) << min_2;
      // cerr << setw(10) << MST_weight << setw(10) << promise << "  "
      //      << (promise < estimate) << '\n';
      return promise < estimate;
    }
    void genPerms(size_t permLength) {
      if (permLength == path.size()) {
        // Do something with the path
        // add closing edge
        // check if update better than best so far
        // subtract closing edge
        // update path length
        weight += edge_calc(vertex_list, path.back(), path.front());
        if (weight < estimate) {
          estimate = weight;
          optimal = path;
        }
        weight -= edge_calc(vertex_list, path.back(), path.front());
        return;
      } // if ..complete path

      if (!promising(static_cast<int>(permLength))) {
        return;
      } // if ..not promising

      for (size_t i = permLength; i < path.size(); ++i) {
        swap(path[permLength], path[i]);
        // curL += (New edge length)
        weight +=
            edge_calc(vertex_list, path[permLength - 1], path[permLength]);
        genPerms(permLength + 1);
        // curL -= (New edge length)
        weight -=
            edge_calc(vertex_list, path[permLength - 1], path[permLength]);
        swap(path[permLength], path[i]);
      } // for ..unpermuted elements
    }   // genPerms()

    void print() {
      cout << fixed << setprecision(2) << estimate << '\n';
      for (auto node : optimal) {
        cout << node << ' ';
      }
      cout << '\n';
    }
  };
  OPT opt;
  opt.read_input();
  opt.genPerms(1);
  opt.print();
  return;
}

void getOptions(int argc, char *argv[]) {
  opterr = false;
  int choice;
  int index = 0;
  option long_options[] = {
      {"help", no_argument, nullptr, 'h'},
      {"mode", required_argument, nullptr, 'm'},
      {nullptr, 0, nullptr, '\0'},
  };

  while ((choice = getopt_long(argc, argv, "hm:", long_options, &index)) !=
         -1) {
    switch (choice) {
    case 'h':
      cout << "This program is a TSP implementation \n";
      exit(0);
      break;

    case 'm':
      switch (optarg[0]) {
      case 'M':
        MST();
        break;
      case 'F':
        Arbitrary_Insertion();
        // Cheapest_Insertion();
        // Nearest_insertion();
        break;
      case 'O':
        Optimal();
        break;
      }
      break;

    default:
      cerr << "Unknown command line option" << endl;
      exit(1);
    }
  }
}
// Part C
// make it a class
// make genperm a member function of the class only paramter size_t permlength
// double running_total
// path fixed perml indicate new edge double index
// start pass at zero first call permlength fixed 1
// get MST fre edge
// do not worry about campus
// reset prim table
// double index
// call part B for optimization
int main(int argc, char *argv[]) {
  ios_base::sync_with_stdio(false);
  getOptions(argc, argv);
  return 0;
}
