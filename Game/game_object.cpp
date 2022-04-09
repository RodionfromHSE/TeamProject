#include "game_object.h"

GameObject::GameObject(std::string name, std::unordered_map<std::string, std::shared_ptr<Component>> newComponents) :
        namePlayer(std::move(name)), components(std::move(newComponents)) {}
