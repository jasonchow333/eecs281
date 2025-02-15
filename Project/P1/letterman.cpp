// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include "letterman.hpp"

letterman::letterman(string &b_w, string &e_w, bool sq, bool output, bool change, bool length, bool swap)
{
    this->begin_w = b_w;
    this->end_w = e_w;
    this->stque = sq;
    this->op = output;
    this->change = change;
    this->length = length;
    this->swap = swap;
    this->dict_size = 0;
    this->b_pos = 0;
}

void letterman::print_dict(){
    for (size_t i = 0; i < dict_size; i++){
        cout << dictionary[i].wd << endl;
    }
}

bool letterman::compare_equal(const string &dict, const string &word){
    if (dict.size() != word.size()){
        return false;
    }
    for (size_t i = 0; i < dict.size(); i++){
        if (dict[i] != word[i]){
            return false;
        }
    }
    return true;
}

string letterman::reversal(string &word){
    word.erase(word.size()-1);
    string re_word = word;
    for (size_t i = 0; i < word.size()/2; i++){
        re_word[i] = word[word.size() - i - 1];
        re_word[word.size() - i - 1] = word[i];
    }
    return re_word;
}

void letterman::swap_let(string &word, size_t pos){
    char temp = word[pos-1];
    word[pos-1] = word[pos-2];
    word[pos-2] = temp;
}

void letterman::change_let(string &word, size_t pos, char ch){
    word[pos] = ch;
}

// void letterman::insert_let(string &word, size_t pos, char ch){
//     word = word.substr(0, pos) + ch + word.substr(pos);
// }

void letterman::delete_let(string &word, size_t pos){
    word = word.substr(0, pos) + word.substr(pos + 1);
}

void letterman::read_dict(){
    Word word;
    word.dis_ed = false;
    word.prev = 0;
    size_t b_size = begin_w.size();
    char type;
    bool bexist = false;
    bool eexist = false;
    cin >> type;
    cin >> dict_size;
    dictionary.reserve(dict_size);
    if (type == 'S'){
        while (cin >> word.wd){
            if (word.wd[0] == '/' && word.wd[1] == '/'){
                getline(cin, word.wd);
                continue;
            }
            if (word.wd[0] == '\n'){
                break;
            }
            if (!length){
                if (word.wd.size() != b_size){
                    dict_size--;
                    continue;
                }
            }
            if (!bexist){
                bexist = compare_equal(word.wd, begin_w);
                b_pos++;
            }
            if (!eexist){
                eexist = compare_equal(word.wd, end_w);
            }
            dictionary.push_back(word);
        }
    }
    else if (type == 'C'){
        Word new_word;
        new_word.dis_ed = false;
        bool special = false;
        bool ins_not = false;
        while (cin >> word.wd){
            if (word.wd[0] == '/' && word.wd[1] == '/'){
                getline(cin, word.wd);
                continue;
            }
            if (word.wd[0] == '\n'){
                break;
            }
            if (word.wd[word.wd.size() - 1] == '&'){
                if (!length){
                    if (word.wd.size() - 1 != begin_w.size()){
                        dict_size--;
                        continue;
                    }
                }
                new_word.wd = reversal(word.wd);
                special = true;
            }
            else{
                size_t op_pos = word.wd.find_first_of("![?");
                if (op_pos != string::npos){
                    char spec_char = word.wd[op_pos];
                    if (spec_char == '!'){
                        if (!length){
                            if (word.wd.size() - 1 != begin_w.size()){
                                dict_size--;
                                continue;
                            }
                        }
                        word.wd.erase(op_pos, 1);
                        new_word.wd = word.wd;
                        swap_let(new_word.wd, op_pos);
                        special = true;
                    }
                    else if (spec_char == '?'){
                        if (!length){
                            bool same_length = false; // false: small size same; true: big size same
                            if (word.wd.size() - 1 > begin_w.size() || word.wd.size() < begin_w.size()){
                                dict_size--;
                                continue;
                            }
                            else{
                                if (word.wd.size() == begin_w.size()){
                                    same_length = true;
                                }
                                if (same_length){
                                    change_let(word.wd, op_pos, word.wd[op_pos-1]);
                                }
                                else{
                                    delete_let(word.wd, op_pos);
                                }
                                if (!bexist){
                                    bexist = compare_equal(word.wd, begin_w);
                                    b_pos++;
                                }
                                if (!eexist){
                                    eexist = compare_equal(word.wd, end_w);
                                }
                                dictionary.push_back(word);
                                continue;
                            }
                        }
                        new_word.wd = word.wd;
                        change_let(new_word.wd, op_pos, new_word.wd[op_pos-1]);
                        delete_let(word.wd, op_pos);
                        special = true;
                    }
                    else{
                        size_t ins_pos = word.wd.find(']');
                        if (!length){
                            if (word.wd.size() - ins_pos + op_pos != begin_w.size()){
                                dict_size--;
                                continue;
                            }
                        }
                        ins_not = true;
                        dict_size += ins_pos - op_pos - 2;
                        for (size_t i = op_pos; i < ins_pos - 1; i++){
                            new_word.wd = word.wd.substr(0, op_pos) + word.wd[1 + i] + word.wd.substr(ins_pos + 1);
                            if (!bexist){
                                bexist = compare_equal(new_word.wd, begin_w);
                                b_pos++;
                            }
                            if (!eexist){
                                eexist = compare_equal(new_word.wd, end_w);
                            }
                            dictionary.push_back(new_word);
                        }
                    }
                }
            }

            if (!bexist){
                bexist = compare_equal(word.wd, begin_w);
                b_pos++;
            }
            if (!eexist){
                eexist = compare_equal(word.wd, end_w);
            }
            if (!ins_not){
                dictionary.push_back(word);
            }
            if (special){
                dict_size++;
                if (!bexist){
                    bexist = compare_equal(new_word.wd, begin_w);
                    b_pos++;
                }
                if (!eexist){
                    eexist = compare_equal(new_word.wd, end_w);
                }
                dictionary.push_back(new_word);
            }
            special = false;
            ins_not = false;
        }
    }
    else{
        cerr << "Incorrect cin file" << endl;
        exit(1);
    }
    b_pos--;
    //std::cout << dictionary.size() << dict_size << endl;
    // for (size_t i = 0; i < dict_size; i++){
    //     std::cout<<dictionary[i].wd<<"\n";
    // }
    if (!bexist){
        cerr << "Beginning word does not exist in the dictionary" << endl;
        exit(1);
    }
    if (!eexist){
        cerr << "Ending word does not exist in the dictionary" << endl;
        exit(1);
    }
}

