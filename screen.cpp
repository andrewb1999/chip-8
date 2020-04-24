#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "screen.hpp"
#include <iostream>

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

screen::screen() : display {y_size, vector<bool>(x_size)}, 
                   window {sf::VideoMode(x_size, y_size), "CHIP-8 Emulator", 
                   (sf::Style::Resize + sf::Style::Close)} 
{
    view.setSize(x_size, y_size);
    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    view = getLetterboxView(view, x_size, y_size);
    sf::Vector2u size {x_size*40, y_size*40};
    window.setSize(size);
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

	for(unsigned int i = 0; i < x_size*y_size*4; ++i) {
        unsigned int y = i / (x_size*4);
        unsigned int x = (i-(y*x_size*4))/4;
		pixels[i] = display[y][x] ? 255 : 0;
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
/*
    for (auto &v : display) {   
        for (bool i : v) {
                cout << i;
        }
        cout << endl;
    }
    */
//    cout << endl;
    
}

void screen::draw(sprite & s, unsigned char vx, unsigned char vy,
                  unsigned int n, unsigned char & vf) {
    bool vf_bool = false;
    for (unsigned int y = vy; y < vy + n; y++) {
        auto row = s.get_row(y - vy);
        for (unsigned int x = vx; x < (vx + 8u); x++) {
            bool temp = display[y][x];
            display[y][x] = display[y][x] ^ row[x - vx];
            vf_bool = vf_bool || (temp == true && display[y][x] == false);
        }
    }

    if (vf_bool) {
        vf = 1;
    } else {
        vf = 0;
    }
}

