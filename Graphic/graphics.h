#ifndef __PROJECT_GRAPHICS_H
#define __PROJECT_GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "game_object.h"
#include "position.h"
#include "system.h"

#include <vector>

enum class Layer {
    Background,
    Focalground,
    Foreground,
    GUI
};

struct RenderingComponent : Component {
    const sf::Texture texture; // because the sprite only keeps a reference to the texture

    Layer layer;
    bool horizontalFlip = false;
    bool verticalFlip = false;
    sf::IntRect rect;
    sf::Vector2i origin{};

    explicit RenderingComponent(const sf::Texture &texture,
                                sf::Vector2i origin = sf::Vector2i{},
                                Layer layer = Layer::Focalground)
            : texture(texture), origin(origin), layer(layer) {
        rect = sf::IntRect({}, static_cast<sf::Vector2i>(texture.getSize()));
    }
};

struct CameraComponent : Component {
    float viewWidth;

    explicit CameraComponent(float viewWidth) : viewWidth(viewWidth) {}
};

using CameraPtr = GameObject *;

struct RenderingSystem : System {
    explicit RenderingSystem(GameObjects &gameObjects,
                             sf::RenderWindow &app,
                             const CameraPtr &currentCamera)
            : gameObjects(gameObjects), app(app), currentCamera(currentCamera) {}

    void init() override {
        app.setFramerateLimit(30);
        app.setView(view);
    }

    void update() override {
        updateView();

        drawLayer(Layer::Background);
        drawLayer(Layer::Focalground);
        drawLayer(Layer::Foreground);
        drawLayer(Layer::GUI);

        app.display();
    }

private:
    sf::RenderWindow &app;
    sf::View view;
    GameObjects &gameObjects;
    const CameraPtr &currentCamera;

    void updateView() {
        if (!currentCamera)
            return;

        auto cameraComponent = currentCamera->getComponent<CameraComponent>();
        auto cameraPositionComponent = currentCamera->getComponent<PositionComponent>();

        assert(cameraComponent);
        assert(cameraPositionComponent);

        auto cameraWidth = cameraComponent->viewWidth;
        float aspectRatio = (float) app.getSize().y / (float) app.getSize().x;

        sf::Vector2f cameraPos((float) cameraPositionComponent->x,
                               (float) cameraPositionComponent->y);
        sf::Vector2f cameraSize(cameraWidth, cameraWidth * aspectRatio);
        app.setView(sf::View(cameraPos, cameraSize));
    }

    void drawLayer(Layer layer) {
        for (auto &gameObject: gameObjects) {
            auto renderingComponent = gameObject->getComponent<RenderingComponent>();
            auto positionComponent = gameObject->getComponent<PositionComponent>();
            if (renderingComponent && positionComponent && renderingComponent->layer == layer)
                drawSprite(renderingComponent, positionComponent);
        }
    }

    void drawSprite(const RenderingComponent *renderingComponent,
                    const PositionComponent *positionComponent) {

        sf::Sprite sprite(renderingComponent->texture);

        sprite.setTextureRect(renderingComponent->rect);
        sprite.setOrigin(static_cast<sf::Vector2f>(renderingComponent->origin));
        sprite.setScale(renderingComponent->horizontalFlip ? -1 : 1,
                        renderingComponent->verticalFlip ? -1 : 1);
        sprite.setPosition((float) positionComponent->x,
                           (float) positionComponent->y);
        app.draw(sprite);
    }
};


#endif //__PROJECT_GRAPHICS_H
