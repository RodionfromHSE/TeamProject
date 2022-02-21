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
    PositionComponent(int newX, int newY) : x(newX), y(newY){}
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
    unordered_map<string, std::shared_ptr<Component>> components;
    /*void update() {
        for (auto c : components) {
            c.second.update();
        }
    }*/
    GameObject(string name, unordered_map<string, std::shared_ptr<Component>> newComponents) : namePlayer(std::move(name)), components(std::move(newComponents)){
    }
    int& get_positionX(){
        return dynamic_cast<PositionComponent *>(components["position"].get())->x;
    }
    int& get_positionY(){
        return dynamic_cast<PositionComponent *>(components["position"].get())->y;
    }
    char& get_texture(){
        return dynamic_cast<TextureComponent *>(components["texture"].get())->texture;
    }
};

GameObject* get_nearby_object(vector<GameObject> &gameObjects, GameObject &object) {
    for (auto &gameObject : gameObjects) {
        if (gameObject.get_positionX() == object.get_positionX() && gameObject.get_positionY() == object.get_positionY() && gameObject.get_texture() == '$') {
            return &gameObject;
        }
    }
    return nullptr;
}

struct PlayerController : BehaviourComponent {
    int balance ;
    explicit PlayerController(int i) : balance(i){
    }
    static void move_player(GameObject &player) {
        if(pointer == Pointer::Right){
            if(player.get_positionX() == size_filed - 1){
                player.get_positionX() = 0;
            } else {
                player.get_positionX() += 1;
            }
        }
        if(pointer == Pointer::Left){
            if(player.get_positionX() == 0){
                player.get_positionX() = size_filed - 1;
            } else {
                player.get_positionX() -= 1;
            }
        }
        if(pointer == Pointer::Up){
            if(player.get_positionX() == 0){
                player.get_positionX() = size_filed - 1;
            } else {
                player.get_positionX() -= 1;
            }
        }
        if(pointer == Pointer::Bottom){
            if(player.get_positionY() == size_filed - 1){
                player.get_positionY() = 0;
            } else {
                player.get_positionY() += 1;
            }
        }
    }
    void update(vector<GameObject> &gameObjects, GameObject &player)  {
        GameObject* object = get_nearby_object(gameObjects, player);
        if (object != nullptr) {
            balance += 1;
        }
        move_player(player);
    }
};

/*struct NetworkController : BehaviourComponent {

    // получает от сервера позицию игрока по сети и перемещает соответственно
};*/

void input(){ // Сделать объект Input который будет содержать в том числе и Pointer (мотивация - есть же еще мышка, свайпы, другие клавиши)
    char ch;
    cin >> ch;
    if(ch == 'a'){
        pointer = Pointer::Left;
    }
    if(ch == 'd'){
        pointer = Pointer::Right;
    }
    if(ch == 'w'){
        pointer = Pointer::Up;
    }
    if(ch == 's'){
        pointer = Pointer::Bottom;
    }
    if(ch == 'q'){
        pointer = Pointer::Exit;
    }
}

void show_game(vector<GameObject> &gameObjects){ // заменить на отрисовку с SFML, когда текстовая игра снова заработает :)
    vector<vector<char>> buffer(size_filed, vector<char> (size_filed, '.'));
    for (auto &gameObject : gameObjects) {
        int x = gameObject.get_positionX();
        int y = gameObject.get_positionY();
        char texture = gameObject.get_texture();
        buffer[y][x] = texture;
    }
    // рисую буфер на экране
    cout << '\n';
    for(int i = 0; i < size_filed; i++){
        for(int j = 0; j < size_filed; j++){
            cout << buffer[i][j] << " ";
        }
        cout << '\n';
    }
}

void update(vector<GameObject> &gameObjects){
    for(auto &gameObject : gameObjects){
        if(gameObject.components.count("controller")){
            dynamic_cast<PlayerController *>(gameObject.components["controller"].get())->update(gameObjects, gameObject);
        }
    }
}

void initialize_game(vector<GameObject> &gameObjects) {
    // добавить игрока и монетки
    unordered_map<string, std::shared_ptr<Component>> playerComponent;
    string namePlayer = "player0001";
    playerComponent["position"] = std::make_unique<Component>(PositionComponent { size_filed / 2, size_filed / 2 });
    playerComponent["texture"] = std::make_unique<Component>(TextureComponent { '#' });
    playerComponent["controller"] = std::make_unique<Component>(PlayerController {0});
    unordered_map<string, std::shared_ptr<Component>> coinComponent;
    string nameCoin = "coin0001";
    coinComponent["position"] = std::make_unique<Component>(PositionComponent { 0, 0});
    coinComponent["texture"] = std::make_unique<Component>(TextureComponent { '$' });
    GameObject gameObjectPlayer(namePlayer, playerComponent);
    GameObject gameObjectCoin(nameCoin, coinComponent);
}


int main() {
    vector<GameObject> gameObjects;
    initialize_game(gameObjects);
    show_game(gameObjects);
    while(true) {
        input();
        if(pointer == Pointer::Exit){
            return 0;
        }
        update(gameObjects);
        show_game(gameObjects);
    }
}