bool letterman::check_dis(const string &inspect, const string &dict){
    size_t i_size = inspect.size();
    size_t d_size = dict.size();
    bool minsize = d_size > i_size ? true : false; //d_size bigger? false: d_size small; true: i_size small
    if ((minsize && d_size - i_size > 1) || (!minsize && i_size - d_size > 1)){
        return false;
    }
    if (d_size == i_size){
        if (!change && !swap){
            return false;
        }
        bool changed = false;
        bool swaped = false;
        if (change && swap){
            for (size_t i = 0; i < d_size; i++){
                if (inspect[i] != dict[i]){
                    if (!changed){ // first detect difference
                        changed = true;
                        if (i < d_size - 1){
                            if (inspect[i+1] == dict[i] && inspect[i] == dict[i+1]){
                                swaped = true;
                                i++;
                            }
                        }
                    }
                    else{ // second detect difference
                        return false;
                    }
                }
            }
            return changed;
        }
        else if (change){
            for (size_t i = 0; i < d_size; i++){
                if (inspect[i] != dict[i]){
                    if (!changed){
                        changed = true;
                    }
                    else{
                        return false;
                    }
                }
            }
            return changed;
        }
        else{
            for (size_t i = 0; i < d_size; i++){
                if (inspect[i] != dict[i]){
                    if (!swaped){
                        if (i < d_size - 1){
                            if (inspect[i+1] == dict[i] && inspect[i] == dict[i+1]){
                                swaped = true;
                                i++;
                            }
                            else{
                                return false;
                            }
                        }
                        else{
                            return false;
                        }
                    }
                    else{
                        return false;
                    }
                }
            }
            return swaped;
        }
    }
    else{
        if (!length){
            return false;
        }
        bool diff1 = false;
        size_t diff_pos;
        size_t min_size = (minsize) ? i_size : d_size;
        for (size_t i = 0; i < min_size; i++){
            if (inspect[i] != dict[i]){
                diff1 = true;
                diff_pos = i;
                break;
            }
        }
        if (!diff1){
            return true;
        }
        for (size_t i = diff_pos; i < min_size; i++){
            if (!minsize){ // dict is smaller
                if (inspect[i+1] != dict[i]){
                    return false;
                }
            }
            else{
                if (inspect[i] != dict[i+1]){
                    return false;
                }
            }
        }
        return true;
    }
}

