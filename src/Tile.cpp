//
// Created by Insung Seo on 2023/04/18.
//

#include "Tile.h"

Tile::Tile (unsigned int xPos, unsigned int yPos, std::vector<sf::Texture*>& imageTextures) {
    this->revealed = false;
    this->flagged = false;
    this->mined = false;
    this->adjacent = 0;

    this->xPos = xPos;
    this->yPos = yPos;

    tileHidden = imageTextures.at(0);
    tileRevealed = imageTextures.at(1);
    flag = imageTextures.at(2);
    mine = imageTextures.at(3);
    for (int i = 4; i < 12; i++) {
        adjacentTexture[i-3] = imageTextures.at(i);
    }

    tileBase.setTexture(*tileHidden);
    tileBase.setPosition((float)xPos * 32, (float)yPos * 32);
    tileAddition.setTexture(*mine);
    tileAddition.setPosition((float)xPos * 32, (float)yPos * 32);
}

int Tile::isLeftClicked() {
    if (flagged)
        return 2;

    if (!revealed) {
        revealed = true;
        tileBase.setTexture(*tileRevealed);
        if (mined) {
            tileAddition.setTexture(*mine);
            return -1;

        } else {
            if (adjacent > 0) {
                tileAddition.setTexture(*adjacentTexture[adjacent]);
                return 1;
            } else if (adjacent == 0) {
                return 0;
            }
        }
    }

    return 2;
}

int Tile::isRightClicked() { // return 1 if flag added, -1 if removed, and 0 if nothing happened
    if (!revealed) {
        if (flagged) {
            flagged = false;
            if (mined)
                tileAddition.setTexture(*mine);
            return -1;

        } else {
            flagged = true;
            tileAddition.setTexture(*flag);
            return 1;
        }
    }

    return 0;
}

void Tile::display(sf::RenderWindow* window, int gameStatus, bool debug) {
    if (gameStatus == 0) { // ongoing
        window->draw(tileBase);
        if ((revealed && mined) || (revealed && adjacent > 0) || flagged || (debug && mined))
            window->draw(tileAddition);

    } else if (gameStatus == 1) { // win
        window->draw(tileBase);
        if (mined || adjacent > 0)
            window->draw(tileAddition);

    } else if (gameStatus == 2) { // lost
        window->draw(tileBase);
        if (mined || (revealed && adjacent > 0) || flagged || mined)
            window->draw(tileAddition);

    } else if (gameStatus == 3) { // paused
        window->draw(tileBase);
    }
}


// getter
bool Tile::isMined() const {
    return mined;
}


// setter
void Tile::reset() {
    tileBase.setTexture(*tileHidden);
    tileAddition.setTexture(*mine);
    revealed = false;
    flagged = false;
    mined = false;
    adjacent = 0;
}

void Tile::setLost() {
    if (mined) {
        tileBase.setTexture(*tileRevealed);
        tileAddition.setTexture(*mine);
    }
}

void Tile::setWin() {
    if (mined && !flagged) {
        tileAddition.setTexture(*flag);
        flagged = true;
    }
}

void Tile::setPause() {
    tileBase.setTexture(*tileRevealed);
}

void Tile::setResume() {
    if (!revealed) tileBase.setTexture(*tileHidden);
}

void Tile::setMine() {
    mined = true;
}

void Tile::setAdjacent(int num) {
    adjacent = num;
}
