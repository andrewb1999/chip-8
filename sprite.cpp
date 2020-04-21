#include "sprite.hpp"

using namespace std;

sprite::sprite(vector<unsigned char>::iterator first, vector<unsigned char>::iterator last) {
    int l = distance(first, last);
    spr.resize(l, vector<sf::Uint8>(8));
    for(int i = 0; first != last; ++first, ++i) {
        for(int j = 0; j < 8; j++) {
            spr[i][j] = *first & (1 << j);          
        }
    }
}

std::vector<sf::Uint8> const & sprite::get_row(const int row) {
    return spr[row];
}

