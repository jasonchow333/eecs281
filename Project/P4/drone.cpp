// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0
#include "getopt.h"
#include "string.h"
#include <iostream>
#include <math.h>
#include "drone.hpp"
#include <iomanip>

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    cout << setprecision(2); // Always show 2 decimal places
    cout << fixed;           // Disable scientific notation for large numbers
    size_t mode = opt_read(argc, argv);
    if (mode == 0)
    {
        MST mst;
        mst.read();
        mst.find();
        mst.print();
    }
    if (mode == 1)
    {
        FAST fast;
        fast.read();
        fast.find();
        fast.print();
    }
    if (mode == 2)
    {
        OPT opt;
        opt.read();
        opt.find();
        opt.print();
    }
    return 0;
}

size_t opt_read(int argc, char *argv[])
{
    int option;
    const char *short_options = "hm:";
    const struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"mode", required_argument, nullptr, 'm'},
        {nullptr, 0, nullptr, 0} // Terminate the array
    };

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1)
    {
        switch (option)
        {
        case 'h':
            cout << "Help: This program accepts one required flags. -m/--mode, with one following argument\n";
            exit(0);
        case 'm':
            if (!strcmp(optarg, ""))
            {
                cerr << "Error: No mode specified" << endl;
                exit(1);
            }
            else if (!strcmp(optarg, "MST"))
            {
                return 0;
            }
            else if (!strcmp(optarg, "FASTTSP"))
            {
                return 1;
            }
            else if (!strcmp(optarg, "OPTTSP"))
            {
                return 2;
            }
            else
            {
                cerr << "Error: Invalid mode" << endl;
                exit(1);
            }
            break;
        case '?':
            cerr << "Error: Invalid command line option" << endl;
            exit(1);
        default:
            // Should not reach here
            exit(1);
        }
    }
    return 0;
}

void MST::read()
{
    size_t n;
    cin >> n;
    map.reserve(n);
    bool main_c = false;
    bool medi_c = false;
    bool border = false;
    for (size_t i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        map.emplace_back(x, y);
        switch (map[i].loc)
        {
        case MAIN:
            if (!main_c)
                main_c = true;
            break;
        case MEDICAL:
            if (!medi_c)
                medi_c = true;
            break;
        case BORDER:
            if (!border)
                border = true;
            break;
        default:
            break;
        }
    }
    if (medi_c && main_c && !border)
    {
        cerr << "Cannot construct MST" << endl;
        exit(1);
    }
}

void MST::find()
{
    map[0].min_dist = 0;
    double cur_dist = 0; // distance between mst and current point
    double min_dist;
    double weight = 0;
    size_t pos = 0; // discovered position from last round
    for (size_t i = 0; i < map.size(); i++)
    {
        min_dist = numeric_limits<double>::infinity();
        for (size_t j = 0; j < map.size(); j++)
        { // find smallest d_v
            if (!map[j].dis)
            {
                if (map[j].min_dist < min_dist)
                {
                    min_dist = map[j].min_dist;
                    pos = j;
                }
            }
        }
        map[pos].dis = true; // set it discovered
        for (size_t j = 0; j < map.size(); j++)
        { // update minimum distance
            if (!map[j].dis)
            {
                cur_dist = dist_cal(pos, j);
                if (cur_dist < map[j].min_dist)
                {
                    map[j].min_dist = cur_dist;
                    map[j].neighbor = pos;
                }
            }
        }
        weight += map[pos].min_dist;
    }
    cout << weight << "\n";
}

double MST::dist_cal(size_t a, size_t b)
{
    if (map[a].loc + map[b].loc == 1)
    { // one at main, one at medical
        return numeric_limits<double>::infinity();
    }
    double dx = map[a].x - map[b].x;
    double dy = map[a].y - map[b].y;
    return sqrt(dx * dx + dy * dy);
}

void MST::print()
{
    for (size_t i = 1; i < map.size(); i++)
    {
        cout << (i < map[i].neighbor ? i : map[i].neighbor) << " " << (i < map[i].neighbor ? map[i].neighbor : i) << "\n";
    }
}

void FAST::read()
{
    size_t n;
    cin >> n;
    int x, y;
    map.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        cin >> x >> y;
        map.emplace_back(x, y);
    }
}

double FAST::dist_cal(size_t a, size_t b)
{
    double dx = map[a].x - map[b].x;
    double dy = map[a].y - map[b].y;
    return sqrt(dx * dx + dy * dy);
}

