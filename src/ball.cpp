#include "ball.h"

ball::ball(){
    circle.setRadius(5.0f);
    circle.setFillColor(sf::Color(255, 255, 255, 20));
}

void ball::setTarget(float tx, float ty){
    this->tx = tx;
    this->ty = ty;  
}

void ball::resetTransform(){
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
}

void ball::reset(float tx, float ty){
    this->tx = tx;
    this->ty = ty;

    x = 0;
    y = 0;
    vx = 0;
    vy = 0;

    fitness = 0;
}

void ball::mutate(){
    brain.mutateWeights(0.01f,0.1f);
    brain.mutateBiases(0.01f,0.1f);
}

void ball::update(){
    brain.setInputs(0,x);
    brain.setInputs(1,y);
    brain.setInputs(2,tx);
    brain.setInputs(3,ty);

    brain.calculate();

    float* output = brain.returnOutputs();

    physics(output[0],output[1]);

    float dx = x - tx;
    float dy = y - ty;

    fitness += std::sqrt((dx*dx)+(dy*dy));

    circle.setPosition(sf::Vector2f(x, y));
}

void ball::physics(float x1, float y1){

    vx = x1;
    vy = y1;

    x += vx;
    y += vy;
}