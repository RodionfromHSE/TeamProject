#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <memory>

using namespace std;

int size_filed = 9;

enum Pointer {
    Left, Right, Up, Bottom, Stop, Exit
};


Pointer pointer = Pointer::Right;

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
    char texture = '.';

    explicit TextureComponent(char newTexture) : texture(newTexture) {}
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
    for (auto &gameObject: gameObjects) {
        std::shared_ptr <PositionComponent> objectPosition = object.getComponent<PositionComponent>("position");
        std::shared_ptr <PositionComponent> otherPosition = gameObject.getComponent<PositionComponent>("position");
        auto textureComponent = gameObject.getComponent<TextureComponent>("texture");


        bool isCoin = textureComponent->texture == '$';
        if (objectPosition->x == otherPosition->x && objectPosition->y == otherPosition->y && isCoin) {
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
            if (positionComponent->x == size_filed - 1) {
                positionComponent->x = 0;
            } else {
                positionComponent->x += 1;
            }
        }
        if (pointer == Pointer::Left) {
            if (positionComponent->x == 0) {
                positionComponent->x = size_filed - 1;
            } else {
                positionComponent->x -= 1;
            }
        }
        if (pointer == Pointer::Up) {
            if (positionComponent->y == 0) {
                positionComponent->y = size_filed - 1;
            } else {
                positionComponent->y -= 1;
            }
        }
        if (pointer == Pointer::Bottom) {
            if (positionComponent->y == size_filed - 1) {
                positionComponent->y = 0;
            } else {
                positionComponent->y += 1;
            }
        }
    }

    void update(vector <GameObject> &gameObjects, GameObject &player) {
        move_player(player);
        GameObject *object = get_nearby_object(gameObjects, player);
        if (object != nullptr) {
            balance += 1;
            object->getComponent<PositionComponent>("position")->x = rand() % size_filed;
            object->getComponent<PositionComponent>("position")->y = rand() % size_filed;
        }
    }
};

/*struct NetworkController : BehaviourComponent {
    // получает от сервера позицию игрока по сети и перемещает соответственно
};*/


void input() { // Сделать объект Input который будет содержать в том числе и Pointer (мотивация - есть же еще мышка, свайпы, другие клавиши)
    char ch;
    cin >> ch;
    if (ch == 'a') {
        pointer = Pointer::Left;
    }
    if (ch == 'd') {
        pointer = Pointer::Right;
    }
    if (ch == 'w') {
        pointer = Pointer::Up;
    }
    if (ch == 's') {
        pointer = Pointer::Bottom;
    }
    if (ch == 'q') {
        pointer = Pointer::Exit;
    }
}

void
show_game(vector <GameObject> &gameObjects) { // заменить на отрисовку с SFML, когда текстовая игра снова заработает :)
    vector <vector<char>> buffer(size_filed, vector<char>(size_filed, '.'));
    for (auto &gameObject: gameObjects) {
        //std::shared_ptr <PositionComponent> positionComponent = gameObject.getComponent<PositionComponent>("position");
        int x = gameObject.getComponent<PositionComponent>("position").get()->x;
        int y = gameObject.getComponent<PositionComponent>("position").get()->y;

        auto textureComponent = gameObject.getComponent<TextureComponent>("texture");

        char texture = textureComponent->texture;
        buffer[y][x] = texture;
    }
    // рисую буфер на экране
    cout << '\n';
    for (int i = 0; i < size_filed; i++) {
        for (int j = 0; j < size_filed; j++) {
            cout << buffer[i][j] << " ";
        }
        cout << '\n';
    }
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
    playerComponent["position"] = std::make_shared<PositionComponent>(PositionComponent{size_filed - 1, size_filed - 1});
    playerComponent["texture"] = std::make_shared<TextureComponent>(TextureComponent{'#'});
    playerComponent["controller"] = std::make_shared<PlayerController>(PlayerController{0});
    unordered_map <string, std::shared_ptr<Component>> coinComponent;
    string nameCoin = "coin0001";
    coinComponent["position"] = std::make_shared<PositionComponent>(PositionComponent{0, 0});
    coinComponent["texture"] = std::make_shared<TextureComponent>(TextureComponent{'$'});
    GameObject gameObjectPlayer(namePlayer, playerComponent);
    GameObject gameObjectCoin(nameCoin, coinComponent);
    gameObjects.push_back(gameObjectPlayer);
    gameObjects.push_back(gameObjectCoin);
}


int main() {
    vector <GameObject> gameObjects;
    initialize_game(gameObjects);
    show_game(gameObjects);
    while (true) {
        input();
        if (pointer == Pointer::Exit) {
            return 0;
        }
        update(gameObjects);
        show_game(gameObjects);
    }
}
