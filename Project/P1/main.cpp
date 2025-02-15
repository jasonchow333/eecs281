// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include <unistd.h>
#include <getopt.h>
#include "letterman.hpp"

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    int option;
    const char* short_options = "hqsb:e:o:clp";
    const struct option long_options[] = {
        {"help",    no_argument,       nullptr, 'h'},
        {"queue", no_argument,       nullptr, 'q'},
        {"stack", no_argument,      nullptr, 's'},
        {"begin", required_argument, nullptr, 'b'},
        {"end", required_argument, nullptr, 'e'},
        {"output", required_argument, nullptr, 'o'},
        {"change", no_argument, nullptr, 'c'},
        {"length", no_argument, nullptr, 'l'},
        {"swap", no_argument, nullptr, 'p'},
        {nullptr,   0,                 nullptr, 0}  // Terminate the array
    };
    string b_w;
    string e_w;
    bool queue = false;
    bool stack = false;
    bool output = false; // false to be W, true to be M
    bool change = false;
    bool length = false;
    bool swap = false;

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                cout << "Help: This program accepts a single option.\n";
                return 0;
            case 'q':
                if (queue || stack){
                    cerr << "Conflicting or duplicate stack and queue specified" << endl;
                    exit(1);
                }
                queue = true;
                break;
            case 's':
                if (stack || queue){
                    cerr << "Conflicting or duplicate stack and queue specified" << endl;
                    exit(1);
                }
                stack = true;
                break;
            case 'b':
                if (optarg[0] == '-'){
                    cerr << "Beginning word not specified" << endl;
                    exit(1);
                }
                b_w = optarg;
                break;
            case 'e':
                if (optarg[0] == '-'){
                    cerr << "Ending word not specified" << endl;
                    exit(1);
                }
                e_w = optarg;
                break;
            case 'o':
                if (!optarg){
                    cerr << "Invalid output mode specified" << endl;
                    exit(1);
                }
                if (optarg[0] == '-'){
                    cerr << "Invalid output mode specified" << endl;
                    exit(1);
                }
                if (optarg[0] == 'M'){
                    output = true;
                }
                else{
                    if (optarg[0] != 'W'){
                        cerr <<optarg<< "Invalid output mode specified" << endl;
                        exit(1);
                    }
                }
                break;
            case 'c':
                change = true;
                break;
            case 'l':
                length = true;
                break;
            case 'p':
                swap = true;
                break;
            case '?':
                cerr << "Unknown command line option" << endl;
                exit(1);

            default:
                // Should not reach here
                return 1;
        }
    }
    if (!(stack || queue)){
        cerr << "Must specify one of stack or queue" << endl;
        exit(1);
    }
    if (!(change || length || swap)){
        cerr << "Must specify at least one modification mode (change length swap)" << endl;
        exit(1);
    }
    if (!length && b_w.size() != e_w.size()){
        cerr << "The first and last words must have the same length when length mode is off" << endl;
        exit(1);
    }
    //cout << b_w << e_w << endl;
    letterman letter_m(b_w, e_w, !queue && stack, output, change, length, swap);
    letter_m.read_dict();
    //letter_m.print_dict();
    if (letter_m.compare_equal(b_w, e_w)){
        cout << "Words in morph: 1\n" << b_w << "\n";
        return 0;
    }
    letter_m.search();

    return 0;
}
