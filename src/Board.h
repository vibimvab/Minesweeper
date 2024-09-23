//
// Created by Insung Seo on 2023/04/18.
//

#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Tile.h"
#include "ImageTextures.h"
#include "Button.h"

class Board {
    sf::RenderWindow* window;
    sf::Font* font;
    std::string username;

    float width, height;
    unsigned int tileWidth, tileHeight;

    int totalMine;
    int currentMine;
    unsigned int unrevealedTile;
    Tile** board;
    std::vector<int> mineLocation;
    ImageTextures Images;

    enum GameStatus {
        ongoing = 0,
        win,
        lose,
        pause
    };
    GameStatus gameStatus = ongoing;
    bool debug = false;

    // time
    std::chrono::time_point<std::chrono::steady_clock>  timeStart;
    std::chrono::time_point<std::chrono::steady_clock>  timeEnd;
    unsigned long timePreviousPortion = 0; // in microseconds (see updateTime function in Board.cpp to change)
    unsigned long timeCurrentPortion = 0;

    // sprites
    std::map<std::string, Button*> Buttons; // "face", "debugging", "pause", "leaderboard"
    sf::Sprite mineDigit1;
    sf::Sprite mineDigit2;
    sf::Sprite mineDigit3;
    sf::Sprite minuteDigit1;
    sf::Sprite minuteDigit2;
    sf::Sprite secondDigit1;
    sf::Sprite secondDigit2;
    std::vector<sf::Sprite*> digitSprites;

    // initializers
    void generateMines(); // generate random location of mines in array of integers, get the location of the mine by dividing the integer by tileWidth
    void setMines();
    static int countAdjacentMines(std::vector<Tile>& list, int length);
    void setAdjacent() const;

    // private functions below are always used within clicked function
    // setter
    void reset();
    void setLose();
    void setWin();
    void setPause();
    void setResume();
    void setCounter();
    void showLeaderBoard();
    void updateLeaderBoard(unsigned long time); // time in seconds

    // button clicked
    void menuClicked(sf::Vector2i mousePos);
    void faceButtonClicked();
    void debugButtonClicked();
    void pauseButtonClicked();
    void leaderboardButtonClicked();

    // board clicked
    bool leftClicked(sf::Vector2i mousePos);
    void leftClicked(int xPos, int yPos); // for recursion
    void rightClicked(sf::Vector2i mousePos);

public:
    Board(sf::RenderWindow* window, sf::Font* font, float width, float height, unsigned int tileWidth, unsigned int tileHeight, int numMine, std::string& username);
    ~Board();

    void display() const;
    void clicked(sf::Vector2i mousePos);
    void updateTime();
};


#endif //MINESWEEPER_BOARD_H
