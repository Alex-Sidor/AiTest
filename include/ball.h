#pragma once

#include "network.h"
#include <algorithm>
#include <cmath>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class ball{
public:
    ball();

    void copyOther(const network& other);

    void reset(float tx, float ty);

    void update();

    void mutate();

    void setTarget(float tx, float ty);

    void resetTransform();

    float fitness;

    float x,y, vx, vy, tx, ty;

    sf::CircleShape circle;
    
    network brain = network(4,2,1,2);
private:
    void physics(float x1, float y1);

};