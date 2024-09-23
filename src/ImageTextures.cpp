//
// Created by Insung Seo on 2023/04/19.
//

#include "ImageTextures.h"

ImageTextures::ImageTextures() {
    // tile
    if (!tileHidden.loadFromFile("files/images/tile_hidden.png")) throw std::runtime_error("image file not loaded");
    if (!tileRevealed.loadFromFile("files/images/tile_revealed.png")) throw std::runtime_error("image file not loaded");
    if (!flag.loadFromFile("files/images/flag.png")) throw std::runtime_error("image file not loaded");
    if (!mine.loadFromFile("files/images/mine.png")) throw std::runtime_error("image file not loaded");
    if (!num1.loadFromFile("files/images/number_1.png")) throw std::runtime_error("image file not loaded");
    if (!num2.loadFromFile("files/images/number_2.png")) throw std::runtime_error("image file not loaded");
    if (!num3.loadFromFile("files/images/number_3.png")) throw std::runtime_error("image file not loaded");
    if (!num4.loadFromFile("files/images/number_4.png")) throw std::runtime_error("image file not loaded");
    if (!num5.loadFromFile("files/images/number_5.png")) throw std::runtime_error("image file not loaded");
    if (!num6.loadFromFile("files/images/number_6.png")) throw std::runtime_error("image file not loaded");
    if (!num7.loadFromFile("files/images/number_7.png")) throw std::runtime_error("image file not loaded");
    if (!num8.loadFromFile("files/images/number_8.png")) throw std::runtime_error("image file not loaded");

    // face
    if (!faceHappy.loadFromFile("files/images/face_happy.png")) throw std::runtime_error("image file not loaded");
    if (!faceLose.loadFromFile("files/images/face_lose.png")) throw std::runtime_error("image file not loaded");
    if (!faceWin.loadFromFile("files/images/face_win.png")) throw std::runtime_error("image file not loaded");

    // button
    if (!debug.loadFromFile("files/images/debug.png")) throw std::runtime_error("image file not loaded");
    if (!pause.loadFromFile("files/images/pause.png")) throw std::runtime_error("image file not loaded");
    if (!play.loadFromFile("files/images/play.png")) throw std::runtime_error("image file not loaded");
    if (!leaderboard.loadFromFile("files/images/leaderboard.png")) throw std::runtime_error("image file not loaded");

    // digits
    for (int i = 0; i < 11; i++) {
        auto* tempDigitTexture = new sf::Texture;
        if (!tempDigitTexture->loadFromFile("files/images/digits.png", sf::IntRect(i*21, 0, 21, 32))) throw std::runtime_error("image file not loaded");
        digits.push_back(tempDigitTexture);
    }
}
