//
// Created by Insung Seo on 2023/04/18.
//

#include <fstream>
#include <ctime>
#include <random>
#include <vector>
#include <chrono>
#include <sstream>
#include "Window.h"
#include "Board.h"
#include "Button.h"
#include "LinkedList.h"

// private functions
// initializers
void Board::generateMines() {
    /* I believe this method to generate mines are way better than picking another random number when repeating number is picked,
     * considering such edge case as generating 400th mine from 400-tiles board */
    std::mt19937 random_mt(time(nullptr));
    std::uniform_int_distribution<int> dist;
    int randomHolder[totalMine];
    for (int i = 0; i < totalMine; i++) {
        dist = std::uniform_int_distribution<>(0, (int)tileWidth * (int)tileHeight - i - 1);
        randomHolder[i] = dist(random_mt);
    }
    LinkedList<int> locations;
    for (int i = 0; i < tileWidth * tileHeight; i++)
        locations.AddTail(i);

    for (unsigned int i = 0; i < totalMine; i++) {
        mineLocation.push_back(locations.Pop(randomHolder[i]));
    }

    // check if mines are distributed correctly
    for (int i = 0; i < totalMine; i++) {
        for (int j = i+1;  j < totalMine; j++) {
            if (mineLocation.at(i) == mineLocation.at(j))
                throw std::runtime_error("mine distributed unsuccessfully");
        }
    }
}

void Board::setMines() {
    for (int i = 0; i < totalMine; i++) {
        board[(mineLocation.at(i) / tileHeight)][mineLocation.at(i) % tileHeight].setMine();
    }
}

int Board::countAdjacentMines(std::vector<Tile>& list, int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (list[i].isMined())
            count++;
    }

    return count;
}

void Board::setAdjacent() const {
    std::vector<Tile> adjacentTiles;
    for (unsigned int i = 0; i < tileWidth; i++) {
        for (unsigned int j = 0; j < tileHeight; j++) {
            if (i == 0) {
                if (j == 0) { // top-left corner
                    adjacentTiles = {               board[i][j+1],
                                     board[i+1][j], board[i+1][j+1]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 3));

                } else if (j == tileHeight - 1) { // top-right corner
                    adjacentTiles = {board[i][j-1],
                                     board[i+1][j-1], board[i+1][j]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 3));

                } else { // top edge
                    adjacentTiles = {board[i][j-1],                  board[i][j+1],
                                     board[i+1][j-1], board[i+1][j], board[i+1][j+1]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 5));
                }

            } else if (i == tileWidth - 1) {
                if (j == 0) { // bottom-left corner
                    adjacentTiles = {board[i-1][j], board[i-1][j+1],
                                                    board[i][j+1]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 3));

                } else if (j == tileHeight - 1) { // bottom-right corner
                    adjacentTiles = {board[i-1][j-1], board[i-1][j],
                                     board[i][j-1]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 3));

                } else { // bottom edge
                    adjacentTiles = {board[i-1][j-1], board[i-1][j], board[i-1][j+1],
                                     board[i][j-1],                  board[i][j+1],};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 5));
                }

            } else {
                if (j == 0) { // left edge
                    adjacentTiles = {board[i-1][j], board[i-1][j+1],
                                                    board[i][j+1],
                                     board[i+1][j], board[i+1][j+1]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 5));

                } else if (j == tileHeight - 1) { // right edge
                    adjacentTiles = {board[i-1][j-1], board[i-1][j],
                                     board[i][j-1],
                                     board[i+1][j-1], board[i+1][j]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 5));

                } else { // middle tile
                    adjacentTiles = {board[i-1][j-1], board[i-1][j], board[i-1][j+1],
                                     board[i][j-1],                  board[i][j+1],
                                     board[i+1][j-1], board[i+1][j], board[i+1][j+1]};
                    board[i][j].setAdjacent(countAdjacentMines(adjacentTiles, 8));

                }
            }
        }
    }
}


// set game status
void Board::reset() {
    Buttons.at("face")->setOngoing();

    for (unsigned int i = 0; i < tileWidth; i++){
        for (unsigned int j = 0; j < tileHeight; j++) {
            board[i][j].reset();
        }
    }

    mineLocation.clear();
    currentMine = totalMine;
    setCounter();
    unrevealedTile = tileWidth * tileHeight;
    gameStatus = ongoing;
    debug = false;

    timePreviousPortion = timeCurrentPortion = 0;
    minuteDigit1.setTexture(*Images.digits.at(0));
    minuteDigit2.setTexture(*Images.digits.at(0));
    secondDigit1.setTexture(*Images.digits.at(0));
    secondDigit2.setTexture(*Images.digits.at(0));

    generateMines();
    setMines();
    setAdjacent();
}

