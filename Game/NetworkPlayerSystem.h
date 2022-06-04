#pragma once

#include "network_system.h"

namespace values {
    constexpr std::size_t groundLevel = 300;
    constexpr std::size_t worldWidth = 2048;
    constexpr std::size_t worldHeight = 600;

    constexpr std::size_t animationFrameSize = 32;
    constexpr int originX = animationFrameSize / 2;
    constexpr int originY = animationFrameSize;

    constexpr float animationFps = 22.0f;
    constexpr Animation idleAnimation = {0, 11, animationFps};
}

struct NetworkPlayerSystem : System {
    explicit NetworkPlayerSystem(GameObject &player, NetworkSystem &networkSystem, int id) : m_player(player),
                                                                                             m_networkSystem(
                                                                                                     networkSystem),
                                                                                             m_id(id) {
    }

    void init() final {
        coors = m_networkSystem.make_synchronized<PlayerCoordinates>(m_id);
    }


    virtual void update() {
        if (coors->isUpdatable()) {
            auto position = m_player.getComponent<PositionComponent>();
            auto tmp = coors->get();
            position->x = tmp.x;
            position->y = tmp.y;
        }
    }

    virtual void shutdown() {}

private:
    int m_id;
    std::shared_ptr<net::Synchronized<PlayerCoordinates, Usage::OnClient>> coors;
    NetworkSystem &m_networkSystem;
    GameObject &m_player;
};

GameObject make_player() {
    using namespace values;
    sf::Texture playerTexture;
    playerTexture.loadFromFile("../Pictures/Free/Main Characters/Pink Man/Sprite Sheet.png");

    auto player = GameObject();

    player.addComponent(std::make_unique<PositionComponent>(0.0f, groundLevel));

//    m_player.addComponent(std::make_unique<Box2dComponent>(0.0f, groundLevel, 32 /* ширина изображения */, 32 /* высота изображения */, true, false, "player"));


    player.addComponent(std::make_unique<RenderingComponent>(
            playerTexture, sf::Vector2i{originX, originY}));
    player.addComponent(std::make_unique<AnimatorComponent>(
            sf::Vector2u{animationFrameSize, animationFrameSize}, idleAnimation));

    return player;
}