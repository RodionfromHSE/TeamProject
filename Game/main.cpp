#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

using namespace std;

int size_filed = 9;

struct PrettyGraphics{
    int window_size = 800;
    int num_of_cell = 8;
    int distance = window_size / num_of_cell;
    Texture coin;
    Texture player;
    Texture background;
};

PrettyGraphics prettyGraphics;

RenderWindow app(VideoMode(prettyGraphics.window_size, prettyGraphics.window_size), "Team PR 1");

enum Pointer {
    Left, Right, Up, Down, Stop, Exit
};


Pointer pointer = Pointer::Stop;

//vector<vector<char>> game_field(size_filed, vector<char> (size_filed, '.'));


struct Component {
    virtual ~Component();
};

Component::~Component() = default;

struct PositionComponent : Component {
    int x;
    int y;

    PositionComponent(int newX, int newY) : x(newX), y(newY) {}

    ~PositionComponent() override;
};

PositionComponent::~PositionComponent() = default;

struct TextureComponent : Component {
    Texture texture;
    string name;

    explicit TextureComponent(Texture newTexture, string newName) : texture(newTexture), name(newName) {}
};

struct BehaviourComponent : Component {

};

struct GameObject {
    string namePlayer;
    unordered_map <string, std::shared_ptr<Component>> components;
    GameObject(string name, unordered_map <string, std::shared_ptr<Component>> newComponents) : namePlayer(
            std::move(name)), components(std::move(newComponents)) {
    }

    template<typename T>
    std::shared_ptr <T> getComponent(const std::string &componentName) {
        return dynamic_pointer_cast<T>(this->components[componentName]);
    }
};

GameObject *get_nearby_object(vector <GameObject> &gameObjects, GameObject &object) {
    std::shared_ptr <PositionComponent> objectPosition = object.getComponent<PositionComponent>("position");
    for (auto &gameObject: gameObjects) {
        std::shared_ptr <PositionComponent> otherPosition = gameObject.getComponent<PositionComponent>("position");
        auto textureComponent = gameObject.getComponent<TextureComponent>("texture");
        bool isCoin = textureComponent->name == "coin";
        /*if (objectPosition->x == otherPosition->x && objectPosition->y == otherPosition->y && isCoin) {
            return &gameObject;
        }*/
        if (abs(objectPosition->x - otherPosition->x) <= prettyGraphics.distance / 2 &&
        abs(objectPosition->y - otherPosition->y) <= prettyGraphics.distance / 2 && isCoin) {
            return &gameObject;
        }
    }
    return nullptr;
}

struct PlayerController : BehaviourComponent {
    int balance;

    explicit PlayerController(int i) : balance(i) {
    }

    static void move_player(GameObject &player) {
        std::shared_ptr <PositionComponent> positionComponent = player.getComponent<PositionComponent>("position");

        if (pointer == Pointer::Right) {
            positionComponent->x += prettyGraphics.distance;
        }
        if (pointer == Pointer::Left) {
            positionComponent->x -= prettyGraphics.distance;
        }
        if (pointer == Pointer::Up) {
            positionComponent->y -= prettyGraphics.distance;
        }
        if (pointer == Pointer::Down) {
            positionComponent->y += prettyGraphics.distance;
        }
        if (positionComponent->x < 0) positionComponent->x = prettyGraphics.window_size - prettyGraphics.distance;
        if (positionComponent->x > prettyGraphics.window_size - prettyGraphics.distance) positionComponent->x = 0;
        if (positionComponent->y < 0) positionComponent->y = prettyGraphics.window_size - prettyGraphics.distance;
        if (positionComponent->y > prettyGraphics.window_size - prettyGraphics.distance) positionComponent->y = 0;
    }

    void update(vector <GameObject> &gameObjects, GameObject &player) {
        move_player(player);
        GameObject *object = get_nearby_object(gameObjects, player);
        if (object != nullptr) {
            balance += 1;
            object->getComponent<PositionComponent>("position")->x = rand() % prettyGraphics.num_of_cell * prettyGraphics.distance;
            object->getComponent<PositionComponent>("position")->y = rand() % prettyGraphics.num_of_cell * prettyGraphics.distance;
        }
    }
};

/*struct NetworkController : BehaviourComponent {
    // получает от сервера позицию игрока по сети и перемещает соответственно
};*/


