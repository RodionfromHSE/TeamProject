//
// Created by aleksandr on 10.03.2022.
//

#ifndef __PROJECT_GRAPHICS_H
#define __PROJECT_GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "game_object.h"

#include "position.h"

#include <vector>

using namespace sf;

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

struct RenderingComponent : Component {
    Texture texture;

    explicit RenderingComponent(const Texture& newTexture) : texture(newTexture) {}

    void draw(GameObject &game_object, RenderWindow &app) const {
        Sprite sprite(texture);
        auto positionComponent = game_object.getComponent<PositionComponent>("position");
        sprite.setPosition(positionComponent->x, positionComponent->y);
        app.draw(sprite);
    }
};

void show_game(std::vector<GameObject> &gameObjects) {
    //vector <vector<char>> buffer(size_filed, vector<char>(size_filed, '.'));
    Sprite back_ground(prettyGraphics.background);
    app.draw(back_ground);

    for (auto &gameObject: gameObjects) {
        auto renderingComponent = gameObject.getComponent<RenderingComponent>("texture");
        renderingComponent->draw(gameObject, app);
    }
    app.display();
}


#endif //__PROJECT_GRAPHICS_H
