/*
** This class is an abstraction around handling video output
*/
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "sprite.hpp"

class screen 
{
    public:
        screen();
        void disp_clear();
        void update_disp();
        void draw(const sprite &, unsigned char vx, unsigned char vy, 
                unsigned int n, unsigned char vf);
    private:
        sf::RenderWindow window;
        sf::View view;
        std::vector<std::vector<sf::Uint8>> display;
};