void FAST::find()
{                        // random insertion
    double cur_dist = 0; // distance between mst and current point
    double min_dist;
    size_t ins_pos = 0;
    map[0].next_pt = 0;
    for (size_t i = 1; i < map.size(); i++)
    {
        min_dist = numeric_limits<double>::infinity();
        for (size_t j = 0; j < i; j++)
        { // find insertion position
            cur_dist = dist_cal(i, j) + dist_cal(i, map[j].next_pt) - map[j].dist_to_next;
            if (cur_dist < min_dist)
            {
                min_dist = cur_dist;
                ins_pos = j;
            }
        }
        map[i].next_pt = map[ins_pos].next_pt;
        map[i].dist_to_next = dist_cal(i, map[i].next_pt);
        map[ins_pos].next_pt = i;
        map[ins_pos].dist_to_next = dist_cal(i, ins_pos);
    }
    for (size_t i = 0; i < map.size(); i++)
    {
        fast_weight += map[i].dist_to_next;
    }
}

void FAST::print()
{
    cout << fast_weight << "\n";
    size_t cur_pos = 0;
    for (size_t i = 0; i < map.size(); i++)
    {
        cout << cur_pos << " ";
        cur_pos = map[cur_pos].next_pt;
    }
    cout << "\n";
}

void OPT::read(){
    FAST::read();
    mst_c.resize(map.size(), mst_pt(numeric_limits<double>::infinity(), false));
    path.resize(map.size());
    path_best.resize(map.size());
    dist_mat.resize(map.size(), vector<double>(map.size()));
    for (size_t i = 0; i < map.size(); i++){
        for (size_t j = i + 1; j < map.size(); j++){
            dist_mat[i][j] = dist_cal(i, j);
            dist_mat[j][i] = dist_mat[i][j];
        }
    }
}

void OPT::genPerms(size_t permLength, double path_length)
{
    if (permLength == path.size())
    {
        // Do something with the path
        double cycle = path_length + dist_mat[0][path[permLength - 1]];
        if (cycle <= best_weight){
            path_best = path;
            best_weight = cycle;
        }
        return;
    } // if ..complete path

    if (!promising(permLength, path_length))
    {
        return;
    } // if ..not promising

    for (size_t i = permLength; i < path.size(); ++i)
    {
        swap(path[permLength], path[i]);
        genPerms(permLength + 1, path_length + dist_mat[path[permLength - 1]][path[permLength]]);
        swap(path[permLength], path[i]);
    } // for ..unpermuted elements
} // genPerms()

bool OPT::promising(size_t permLength, double path_length){
    if (path.size() - permLength < 5){
        return true;
    }
    fill(mst_c.begin(), mst_c.end(), mst_pt(numeric_limits<double>::infinity(), false));
    double cur_dist = 0; // distance between mst and current point
    double minimum_dist;
    double weight = 0;
    size_t pos = 0; // discovered position from last round
    mst_c[permLength].min_dist = 0;
    for (size_t i = permLength; i < path.size(); i++)
    {
        minimum_dist = numeric_limits<double>::infinity();
        for (size_t j = permLength; j < path.size(); j++)
        { // find smallest d_v
            if (!mst_c[j].dis)
            {
                if (mst_c[j].min_dist < minimum_dist)
                {
                    minimum_dist = mst_c[j].min_dist;
                    pos = j;
                }
            }
        }
        mst_c[pos].dis = true; // set it discovered
        for (size_t j = permLength; j < path.size(); j++)
        { // update minimum distance
            if (!mst_c[j].dis)
            {
                cur_dist = dist_mat[path[pos]][path[j]];
                if (cur_dist < mst_c[j].min_dist)
                {
                    mst_c[j].min_dist = cur_dist;
                }
            }
        }
        weight += mst_c[pos].min_dist;
    }
    double mst_to_root = numeric_limits<double>::infinity();
    double mst_to_end = numeric_limits<double>::infinity();
    for (size_t i = permLength; i < path.size(); i++){
        if (dist_mat[0][path[i]] < mst_to_root){
            mst_to_root = dist_mat[0][path[i]];
        }
        if (dist_mat[path[permLength - 1]][path[i]] < mst_to_end){
            mst_to_end = dist_mat[path[permLength - 1]][path[i]];
        }
    }
    return best_weight >= weight + mst_to_root + mst_to_end + path_length;
        // cout << permLength << endl;
        // for (auto it : path){
        //     cout << it << " ";
        // }
        // cout << "\n";
        // cout << weight + mst_to_root + mst_to_end + path_length <<" "<< weight<< " " << mst_to_root<< " " << mst_to_end<< " " << path_length<<endl;
}

void OPT::find(){
    FAST::find();
    size_t pos = 0;
    for (size_t i = 1; i < path.size(); i++){
        pos = map[pos].next_pt;
        path[i] = pos;
    }
    best_weight = fast_weight;
    genPerms(1, 0);
}

void OPT::print(){
    cout << best_weight << "\n";
    for (size_t i = 0; i < path_best.size(); i++){
        cout << path_best[i] << " ";
    }
    cout << "\n";
}
