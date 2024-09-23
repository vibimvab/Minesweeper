//
// Created by Insung Seo on 2023/04/20.
//

#ifndef MINESWEEPER_BUTTON_H
#define MINESWEEPER_BUTTON_H


#include <SFML/Graphics.hpp>

class Button {
public:
    float xPos, yPos;
    float width, height;
    sf::Sprite sprite;

    Button(float xPos, float yPos);
    bool isClicked(sf::Vector2i) const;
    void display(sf::RenderWindow* window) const;

    virtual void setLost() {throw std::runtime_error("invalid call of setLose function of Button class");}
    virtual void setWin() {throw std::runtime_error("invalid call of setWin function of Button class");}
    virtual void setOngoing() {throw std::runtime_error("invalid call of setOngoing function of Button class");}
    virtual void setPause() {throw std::runtime_error("invalid call of setPause function of Button class");}
    virtual void setResume() {throw std::runtime_error("invalid call of setResume function of Button class");}
};

class FaceButton: public Button {
    sf::Texture* happyFaceTexture;
    sf::Texture* winFaceTexture;
    sf::Texture* loseFaceTexture;

public:
    FaceButton(float xPos, float yPos, sf::Texture* happyFaceTexture, sf::Texture* winFaceTexture, sf::Texture* loseFaceTexture);

    void setLost() override;
    void setWin() override;
    void setOngoing() override;
};

class DebugButton: public Button {
public:
    DebugButton(float xPos, float yPos, sf::Texture* debugTexture);
};

class PauseButton: public Button {
    sf::Texture* pauseTexture;
    sf::Texture* playTexture;

public:
    PauseButton(float xPos, float yPos, sf::Texture* pauseTexture, sf::Texture* playTexture);

    void setPause() override;
    void setResume() override;
};

class LeaderboardButton: public Button {
public:
    LeaderboardButton(float xPos, float yPos, sf::Texture* leaderboardTexture);
};


#endif //MINESWEEPER_BUTTON_H
