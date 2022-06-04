#include <utility>
#include <memory>
#include <cassert>
#include <ctime>
#include <iostream>
#include <random>

#include "network_system.h"
#include "game_object.h"
#include "position.h"
#include "graphics.h"
#include "animation.h"
#include "box2d.h"
#include "TextBox.h"
#include "startWindow.h"
#include "NetworkPlayerSystem.h"

constexpr std::size_t groundLevel = 300;
constexpr std::size_t worldWidth = 2048;
constexpr std::size_t worldHeight = 600;

enum Input {
    Left, Right, Up, Down, Stop, Exit
};

//Input input = Input::Stop;
sf::RenderWindow *appPtr = nullptr;
struct ColliderComponent : Component {
    sf::Vector2i boundingBox; // relative to position
};

using PlayerPtr = GameObject *;

struct PlayerSystem : System {
    explicit PlayerSystem(GameObjects &gameObjects, PlayerPtr &player, const DeltaTime &deltaTime)
            : gameObjects(gameObjects), player(player), deltaTime(deltaTime) {}

    void init() override {
        createPlayer();
    }

    void update() override {
        Input input = Input::Stop;
        assert(player);

        if (appPtr->hasFocus()){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                input = Input::Right;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                input = Input::Left;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                input = Input::Up;
            }

        }
        move(input);
    }

private:
    void createPlayer() {
        sf::Texture playerTexture;
        playerTexture.loadFromFile("../Pictures/Free/Main Characters/Pink Man/Sprite Sheet.png");

        auto playerUPtr = std::make_unique<GameObject>();
        player = &*playerUPtr;

        playerUPtr->addComponent(std::make_unique<PositionComponent>(0.0f, groundLevel));

        playerUPtr->addComponent(std::make_unique<Box2dComponent>(0.0f, groundLevel, 32 /* ширина изображения */,
                                                                  32 /* высота изображения */, true, false, "player"));

        playerUPtr->addComponent(std::make_unique<RenderingComponent>(
                playerTexture, sf::Vector2i{originX, originY}));
        playerUPtr->addComponent(std::make_unique<AnimatorComponent>(
                sf::Vector2u{animationFrameSize, animationFrameSize}, idleAnimation));

        gameObjects.push_back(std::move(playerUPtr));
    }

    void stop() {
        auto box2dComponent = player->getComponent<Box2dComponent>();
        b2Vec2 vel = box2dComponent->body->GetLinearVelocity();
        b2Vec2 force = b2Vec2(0, vel.y);
        box2dComponent->body->SetLinearVelocity(force);
    }

    void jump() {
        auto box2dComponent = player->getComponent<Box2dComponent>();
        box2dComponent->body->ApplyLinearImpulse(b2Vec2(0, -150), box2dComponent->body->GetWorldCenter(), true);
    }

    void runRight() {
        auto box2dComponent = player->getComponent<Box2dComponent>();
        b2Vec2 vel = box2dComponent->body->GetLinearVelocity();
        b2Vec2 force = b2Vec2(8, vel.y);
        box2dComponent->body->SetLinearVelocity(force);
    }

    void runLeft() {
        auto box2dComponent = player->getComponent<Box2dComponent>();
        b2Vec2 vel = box2dComponent->body->GetLinearVelocity();
        b2Vec2 force = b2Vec2(-8, vel.y);
        box2dComponent->body->SetLinearVelocity(force);
    }

    bool onGround() {//работает, но костыльная функция, нужно придумать нормальную функцию
        auto box2dComponent = player->getComponent<Box2dComponent>();
        b2Vec2 vel = box2dComponent->body->GetLinearVelocity();
        if (vel.y < 0.1 && vel.y > -0.1) {
            return true;
        } else {
            return false;
        }
    }


    void move(Input &input) {

        if (input != Input::Right && input != Input::Left) {
            stop();
        }

        if (input == Input::Right) {
            runRight();
        }

        if (input == Input::Left) {
            runLeft();
        }


        if (input == Input::Up) {
            if (onGround()) {
                jump();
            }
        }


    }


    void updatePlayerAnimation(int dx) {
        auto animationComponent = player->getComponent<AnimatorComponent>();
        assert(animationComponent);

        if (abs(dx) > 0) {
            animationComponent->setAnimation(runAnimation);
            animationComponent->horizontalFlip = (dx < 0);
        } else {
            animationComponent->setAnimation(idleAnimation);
        }
    }

    static constexpr float speedInPxsPerSecond = 80.0f;

    static constexpr std::size_t animationFrameSize = 32;
    static constexpr int originX = animationFrameSize / 2;
    static constexpr int originY = animationFrameSize;

    static constexpr float animationFps = 22.0f;

    static constexpr Animation idleAnimation = {0, 11, animationFps};
    static constexpr Animation runAnimation = {1, 12, animationFps};
    static constexpr Animation jumpAnimation = {2, 1, animationFps};
    static constexpr Animation hitAnimation = {3, 7, animationFps};
    static constexpr Animation fallAnimation = {4, 1, animationFps};
    static constexpr Animation wallJumpAnimation = {5, 5, animationFps};
    static constexpr Animation doubleJumpAnimation = {6, 6, animationFps};

    PlayerPtr &player;
    GameObjects &gameObjects;
    const DeltaTime &deltaTime;
};