void Board::setLose() {
    for (unsigned int i = 0; i < tileWidth; i++){
        for (unsigned int j = 0; j < tileHeight; j++) {
            board[i][j].setLost();
        }
    }

    Buttons.at("face")->setLost();
}

void Board::setWin() {
    gameStatus = win;
    Buttons.at("face")->setWin();
    for (unsigned int i = 0; i < tileWidth; i++){
        for (unsigned int j = 0; j < tileHeight; j++) {
            board[i][j].setWin();
        }
    }
    updateLeaderBoard((timePreviousPortion + timeCurrentPortion)/1000000);

    currentMine = 0;
    setCounter();
    display();
    window->display();
    showLeaderBoard();
}

void Board::setPause() {
    gameStatus = pause;
    Buttons.at("pause")->setResume(); // set pause button's image to resume icon
    timePreviousPortion += timeCurrentPortion;
    timeCurrentPortion = 0;
    for (unsigned int i = 0; i < tileWidth; i++){
        for (unsigned int j = 0; j < tileHeight; j++) {
            board[i][j].setPause();
        }
    }
}

void Board::setResume() {
    for (unsigned int i = 0; i < tileWidth; i++){
        for (unsigned int j = 0; j < tileHeight; j++) {
            board[i][j].setResume();
        }
    }
    timeStart = std::chrono::steady_clock::now();
    gameStatus = static_cast<GameStatus>(0);
    Buttons.at("pause")->setPause(); // set pause button's image to pause icon
}

void Board::showLeaderBoard() {
    LeaderboardWindow leaderboardWindow(*font, tileWidth, tileHeight);
    leaderboardWindow.run();
}


void Board::updateLeaderBoard(unsigned long time) {
    // reading leaderboard.txt
    std::ifstream leaderBoard("files/leaderboard.txt");
    if (!leaderBoard.is_open()) throw std::invalid_argument("invalid leaderboard.txt file");
    std::string line;
    std::vector<std::string> ranking;

    for (int i = 0; i < 5 && getline(leaderBoard, line); i++) {
        ranking.push_back(line);
    }

    // adjusting minute and second to hold 2 spaces
    std::string minuteString = to_string(time / 60);
    if (time / 60 < 10) minuteString = '0' + minuteString;
    std::string secondString = to_string(time % 60);
    if (time % 60 < 10) secondString = '0' + secondString;

    // insert new record
    std::string newChallenger(minuteString + ':' + secondString + ',' + username + '*');
    for (int i = 0; i < 5 && i < ranking.size(); i++) {
        if (stoi(ranking.at(i).substr(0, 2)) > time / 60
            || (stoi(ranking.at(i).substr(0, 2)) == time / 60 && stoi(ranking.at(i).substr(3, 2)) >= time % 60)) {
            auto it = ranking.begin();
            ranking.insert(it + i, newChallenger);
            break;
        }
    }

    // output
    ofstream outputFile;
    outputFile.open("files/leaderboard.txt");
    for (int i = 0; i < 5 && i < ranking.size(); i++) {
        outputFile << ranking.at(i) << '\n';
    }
    outputFile.close();
}


// button clicked
void Board::menuClicked(sf::Vector2i mousePos) {
    if (Buttons.at("face")->isClicked(mousePos)) {
        faceButtonClicked();
    } else if (Buttons.at("debugging")->isClicked(mousePos)) {
        debugButtonClicked();
    } else if (Buttons.at("pause")->isClicked(mousePos)) {
        pauseButtonClicked();
    } else if (Buttons.at("leaderboard")->isClicked(mousePos)) {
        leaderboardButtonClicked();
    }
}

void Board::faceButtonClicked() {
    reset();
}

void Board::debugButtonClicked() {
    debug = !debug;
}

void Board::pauseButtonClicked() {
    if (gameStatus == ongoing) {
        setPause();
    } else if (gameStatus == pause) {
        setResume();
    }
}

void Board::leaderboardButtonClicked(){
    if (gameStatus == ongoing) {
        setPause();
        display();
        window->display();

        showLeaderBoard();

        setResume();
        display();
        window->display();

    } else {
        showLeaderBoard();
    }
}


