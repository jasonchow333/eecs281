// Project identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#include "minescape.hpp"
#include <getopt.h>

void mine::option_r(int argc, char *argv[]){
    int option;
    const char* short_options = "hs:mv";
    const struct option long_options[] = {
        {"help",    no_argument,       nullptr, 'h'},
        {"stats", required_argument,       nullptr, 's'},
        {"median", no_argument,      nullptr, 'm'},
        {"verbose", no_argument, nullptr, 'v'},
        {nullptr,   0,                 nullptr, 0}  // Terminate the array
    };

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                cout << "Help: This program accepts one input files with three optional flags. -s/--stats N; -m/--median; -v/--verbose\n";
                exit(0);
            case 's':
                s = true;
                N = stoi(optarg);
                break;
            case 'm':
                m = true;
                break;
            case 'v':
                v = true;
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


void mine::readin(){
    stringstream ss;
    string useless;
    string stuff;
    string num;
    char mode;
    cin >> mode;
    if (mode != 'M' && mode != 'R'){
        cerr << "Invalid input mode\n";
        exit(1);
    }
    cin >> useless;
    cin >> num; // size
    sz = stoi(num);
    cin >> useless;
    cin >> num;
    cur_row = stoi(num);
    cin >> num;
    cur_col = stoi(num);
    if (cur_row >= sz){
        cerr << "Invalid starting row\n";
        exit(1);
    }
    if (cur_col >= sz){
        cerr << "Invalid starting column\n";
        exit(1);
    }
    map.resize(sz, vector<map_tile>(sz));
    if(mode == 'R'){
        uint32_t seed;
        uint32_t max_r;
        uint32_t tnt;
        cin >> useless;
        cin >> seed;
        cin >> useless;
        cin >> max_r;
        cin >> useless;
        cin >> tnt;
        P2random::PR_init(ss, sz, seed, max_r, tnt);
    }
    istream& input = (mode == 'M') ? cin : ss;
    for (uint32_t i = 0; i < sz; i++){
        for (uint32_t j = 0; j < sz; j++){
            input >> stuff;
            this->map[i][j].property = stoi(stuff);
            this->map[i][j].dis = false;
        }
    }
    Tile temp(this->map[cur_row][cur_col].property, cur_row, cur_col);
    dis_q.push(temp);
    this->map[cur_row][cur_col].dis = true;
}

void mine::discover(const Tile &cur_tile){
    // if (cur_row == 0 || cur_row == sz - 1 || cur_col == 0 || cur_col == sz - 1){
    //     esc = true;
    // }
    Tile temp;
    if (cur_tile.col != 0){
        if (!map[cur_tile.row][cur_tile.col - 1].dis){
            temp.property = map[cur_tile.row][cur_tile.col - 1].property;
            temp.row = cur_tile.row;
            temp.col = cur_tile.col - 1;
            dis_q.push(temp);
            map[cur_tile.row][cur_tile.col - 1].dis = true;
        }
    }
    if (cur_tile.row != 0){
        if (!map[cur_tile.row - 1][cur_tile.col].dis){
            temp.property = map[cur_tile.row - 1][cur_tile.col].property;
            temp.row = cur_tile.row - 1;
            temp.col = cur_tile.col;
            dis_q.push(temp);
            map[cur_tile.row - 1][cur_tile.col].dis = true;
        }
    }
    if (cur_tile.row != sz - 1){
        if (!map[cur_tile.row + 1][cur_tile.col].dis){
            temp.property = map[cur_tile.row + 1][cur_tile.col].property;
            temp.row = cur_tile.row + 1;
            temp.col = cur_tile.col;
            dis_q.push(temp);
            map[cur_tile.row + 1][cur_tile.col].dis = true;
        }
    }
    if (cur_tile.col != sz - 1){
        if (!map[cur_tile.row][cur_tile.col + 1].dis){
            temp.property = map[cur_tile.row][cur_tile.col + 1].property;
            temp.row = cur_tile.row;
            temp.col = cur_tile.col + 1;
            dis_q.push(temp);
            map[cur_tile.row][cur_tile.col + 1].dis = true;
        }
    }
}

void mine::move(){
    Tile next_tile = dis_q.top();
    dis_q.pop();
    while (true){
        cur_row = next_tile.row;
        cur_col = next_tile.col;
        if (next_tile.property == -1){
            if (map[cur_row][cur_col].property != 0){
                tnt.push(next_tile);
                tnt_boom();
            }
        }
        else{
            if (map[cur_row][cur_col].property != 0){
                clear_tile++;
                clear_rubble += next_tile.property;
                if (v){
                    cout << "Cleared: " << next_tile.property << " at [" << cur_row << "," << cur_col << "]\n";
                }
                if (m){
                    cout << "Median difficulty of clearing rubble is: ";
                    get_median(next_tile.property);
                    cout << "\n";
                }
                if (s){
                    discovered.push_back(next_tile);
                }
                map[cur_row][cur_col].property = 0;
            }
            discover(next_tile);
        }

        //print_map();
        next_tile = dis_q.top();
        dis_q.pop();
        //cout << "r: " << cur_row << " c: " << cur_col << "\n";
        if (cur_row == 0 || cur_row == sz-1 || cur_col == 0 || cur_col == sz-1){
            break;
        }
    }
    cout << "Cleared " << clear_tile << " tiles containing " << clear_rubble << " rubble and escaped.\n";
    if (s){
        size_t op_num = N < discovered.size() ? N : discovered.size();
        cout << "First tiles cleared:\n";
        for (uint32_t i = 0; i < op_num; i++){
            if (discovered[i].property == -1){
                cout << "TNT";
            }
            else{
                cout << discovered[i].property;
            }
            cout << " at [" << discovered[i].row << "," << discovered[i].col << "]\n";
        }
        cout << "Last tiles cleared:\n";
        for (uint32_t i = 1; i <= op_num; i++){
            if (discovered[discovered.size() - i].property == -1){
                cout << "TNT";
            }
            else{
                cout << discovered[discovered.size() - i].property;
            }
            cout << " at [" << discovered[discovered.size() - i].row << "," << discovered[discovered.size() - i].col << "]\n";
        }
        sort(discovered.begin(), discovered.end(), tilecomp_max());
        cout << "Easiest tiles cleared:\n";
        for (uint32_t i = 0; i < op_num; i++){
            if (discovered[i].property == -1){
                cout << "TNT";
            }
            else{
                cout << discovered[i].property;
            }
            cout << " at [" << discovered[i].row << "," << discovered[i].col << "]\n";
        }
        cout << "Hardest tiles cleared:\n";
        for (uint32_t i = 1; i <= op_num; i++){
            if (discovered[discovered.size() - i].property == -1){
                cout << "TNT";
            }
            else{
                cout << discovered[discovered.size() - i].property;
            }
            cout << " at [" << discovered[discovered.size() - i].row << "," << discovered[discovered.size() - i].col << "]\n";
        }
    }
}

void mine::tnt_boom(){
    Tile next_tile;
    Tile temp;
    while (!tnt.empty() && tnt.top().property == -1){
        next_tile = tnt.top();
        tnt.pop();
        map[next_tile.row][next_tile.col].property = 0;
        if (v){
            cout << "TNT explosion at [" << next_tile.row << "," << next_tile.col << "]!\n";
        }
        if (s){
            discovered.push_back(next_tile);
        }
        // if (next_tile.col == 0 || next_tile.col == sz - 1 || next_tile.row == 0 || next_tile.row == sz - 1){
        //     esc = true;
        // }
        if (next_tile.col != 0){
            if (map[next_tile.row][next_tile.col - 1].property != 0 || !map[next_tile.row][next_tile.col - 1].dis){
                temp.property = map[next_tile.row][next_tile.col - 1].property;
                temp.row = next_tile.row;
                temp.col = next_tile.col - 1;
                tnt.push(temp);
                map[next_tile.row][next_tile.col - 1].property = 0;
            }
        }
        if (next_tile.row != 0){
            if (map[next_tile.row - 1][next_tile.col].property != 0 || !map[next_tile.row - 1][next_tile.col].dis){
                temp.property = map[next_tile.row - 1][next_tile.col].property;
                temp.row = next_tile.row - 1;
                temp.col = next_tile.col;
                tnt.push(temp);
                map[next_tile.row - 1][next_tile.col].property = 0;
            }
        }
        if (next_tile.col != sz - 1){
            if (map[next_tile.row][next_tile.col + 1].property != 0 || !map[next_tile.row][next_tile.col + 1].dis){
                temp.property = map[next_tile.row][next_tile.col + 1].property;
                temp.row = next_tile.row;
                temp.col = next_tile.col + 1;
                tnt.push(temp);
                map[next_tile.row][next_tile.col + 1].property = 0;
            }
        }
        if (next_tile.row != sz - 1){
            if (map[next_tile.row + 1][next_tile.col].property != 0 || !map[next_tile.row + 1][next_tile.col].dis){
                temp.property = map[next_tile.row + 1][next_tile.col].property;
                temp.row = next_tile.row + 1;
                temp.col = next_tile.col;
                tnt.push(temp);
                map[next_tile.row + 1][next_tile.col].property = 0;
            }
        }
    }
    while (!tnt.empty()){
        next_tile = tnt.top();
        tnt.pop();
        if (next_tile.property != 0){
            clear_tile++;
            clear_rubble += next_tile.property;
            if (v){
                cout << "Cleared by TNT: " << next_tile.property << " at [" << next_tile.row << "," << next_tile.col << "]\n";
            }
            if (m){
                cout << "Median difficulty of clearing rubble is: ";
                get_median(next_tile.property);
                cout << "\n";
            }
            if (s){
                discovered.push_back(next_tile);
            }
        }
        next_tile.property = 0;
        next_tile.dis = true;
        dis_q.push(next_tile);
    }
}

void mine::get_median(int tile_pro){
    cout << fixed << setprecision(2);
    if (leftpq.empty()){
        leftpq.push(tile_pro);
    }
    else if (rightpq.empty()){
        if (tile_pro >= leftpq.top()){
            rightpq.push(tile_pro);
        }
        else{
            int temp = leftpq.top();
            leftpq.pop();
            leftpq.push(tile_pro);
            rightpq.push(temp);
        }
    }
    else{
        if (tile_pro >= rightpq.top()){
            if (rightpq.size() <= leftpq.size()){
                rightpq.push(tile_pro);
            }
            else{
                int temp = rightpq.top();
                rightpq.pop();
                rightpq.push(tile_pro);
                leftpq.push(temp);
            }
        }
        else if (tile_pro <= leftpq.top()){
            if (leftpq.size() <= rightpq.size()){
                leftpq.push(tile_pro);
            }
            else{
                int temp = leftpq.top();
                leftpq.pop();
                leftpq.push(tile_pro);
                rightpq.push(temp);
            }
        }
        else{
            if (leftpq.size() <= rightpq.size()){
                leftpq.push(tile_pro);
            }
            else{
                rightpq.push(tile_pro);
            }
        }
    }
    if (rightpq.size() == leftpq.size()){
        cout << double((rightpq.top() + leftpq.top()))/2;
    }
    else if (rightpq.size() > leftpq.size()){
        cout << double(rightpq.top());
    }
    else{
        cout << double(leftpq.top());
    }
}

void mine::print_map(){
    for (uint32_t i=0; i < sz; i++){
        for (uint32_t j=0; j < sz; j++){
            cout << setw(4) << map[i][j].property << " ";
        }
        cout << "\n";
    }
}
