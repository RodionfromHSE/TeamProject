#include "fwd.h"
#include "game_object.h"
#include "position.h"
#include "graphics.h"


using namespace std;

int size_filed = 9;

enum Input {
    Left, Right, Up, Down, Stop, Exit
};


//Pointer pointer = Pointer::Stop;

//vector<vector<char>> game_field(size_filed, vector<char> (size_filed, '.'));


struct BehaviourComponent : Component {

};


GameObject *get_nearby_object(vector <GameObject> &gameObjects, GameObject &object) {
    std::shared_ptr <PositionComponent> objectPosition = object.getComponent<PositionComponent>("position");
    for (auto &gameObject: gameObjects) {
        std::shared_ptr <PositionComponent> otherPosition = gameObject.getComponent<PositionComponent>("position");
        auto renderingComponent = gameObject.getComponent<RenderingComponent>("texture");
        /*if (objectPosition->x == otherPosition->x && objectPosition->y == otherPosition->y && isCoin) {
            return &gameObject;
        }*/
        if (abs(objectPosition->x - otherPosition->x) <= prettyGraphics.distance / 2 &&
            abs(objectPosition->y - otherPosition->y) <= prettyGraphics.distance / 2 && &gameObject != &object) {
            return &gameObject;
        }
    }
    return nullptr;
}

struct PlayerController : BehaviourComponent {
    int balance;

    explicit PlayerController(int i) : balance(i) {
    }

    static void move_player(GameObject &player, Input& input) {
        std::shared_ptr <PositionComponent> positionComponent = player.getComponent<PositionComponent>("position");

        if (input == Input::Right) {
            positionComponent->x += prettyGraphics.distance;
        }
        if (input == Input::Left) {
            positionComponent->x -= prettyGraphics.distance;
        }
        if (input == Input::Up) {
            positionComponent->y -= prettyGraphics.distance;
        }
        if (input == Input::Down) {
            positionComponent->y += prettyGraphics.distance;
        }
        if (positionComponent->x < 0) positionComponent->x = prettyGraphics.window_size - prettyGraphics.distance;
        if (positionComponent->x > prettyGraphics.window_size - prettyGraphics.distance) positionComponent->x = 0;
        if (positionComponent->y < 0) positionComponent->y = prettyGraphics.window_size - prettyGraphics.distance;
        if (positionComponent->y > prettyGraphics.window_size - prettyGraphics.distance) positionComponent->y = 0;
    }

    void update(vector <GameObject> &gameObjects, GameObject &player, Input input) {
        move_player(player, input);
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


Input input() {
    // Сделать объект Input который будет содержать в том числе и Pointer (мотивация - есть же еще мышка, свайпы, другие клавиши)
    Event e;
    while (app.pollEvent(e))
    {
        if (e.type == Event::Closed)
            app.close();
    }
    Input pointer;
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        pointer = Input::Right;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        pointer = Input::Left;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        pointer = Input::Down;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        pointer = Input::Up;
    }
    return pointer;
}

void update(vector <GameObject> &gameObjects, Input& input) {
    for (auto &gameObject: gameObjects) {
        if (gameObject.components.count("controller")) {
            /*dynamic_pointer_cast<PlayerController>(gameObject.components["controller"])->update(gameObjects,
                                                                                                gameObject);*/
            gameObject.getComponent<PlayerController>("controller")->update(gameObjects, gameObject, input);
        }
    }
}

void initialize_game(vector <GameObject> &gameObjects) {
    // добавить игрока и монетки
    unordered_map <string, std::shared_ptr<Component>> playerComponent;
    string namePlayer = "player0001";
    playerComponent["position"] = std::make_shared<PositionComponent>(PositionComponent{prettyGraphics.window_size - prettyGraphics.distance, prettyGraphics.window_size - prettyGraphics.distance});
    playerComponent["texture"] = std::make_shared<RenderingComponent>(RenderingComponent{prettyGraphics.player});
    playerComponent["controller"] = std::make_shared<PlayerController>(PlayerController{0});
    unordered_map <string, std::shared_ptr<Component>> coinComponent;
    string nameCoin = "coin0001";
    coinComponent["position"] = std::make_shared<PositionComponent>(PositionComponent{rand() % prettyGraphics.num_of_cell * prettyGraphics.distance, rand() % prettyGraphics.num_of_cell * prettyGraphics.distance});
    coinComponent["texture"] = std::make_shared<RenderingComponent>(RenderingComponent{prettyGraphics.coin});
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
    vector<GameObject> gameObjects;
    initialize_game(gameObjects);
    show_game(gameObjects);
    while (app.isOpen()) {
        Input pointer = input();
        update(gameObjects, pointer);
        show_game(gameObjects);
        //pointer = Pointer::Stop;
    }
}

int main() {
    play();
}