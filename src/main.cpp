#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <Random.hpp>
 
#include <iostream>
#include <fstream>

#include <memory> 
#include <chrono>
#include <thread>
#include <algorithm> 

#include "network.h"

#include "ball.h"

const int populationCount = 100;
ball population[populationCount];

void copyWeightsAndBiasesFrom(const ball& other,const ball& recieving,int totalWeights, int totalBiases) {
    for(int i = 0; i < totalWeights; i++)
        recieving.brain.networkWeights[i] = other.brain.networkWeights[i];
    for(int i = 0; i < totalBiases; i++)
        recieving.brain.networkBiases[i] = other.brain.networkBiases[i];
}

int main()
{   
    using namespace std::chrono;
    double fps = 200.0f;
    milliseconds frameDuration(static_cast<int>(1000.0 / fps));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    sf::View view;
    view.setSize(sf::Vector2f(800.f, 600.f));
    view.setCenter(sf::Vector2f(0.f, 0.f));
    window.setView(view);
    
    sf::Font font("arial.ttf");

    sf::Text text1(font);
    text1.setCharacterSize(10);
    text1.setFillColor(sf::Color::White);
    text1.setPosition(sf::Vector2f(-400.f, -300.f));

    sf::CircleShape TargetPositon(10.f);


    TargetPositon.setFillColor(sf::Color(255,0,0));
    TargetPositon.setPosition({10.0f,100.0f});

    std::ofstream file("data.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    int trainFrameCount = 0;

    float tx, ty;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
                if(fps == 200){
                   fps = 1000000; 
                }else{
                    fps = 200;
                }
                frameDuration = milliseconds(static_cast<int>(1000.0 / fps));
            }
        }

        text1.setString(std::to_string(fps));

        auto frameStart = steady_clock::now();

        window.clear();

        if(trainFrameCount == 500){
            int indices[populationCount];
            for (int i = 0; i < populationCount; i++)
                indices[i] = i;

            std::sort(indices, indices + populationCount, [&](int a, int b) {
                return population[a].fitness < population[b].fitness;
            });

            const int staying = populationCount / 4;

            for (int i = staying; i < populationCount; i++) {
                int parentIdx = indices[Random::GetInt(0, staying)];
                copyWeightsAndBiasesFrom(population[parentIdx],population[i],population[i].brain.totalWeights,population[i].brain.totalBiases);
                population[i].mutate();
            }
            
            //start new round
            float avgFitness = 0.0f;  

            std::cout << population[indices[0]].fitness << "\n";
            file << population[indices[0]].fitness  << " ";

            tx = Random::GetFloat(-100.0f, 100.0f);
            ty = Random::GetFloat(-100.0f, 100.0f);

            TargetPositon.setPosition({tx,ty});

            for(int i = 0; i < populationCount; i++){
                avgFitness += (population[i].fitness)/static_cast<float>(populationCount);
                population[i].reset(tx,ty);
            }

            file << avgFitness << "\n";
            
            trainFrameCount = 0;
        
        }else{
            for(int i = 0; i < populationCount; i++){
                population[i].update();
            }
            trainFrameCount++; 

            if(trainFrameCount == 250){
                tx = Random::GetFloat(-100.0f, 100.0f);
                ty = Random::GetFloat(-100.0f, 100.0f);
                
                TargetPositon.setPosition({tx,ty});
                
                for(int i = 0; i < populationCount; i++){
                    population[i].setTarget(tx,ty);
                    population[i].resetTransform();
                }
            }
        }


        window.draw(TargetPositon);

        for(int i = 0; i < populationCount; i++){
            window.draw(population[i].circle);
        }
        
        window.draw(text1);
        window.display();

        auto frameEnd = steady_clock::now();
        auto elapsed = duration_cast<milliseconds>(frameEnd - frameStart);

        if (elapsed < frameDuration){
            std::this_thread::sleep_for(frameDuration - elapsed);
        }
    }
    file.close();
}