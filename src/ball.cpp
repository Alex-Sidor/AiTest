#include "ball.h"

ball::ball(){
    circle.setRadius(5.0f);
    circle.setFillColor(sf::Color(255, 255, 255, 5));
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
    brain.mutateWeights(0.3f,0.3f);
    brain.mutateBiases(0.01f,0.01f);
}

void ball::update(){
    brain.setInputs(0,x);
    brain.setInputs(1,y);
    brain.setInputs(2,tx);
    brain.setInputs(3,ty);
    brain.setInputs(2,vx);
    brain.setInputs(3,vy);

    brain.calculate();

    float* output = brain.returnOutputs();

    physics(output[0],output[1]);

    float dx = x - tx;
    float dy = y - ty;

    fitness += std::sqrt((dx*dx)+(dy*dy));

    circle.setPosition(sf::Vector2f(x, y));
}

void ball::physics(float x1, float y1){

    vx = sin(x1*8.0f)*y1;
    vy = cos(x1*8.0f)*y1;

    x += vx;
    y += vy;
}