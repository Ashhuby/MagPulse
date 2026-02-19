#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// Window 
const float WindowWidth = 800;
const float WindowHeight = 600;

// Global variables
const float GravityMultiplier = 100.0;
const float Restitution = 1.0f; // 1 = perfectly elastic 0 = perfectly ineleastic
const float Gravity = 9.80f * GravityMultiplier;
const float MaxSpeed = 3000.0;

// Walls
const float FloorPosition = WindowHeight;
const float RightWallPosition = WindowWidth;
const float CeilingPosition = 0.0;
const float LeftWallPosition = 0.0;

struct Ball{
    sf::Vector2f position;
    float radius;
    sf::Color colour;
    sf::Vector2f velocity;

public:    
    Ball(sf::Vector2f Position, float Radius = 1.0f, sf::Color Colour = sf::Color::White, sf::Vector2f Velocity = {0.0f,0.0f}) {
        radius = Radius;
        colour = Colour;
        position = Position;
        velocity = Velocity;
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

void DetectBallCollisions(std::vector<Ball>& b) {
    for (int i = 0; i < b.size(); i++) {
        Ball& currentBall = b[i];
        for (int j = i + 1; j < b.size(); j++) {
            Ball& otherBall = b[j];
            float distanceX = currentBall.position.x - otherBall.position.x;
            float distanceY = currentBall.position.y - otherBall.position.y;
            float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY); // Dot product between x and y then abs value
            if (std::abs(distance) <= (currentBall.radius + otherBall.radius)) {
                //std::cout << "Collision Detected" << std::endl;
                sf::Vector2f normal = (currentBall.position - otherBall.position) / distance; // Vector pointing from ball j to ball i
                float overlap = (currentBall.radius + otherBall.radius) - distance; // This is how much overlap between the balls duh
                sf::Vector2f relativeVelocity = currentBall.velocity - otherBall.velocity;

                // Seperate balls
                currentBall.position += normal * (overlap * 0.5f);  // Half the overlap distance (equal masses)
                otherBall.position -= normal * (overlap * 0.5f);

                float velAlongNormal = (relativeVelocity.x * normal.x) + (relativeVelocity.y * normal.y);
                if (velAlongNormal > 0) continue;

                float impulse = (- (1 + Restitution) * velAlongNormal) / 2; // Divide by 2 because masses are equal (1/m1 + 1/m2 = 2)               
                sf::Vector2f impulseVector = normal * impulse;
                currentBall.velocity += impulseVector;
                otherBall.velocity -= impulseVector;


            }
        }
    }
}

int main()
{
    std::cout << "=== BounceLab Starting ===" << std::endl;

    sf::RenderWindow window(sf::VideoMode({ (int)WindowWidth, (int)WindowHeight}), "BounceLab");
    //window.setFramerateLimit(60);
    //std::cout << "FPS";
             
    // Set up balls
    std::vector<Ball> balls;

    // Set up dt
    sf::Clock dtClock;
    float dt; // 1/60 0.16ms

    // Set up drag state
    bool isDragging = false;
    float initialMouseX;
    float initialMouseY;
    float currentMouseX;
    float currentMouseY;

    //
    sf::Color randomColour = sf::Color::White;  // Default color
    float randomRadius = 25.0f;                  // Default radius

    // Loop
    while (window.isOpen())
    {
        // Events here
        dt = (float)dtClock.getElapsedTime().asSeconds();   //std::cout << "DT: " << dt << std::endl;
        dtClock.restart();

        // Add balls with mouse
        while (const std::optional event = window.pollEvent()) {           
            // TODO: drag state events
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    //Store inital mouse pos and show indicator 
                    isDragging = true;
                    initialMouseX = mousePressed->position.x;
                    initialMouseY = mousePressed->position.y;

                    // Set current to sanme position initally 
                    currentMouseX = initialMouseX;
                    currentMouseY = initialMouseY;
                }
             }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                // TODO: Update current mouse position during drag
                if (isDragging) {
                currentMouseX = mouseMoved->position.x;
                currentMouseY = mouseMoved->position.y;
                }
            }
            // RELEASE MEEE
            if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (isDragging) {
                    // TODO: Velocity Calculation
                    float dragX = currentMouseX - initialMouseX;
                    float dragY = currentMouseY - initialMouseY;

                    float strength = 5.0f;
                    float velocityX = (initialMouseX - currentMouseX) * strength;
                    float velocityY = (initialMouseY - currentMouseY) * strength;
                    randomColour = sf::Color(rand() % 256, rand() % 256, rand() % 256);
                    randomRadius = 25 + (rand() % 5);
                    Ball ball0 = Ball({initialMouseX, initialMouseY }, randomRadius, randomColour,{velocityX, velocityY});
                    balls.push_back(ball0);

                   isDragging = false;
                }                
            }                                         
        }

        window.clear(sf::Color::Black);   
                
        // Draw and update balls
        for (Ball& b : balls) {           
            UpdateBall(b,dt);
            DrawBall(b, window);
        }
        
        // Draw indicator and ghost ball
        if (isDragging) {
            // Indicator vertis
            sf::Vertex point1;
            sf::Vertex point2;
            point1.position = sf::Vector2f(initialMouseX, initialMouseY);
            point2.position = sf::Vector2f(currentMouseX, currentMouseY);
            point1.color = sf::Color::Yellow;
            point2.color = sf::Color::Yellow;
            sf::Vertex line[] = { point1, point2 };
            window.draw(line, 2, sf::PrimitiveType::Lines);

            // Ghost ball
            sf::CircleShape ghostCircle;
            float ghostCirleRadius = 12.0;
            ghostCircle.setRadius(ghostCirleRadius);
            ghostCircle.setFillColor(sf::Color(255,255,255,100));

            ghostCircle.setOutlineThickness(1.0f);
            ghostCircle.setOutlineColor(sf::Color::White);
            ghostCircle.setPosition({ currentMouseX - ghostCirleRadius, currentMouseY - ghostCirleRadius });
            window.draw(ghostCircle);
        }

        // Detect ball collisions
        DetectBallCollisions(balls);

        window.display();
    }
    return 0;
}

