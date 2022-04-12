#ifndef TEAMPROJECT_ANIMATION_H
#define TEAMPROJECT_ANIMATION_H

#include "graphics.h"

struct Animation {
    std::size_t row = 0;
    std::size_t frames = 0;
    float fps = 1.0f;
    bool horizontalFlip = false;
    bool verticalFlip = false;
};

struct AnimationSystem;

struct AnimatorComponent : Component {
    sf::Vector2u frameSize;
    bool horizontalFlip = false;
    bool verticalFlip = false;

    AnimatorComponent(sf::Vector2u frameSize, Animation initialAnimation)
            : currentAnimation(initialAnimation), frameSize(frameSize) {}

    void setAnimation(Animation newAnimation) {
        if (newAnimation.row != currentAnimation.row) {
            currentFrame = 0;
            timeSinceLastFrame = 0.f;
        }
        currentAnimation = newAnimation;
    }

private:
    Animation currentAnimation;
    std::size_t currentFrame = 0;
    float timeSinceLastFrame = 0.f;

    friend AnimationSystem;
};

struct AnimationSystem : System {
    explicit AnimationSystem(GameObjects &gameObjects, const DeltaTime &deltaTime)
            : gameObjects(gameObjects), deltaTime(deltaTime) {}

    void init() override {
        updateRenderingComponents();
    }

    void update() override {
        updateAnimatorComponents();
        updateRenderingComponents();
    }

private:
    GameObjects &gameObjects;
    const DeltaTime &deltaTime;

    void updateRenderingComponents() {
        for (auto &gameObject : gameObjects) {
            auto animatorComponent = gameObject->getComponent<AnimatorComponent>();
            auto renderingComponent = gameObject->getComponent<RenderingComponent>();

            if (animatorComponent && renderingComponent) {
                int rectX = (int) animatorComponent->currentFrame * (int) animatorComponent->frameSize.x;
                int rectY = (int) animatorComponent->currentAnimation.row * (int) animatorComponent->frameSize.y;
                int rectW = (int) animatorComponent->frameSize.x;
                int rectH = (int) animatorComponent->frameSize.y;
                renderingComponent->rect = sf::IntRect(rectX, rectY, rectW, rectH);

                renderingComponent->verticalFlip =
                        animatorComponent->verticalFlip ^ animatorComponent->currentAnimation.verticalFlip;
                renderingComponent->horizontalFlip =
                        animatorComponent->horizontalFlip ^ animatorComponent->currentAnimation.horizontalFlip;
            }
        }
    }

    void updateAnimatorComponents() {
        for (auto &gameObject : gameObjects) {
            if (auto animatorComponent = gameObject->getComponent<AnimatorComponent>()) {
                float timePerFrame = 1.0f / animatorComponent->currentAnimation.fps;
                float dt = deltaTime.seconds();

                std::size_t &currentFrame = animatorComponent->currentFrame;
                float &timeSinceLastFrame = animatorComponent->timeSinceLastFrame;

                timeSinceLastFrame += dt;
                auto framesElapsed = (std::size_t) floor(timeSinceLastFrame / timePerFrame);
                timeSinceLastFrame -= timePerFrame * (float) framesElapsed;

                currentFrame += framesElapsed;
                currentFrame %= animatorComponent->currentAnimation.frames;
            }
        }
    }
};

#endif //TEAMPROJECT_ANIMATION_H
