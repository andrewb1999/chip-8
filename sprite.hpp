#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class sprite
{
    public:
        sprite(std::vector<unsigned char> &);
        std::vector<sf::Uint8> const & get_row(int);
    private:
        std::vector<std::vector<sf::Uint8>> spr;
};
