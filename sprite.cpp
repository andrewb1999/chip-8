#include "sprite.hpp"

using namespace std;

sprite::sprite(vector<unsigned char> & s) {
    int l = s.size();
    spr.resize(l, vector<sf::Uint8>(8));
    for(int i = 0; i < s.size(); i++) {
        for(int j = 0; j < 8; j++) {
            spr[i][j] = s[i] & (1 << j);          
        }
    }
}

std::vector<sf::Uint8> const & sprite::get_row(const int row) {
    return spr[row];
}