struct PlayerCameraSystem : System {
    explicit PlayerCameraSystem(GameObjects &gameObjects,
                                const PlayerPtr &player, CameraPtr &camera)
            : gameObjects(gameObjects),
              player(player), camera(camera) {}

    void init() override {
        auto cameraUPtr = std::make_unique<GameObject>();
        camera = &*cameraUPtr;
        gameObjects.push_back(std::move(cameraUPtr));


        camera->addComponent(std::make_unique<PositionComponent>(0, 0));
        camera->addComponent(std::make_unique<CameraComponent>(0.0f));
    }


    void update() override {
        assert(player);
        assert(camera);

        auto cameraPositionComponent = camera->getComponent<PositionComponent>();
        auto cameraComponent = camera->getComponent<CameraComponent>();
        auto playerPositionComponent = player->getComponent<PositionComponent>();


        assert(cameraPositionComponent);
        assert(cameraComponent);
        assert(playerPositionComponent);

        cameraComponent->viewWidth = 300;
        cameraPositionComponent->x = playerPositionComponent->x;
        cameraPositionComponent->y = playerPositionComponent->y;
    }

    GameObjects &gameObjects;
    CameraPtr &camera;
    const PlayerPtr &player;
};

struct SurroundingsSystem : System {
    explicit SurroundingsSystem(GameObjects &gameObjects)
            : gameObjects(gameObjects) {}

    void init() override {
        createBackground();
        createGround();
    }

private:
    void createBackground() {
        sf::Texture backgroundTexture;
        backgroundTexture.loadFromFile("../Pictures/Free/Background/Blue.png");
        backgroundTexture.setRepeated(true);

        auto background = std::make_unique<GameObject>();
        background->addComponent(std::make_unique<PositionComponent>(-worldWidth, groundLevel - worldHeight));
        auto renderingComponent =
                background->addComponent(std::make_unique<RenderingComponent>(
                        backgroundTexture, sf::Vector2i{}, Layer::Background));
        renderingComponent->rect = sf::IntRect(0, 0, 2 * worldWidth, worldHeight);
        //background->addComponent(std::make_unique<Box2dComponent>(worldWidth, worldHeight / 2 + groundLevel + 32, 2 * worldWidth, worldHeight / 2, "Ground",
        //                                                           false));

        gameObjects.push_back(std::move(background));
    }

    void createGround() {
        sf::Texture groundTexture;
        groundTexture.loadFromFile("../Pictures/Free/Background/Pink.png");
        groundTexture.setRepeated(true);

        auto ground = std::make_unique<GameObject>();
        ground->addComponent(std::make_unique<PositionComponent>(-worldWidth, groundLevel));
        ground->addComponent(std::make_unique<RenderingComponent>(groundTexture))
                ->rect = sf::IntRect(0, 0, 2 * worldWidth, worldHeight);
        ground->addComponent(
                std::make_unique<Box2dComponent>(worldWidth, worldHeight / 2 + groundLevel + 32, 2 * worldWidth,
                                                 worldHeight / 2,
                                                 false, false, "ground"));
        gameObjects.push_back(std::move(ground));
    }


    GameObjects &gameObjects;
};

struct CoinsSystem : System {
    explicit CoinsSystem(GameObjects &gameObjects, const PlayerPtr &player)
            : gameObjects(gameObjects), player(player) {}

    void init() override {
        createStrawberry();
    }

private:
    void createStrawberry() {
        sf::Texture fruitTexture;
        fruitTexture.loadFromFile("../Pictures/Free/Items/Fruits/Cherries.png");

        auto fruitUPtr = std::make_unique<GameObject>();
        fruitUPtr->addComponent(std::make_unique<PositionComponent>(40.0f, groundLevel));
        fruitUPtr->addComponent(std::make_unique<RenderingComponent>(
                fruitTexture, sf::Vector2i{16, 32}));
        fruitUPtr->addComponent(std::make_unique<AnimatorComponent>(
                sf::Vector2u{32, 32}, Animation{0, 17, 22.0f}));

        //конструктор для компоненты box2d0
        fruitUPtr->addComponent(
                std::make_unique<Box2dComponent>(40.0f, groundLevel, 2 /*ширина изображения*/, 2 /*высота изображения*/,
                                                 false, true, "cherry"));

        gameObjects.push_back(std::move(fruitUPtr));

    }

