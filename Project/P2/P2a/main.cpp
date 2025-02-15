// Project identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#include "minescape.hpp"

int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false);
    mine gold_mine;
    gold_mine.option_r(argc, argv);
    gold_mine.readin();
    gold_mine.move();
}
