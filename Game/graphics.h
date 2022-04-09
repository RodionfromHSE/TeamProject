#pragma once

#include "fwd.h"
#include "game_object.h"
#include "position.h"

using namespace sf;

struct PrettyGraphics {
    int window_size = 800;
    int num_of_cell = 8;
    int distance = window_size / num_of_cell;
    Texture coin;
    Texture player;
    Texture background;
};

inline static PrettyGraphics prettyGraphics;

inline static RenderWindow app(VideoMode(prettyGraphics.window_size, prettyGraphics.window_size), "Team PR 1");

struct RenderingComponent : Component {
    Texture texture;

    explicit RenderingComponent(const Texture &newTexture) : texture(newTexture) {}

    void draw(GameObject &game_object, RenderWindow &app) const;
};

void show_game(std::vector<GameObject> &gameObjects);
