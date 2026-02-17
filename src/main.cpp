#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Ball {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
    sf::Color color;
    
    Ball(sf::Vector2f pos, float r, sf::Color col) 
        : position(pos)
        , velocity({0.f, 0.f})
        , radius(r)
        , color(col)
    {
        std::cout << "Ball created at (" << pos.x << ", " << pos.y << ")" << std::endl;
    }
    
    // NEW: Function to draw the ball
    void draw(sf::RenderWindow& window) const {
        // Create a circle shape with our radius
        sf::CircleShape circle(radius);
        
        // Set its color
        circle.setFillColor(color);
        
        // IMPORTANT: CircleShape's position is the TOP-LEFT corner of its bounding box
        // So we need to subtract radius from x and y to center it at our position
        circle.setPosition({position.x - radius, position.y - radius});
        
        // Draw it to the window
        window.draw(circle);
    }
};

int main()
{
    std::cout << "BounceLab Starting..." << std::endl;



    // ========== VERSION CHECKING CODE ==========
    std::cout << "\n=== COMPILER INFORMATION ===" << std::endl;
    std::cout << "__cplusplus: " << __cplusplus << std::endl;

#ifdef _MSVC_LANG
    std::cout << "_MSVC_LANG: " << _MSVC_LANG << std::endl;
#endif

#ifdef _HAS_CXX17
    std::cout << "_HAS_CXX17 is defined" << std::endl;
#else
    std::cout << "_HAS_CXX17 is NOT defined" << std::endl;
#endif

#ifdef _MSC_VER
    std::cout << "MSVC version: " << _MSC_VER << std::endl;
#endif

    std::cout << "\n=== SFML INFORMATION ===" << std::endl;
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "."
        << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    std::cout << "\n=== STARTING SIMULATION ===" << std::endl;
    // ==========================================

    
    sf::RenderWindow window(sf::VideoMode({800, 600}), "BounceLab");
    window.setFramerateLimit(60);
    
    // Create a vector to hold our balls
    std::vector<Ball> balls;
    
    // Add one ball at the top center
    balls.push_back(Ball({400.f, 100.f}, 20.f, sf::Color::Red));
    
    sf::Clock clock;
    
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        // Clear with dark gray
        window.clear(sf::Color(50, 50, 50));
        
        // NEW: Draw all balls
        for (const auto& ball : balls) {
            ball.draw(window);
        }
        
        window.display();
    }
    
    return 0;
}