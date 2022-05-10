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
        if(name == "player") {
            body->SetUserData((void *) "player");
        }
        if(name == "coin"){
            body->SetUserData((void *) "coin");
        }
        if(name == "obstacle"){
            body->SetUserData((void *) "obstacle");
        }
    }
};

#endif //__PROJECT_BOX2D_H
