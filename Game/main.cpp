#include <utility>
#include <memory>
#include <cassert>
#include <ctime>
#include <iostream>

//#include "network_system.h"
#include "game_object.h"
#include "position.h"
#include "graphics.h"
#include "animation.h"
#include "box2d.h"

constexpr std::size_t groundLevel = 300;
constexpr std::size_t worldWidth = 2048;
constexpr std::size_t worldHeight = 600;
constexpr uint16_t PORT = 60'000;

enum Input {
    Left, Right, Up, Down, Stop, Exit
};

//Input input = Input::Stop;

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

    /*void update() override {
        assert(player);

        int dx = (int) sf::Keyboard::isKeyPressed(sf::Keyboard::Right) -
                 (int) sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        movePlayer(dx);
        updatePlayerAnimation(dx);
    }*/

    void update() override{
        World.Step(1/60.f, 8, 3);
        Input input = Input::Stop;
        assert(player);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            input = Input::Right;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            input = Input::Left;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            input = Input::Up;
        }
        movePlayerBox2D(input);
        //std::cout << input << '\n';
    }

private:
    void createPlayer() {
        sf::Texture playerTexture;
        playerTexture.loadFromFile("../Pictures/Free/Main Characters/Pink Man/Sprite Sheet.png");

        auto playerUPtr = std::make_unique<GameObject>();
        player = &*playerUPtr;

        playerUPtr->addComponent(std::make_unique<PositionComponent>(0.0f, groundLevel));
         //конструктор для компоненты box2d
        playerUPtr->addComponent(std::make_unique<Box2dComponent>(0.0f, groundLevel, 32 /* ширина изображения */, 32 /* высота изображения */, "player", 1));

        playerUPtr->addComponent(std::make_unique<RenderingComponent>(
                playerTexture, sf::Vector2i{originX, originY}));
        playerUPtr->addComponent(std::make_unique<AnimatorComponent>(
                sf::Vector2u{animationFrameSize, animationFrameSize}, idleAnimation));

        gameObjects.push_back(std::move(playerUPtr));
    }

    void movePlayer(int dx) {
        auto positionComponent = player->getComponent<PositionComponent>();
        assert(positionComponent);

        positionComponent->x += (int) ((float) dx * speedInPxsPerSecond * deltaTime.seconds());

    }

    void movePlayerBox2D(Input& input){
        auto box2dComponent = player->getComponent<Box2dComponent>();

        b2Body *pBody = box2dComponent->body;

        b2Vec2 vel = pBody->GetLinearVelocity();

        float velChange;

        if (input == Input::Right) {
            velChange = 5 - vel.x;
        }

        if (input == Input::Left) {
            velChange = -5 - vel.x;
        }

        if(input == Input::Right || input == Input::Left){
            float impulse = pBody->GetMass() * velChange; //disregard time factor
            pBody->ApplyLinearImpulse( b2Vec2(impulse,0), pBody->GetWorldCenter(), true);
        }


        if (input == Input::Up)  {
            if(vel.y < 0.1 && vel.y > -0.1 /* проверка onGround */) {
                pBody->ApplyLinearImpulse(b2Vec2(0, -120), pBody->GetWorldCenter(), true);
            }
        }

        b2Vec2 positionBody = pBody->GetPosition();

        player->getComponent<PositionComponent>()->x = positionBody.x * SCALE;
        player->getComponent<PositionComponent>()->y = positionBody.y * SCALE;
    }

    bool onGround(b2Body *pBody){
        /*b2Vec2 positionBody = pBody->GetPosition();
        float bodyPositionY = positionBody.y * SCALE;
        if(bodyPositionY < groundLevel + 0.5 && bodyPositionY > groundLevel - 0.5){
            return true;
        } else {
            return false;
        }*/
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

        gameObjects.push_back(std::move(ground));

        setWall(worldWidth, worldHeight / 2 + groundLevel + 32, 2 * worldWidth, worldHeight / 2);//установил нижнию стенку
    }

    void setWall(float x, float y, float width, float height)
    {
        b2PolygonShape ground;
        ground.SetAsBox(width / SCALE, height / SCALE);
        //b2FixtureDef GroundDef;
        //GroundDef.density = 1; //плотность
        //GroundDef.restitution = 0; //упругость
        //GroundDef.friction = 100; // трение
        //GroundDef.shape = &ground;
        b2BodyDef bdef;
        bdef.position.Set(x / SCALE, y / SCALE);
        b2Body *b_ground = World.CreateBody(&bdef);
        b_ground->CreateFixture(&ground, 1);
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

        //конструктор для компоненты box2d
        fruitUPtr->addComponent(std::make_unique<Box2dComponent>(40.0f, groundLevel, 2 /*ширина изображения*/, 2 /*высота изображения*/, "coin", 0));

        gameObjects.push_back(std::move(fruitUPtr));

    }

    GameObjects &gameObjects;
    const PlayerPtr &player;
};

void pollEvents(sf::RenderWindow &app) {
    /// TODO: Сделать объект Input который будет содержать в том числе и Pointer
    /// (мотивация - есть же еще мышка, свайпы, другие клавиши)
    sf::Event e{};
    while (app.pollEvent(e))
        if (e.type == sf::Event::Closed)
            app.close();
}

int main() {
    sf::RenderWindow app(sf::VideoMode(1600, 1200), "Team PR 1");

    DeltaTime deltaTime;
    GameObjects gameObjects;
    PlayerPtr player = nullptr;
    CameraPtr currentCamera = nullptr;

    Systems systems;
    systems.push_back(std::make_unique<RenderingSystem>(gameObjects, app, currentCamera));
    systems.push_back(std::make_unique<PlayerSystem>(gameObjects, player, deltaTime));
    systems.push_back(std::make_unique<SurroundingsSystem>(gameObjects));
    systems.push_back(std::make_unique<AnimationSystem>(gameObjects, deltaTime));
    systems.push_back(std::make_unique<PlayerCameraSystem>(gameObjects, player, currentCamera));
    systems.push_back(std::make_unique<CoinsSystem>(gameObjects, player));
//    NetworkSystem networkSystem(PORT);

    for (auto &system: systems)
        system->init();
    deltaTime.update();

    while (app.isOpen()) {
        deltaTime.update();
        pollEvents(app);

        for (auto &system : systems)
                system->update();
        std::cout.flush();
    }

    for (auto &system: systems)
        system->shutdown();
}