// board clicked
bool Board::leftClicked(sf::Vector2i mousePos) {
    int xPos = mousePos.x/32;
    int yPos = mousePos.y/32;
    int result = board[xPos][yPos].isLeftClicked();
    if (unrevealedTile == tileWidth * tileHeight && timePreviousPortion + timeCurrentPortion == 0) {
        timeStart = std::chrono::steady_clock::now();
    }

    if (result == 0) {
        unrevealedTile--;
        Board::leftClicked(xPos + 1, yPos + 1);
        Board::leftClicked(xPos + 1, yPos);
        Board::leftClicked(xPos + 1, yPos - 1);
        Board::leftClicked(xPos, yPos + 1);
        Board::leftClicked(xPos, yPos - 1);
        Board::leftClicked(xPos - 1, yPos + 1);
        Board::leftClicked(xPos - 1, yPos);
        Board::leftClicked(xPos - 1, yPos - 1);
        if (unrevealedTile == totalMine)
            setWin();
        return true;

    } else if (result == -1) {
        return false;
    } else if (result == 1) {
        unrevealedTile--;
        if (unrevealedTile == totalMine)
            setWin();
        return true;

    } else if (result == 2)
        return true;

    throw runtime_error("impossible entry");
}

void Board::leftClicked(int xPos, int yPos) {
    if (xPos < 0 || xPos >= tileWidth || yPos < 0 || yPos >= tileHeight) return;
    int result = board[xPos][yPos].isLeftClicked();
    if (result == 0 || result == 1) unrevealedTile--;

    if (result == 0) {
        Board::leftClicked(xPos + 1, yPos + 1);
        Board::leftClicked(xPos + 1, yPos);
        Board::leftClicked(xPos + 1, yPos - 1);
        Board::leftClicked(xPos, yPos + 1);
        Board::leftClicked(xPos, yPos - 1);
        Board::leftClicked(xPos - 1, yPos + 1);
        Board::leftClicked(xPos - 1, yPos);
        Board::leftClicked(xPos - 1, yPos - 1);
    }
}

void Board::rightClicked(sf::Vector2i mousePos) {
    currentMine -= board[mousePos.x/32][mousePos.y/32].isRightClicked();
    setCounter();
}


void Board::setCounter() {
    if (currentMine >= 0) {
        mineDigit1.setTexture(*Images.digits.at(currentMine / 100));
        mineDigit2.setTexture(*Images.digits.at((currentMine % 100) / 10));
        mineDigit3.setTexture(*Images.digits.at(currentMine % 10));

    } else {
        mineDigit1.setTexture(*Images.digits.at(10));
        mineDigit2.setTexture(*Images.digits.at(-(currentMine % 100) / 10));
        mineDigit3.setTexture(*Images.digits.at(-currentMine % 10));
    }
}


