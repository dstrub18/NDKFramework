/*
  ==============================================================================

    StateSpaceProcessor.h
    Created: 19 Mar 2019 4:32:27pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#ifndef STATESPACEPROCESSOR_H
#define STATESPACEPROCESSOR_H

#include <Eigen>
#include "ConstantMatrices.h"
#include "NonlinearSolver.h"

class StateSpaceProcessor
{
private:
    std::unique_ptr<NonlinearSolver> mNonlinearSolver;
    std::vector<std::string> mNodeNames;
    std::vector<std::string> mPotNames;
    
    Eigen::MatrixXd mState;
    Eigen::MatrixXd mSourceVoltages;
    Eigen::MatrixXd mNonlinearCurrents;
    Eigen::MatrixXd mNonlinearVoltages;
    
    // Real-time matrices
    Eigen::MatrixXd mA, mB, mC, mD, mE, mF, mG, mH, mK, mRv;
    
    // Constant matrices
    Eigen::MatrixXd mA0, mB0, mC0, mD0, mE0, mF0, mG0, mH0, mK0, mRv0;
    Eigen::MatrixXd mUo, mUx, mUn, mUu, mUxT, mUnT, mUuT, mS0inv, mGx, mQ, mZ;

    Eigen::MatrixXd mP;
    Eigen::MatrixXd mOutput;

    unsigned int mNumNonlinears;
    unsigned int mNumPotentiometers;
    bool mPotMoved;
    
    
public:
    StateSpaceProcessor(std::string jsonPath, int fs);
    
    void updateRtMatrices();
    void updatePotValue(int idx, float value);
    void setConstantMatrices(ConstantMatrices constantMatrices);
    void setParsedCircuit(CircuitParser circuitParser);
    
    
    float process(float input);
    

    void printMatrices() {
        std::cout << "mA is\n" << mA << "\n";
        std::cout << "mB is\n" << mB << "\n";
        std::cout << "mC is\n" << mC << "\n";
        std::cout << "mD is\n" << mD << "\n";
        std::cout << "mE is\n" << mE << "\n";
        std::cout << "mF is\n" << mF << "\n";
        std::cout << "mG is\n" << mG << "\n";
        std::cout << "mH is\n" << mH << "\n";
        std::cout << "mK is\n" << mK << "\n";
        std::cout << "mS0inv is\n" << mS0inv << "\n";
        std::cout << "mGx is\n" << mGx << "\n";
        std::cout << "mZ is\n" << mZ << "\n";
    }
    
    
    
};

#endif
