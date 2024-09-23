//
// Created by Insung Seo on 2023/04/19.
//

#ifndef MINESWEEPER_IMAGETEXTURES_H
#define MINESWEEPER_IMAGETEXTURES_H

#include <SFML/Graphics.hpp>
#include <vector>

class ImageTextures { // call this class only once
public:
    // tile
    sf::Texture tileHidden;
    sf::Texture tileRevealed;
    sf::Texture flag;
    sf::Texture mine;
    sf::Texture num1;
    sf::Texture num2;
    sf::Texture num3;
    sf::Texture num4;
    sf::Texture num5;
    sf::Texture num6;
    sf::Texture num7;
    sf::Texture num8;

    // face
    sf::Texture faceHappy;
    sf::Texture faceLose;
    sf::Texture faceWin;

    // buttons
    sf::Texture debug;
    sf::Texture pause;
    sf::Texture play;
    sf::Texture leaderboard;

    // digits, (negative sign on 10)
    std::vector<sf::Texture*> digits;

    ImageTextures();
};


#endif //MINESWEEPER_IMAGETEXTURES_H
