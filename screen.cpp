#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "screen.hpp"

using namespace std;

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {
    
    float windowRatio = windowWidth / (float) windowHeight;
    float viewRatio = view.getSize().x / (float) view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }

    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

    return view;
}

screen::screen() : display {64, vector<bool>(32)}, 
                   window {sf::VideoMode(64*10, 32*10), "CHIP-8 Emulator", 
                   (sf::Style::Resize + sf::Style::Close)} 
{
    view.setSize(x_size*10, y_size*10);
    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    view = getLetterboxView(view, x_size, y_size);
}


void screen::disp_clear() {
    window.clear();
    for (auto &i : display)
        std::fill(i.begin(), i.end(), 0);
}

void screen::update_disp() {
    sf::Uint8 pixels[x_size*y_size*4];
    texture.create(x_size, y_size);
    sf::Sprite sprite(texture);
    for (int i = 0; i < y_size; i++) {
       for (int j = 0; j < x_size; j++) {
           pixels[j + y_size] = display[i][j] ? 255 : 0;
           pixels[j + (y_size + i)] = display[i][j] ? 255 : 0;
           pixels[j + (y_size + (2 * i))] = display[i][j] ? 255 : 0;
           pixels[j + (y_size + (3 * i))] = 255;
       }
    }
    
    texture.update(pixels);


    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::Resized) {
            view = getLetterboxView(view, event.size.width, event.size.height);
        }
    }
    
    window.setView(view);
    window.clear();
    window.draw(sprite);
    window.display();
    
}

void screen::draw(sprite & s, unsigned char vx, unsigned char vy,
                  unsigned int n, unsigned char & vf) {
    
    for (unsigned int i = vy; i < vy + n; i++) {
        auto row = s.get_row(i - vy);
        for (unsigned int j = vx; j < (vx + 8u); j++) {
            bool temp = display[i][j];
            display[i][j] = display[i][j] ^ row[j - vx];
            vf &= (temp != display[i][j]);
        }
    }
}

