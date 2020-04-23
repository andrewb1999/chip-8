#include "sprite.hpp"

using namespace std;

sprite::sprite(vector<unsigned char>::iterator first, vector<unsigned char>::iterator last) {
    int l = distance(first, last);
    spr.resize(l, vector<unsigned char>(8));
    for(int i = 0; first != last; ++first, ++i) {
        unsigned char n = *first;
        for(int j = 0; j < 8; j++) {
            spr[i][j] = (n & (1 << (7-j))) >> (7-j);          
        }
    }
}

std::vector<unsigned char> const & sprite::get_row(const int row) {
    return spr[row];
}

