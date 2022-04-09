#include "graphics.h"

void RenderingComponent::draw(GameObject &game_object, RenderWindow &app) const {
    Sprite sprite(texture);
    auto positionComponent = game_object.getComponent<PositionComponent>("position");
    sprite.setPosition(positionComponent->x, positionComponent->y);
    app.draw(sprite);
}

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