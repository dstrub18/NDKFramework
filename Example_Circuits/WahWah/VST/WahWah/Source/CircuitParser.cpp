/*
  ==============================================================================

    CircuitParser.cpp
    Created: 19 Mar 2019 4:32:54pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "CircuitParser.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <cstring>




CircuitParser::CircuitParser(std::string jsonPath){

    std::string temp = jsonPath;
    char pathAsChars[temp.length()];
    strcpy(pathAsChars, temp.c_str());
    
    std::ifstream i(pathAsChars, std::ifstream::in); // Get json filepath from the console

    nlohmann::json jsonData;

    try {
        i >> jsonData; //Copy the data from i into jsonFile.
    } catch (nlohmann::json::parse_error) {
        std::cerr << "CircuitParser: Json data could not be read from file " << jsonPath << ".\nPlease make sure that the file exists.\n";
    }

    parseCircuit(jsonData);
}


void CircuitParser::parseCircuit(nlohmann::json jsonData) {

    parseQuantity(jsonData, "numNodes", &mNumNodes);
    parseQuantity(jsonData, "numResistors", &mNumResistors);
    parseQuantity(jsonData, "numReactives", &mNumReactives);
    parseQuantity(jsonData, "numInputs", &mNumInputs);
    parseQuantity(jsonData, "numSources", &mNumSources);
    parseQuantity(jsonData, "numPotentiometers", &mNumPotentiometers);
    parseQuantity(jsonData, "numOutputs", &mNumOutputs);
    parseQuantity(jsonData, "numNonlinears", &mNumNonlinears);
    parseQuantity(jsonData, "numOpamps", &mNumOpamps);

    parseMatrix(jsonData,"Nr",&mNr);
    parseMatrix(jsonData,"Nx",&mNx);
    parseMatrix(jsonData,"Nv",&mNv);
    parseMatrix(jsonData,"Nu",&mNu);
    parseMatrix(jsonData,"No",&mNo);
    parseMatrix(jsonData,"Nn",&mNn);
    parseMatrix(jsonData,"Gr",&mGr);
    parseMatrix(jsonData,"Xlc",&mXlc);
    parseMatrix(jsonData,"Rv0",&mRv0);
    parseMatrix(jsonData,"Z",&mZ);
    parseMatrix(jsonData,"Nopai",&mNopai);
    parseMatrix(jsonData,"Nopao",&mNopao);
    
    parseMatrix(jsonData, "sourceVoltages", &mSourceVoltages);

    parseNames(jsonData, "nodeNames", &mNodeNames);
    parseNames(jsonData, "potentiometerNames", &mPotentiometerNames);
    parseNames(jsonData, "nonlinearModels", &mNonlinearModels);

    
}

void CircuitParser::parseMatrix(nlohmann::json jsonData, std::string key, Eigen::MatrixXd* target) {
    std::vector<std::vector<float>> vals = jsonData.at(key);

    if (vals.size() == 0) {
        if (key == "Gr" || key == "Xlc" || key == "Rv0" || key == "Z") {
            *target = Eigen::MatrixXd::Zero(0, 0);
            return;
        } else {
            *target = Eigen::MatrixXd::Zero(0, mNumNodes);
            return;
        }
    } else {
        *target = Eigen::MatrixXd::Zero(vals.size(), vals[0].size());

        for (int i = 0; i < vals.size(); ++i) {
            for (int j = 0; j < vals[i].size(); ++j) {
                (*target)(i, j) = vals[i][j];
            }
        }
    }
    
}

void CircuitParser::parseQuantity(nlohmann::json jsonData, std::string key, unsigned int* quantity){
    *quantity = jsonData.at(key);
}

void CircuitParser::parseNames(nlohmann::json jsonData, std::string key, std::vector<std::string>* names){
    *names = jsonData.at(key).get<std::vector<std::string>>();
}