    GameObjects &gameObjects;
    const PlayerPtr &player;
};

/*struct Box2dSystem : System{
    b2World World = b2World(b2Vec2(0.f, 50.8f));

    MyContactListener myContactListenerInstance;

    //in FooTest constructor

    explicit Box2dSystem(GameWorld &gameObjects) : gameObjects(gameObjects){
    }


    void init() override{
        World.SetContactListener(&myContactListenerInstance);
    }
private:

    void setWall(float x, float y, float width, float height) //должно быть где-то в инициализации, стенка тоже игровой обьект с компонентой box2d
    {
        b2PolygonShape ground;
        ground.SetAsBox(width / SCALE, height / SCALE);
        b2BodyDef bdef;
        bdef.position.Set(x / SCALE, y / SCALE);
        b2Body *b_ground = World.CreateBody(&bdef);
        b_ground->CreateFixture(&ground, 1);
    }

    void updatePosition(GameObject &gameObject){
        if(!gameObject.hasComponent<Box2dComponent>() || gameObject.getComponent<Box2dComponent>()->bodyDef.type != b2_dynamicBody  у земли не нужно обновлять координаты ){
            return;
        }
        auto box2dComponent = gameObject.getComponent<Box2dComponent>();
        auto positionComponent = gameObject.getComponent<PositionComponent>();
        b2Vec2 positionBody = box2dComponent->body->GetPosition();
        positionComponent->x = positionBody.x * SCALE;
        positionComponent->y = positionBody.y * SCALE;
    }

    void itemCollection(){
        for (b2Contact* contact = World.GetContactList(); contact; contact = contact->GetNext()) {
            b2Fixture *fixture = contact->GetFixtureA();
            fixture->IsSensor();
            if(contact->GetFixtureA()->IsSensor()) {
            }
        }
    }

    void update(){
        World.Step(1/60.f, 8, 3);
        for (auto &gameObject: gameObjects) {
            updatePosition(*gameObject);
        }
        // TODO: restore this strange function
        itemCollection();
    }

    GameWorld &gameObjects;

};*/

void initialization(Systems &systems, GameObjects &gameObjects, DeltaTime &deltaTime, PlayerPtr &player,
                    CameraPtr &currentCamera, sf::RenderWindow &app, NetworkSystem &networkSystem) {
    int id1 = 1, id2 = 2;
    int id = (networkSystem.getId() == id1 ? id2 : id1);
    gameObjects.push_back(std::make_unique<GameObject>(make_player()));
    systems.push_back(std::make_unique<NetworkPlayerSystem>(*gameObjects.back(), networkSystem, id));
    systems.push_back(std::make_unique<PlayerSystem>(gameObjects, player, deltaTime));
    systems.push_back(std::make_unique<Box2dSystem>(gameObjects));
    systems.push_back(std::make_unique<SurroundingsSystem>(gameObjects));
    systems.push_back(std::make_unique<AnimationSystem>(gameObjects, deltaTime));
    systems.push_back(std::make_unique<PlayerCameraSystem>(gameObjects, player, currentCamera));
    systems.push_back(std::make_unique<CoinsSystem>(gameObjects, player));
    systems.push_back(std::make_unique<RenderingSystem>(gameObjects, app, currentCamera));

    for (auto &system: systems)
        system->init();
    deltaTime.update();
}

void pollEvents(sf::RenderWindow &app) {
    /// TODO: Сделать объект Input который будет содержать в том числе и Pointer
    /// (мотивация - есть же еще мышка, свайпы, другие клавиши)
    sf::Event e{};
    while (app.pollEvent(e))
        if (e.type == sf::Event::Closed)
            app.close();
}


int main() {
    int id = login();
    NetworkSystem networkSystem(id);

    sf::RenderWindow app(sf::VideoMode(1600, 1200), "Team PR 1");
    appPtr = &app;


    DeltaTime deltaTime;
    GameObjects gameObjects;
    PlayerPtr player = nullptr;
    CameraPtr currentCamera = nullptr;

    Systems systems;
    initialization(systems, gameObjects, deltaTime, player, currentCamera, app, networkSystem);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist10(1, 10); // distribution in range [1, 6]


    int x, y;
    while (app.isOpen()) {
        deltaTime.update();
        pollEvents(app);

        for (auto &system: systems)
            system->update();
        networkSystem.set({(int) player->getComponent<PositionComponent>()->x,
                           (int) player->getComponent<PositionComponent>()->y});
        std::cout.flush();
    }

    for (auto &system: systems)
        system->shutdown();
}