// public functions
// constructor
Board::Board(sf::RenderWindow* window, sf::Font* font, float width, float height, unsigned int tileWidth, unsigned int tileHeight, int numMine, std::string& username) {
    this->window = window;
    this->font = font;
    this->width = width;
    this->height = height;
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    this->username = username;
    unrevealedTile = tileWidth * tileHeight;
    currentMine = totalMine = numMine;
    generateMines();

    // setting buttons, timer, counter sprite
    // buttons
    Button* tempButtonPtr = new FaceButton((((float)tileWidth / 2.f) * 32) - 32, 32 *((float)tileHeight+0.5f),&Images.faceHappy, &Images.faceWin, &Images.faceLose);
    Buttons.emplace("face", tempButtonPtr);
    tempButtonPtr = new DebugButton(((float)tileWidth * 32) - 304, 32 *((float)tileHeight+0.5f),&Images.debug);
    Buttons.emplace("debugging", tempButtonPtr);
    tempButtonPtr = new PauseButton(((float)tileWidth * 32) - 240, 32 *((float)tileHeight+0.5f),&Images.pause, &Images.play);
    Buttons.emplace("pause", tempButtonPtr);
    tempButtonPtr = new LeaderboardButton(((float)tileWidth * 32) - 176, 32 * ((float)tileHeight + 0.5f), &Images.leaderboard);
    Buttons.emplace("leaderboard", tempButtonPtr);

    // digit sprites
    mineDigit1.setTexture(*Images.digits.at(totalMine/100));
    mineDigit1.setPosition(33, 32 * ((float)tileHeight+0.5f)+16);
    digitSprites.push_back(&mineDigit1);
    mineDigit2.setTexture(*Images.digits.at((totalMine%100)/10));
    mineDigit2.setPosition(33 + 21, 32 * ((float)tileHeight+0.5f) + 16);
    digitSprites.push_back(&mineDigit2);
    mineDigit3.setTexture(*Images.digits.at(totalMine%10));
    mineDigit3.setPosition(33 + 42, 32 * ((float)tileHeight+0.5f) + 16);
    digitSprites.push_back(&mineDigit3);

    minuteDigit1.setTexture(*Images.digits.at(0));
    minuteDigit1.setPosition(((float)tileWidth*32)-97, 32*((float)tileHeight+0.5f )+16);
    digitSprites.push_back(&minuteDigit1);
    minuteDigit2.setTexture(*Images.digits.at(0));
    minuteDigit2.setPosition(((float)tileWidth*32)-97 + 21, 32*((float)tileHeight+0.5f ) + 16);
    digitSprites.push_back(&minuteDigit2);

    secondDigit1.setTexture(*Images.digits.at(0));
    secondDigit1.setPosition(((float)tileWidth*32)-54, 32*((float)tileHeight+0.5f ) + 16);
    digitSprites.push_back(&secondDigit1);
    secondDigit2.setTexture(*Images.digits.at(0));
    secondDigit2.setPosition(((float)tileWidth*32)-54 + 21, 32*((float)tileHeight+0.5f ) + 16);
    digitSprites.push_back(&secondDigit2);


    // Tiles
    // setting tile textures to pass into tile class
    std::vector<sf::Texture*> tileTextures = {&Images.tileHidden, &Images.tileRevealed, &Images.flag, &Images.mine,
                                              &Images.num1, &Images.num2, &Images.num3, &Images.num4,
                                              &Images.num5, &Images.num6, &Images.num7, &Images.num8};
    board = new Tile*[tileWidth];
    Tile* col;
    for (unsigned int i = 0; i < tileWidth; i++){
        col = new Tile[tileHeight];
        for (unsigned int j = 0; j < tileHeight; j++) {
            col[j] = Tile(i, j, tileTextures);
        }
        board[i] = col;
    }

    this->setMines();
    this->setAdjacent();
}

Board::~Board() {
    for (unsigned int i = 0; i < tileWidth; i++) {
        delete[] board[i];
    }
    delete[] board;
}


// public functions
void Board::display() const {
    for (unsigned int i = 0; i < tileWidth; i++){
        for (unsigned int j = 0; j < tileHeight; j++) {
            board[i][j].display(window, gameStatus, debug);
        }
    }

    for (auto& buttonMap: Buttons) {
        buttonMap.second->display(window);
    }
    for (sf::Sprite* sprite: digitSprites) {
        window->draw(*sprite);
    }
}

void Board::clicked(sf::Vector2i mousePos) {
    // if bottom area of the window is clicked
    if (0 <= mousePos.x && (float)mousePos.x < width
        && tileHeight <= mousePos.y/32 && (float)mousePos.y < height) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            menuClicked(mousePos);

    // if game board is clicked
    } else if (0 <= mousePos.x && (float)mousePos.x/32 < width
               && 0 <= mousePos.y && mousePos.y/32 < tileHeight) {
        if (gameStatus == ongoing) {
            // left click
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (!leftClicked(mousePos)) {
                    setLose();
                    gameStatus = lose;
                }

            // right click
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                rightClicked(mousePos);

        }
    }
}

void Board::updateTime() {
    if (unrevealedTile != tileWidth * tileHeight && gameStatus == ongoing) {
        timeEnd = std::chrono::steady_clock::now();
        auto timeSpent =  std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart);
        timeCurrentPortion = timeSpent.count();

        unsigned int totalTime = (timePreviousPortion + timeCurrentPortion)/1000000;
        unsigned int totalMin = totalTime/60;
        unsigned char totalSec = totalTime%60;
        minuteDigit1.setTexture(*Images.digits.at((totalMin%100)/10));
        minuteDigit2.setTexture(*Images.digits.at(totalMin%10));
        secondDigit1.setTexture(*Images.digits.at(totalSec/10));
        secondDigit2.setTexture(*Images.digits.at(totalSec%10));
    }
}
