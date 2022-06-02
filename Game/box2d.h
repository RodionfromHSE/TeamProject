//
// Created by aleksandr on 05.05.22.
//

#ifndef __PROJECT_BOX2D_H
#define __PROJECT_BOX2D_H

#include <utility>

#include "game_object.h"


b2World world = b2World(b2Vec2(0.f, 50.8f));

struct Box2dComponent;


struct Box2dComponent : Component {
    std::string name;
    float hardness = 2;
    b2PolygonShape shape;
    b2BodyDef bodyDef;
    b2Body *body;

    Box2dComponent(float x, float y, float widthShape, float heightShape, bool isDynamic, bool isSensor, std::string m_name) {
        shape.SetAsBox(widthShape / SCALE, heightShape / SCALE); //создаем выпуклую оболочку
        name = std::move(m_name);
        if(isDynamic) {
            bodyDef.type = b2_dynamicBody;
        }

        bindingToShape(x, y, isSensor);
    }

    void bindingToShape(float x, float y, bool isSensor) {
        bodyDef.position.Set(x / SCALE, y / SCALE);
        body = world.CreateBody(&bodyDef);
        body->CreateFixture(&shape, hardness);
        body->GetFixtureList()->SetSensor(isSensor);
    }

    void init() {
        body->SetUserData((void *) owner);
    }

};

class MyContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {
        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();
        GameObject *gameObjectA = static_cast<GameObject *>(fixtureA->GetBody()->GetUserData());
        GameObject *gameObjectB = static_cast<GameObject *>(fixtureB->GetBody()->GetUserData());
        auto box2dComponentA = gameObjectA->getComponent<Box2dComponent>();
        auto box2dComponentB = gameObjectB->getComponent<Box2dComponent>();

        if(box2dComponentA->name == "cherry"){
            moveCherry(gameObjectA);
        }
        if(box2dComponentB->name == "cherry"){
            moveCherry(gameObjectB);
        }
        /*GameObject *gameObject = (GameObject *)contact->GetFixtureA()->GetBody()->GetUserData();
        gameObject->onContact((GameObject *)contact->GetFixtureB()->GetBody()->GetUserData());*/
    }

    void moveCherry(GameObject *cherry){
        auto box2dComponentCherry = cherry->getComponent<Box2dComponent>();
        auto positionComponentCherry = cherry->getComponent<PositionComponent>();

        b2Vec2 positionCherry = box2dComponentCherry->body->GetPosition();
        int travel_side = rand() % 2;
        if(travel_side == 0){
            travel_side -= 1;
        }
        positionCherry.x = positionCherry.x + travel_side * (50 + rand() % 50);
        positionComponentCherry->x = positionCherry.x;

    }

};

struct Box2dSystem : System{

    MyContactListener myContactListenerInstance;

    explicit Box2dSystem(GameObjects &gameObjects) : gameObjects(gameObjects){
    }


    void init() override{
        world.SetContactListener(&myContactListenerInstance);
    }
private:

    void setWall(float x, float y, float width, float height) //должно быть где-то в инициализации, стенка тоже игровой обьект с компонентой box2d
    {
        b2PolygonShape ground;
        ground.SetAsBox(width / SCALE, height / SCALE);
        b2BodyDef bdef;
        bdef.position.Set(x / SCALE, y / SCALE);
        b2Body *b_ground = world.CreateBody(&bdef);
        b_ground->CreateFixture(&ground, 1);
    }

    void updatePosition(GameObject &gameObject){
        if(!gameObject.hasComponent<Box2dComponent>() || gameObject.getComponent<Box2dComponent>()->bodyDef.type != b2_dynamicBody /* у земли не нужно обновлять координаты */){
            return;
        }
        auto box2dComponent = gameObject.getComponent<Box2dComponent>();
        auto positionComponent = gameObject.getComponent<PositionComponent>();
        b2Vec2 positionBody = box2dComponent->body->GetPosition();
        positionComponent->x = positionBody.x * SCALE;
        positionComponent->y = positionBody.y * SCALE;
    }



    void update(){
        world.Step(1/60.f, 8, 3);
        for (auto &gameObject: gameObjects) {
            updatePosition(*gameObject);
        }
        // TODO: restore this strange function
        //itemCollection();
    }

    GameObjects &gameObjects;

};



#endif //__PROJECT_BOX2D_H
