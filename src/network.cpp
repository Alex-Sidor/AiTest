#include "network.h"

network::~network(){
    delete[] networkWeights;
    delete[] networkBiases;
    delete[] neuronCalculatedValues;
}

network::network(int input, int output, int hidden, int hiddenDensity){
    this->input = input;
    this->output = output;
    this->hidden = hidden;
    this->hiddenDensity = hiddenDensity;

    totalWeights = (input*hiddenDensity) + ((hidden-1)*hiddenDensity*hiddenDensity) + (hiddenDensity*output);
    totalBiases = input + (hidden*hiddenDensity) + output;

    networkWeights = new float[totalWeights];

    networkBiases = new float[totalBiases];
    neuronCalculatedValues = new float[totalBiases];
    
    /*for(int i = 0; i < totalBiases; i++){
        networkBiases[i] = Random::GetFloat(-1.0f, 1.0f);
    }

    for(int i = 0; i < (totalWeights); i++){
        networkWeights[i] = Random::GetFloat(-1.0f, 1.0f);
    }*/
}

void network::setInputs(int select, float value){
    neuronCalculatedValues[select] = value;
}

float* network::returnOutputs(){
    return (neuronCalculatedValues + (input + hidden*hiddenDensity));
}

void network::addBiasAndActivation(int neuronIndex,bool isOutput){
    neuronCalculatedValues[neuronIndex] += networkBiases[neuronIndex];

    if(isOutput){
        neuronCalculatedValues[neuronIndex] = std::tanh(neuronCalculatedValues[neuronIndex]);
    }
}

void network::calculate(){
    for(int i = input; i < totalBiases; i++){
        neuronCalculatedValues[i] = 0.0f;
    }

    for(int neuron = 0; neuron <  input; neuron++){
        int thisNeuronIndex = indexOfNeuron(0,neuron);
        
        addBiasAndActivation(thisNeuronIndex,false);

        float thisNeuronCurrentActivation = neuronCalculatedValues[thisNeuronIndex];
        for(int connection = 0; connection < hiddenDensity; connection ++){
            float weight = networkWeights[indexOfWeight(0,neuron,connection)];
            
            neuronCalculatedValues[indexOfNeuron(1,connection)] += weight * thisNeuronCurrentActivation;
        }
    }

    //hidden layers
    for(int layer = 1; layer <  hidden; layer++){
        for(int neuron = 0; neuron <  hiddenDensity; neuron++){
            int thisNeuronIndex = indexOfNeuron(layer, neuron);
        
            addBiasAndActivation(thisNeuronIndex,false);
            
            float thisNeuronCurrentActivation = neuronCalculatedValues[thisNeuronIndex];

            for(int connection = 0; connection < hiddenDensity; connection ++){
                float weight = networkWeights[indexOfWeight(layer,neuron,connection)];
                
                neuronCalculatedValues[indexOfNeuron(layer+1,connection)] += weight * thisNeuronCurrentActivation;
            }
        }
    }

    //to output layer
    for(int neuron = 0; neuron <  hiddenDensity; neuron++){
        int thisNeuronIndex = indexOfNeuron(hidden, neuron);
        
        addBiasAndActivation(thisNeuronIndex,false);
            
        float thisNeuronCurrentActivation = neuronCalculatedValues[thisNeuronIndex];

        for(int connection = 0; connection < output; connection ++){
            float weight = networkWeights[indexOfWeight(hidden,neuron,connection)];
            
            neuronCalculatedValues[indexOfNeuron(hidden+1,connection)] += weight * thisNeuronCurrentActivation;
        }
    }

    //hyperbolic tangent on output

    for(int i = input + (hidden*hiddenDensity); i < totalBiases; i++){
        addBiasAndActivation(i,true);
    }
}

int network::indexOfNeuron(int layer, int neuron){
    if(layer == 0){
        return neuron;
    }
    else if(layer <= hidden){
        return input + (layer-1)*hiddenDensity + neuron;
    }
    else{
        return input + hidden*hiddenDensity + neuron;
    }
}

int network::indexOfWeight(int layer, int neuron, int connection){
    if(layer == 0){
        return((neuron*hiddenDensity) + connection);
    }
    else if(layer != hidden){
        return((input*hiddenDensity) + ((layer-1)*hiddenDensity*hiddenDensity) + (neuron*hiddenDensity) + connection);
    }
    else{
        return((input*hiddenDensity) + ((hidden-1)*hiddenDensity*hiddenDensity)+(neuron*output)+connection);
    }
}

void network::mutateWeights(float chance, float strength){
    for(int i = 0;i < totalWeights; i++){
        if(Random::GetFloat(0.0f, 1.0f) < chance){
            networkWeights[i] += Random::GetFloat(-strength, strength);
        }
    }
}

void network::mutateBiases(float chance, float strength){
    for(int i = 0;i < totalBiases; i++){
        if(Random::GetFloat(0.0f, 1.0f) < chance){
            networkBiases[i] += Random::GetFloat(-strength, strength);
        }
    }
}