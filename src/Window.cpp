//
// Created by Insung Seo on 2023/04/17.
//

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "Window.h"


Window::~Window() {
    delete window;
}

// Window base class private functions
void Window::setMiddle(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

sf::Text& Window::setText(sf::Font& font, const std::string& string, int size, const sf::Color& color, float x, float y) {
    auto* text = new sf::Text;
    text->setFont(font); // select the font
    text->setString(string); // set the string to display
    text->setCharacterSize(size); // set the character size in pixels, not points
    text->setFillColor(color); // set the color
    setMiddle(*text, x, y); // set position

    return *text;
}


// WelcomeWindow private functions
unsigned char WelcomeWindow::capChar(unsigned long stringSize, unsigned char charToAdd) {
    if (stringSize > 10)
        return 0; // when the program shouldn't accept more character

    if (stringSize == 1)
        return charToAdd + 65;
    else
        return charToAdd + 32 + 65;
}

// WelcomeWindow constructor
WelcomeWindow::WelcomeWindow(sf::Font& font, float width, float height) {
    this->window = new sf::RenderWindow(sf::VideoMode((int)width, (int)height), "Welcome Window", sf::Style::Close);
    window->setFramerateLimit(60);
    this->width = width;
    this->height = height;
    this->font = font;
}

// WelcomeWindow run
void WelcomeWindow::run(std::string& userName) {
    //texts
    sf::Text welcomeText = setText(font, (std::string) "WELCOME TO MINESWEEPER", 24,sf::Color::White, width/2.0f, height/2.0f - 150);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Text enterNameText = setText(font, (std::string) "Enter your name:", 20,sf::Color::White, width/2.0f, height/2.0f - 75);
    welcomeText.setStyle(sf::Text::Bold);
    sf::Text nameText = setText(font, (std::string) "|", 18,sf::Color::Yellow, width/2.0f, height/2.0f - 45);
    welcomeText.setStyle(sf::Text::Bold);

    while (window->isOpen()) {
        // input
        while (window->pollEvent(event)) {
            // window closed
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            // keyboard input
            if (event.type == sf::Event::KeyPressed) {
                // alphabet key
                unsigned char characterToAdd;
                for (unsigned char i = 0; i < 26; i++){
                    if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i)) {
                        characterToAdd = WelcomeWindow::capChar(nameText.getString().getSize(), i);
                        if (characterToAdd != 0)
                            nameText.setString(nameText.getString().substring(0, nameText.getString().getSize() - 1) + (char)characterToAdd + '|');
                    }
                }
                setMiddle(nameText, width/2.0f, height/2.0f - 45);

                // backspace
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && nameText.getString().getSize() > 1) {
                    nameText.setString(nameText.getString().substring(0, nameText.getString().getSize() - 2) + "|");
                    setMiddle(nameText, width/2.0f, height/2.0f - 45);
                }

                // enter, go to game window
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && nameText.getString().getSize() != 1) {
                    userName = nameText.getString().substring(0, nameText.getString().getSize() - 1);
                    window->close();
                    return;
                }
            }
        }

        // display
        window->clear(sf::Color(0, 0, 255, 0));
        window->draw(welcomeText);
        window->draw(enterNameText);
        window->draw(nameText);
        window->display();
    }
}



// GameWindow constructor
GameWindow::GameWindow(sf::Font& font, const std::string& configFile, std::string& userName) {
    this->font = font;

    // reading config file
    std::ifstream config(configFile);
    if (!config.is_open())
        throw std::invalid_argument( "invalid configuration file" );
    std::string tempString;
    getline(config, tempString);
    tileWidth = std::stoi(tempString);
    width = (float)tileWidth*32;
    getline(config, tempString);
    tileHeight = std::stoi(tempString);
    height = (float)tileHeight*32 + 100;
    getline(config, tempString);
    int numMine = std::stoi(tempString);

    if (tileWidth*tileHeight < numMine)
        throw std::invalid_argument( "invalid configuration file" );
    // end of reading config file

    this->window = new sf::RenderWindow(sf::VideoMode(tileWidth*32, tileHeight*32 + 100), "Game Window", sf::Style::Close);
    window->setFramerateLimit(64);
    this->board = new Board(window, &font, width, height, tileWidth, tileHeight, numMine, userName);
}

// GameWindow run
void GameWindow::run() {
    sf::Vector2i mousePos;

    while (window->isOpen()) {
        // input
        while (window->pollEvent(event)) {
            // close window
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            // mouse input
            if (event.type == sf::Event::MouseButtonPressed) {
                mousePos = sf::Mouse::getPosition(*window);
                // if board is clicked
                if (0 <= mousePos.x && (float)mousePos.x < width && 0 <= mousePos.y && (float)mousePos.y < height)
                    board->clicked(mousePos);
            }
        }

        // display
        window->clear(sf::Color::White);
        board->updateTime();
        board->display();
        window->display();
    }
}


// LeaderboardWindow
LeaderboardWindow::LeaderboardWindow(sf::Font& font, unsigned int tileWidth, unsigned int tileHeight) {
    window = new sf::RenderWindow(sf::VideoMode(tileWidth*16, tileHeight*16 + 50), "Leaderboard Window", sf::Style::Close);
    this->font = font;
    width = (float)tileWidth*16;
    height = (float)tileHeight*16 + 50;

    // reading leaderboard.txt
    std::ifstream leaderBoard("files/leaderboard.txt");
    if (!leaderBoard.is_open()) throw std::invalid_argument( "invalid leaderboard.txt file" );
    std::string line;
    std::string tempTime;
    std::string tempName;

    for (int i = 0; i < 5 && getline(leaderBoard, line); i++) {
        std::istringstream stream(line);
        getline(stream, tempTime, ',');
        getline(stream, tempName, ',');

        stringToDisplay += std::to_string(i+1);
        stringToDisplay += ".\t";
        stringToDisplay += tempTime;
        stringToDisplay += "\t";
        stringToDisplay += tempName;
        stringToDisplay += "\n\n";
    }
    // end of reading leaderboard.txt
}


void LeaderboardWindow::run() {
    //texts
    sf::Text leaderboard = Window::setText(font, (std::string) "LEADERBOARD", 20,sf::Color::White, width/2.0f, height/2.0f - 120);
    leaderboard.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Text ranking = Window::setText(font, stringToDisplay, 20,sf::Color::White, width/2.0f, height/2.0f + 20);
    ranking.setStyle(sf::Text::Bold);

    // display
    window->clear(sf::Color(0, 0, 255, 0));
    window->draw(leaderboard);
    window->draw(ranking);
    window->display();

    while (window->isOpen()) {
        // input
        while (window->pollEvent(event)) {
            // window closed
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }

        usleep(10000); // preventing while loop from running infinitely many times in a second
    }
}