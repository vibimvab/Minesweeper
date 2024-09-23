//
// Created by Insung Seo on 2023/04/20.
//

#include "Button.h"

// base button class
Button::Button(float xPos, float yPos) {
    this->xPos = xPos;
    this->yPos = yPos;
    sprite.setPosition(xPos, yPos);
    width = height = 64.f;
}

bool Button::isClicked(sf::Vector2i mousePos) const {
    return (xPos < (float)mousePos.x && (float)mousePos.x < xPos + width
            && yPos < (float)mousePos.y && (float)mousePos.y < yPos + height);
}

void Button::display(sf::RenderWindow* window) const {
    window->draw(sprite);
}


// face button class
FaceButton::FaceButton(float xPos, float yPos, sf::Texture* happyFaceTexture, sf::Texture* winFaceTexture, sf::Texture* loseFaceTexture) : Button(xPos, yPos) {
    this->happyFaceTexture = happyFaceTexture;
    this->winFaceTexture = winFaceTexture;
    this->loseFaceTexture = loseFaceTexture;

    sprite.setTexture(*happyFaceTexture);
}

void FaceButton::setLost() {
    sprite.setTexture(*loseFaceTexture);
}

void FaceButton::setWin() {
    sprite.setTexture(*winFaceTexture);
}

void FaceButton::setOngoing() {
    sprite.setTexture(*happyFaceTexture);
}


// debugging button class
DebugButton::DebugButton(float xPos, float yPos, sf::Texture* debugTexture) : Button(xPos, yPos) {
    sprite.setTexture(*debugTexture);
}


// paused button class
PauseButton::PauseButton(float xPos, float yPos, sf::Texture* pauseTexture, sf::Texture* playTexture) : Button(xPos, yPos) {
    this->pauseTexture = pauseTexture;
    this->playTexture = playTexture;

    sprite.setTexture(*pauseTexture);
}

void PauseButton::setPause() {
    sprite.setTexture(*pauseTexture);
}

void PauseButton::setResume() {
    sprite.setTexture(*playTexture);
}


// leaderboard button class
LeaderboardButton::LeaderboardButton(float xPos, float yPos, sf::Texture* leaderboardTexture) : Button(xPos, yPos) {
    sprite.setTexture(*leaderboardTexture);
}
