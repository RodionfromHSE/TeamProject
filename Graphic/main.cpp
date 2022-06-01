#include <SFML/Graphics.hpp>

#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>


int speed = 10;
int horizontal = 1700;
int vertical = 600;
int leftBorder = 600;
int jump = 50;

struct World {
    const float groundLevel = 600;
    const float gravitation = 1500;
    const float rightBorder = 1200;
    const float leftBorder = 600;

    float cameraX = 0;
    std::vector<sf::Vector2f> obstacles;
};

struct Player {
    Player(const sf::Texture &texture,
           sf::Vector2f initialPos,
           sf::Vector2f scale,
           sf::Vector2f origin,
           World *world)
            : origin(origin),
              position(initialPos),
              scale(scale),
              sprite(texture),
              world(world) {}

    void processEvents() {
        float right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        float left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

        velocity.x = horSpeed * (right - left);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onTheGround)
            velocity.y -= jumpSpeed;
    }

    void update(float dt) {
        velocity.y += world->gravitation * dt;
        position += velocity * dt;

        position.y = std::min(world->groundLevel, position.y);
        onTheGround = position.y >= world->groundLevel - 1e-5;
        if (onTheGround) velocity.y = 0;

        for (auto obPos : world->obstacles) { //TODO: убрать магические константы
            if ((position.x >= obPos.x - 60) && (position.x < obPos.x + 15) && position.y >= 560) {
                position.x -= 11;
            }
            if ((position.x >= obPos.x + 15) && (position.x < obPos.x + 140) && position.y >= 560) {
                position.x += 11;
            }
        }

        float screenX = -world->cameraX + position.x;
        if (screenX > world->rightBorder)
            world->cameraX -= world->rightBorder - screenX;
        if (screenX < world->leftBorder)
            world->cameraX -= world->leftBorder - screenX;
    }

    void draw(sf::RenderWindow &render) {
        if (std::abs(velocity.x) > 0)
            isFlipped = velocity.x > 0;

        float flipFactor = isFlipped ? 1 : -1;
        auto drawPos = position
                       - sf::Vector2f{origin.x * scale.x * flipFactor, origin.y * scale.y}
                       - sf::Vector2f{world->cameraX, 0};
        sprite.setScale(flipFactor * scale.x, scale.y);
        sprite.setPosition(drawPos);
        render.draw(sprite);
    }

private:
    const sf::Vector2f origin; // center of object relative to sprite
    const sf::Vector2f scale;
    const float jumpSpeed = 800;
    const float horSpeed = 500;
    sf::Vector2f velocity;
    sf::Vector2f position;
    sf::Sprite sprite;

    World *world;

    bool isFlipped = false;
    bool onTheGround = false;
};

int main() {
    srand(time(0));

    sf::RenderWindow app(sf::VideoMode(1700, 1000), "Team PR 1");
    app.setFramerateLimit(60);

    sf::Texture bgTex, obstacleTex, persATex, persBTex;
    std::string pref = "../Pictures/Free/";
    bgTex.loadFromFile(pref + "Background/Blue.png");
    obstacleTex.loadFromFile(pref + "Items/Fruits/Apple.png");
    persATex.loadFromFile(pref + "Main Characters/Mask Dude/Idle/0.png");
    persBTex.loadFromFile(pref + "Main Characters/Mask Dude/Idle/0.png");

    sf::Sprite sBackground(bgTex), sObstacle(obstacleTex);


    World world;
    Player persA(persATex, {1000, 300}, {-1, 1}, {75, 0}, &world);

    world.obstacles.emplace_back(100, world.groundLevel);
    for (int i = 0; i < 100; ++i) {
        float lastX = world.obstacles.back().x;
        world.obstacles.emplace_back(lastX + rand() % 500 + 400, world.groundLevel);
    }

    sf::Clock deltaClock;
    while (app.isOpen()) {
        sf::Time dt = deltaClock.restart();

        sf::Event e{};
        while (app.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                app.close();
        }

        app.draw(sBackground);

        for (auto pos: world.obstacles) {
            sObstacle.setPosition(pos.x - world.cameraX, pos.y);
            app.draw(sObstacle);
        }

        persA.processEvents();
        persA.update(dt.asSeconds());
        persA.draw(app);

        app.display();
        app.clear();
    }
}
