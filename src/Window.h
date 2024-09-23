//
// Created by Insung Seo on 2023/04/17.
//

#ifndef MINESWEEPER_WINDOW_H
#define MINESWEEPER_WINDOW_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Board.h"


class Window {
protected:
    static void setMiddle(sf::Text &text, float x, float y);
    static sf::Text& setText(sf::Font& font, const std::string& string, int size, const sf::Color& color, float x, float y);

public:
    sf::RenderWindow* window;
    sf::Event event{};
    float width, height;
    sf::Font font;

    virtual ~Window();

    virtual void run(std::string& userName){};
    virtual void run(){};
};

class WelcomeWindow: public Window {
    static unsigned char capChar(unsigned long stringSize, unsigned char charToAdd);

public:
    WelcomeWindow(sf::Font& font, float width, float height);
    void run(std::string& userName) override;
};

class GameWindow: public Window {
    Board* board;
    unsigned int tileWidth, tileHeight;

public:
    GameWindow(sf::Font& font, const std::string& configFile, std::string& userName);
    void run() override;
};

class LeaderboardWindow: public Window {
    std::string stringToDisplay;

public:
    LeaderboardWindow(sf::Font& font, unsigned int tileWidth, unsigned int tileHeight);
    void run() override;
};

#endif //MINESWEEPER_WINDOW_H
