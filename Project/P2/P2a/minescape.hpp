// Project identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include "P2random.h"
using namespace std;

struct map_tile
{
    int property;
    bool dis = false;
};


struct Tile
{
    Tile(): property{0}, row{0}, col{0}{}
    Tile(int pro, uint32_t r, uint32_t c):
        property{pro}, row{r}, col{c}{}
    int property;
    uint32_t row;
    uint32_t col;
    bool dis = false;
};

struct tilecomp
{
    bool operator()(const Tile &enter, const Tile &in){
        if (enter.property == in.property){
            if (enter.col == in.col){
                return enter.row > in.row;
            }
            else{
                return enter.col > in.col;
            }
        }
        else{
            return enter.property > in.property;
        }
    } // tile of smaller property has higher priority
};

struct tilecomp_max
{
    bool operator()(const Tile &in, const Tile &enter){
        if (enter.property == in.property){
            if (enter.col == in.col){
                return enter.row > in.row;
            }
            else{
                return enter.col > in.col;
            }
        }
        else{
            return enter.property > in.property;
        }
    } // tile of bigger property has higher priority
};

class mine
{
private:
    vector<vector<map_tile>> map;
    priority_queue<Tile, vector<Tile>, tilecomp> dis_q;
    priority_queue<Tile, vector<Tile>, tilecomp> tnt;
    priority_queue<int, vector<int>, greater<int>> rightpq; // min pq
    priority_queue<int, vector<int>> leftpq; // max pq
    vector<Tile> discovered;
    size_t N;
    uint32_t cur_row;
    uint32_t cur_col;
    uint32_t clear_tile;
    uint32_t clear_rubble;
    uint32_t sz;
    bool v;
    bool m;
    bool s;
    bool zero;
public:
    mine(): N{0}, cur_row{0}, cur_col{0}, clear_tile{0}, clear_rubble{0}, sz{0}, v{false}, m{false}, s{false}{};
    ~mine(){};
    void option_r(int argc, char *argv[]);
    void readin();
    void discover(const Tile &cur_tile);
    void move();
    void tnt_boom();
    void get_median(int tile_pro);
    void print_map();
};
