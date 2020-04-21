/*
** This class is an abstraction around handling video output
*/
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include "sprite.hpp"

class screen 
{
    public:
        screen();
        void disp_clear();
        void update_disp();
        void draw(sprite &, unsigned char vx, unsigned char vy, 
                unsigned int n, unsigned char & vf);
    private:
        std::vector<std::vector<bool>> display;
        sf::RenderWindow window;
        sf::View view;
        sf::Texture texture;
        const int x_size = 64;
        const int y_size = 32;
};
