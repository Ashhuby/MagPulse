#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Global variables
const float GravityMultiplier = 100.0;
const float Restitution = 0.8f; // 1 = perfectly elastic 0 = perfectly ineleastic
const float Gravity = 9.80f * GravityMultiplier;
const float MaxSpeed = 3000.0;

// Walls
//std::vector<float> Walls = {};
const float FloorPosition = 600.0;
const float RightWallPosition = 800.0;
const float CeilingPosition = 0.0;
const float LeftWallPosition = 0.0;


struct Ball{
    sf::Vector2f position;
    float radius;
    sf::Color colour;
    sf::Vector2f velocity;

public:
    Ball(sf::Vector2f Position, float Radius = 1.0f, sf::Color Colour = sf::Color::White) {
        radius = Radius;
        colour = Colour;
        position = Position;
        velocity = { 0.0f,0.0f };
    }
};


void DrawBall(Ball b, sf::RenderWindow& window) {
    sf::CircleShape circle;
    circle.setRadius(b.radius);
    circle.setPosition({b.position.x - b.radius, b.position.y - b.radius}); // Centre the pos 
    circle.setFillColor(b.colour); 
    window.draw(circle); 
}

void UpdateBall(Ball& b, float delta) {
    // Gravity is a thing?
    b.velocity.y += Gravity * delta;

    b.position += b.velocity * delta;

    // Floor detection
    if (b.position.y >= (FloorPosition - b.radius)) {
        b.position.y = FloorPosition - b.radius;   // Move back 
        b.velocity.y *= -1 * Restitution;
    }  
    if (b.position.y <= (CeilingPosition + b.radius)) {
         b.position.y = CeilingPosition + b.radius;   
         b.velocity.y *= -1 * Restitution;
    }


    // Wall detection
    if (b.position.x >= (RightWallPosition - b.radius)) {
        b.position.x = RightWallPosition - b.radius;
        b.velocity.x *= -1 * Restitution;
    }
    if (b.position.x <= (LeftWallPosition + b.radius)) {
        b.position.x = LeftWallPosition + b.radius;
        b.velocity.x *= -1 * Restitution;
    }
    
    if (b.velocity.y >= MaxSpeed) b.velocity.y = MaxSpeed; //std::cout << b.velocity.y;
    if (b.velocity.x >= MaxSpeed) b.velocity.x = MaxSpeed;
}

int main()
{
    std::cout << "=== BounceLab Starting ===" << std::endl;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "BounceLab");
    //window.setFramerateLimit(60);
    //std::cout << "FPS";
             
    // Set up balls
    std::vector<Ball> balls;

    Ball ball1 = Ball({ 400.0f,300.0f }, 25.0f, sf::Color::Red);
    Ball ball2 = Ball({ 250.0f,300.0f }, 45.0f, sf::Color::Magenta);
    balls.push_back(ball1);
    balls.push_back(ball2);

    // Set up dt
    sf::Clock dtClock;
    float dt; // 1/60 0.16ms

    // Loop
    while (window.isOpen())
    {
        // Events here
        dt = (float)dtClock.getElapsedTime().asSeconds();   //std::cout << "DT: " << dt << std::endl;
        dtClock.restart();

        // Click to add balls
        while (const std::optional event = window.pollEvent()) {
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                float x = mousePressed->position.x;
                float y = mousePressed->position.y;
                sf::Color randomColour(rand() % 256, rand() % 256, rand() % 256);
                float randomRadius = 10 + (rand() % 30);
                
                std::cout << x << ", " << y << std::endl;

                Ball ball0 = Ball({x,y}, randomRadius, randomColour);
                balls.push_back(ball0);
            }
        }

        window.clear(sf::Color::Black);

        // Draw and update balls
        for (Ball& b : balls) {           
            UpdateBall(b,dt);
            DrawBall(b, window);
        }

        window.display();
    }

    return 0;
}