void input() {
    // Сделать объект Input который будет содержать в том числе и Pointer (мотивация - есть же еще мышка, свайпы, другие клавиши)
    Event e;
    while (app.pollEvent(e))
    {
        if (e.type == Event::Closed)
            app.close();
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        pointer = Pointer::Right;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        pointer = Pointer::Left;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        pointer = Pointer::Down;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        pointer = Pointer::Up;
    }
}

void show_game(vector <GameObject> &gameObjects) { // заменить на отрисовку с SFML, когда текстовая игра снова заработает :)
    //vector <vector<char>> buffer(size_filed, vector<char>(size_filed, '.'));
    Sprite back_ground(prettyGraphics.background);
    app.draw(back_ground);
    Sprite player(prettyGraphics.player);
    Sprite coin(prettyGraphics.coin);
    for (auto &gameObject: gameObjects) {
        //std::shared_ptr <PositionComponent> positionComponent = gameObject.getComponent<PositionComponent>("position");
        int x = gameObject.getComponent<PositionComponent>("position").get()->x;
        int y = gameObject.getComponent<PositionComponent>("position").get()->y;

        auto textureComponent = gameObject.getComponent<TextureComponent>("texture");

        Texture texture = textureComponent->texture;

        if(textureComponent->name == "player"){
            player.setPosition(x, y);
            app.draw(player);
        }
        if(textureComponent->name == "coin"){
            coin.setPosition(x, y);
            app.draw(coin);
        }
    }
    app.display();
    // рисую буфер на экране
    /*cout << '\n';
    for (int i = 0; i < size_filed; i++) {
        for (int j = 0; j < size_filed; j++) {
            cout << buffer[i][j] << " ";
        }
        cout << '\n';
    }*/
}

void update(vector <GameObject> &gameObjects) {
    for (auto &gameObject: gameObjects) {
        if (gameObject.components.count("controller")) {
            /*dynamic_pointer_cast<PlayerController>(gameObject.components["controller"])->update(gameObjects,
                                                                                                gameObject);*/
            gameObject.getComponent<PlayerController>("controller")->update(gameObjects, gameObject);
        }
    }
}

void initialize_game(vector <GameObject> &gameObjects) {
    // добавить игрока и монетки
    unordered_map <string, std::shared_ptr<Component>> playerComponent;
    string namePlayer = "player0001";
    playerComponent["position"] = std::make_shared<PositionComponent>(PositionComponent{prettyGraphics.window_size - prettyGraphics.distance, prettyGraphics.window_size - prettyGraphics.distance});
    playerComponent["texture"] = std::make_shared<TextureComponent>(TextureComponent{prettyGraphics.player, "player"});
    playerComponent["controller"] = std::make_shared<PlayerController>(PlayerController{0});
    unordered_map <string, std::shared_ptr<Component>> coinComponent;
    string nameCoin = "coin0001";
    coinComponent["position"] = std::make_shared<PositionComponent>(PositionComponent{rand() % prettyGraphics.num_of_cell * prettyGraphics.distance, rand() % prettyGraphics.num_of_cell * prettyGraphics.distance});
    coinComponent["texture"] = std::make_shared<TextureComponent>(TextureComponent{prettyGraphics.coin, "coin"});
    GameObject gameObjectPlayer(namePlayer, playerComponent);
    GameObject gameObjectCoin(nameCoin, coinComponent);
    gameObjects.push_back(gameObjectPlayer);
    gameObjects.push_back(gameObjectCoin);
}

void graphics(){
    prettyGraphics.player.loadFromFile("/home/aleksandr/Pictures/Cube.png");
    prettyGraphics.coin.loadFromFile("/home/aleksandr/Pictures/Coin.png");
    prettyGraphics.background.loadFromFile("/home/aleksandr/Pictures/Background.png");
}

void play(){
    srand(time(0));
    app.setFramerateLimit(10);
    graphics();
    while (app.isOpen()) {
        vector<GameObject> gameObjects;
        initialize_game(gameObjects);
        show_game(gameObjects);
        while (true){
            if(pointer == Pointer::Exit){
                break;
            }
            input();
            update(gameObjects);
            show_game(gameObjects);
            pointer = Pointer::Stop;
        }
    }
}

int main() {
    play();
}
