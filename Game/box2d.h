//
// Created by aleksandr on 05.05.22.
//

#ifndef __PROJECT_BOX2D_H
#define __PROJECT_BOX2D_H

#include "game_object.h"

struct Box2dComponent : Component {
    float hardness = 2;
    b2PolygonShape shape;
    b2BodyDef bodyDef;
    b2Body *body;

    Box2dComponent(float x, float y, float widthShape, float heightShape, std::string name, bool isDynamic) {
        shape.SetAsBox(widthShape / SCALE, heightShape / SCALE); //создаем выпуклую оболочку
        if(isDynamic) {
            bodyDef.type = b2_dynamicBody;
        }

        bindingToShape(name, x, y);
    }

    void bindingToShape(std::string name, float x, float y) {
        bodyDef.position.Set(x / SCALE, y / SCALE);
        body = World.CreateBody(&bodyDef);
        body->CreateFixture(&shape, hardness);
        body->SetUserData((void *) name.c_str());
    }

    void jump(){
        body->ApplyLinearImpulse(b2Vec2(0, -120), body->GetWorldCenter(), true);
    }

    void runRight(){
        b2Vec2 vel = body->GetLinearVelocity();
        float velChange = 5 - vel.x;
        float impulse = body->GetMass() * velChange; //disregard time factor
        body->ApplyLinearImpulse( b2Vec2(impulse,0), body->GetWorldCenter(), true);
    }

    void runLeft(){
        b2Vec2 vel = body->GetLinearVelocity();
        float velChange = -5 - vel.x;
        float impulse = body->GetMass() * velChange; //disregard time factor
        body->ApplyLinearImpulse( b2Vec2(impulse,0), body->GetWorldCenter(), true);
    }

    bool onGround(){ //работает, но костыльная функция, нужно придумать нормальную функцию
        b2Vec2 vel = body->GetLinearVelocity();
        if(vel.y < 0.1 && vel.y > -0.1){
            return true;
        } else {
            return false;
        }
    }

    void updatePosition(int &x, int &y){
        b2Vec2 positionBody = body->GetPosition();
        x = positionBody.x * SCALE;
        y = positionBody.y * SCALE;
    }
};

#endif //__PROJECT_BOX2D_H
