/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {
  
  NeuralNetwork::NeuralNetwork(UInt32 inputs, UInt32 hidden, UInt32 outputs) {
    objectType = (char*)"NeuralNetwork";
    inputWeights = NULL;
    inputValues = NULL;
    hiddenWeights = NULL;
    hiddenValues = NULL;
    hiddenThresholds = NULL;
    outputValues = NULL;
    outputThresholds = NULL;
    expectedValues = NULL;
    SetTopography(inputs, hidden, outputs);
    learningRate = 2.0;
    }

  NeuralNetwork::~NeuralNetwork() {
    }

  void NeuralNetwork::activateNetwork() {
    UInt32 i;
    UInt32 j;
    Double weightedInput;
    for (j=0; j<numHiddenNodes; j++) {
      weightedInput = 0.0;
      for (i=0; i<numInputNodes; i++) {
        weightedInput += inputWeights[i][j] * inputValues[i];
        }
      weightedInput += (-1 * hiddenThresholds[j]);
      hiddenValues[j] = 1.0 / (1.0 + Math::Pow(Math::E, -weightedInput));
      }
    for (j=0; j<numOutputNodes; j++) {
      weightedInput = 0.0;
      for (i=0; i<numHiddenNodes; i++) {
        weightedInput += hiddenWeights[i][j] * hiddenValues[j];
        }
      weightedInput += (-1 * outputThresholds[j]);
      outputValues[j] = 1.0 / (1.0 + Math::Pow(Math::E, -weightedInput));
      }
    }

  void NeuralNetwork::cleanStorage() {
    UInt32 i;
    if (inputWeights != NULL) {
      for (i=0; i<numInputNodes; i++) free(inputWeights[i]);
      free(inputWeights);
      inputWeights = NULL;
      }
    if (inputValues != NULL) {
      free(inputValues);
      inputValues = NULL;
      }
    if (hiddenWeights != NULL) {
      for (i=0; i<numHiddenNodes; i++) free(hiddenWeights[i]);
      free(hiddenWeights);
      hiddenWeights = NULL;
      }
    if (hiddenValues != NULL) {
      free(hiddenValues);
      hiddenValues = NULL;
      }
    if (hiddenThresholds != NULL) {
      free(hiddenThresholds);
      hiddenThresholds = NULL;
      }
    if (outputValues != NULL) {
      free(outputValues);
      outputValues = NULL;
      }
    if (outputThresholds != NULL) {
      free(outputThresholds);
      outputThresholds = NULL;
      }
    if (expectedValues != NULL) {
      free(expectedValues);
      expectedValues = NULL;
      }
    }

  void NeuralNetwork::setupNodes() {
    UInt32  i;
    UInt32  j;
    Random* rng;
    rng = new Random();
    for (i=0; i<numInputNodes; i++) {
      inputValues[i] = 0.0;
      for (j=0; j<numHiddenNodes; j++) {
        inputWeights[i][j] = rng->Next(200) / 100.0;
        }
      }
    for (i=0; i<numHiddenNodes; i++) {
      hiddenValues[i] = 0.0;
      hiddenThresholds[i] = (Double)(rng->Next(65536)) / (double)(rng->Next(65536));
      for (j=0; j<numOutputNodes; j++) {
        hiddenWeights[i][j] = rng->Next(200) / 100.0;
        }
      }
    for (i=0; i<numOutputNodes; i++) {
      outputValues[i] = 0.0;
      outputThresholds[i] = rng->Next(200) / 100.0;
      }
    delete(rng);
    }

  Double NeuralNetwork::updateWeights() {
    Double sumOfSquaredErrors;
    Double absoluteError;
    Double outputErrorGradient;
    Double hiddenErrorGradient;
    Double thresholdDelta;
    Double delta;
    UInt32 i,o,h;
    sumOfSquaredErrors = 0.0;
    for (o=0; o<numOutputNodes; o++) {
      absoluteError = expectedValues[o] - outputValues[o];
      sumOfSquaredErrors += Math::Pow(absoluteError, 2);
      outputErrorGradient = outputValues[o] * (1.0 - outputValues[o]) * absoluteError;
      for (h=0; h<numHiddenNodes; h++) {
        delta = learningRate * hiddenValues[h] * outputErrorGradient;
        hiddenWeights[h][o] += delta;
        hiddenErrorGradient = hiddenValues[h] * (1.0 - hiddenValues[h])
                              * outputErrorGradient * hiddenWeights[h][o];
        for (i=0; i<numInputNodes; i++) {
          delta = learningRate * inputValues[i] * hiddenErrorGradient;
          inputWeights[i][h] += delta;
          }
        thresholdDelta = learningRate * -1.0 * hiddenErrorGradient;
        hiddenThresholds[h] += thresholdDelta;
        }
      delta = learningRate * -1.0 * outputErrorGradient;
      outputThresholds[o] += delta;
      }
    return sumOfSquaredErrors;
    }

  List<Double>* NeuralNetwork::Execute(List<Double>* inputs) {
    UInt32 i;
    List<Double>* ret;
    if (inputs->Count() != numInputNodes) 
      throw InvalidOpException(this, "Inputs do not match network topography");
    for (i=0; i<numInputNodes; i++) inputValues[i] = inputs->At(i);
    activateNetwork();
    ret = new List<Double>();
    for (i=0; i<numOutputNodes; i++) ret->Add(outputValues[i]);
    return ret;
    }

  Double NeuralNetwork::LearningRate() {
    return learningRate;
    }

  Double NeuralNetwork::LearningRate(Double d) {
    learningRate = d;
    return learningRate;
    }

  UInt32 NeuralNetwork::NumInputNodes() {
    return numInputNodes;
    }

  UInt32 NeuralNetwork::NumHiddenNodes() {
    return numHiddenNodes;
    }

  UInt32 NeuralNetwork::NumOutputNodes() {
    return numOutputNodes;
    }

  void NeuralNetwork::SetTopography(UInt32 inputs, UInt32 hidden, UInt32 outputs) {
    UInt32 i;
    cleanStorage();
    numInputNodes = inputs;
    numHiddenNodes = hidden;
    numOutputNodes = outputs;
    inputWeights = (Double**)malloc(sizeof(Double*) * numInputNodes);
    if (inputWeights == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }
    for (i=0; i<numInputNodes; i++) inputWeights[i] = NULL;
    for (i=0; i<numInputNodes; i++) {
      inputWeights[i] = (Double*)malloc(sizeof(Double) * numHiddenNodes);
      if (inputWeights[i] == NULL) {
        cleanStorage();
        throw MemoryException(this, "Could not allocate memory");
        }
      }
    inputValues = (Double*)malloc(sizeof(Double) * numInputNodes);
    if (inputValues == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }

    hiddenWeights = (Double**)malloc(sizeof(Double*) * numHiddenNodes);
    if (hiddenWeights == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }
    for (i=0; i<numHiddenNodes; i++) hiddenWeights[i] = NULL;
    for (i=0; i<numHiddenNodes; i++) {
      hiddenWeights[i] = (Double*)malloc(sizeof(Double) * numOutputNodes);
      if (hiddenWeights[i] == NULL) {
        cleanStorage();
        throw MemoryException(this, "Could not allocate memory");
        }
      }
    hiddenValues = (Double*)malloc(sizeof(Double) * numHiddenNodes);
    if (hiddenValues == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }
    hiddenThresholds = (Double*)malloc(sizeof(Double) * numHiddenNodes);
    if (hiddenThresholds == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }

    outputValues = (Double*)malloc(sizeof(Double) * numOutputNodes);
    if (outputValues == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }
    outputThresholds = (Double*)malloc(sizeof(Double) * numOutputNodes);
    if (outputThresholds == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }
    expectedValues = (Double*)malloc(sizeof(Double) * numOutputNodes);
    if (expectedValues == NULL) {
      cleanStorage();
      throw MemoryException(this, "Could not allocate memory");
      }
    setupNodes();
    }


  }

