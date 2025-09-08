#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;


class FlappyBirdGame {
protected:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text scoreText;

    int screenWidth;
    int screenHeight;
    int score;
    bool isGameOver;

public:
    FlappyBirdGame() {
        screenWidth = 800;
        screenHeight = 600;
        score = 0;
        isGameOver = false;

        window.create(sf::VideoMode(screenWidth, screenHeight), "Flappy Bird");

        if (!font.loadFromFile("arial.ttf")) {
            cout << "Error loading font!" << endl;
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        scoreText.setString("Score: 0");

        window.setFramerateLimit(60);
        cout << "SFML Flappy Bird Game Initialized!" << endl;
    }

    virtual void startGame() { //Run time polymorphism
        score = 0;
        isGameOver = false;
        cout << "Game Started!" << endl;
    }

    void run() {
        sf::Clock clock;

        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            handleEvents();

            if (!isGameOver) {
                update(dt);
            }

            render();
        }
    }

    virtual void handleEvents() { //Run time polymorphism
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    virtual void update(float dt) { //Run time polymorphism
        scoreText.setString("Score: " + to_string(score));
    }

    virtual void render() { //Run time polymorphism
        window.clear(sf::Color::Cyan);
        window.draw(scoreText);
        window.display();
    }

    void endGame() {
        isGameOver = true;
        cout << "Game Over! Final Score: " << score << endl;
    }

    void increaseScore(int value = 1) {
        score += value;
    }

    bool gameOver() const {
        return isGameOver;
    }

    int getScore() const {
        return score;
    }

    sf::RenderWindow& getWindow() {
        return window;
    }
};


class Bird {
private:
    sf::CircleShape shape;
    float velocity;
    float gravity;
    float jump;

public:
    float positionY;

    Bird(float x, float y) {
        shape.setRadius(20);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(x, y);

        velocity = 0;
        gravity = 900;
        jump = -350;
        positionY = y;
    }

    void flap() {
        velocity = jump;
    }

    void update(float dt) {
        velocity += gravity * dt;
        positionY += velocity * dt;

        if (positionY < 0) positionY = 0;
        if (positionY > 580) {
            positionY = 580;
            velocity = 0;
        }

        shape.setPosition(shape.getPosition().x, positionY);
    }

    void draw(sf::RenderWindow& window) { //Association
        window.draw(shape);
    }
     
    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
};

class Pipe {
private:
    sf::RectangleShape top, bottom;
    bool passed;

public:
    Pipe(float x, float gapStart, float gapSize) {
        top.setSize(sf::Vector2f(60, gapStart));
        top.setFillColor(sf::Color::Green);
        top.setPosition(x, 0);

        bottom.setSize(sf::Vector2f(60, 600 - gapStart - gapSize));
        bottom.setFillColor(sf::Color::Green);
        bottom.setPosition(x, gapStart + gapSize);

        passed = false;
    }

    void update(float dt, float speed) {
        top.move(-speed * dt, 0);
        bottom.move(-speed * dt, 0);
    }

    bool isOffScreen() const {
        return top.getPosition().x + top.getSize().x < 0;
    }

    bool collidesWith(const sf::FloatRect& rect) {
        return top.getGlobalBounds().intersects(rect) || bottom.getGlobalBounds().intersects(rect);
    }

    float getX() const {
        return top.getPosition().x;
    }

    bool isPassed() const {
        return passed;
    }

    void markPassed() {
        passed = true;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(top);
        window.draw(bottom);
    }
};

class MyFlappyGame : public FlappyBirdGame {
private:
    Bird bird;//Composition with Bird class
    vector<Pipe> pipes; //Composition with Pipe
    float pipeTimer;
    float pipeInterval;
    float pipeSpeed;

public:
    MyFlappyGame() : bird(100, 300) {
        pipeTimer = 0;
        pipeInterval = 1.5f;
        pipeSpeed = 200;
        srand(static_cast<unsigned>(time(0)));
    }

    void startGame() { //Run time POlymorphism overriding
        FlappyBirdGame::startGame();
        pipes.clear();
        bird = Bird(100, 300);
        pipeTimer = 0;
    }

    void handleEvents() { //Run time POlymorphism overriding
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!isGameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                bird.flap();
            }

            if (isGameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                startGame();
            }
        }
    }

    void update(float dt) { //Run time POlymorphism overriding
        bird.update(dt);
        pipeTimer += dt;

        if (pipeTimer >= pipeInterval) {
            float gapY = rand() % 300 + 100;
            pipes.push_back(Pipe(800, gapY, 150));
            pipeTimer = 0;
        }

        for (int i = 0; i < pipes.size(); i++) {
            pipes[i].update(dt, pipeSpeed);
        }

        // Remove pipes that are off-screen
        vector<Pipe> remainingPipes;
        for (int i = 0; i < pipes.size(); i++) {
            if (!pipes[i].isOffScreen()) {
                remainingPipes.push_back(pipes[i]);
            }
        }
        pipes = remainingPipes;

        // Checks for collisions and scoring
        for (int i = 0; i < pipes.size(); i++) {
            if (pipes[i].collidesWith(bird.getBounds())) {
                endGame();
                return;
            }

            if (!pipes[i].isPassed() && pipes[i].getX() + 60 < 100) {
                pipes[i].markPassed();
                increaseScore();
            }
        }

        if (bird.positionY >= 580) {
            endGame();
        }
    }

    void render() { //Run time POlymorphism overriding
        window.clear(sf::Color::Cyan);

        if (isGameOver) {
            scoreText.setString("Game Over! Final Score: " + to_string(score) + "\nPress Space to Restart");
        }
        else {
            scoreText.setString("Score: " + to_string(score));
        }

        window.draw(scoreText);
        bird.draw(window);

        for (int i = 0; i < pipes.size(); i++) {
            pipes[i].draw(window);
        }

        window.display();
    }
};

int main() {
    MyFlappyGame game;
    game.startGame();
    game.run();
    return 0;
}
