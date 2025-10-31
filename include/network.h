#pragma once

#include <vector>

#include <Random.hpp>

#include <cmath>

class network{
public:

    network(int input, int output, int hidden, int hiddenDensity);

    ~network();

    void setInputs(int input, float value);

    void calculate();

    void mutateWeights(float chance, float strength);
    void mutateBiases(float chance, float strength);

    float* returnOutputs();

    int totalWeights, totalBiases;
    
    float* networkWeights;
    float* networkBiases;

private:

    void addBiasAndActivation(int neuronIndex,bool isOutput);

    int input, output, hidden, hiddenDensity;

    int indexOfWeight(int layer, int neuron, int connection);

    int indexOfNeuron(int layer, int neuron);



    float* neuronCalculatedValues;
};