#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>  
#include <vector>
#include <cmath>
#include <optional>

int main() {
    std::cout << "Circle drawn.";

    sf::RenderWindow window(sf::VideoMode({800, 600}), "MagPulse SFML 3.0 Test");
    
    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({375.f, 275.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}
