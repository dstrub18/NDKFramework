/*
  ==============================================================================

    CircuitParser.h
    Created: 19 Mar 2019 4:32:54pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/


#include <iostream>
#include "Eigen"
#include "json.hpp"

class CircuitParser{

public:
    CircuitParser(std::string jsonPath);
    ~CircuitParser() = default;

    const Eigen::MatrixXd& getNr() const {
        return mNr;
    }

    const Eigen::MatrixXd& getNx() const {
        return mNx;
    }

    const Eigen::MatrixXd& getNv() const {
        return mNv;
    }

    const Eigen::MatrixXd& getNu() const {
        return mNu;
    }

    const Eigen::MatrixXd& getNn() const {
        return mNn;
    }

    const Eigen::MatrixXd& getNo() const {
        return mNo;
    }
    const Eigen::MatrixXd& getNopai() const {
        return mNopai;
    }
    
    const Eigen::MatrixXd& getNopao() const {
        return mNopao;
    }

    const Eigen::MatrixXd& getGr() const {
        return mGr;
    }

    const Eigen::MatrixXd& getXlc() const {
        return mXlc;
    }

    const Eigen::MatrixXd& getRv0() const {
        return mRv0;
    }

    const Eigen::MatrixXd& getZ() const {
        return mZ;
    }

    const Eigen::MatrixXd& getSourceVoltages() const {
        return mSourceVoltages;
    }

    unsigned int getNumResistors() const {
        return mNumResistors;
    }

    unsigned int getNumReactives() const {
        return mNumReactives;
    }

    unsigned int getNumNonlinears() const {
        return mNumNonlinears;
    }

    unsigned int getNumSources() const {
        return mNumSources;
    }
    
    unsigned int getNumOpamps() const {
        return mNumOpamps;
    }

    unsigned int getNumOutputs() const {
        return mNumOutputs;
    }

    unsigned int getNumNodes() const {
        return mNumNodes;
    }

    unsigned int getNumPotentiometers() const {
        return mNumPotentiometers;
    }

    const std::vector<std::string>& getNodeNames() const {
        return mNodeNames;
    }

    const std::vector<std::string>& getPotNames() const {
        return mPotentiometerNames;
    }

    const std::vector<std::string>& getNonlinearModels() const {
        return mNonlinearModels;
    }

    void printParser() {
        std::cout << "mNr is\n" << mNr << "\n";
        std::cout << "mNx is\n" << mNx << "\n";
        std::cout << "mNv is\n" << mNv << "\n";
        std::cout << "mNu is\n" << mNu << "\n";
        std::cout << "mNn is\n" << mNn << "\n";
        std::cout << "mNo is\n" << mNo << "\n";
        std::cout << "mGr is\n" << mGr << "\n";
        std::cout << "mXlc is\n" << mXlc << "\n";
        std::cout << "mRv0 is\n" << mRv0 << "\n";
        std::cout << "mZ is\n" << mZ << "\n";
        std::cout << "mSourceVoltages is\n" << mSourceVoltages << "\n";
    }

private:
    
    
    Eigen::MatrixXd mNr, mNx, mNv, mNu, mNn, mNo, mGr, mXlc, mRv0, mZ, mNopai, mNopao;
    Eigen::MatrixXd mSourceVoltages;
    
    unsigned int mNumResistors, mNumReactives, mNumNonlinears, mNumSources,
            mNumInputs, mNumOutputs, mNumNodes, mNumPotentiometers, mNumOpamps;
    
    
    std::vector<std::string> mNodeNames;
    std::vector<std::string> mPotentiometerNames;
    std::vector<std::string> mNonlinearModels;

    
    void parseCircuit(nlohmann::json jsonData);
    void parseMatrix(nlohmann::json jsonData, std::string key, Eigen::MatrixXd* target);
    
    void parseQuantity(nlohmann::json jsonData, std::string key, unsigned int* quantity);
    void parseNames(nlohmann::json jsonData, std::string key, std::vector<std::string>* names);
    
};
