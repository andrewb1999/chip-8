#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include "screen.hpp"

using namespace std;

screen::screen() : display {64, vector<sf::Uint8>(32)}, 
                   window {sf::VideoMode(64, 32), "CHIP-8 Emulator", 
                   (sf::Style::Resize + sf::Style::Close)} {}

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

void screen::disp_clear() {
    window.clear();
    for (auto &i : display)
        std::fill(i.begin(), i.end(), 0);
}

void screen::update_disp() {
    const int x_size = 64;
    const int y_size = 32;
    view.setSize(x_size, y_size);
    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    view = getLetterboxView(view, x_size, y_size);

    while (window.isOpen())
    {
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
        window.display();
    }
    
}

void screen::draw(const sprite &, unsigned char vx, unsigned char vy,
                  unsigned int n, unsigned char vf) {
    
}