void letterman::search(){
    string cur_word = begin_w;
    size_t cur_pos = b_pos;
    size_t dis_num = 0;
    bool found = false;
    //std::cout << b_pos << endl;
    while (!found){
        //std::cout<<"current: "<<cur_word <<cur_pos<<"\n";
        for (size_t i = 0; i < dict_size; i++){
            if (!dictionary[i].dis_ed){
                dictionary[i].dis_ed = check_dis(cur_word, dictionary[i].wd);
                if (dictionary[i].dis_ed){
                    container.push_back(i);
                    dictionary[i].prev = cur_pos;
                    dis_num++;
                    //std::cout <<dis_num<< " dis: " << dictionary[i].wd << endl;
                    if (compare_equal(end_w, dictionary[i].wd)){
                        found = true;
                        //std::cout <<"end word: "<< i << endl;
                        break;
                    }
                }
                else{
                    if (compare_equal(dictionary[i].wd, begin_w)){
                        dictionary[i].dis_ed = true;
                        dictionary[i].prev = b_pos;
                        dis_num++;
                    }
                }
            }
        }
        if (container.empty()){
            std::cout << "No solution, " << dis_num << " words discovered.\n";
            return;
        }
        // for (size_t i = 0; i < container.size(); i++){
        //     std::cout<< dictionary[container[i]].wd <<dictionary[container[i]].prev<<endl;
        // }
        if (found){
            cur_pos = container.back();
            //std::cout <<"end_w pos: "<< cur_pos << endl;
        }
        else{
            if (stque){ // stack mode
                cur_word = dictionary[container.back()].wd;
                cur_pos = container.back();
                container.pop_back();
            }
            else{ // queue mode
                cur_word = dictionary[container.front()].wd;
                cur_pos = container.front();
                container.pop_front();
            }
        }
    }
    vector<size_t> path;
    while (cur_pos != b_pos){
        path.push_back(cur_pos);
        cur_pos = dictionary[cur_pos].prev;
    }
    std::cout << "Words in morph: " << path.size() + 1 << "\n" << begin_w << "\n";
    if (!op){
        for (size_t i = path.size(); i > 0; i--){
            std::cout << dictionary[path[i-1]].wd << "\n";
        }
    }
    else{
        mod_output(begin_w, dictionary[path[path.size()-1]].wd);
        for (size_t i = path.size() - 1; i > 0; i--){
            mod_output(dictionary[path[i]].wd, dictionary[path[i-1]].wd);
        }
    }
}

void letterman::mod_output(const string &prev, const string &later){
    size_t pos = 0;
    char mode = '\0';
    char letter = '\0';

    if (prev.size() > later.size()){
        mode = 'd';
        for (size_t i = 0; i < later.size(); i++){
            if (prev[i] == later[i]){
                pos++;
            }
            else{
                break;
            }
        }
    }
    else if (prev.size() < later.size()){
        mode = 'i';
        for (size_t i = 0; i < prev.size(); i++){
            if (prev[i] == later[i]){
                pos++;
            }
            else{
                letter = later[i];
                break;
            }
        }
        if (pos == prev.size()){
            letter = later[pos];
        }
    }
    else{
        for (size_t i = 0; i < later.size(); i++){
            if (prev[i] == later[i]){
                pos++;
            }
            else{
                if (i == later.size() - 1){
                    mode = 'c';
                    letter = later[i];
                }
                else{
                    if (prev[i+1] == later[i+1]){
                        mode = 'c';
                        letter = later[i];
                    }
                    else{
                        mode = 's';
                    }
                }
                break;
            }
        }
    }

    std::cout << mode << "," << pos;
    if (mode == 'c' || mode == 'i'){
        std::cout << "," << letter;
    }
    std::cout << "\n";
}
