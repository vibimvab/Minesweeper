//
// Created by Insung Seo on 2023/04/18.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H

#include <map>
#include <SFML/Graphics.hpp>
#include "ImageTextures.h"

class Tile {
    bool revealed = false;
    bool flagged = false;
    bool mined = false;
    unsigned char adjacent = 0;

    unsigned int xPos = 0, yPos = 0;

    sf::Texture* tileHidden = nullptr;
    sf::Texture* tileRevealed = nullptr;
    sf::Texture* flag = nullptr;
    sf::Texture* mine = nullptr;
    std::map<int, sf::Texture*> adjacentTexture;

    sf::Sprite tileBase;
    sf::Sprite tileAddition;

public:
    Tile() = default;
    Tile(unsigned int xPos, unsigned int yPos, std::vector<sf::Texture*>& imageTextures);

    int isLeftClicked();
    int isRightClicked();

    void display(sf::RenderWindow* window, int gameStatus, bool debug);

    // getter
    bool isMined() const;

    // setter
    void reset();
    void setLost();
    void setWin();
    void setPause();
    void setResume();
    void setMine();
    void setAdjacent(int num);
};


#endif //MINESWEEPER_TILE_H
