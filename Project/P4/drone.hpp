// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0
#include <string>
#include <vector>
#include <limits>
using namespace std;

enum Campus {MAIN, MEDICAL, BORDER};

size_t opt_read(int argc, char *argv[]);

struct point_mst
{
    double min_dist = numeric_limits<double>::infinity();
    int x;
    int y;
    size_t neighbor;
    Campus loc;
    bool dis = false;
    point_mst(int px, int py){
        x=px;
        y=py;
        if (x < 0 && y < 0){
            loc = MEDICAL;
        }
        else if ((x == 0 && y <= 0 )||(x <= 0 && y == 0)){
            loc = BORDER;
        }
        else{
            loc = MAIN;
        }
    }
};

struct point
{
    double dist_to_next = 0;
    int x;
    int y;
    size_t next_pt;
    point(int px, int py){
        x=px;
        y=py;
    }
};

struct mst_pt
{
    double min_dist;
    bool dis;
    mst_pt(double m, bool d){
        min_dist = m;
        dis = d;
    }
};

class MST
{
private:
    vector<point_mst> map;
public:
    void read();
    void find();
    double dist_cal(size_t a, size_t b);
    void print();
    MST(){};
    ~MST(){};
};

// class TSP
// {
// protected:
//     vector<point> map;
// public:
//     void read();
//     double dist_cal(size_t a, size_t b);
//     TSP(){};
//     ~TSP(){};
// };

class FAST
{
protected:
    vector<point> map;
    double fast_weight = 0;
public:
    void read();
    double dist_cal(size_t a, size_t b);
    void find();
    void print();
    FAST(){};
    ~FAST(){};
};

class OPT : public FAST
{
private:
    vector<vector<double>> dist_mat;
    vector<mst_pt> mst_c;
    vector<size_t> path; // contains index of points
    vector<size_t> path_best;
    double best_weight = numeric_limits<double>::infinity();
public:
    void read();
    void genPerms(size_t permLength, double path_length);
    bool promising(size_t permLength, double path_length);
    void find();
    void print();
    OPT(){};
    ~OPT(){};
};
