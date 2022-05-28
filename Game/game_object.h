//
// Created by aleksandr on 10.03.2022.
//

#ifndef __PROJECT_GAME_OBJECT_H
#define __PROJECT_GAME_OBJECT_H

#include <ctime>
#include <cassert>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <utility>
#include <bitset>
#include <array>
#include <box2d.h>

struct GameObject;

//константы для box2d
b2Vec2 Gravity(0.f, 50.8f);
b2World World(Gravity);
const float SCALE = 30.f;
const float DEG  =  57.29577f;

struct Component {
    GameObject *owner = nullptr;

    virtual ~Component() = default;
};

inline std::size_t getComponentID() {
    static std::size_t id = 0;
    return ++id;
}

template<typename T>
std::size_t getComponentID() {
    static std::size_t id = getComponentID();
    return id;
}

struct GameObject {
    template<typename T>
    [[nodiscard]] T *getComponent() const {
        return reinterpret_cast<T *>(components[getComponentID<T>()].get());
    }

    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        return components[getComponentID<T>()] != nullptr;
    }

    template<typename T>
    T *addComponent(std::unique_ptr<T> component) {
        T *raw = component.get();
        component->owner = this;
        components[getComponentID<T>()] = std::move(component);
        return raw;
    }

private:
    static const std::size_t MaxComponents = 64;
    using ComponentsArray = std::array<std::unique_ptr<Component>, MaxComponents>;

    ComponentsArray components;
};

using GameObjects = std::vector<std::unique_ptr<GameObject>>;

struct DeltaTime {
    DeltaTime() : last(std::chrono::high_resolution_clock::now()) {}

    void update() {
        auto now = std::chrono::high_resolution_clock::now();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count();
        dt = (float) ns / 1e9f;
        last = now;
    }

    [[nodiscard]] float seconds() const {
        return dt;
    }

private:
    std::chrono::high_resolution_clock::time_point last;
    float dt = 0.0;
};

#endif //__PROJECT_GAME_OBJECT_H
