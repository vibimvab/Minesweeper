#include <string>
#include <SFML/Graphics.hpp>
#include "Window.h"

int main() {
    float width = 800, height = 600;

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf"))
        throw std::runtime_error("invalid font file");

    Window* window = new WelcomeWindow(font, width, height);
    std::string userName;
    window->run(userName);
    if (userName.empty())
        return 0;

    window = new GameWindow(font, "files/board_config.cfg", userName);
    window->run();

    return 0;
}
