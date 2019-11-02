/*
  ==============================================================================

    StateSpaceProcessor.cpp
    Created: 19 Mar 2019 4:32:27pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "StateSpaceProcessor.h"
#include "ConstantMatrices.h"
#include "NoNonlinearity.h"
#include "NonlinearComponentParser.h"
#include "NewtonRaphson.h"
#include <Eigen>
#include <iostream>

StateSpaceProcessor::StateSpaceProcessor(std::string jsonPath, int fs){
    // Create the circuit from the parsed json
    CircuitParser circuitParser = CircuitParser(jsonPath);
    setParsedCircuit(circuitParser);
    // Create constant matrices
    ConstantMatrices constantMatrices = ConstantMatrices(circuitParser, fs);
    setConstantMatrices(constantMatrices);
    updateRtMatrices();
    circuitParser.printParser();

    NonlinearComponentParser nonlinearComponentParser(circuitParser.getNonlinearModels());

    // Create nonlinear solvers
    if (mNumNonlinears > 0){
        mNonlinearSolver = std::make_unique<NewtonRaphson>(mNumNonlinears, mK,
                                                           nonlinearComponentParser.getNonlinearComponentList());
                // NewtonRaphson(int numNonlinears, Eigen::MatrixXd K, std::vector<NonlinearComponent>* nonlinearComponents);
    }
    else{
        mNonlinearSolver = std::make_unique<NoNonlinearity>();
    };

    mP = Eigen::MatrixXd::Zero(mNumNonlinears,1);
    mState = Eigen::MatrixXd::Zero(circuitParser.getNumReactives(), 1);
    mNonlinearCurrents = Eigen::MatrixXd::Zero(mNumNonlinears, 1);
    mNonlinearVoltages = Eigen::MatrixXd::Zero(mNumNonlinears, 1);
}

/**  Update the matrices altered by the potentiometers
 */
void StateSpaceProcessor::updateRtMatrices(){
    if(mNumPotentiometers!=0){
        Eigen::MatrixXd RvQinv = (mRv+mQ).inverse();
        mA = mA0-2*mZ*mGx*mUx*RvQinv*mUxT;
        mB = mB0-2*mZ*mGx*mUx*RvQinv*mUuT;
        mC = mC0-2*mZ*mGx*mUx*RvQinv*mUnT;
        mD = mD0-mUo*RvQinv*mUxT;
        mE = mE0-mUo*RvQinv*mUuT;
        mF = mF0-mUo*RvQinv*mUnT;
        mG = mG0-mUn*RvQinv*mUxT;
        mH = mH0-mUn*RvQinv*mUuT;
        mK = mK0-mUn*RvQinv*mUnT;
    } else{
        mA = mA0;
        mB = mB0;
        mC = mC0;
        mD = mD0;
        mE = mE0;
        mF = mF0;
        mG = mG0;
        mH = mH0;
        mK = mK0;
    }
}

void StateSpaceProcessor::setConstantMatrices(ConstantMatrices constantMatrices){
    mA0 = constantMatrices.getA0();
    mB0 = constantMatrices.getB0();
    mC0 = constantMatrices.getC0();
    mD0 = constantMatrices.getD0();
    mE0 = constantMatrices.getE0();
    mF0 = constantMatrices.getF0();
    mG0 = constantMatrices.getG0();
    mH0 = constantMatrices.getH0();
    mK0 = constantMatrices.getK0();
    mUo = constantMatrices.getUo();
    mUx = constantMatrices.getUx();
    mUn = constantMatrices.getUn();
    mUu = constantMatrices.getUu();
    mUxT = constantMatrices.getUxT();
    mUnT = constantMatrices.getUnT();
    mUuT = constantMatrices.getUuT();
    mS0inv = constantMatrices.getS0inv();
    mGx = constantMatrices.getGx();
    mQ = constantMatrices.getQ();
    mZ = constantMatrices.getZ();
    mNumNonlinears = constantMatrices.getNumNonlinears();
    mNumPotentiometers = constantMatrices.getNumPotentiometers();
    mRv = mRv0+Eigen::MatrixXd::Identity(mNumPotentiometers,mNumPotentiometers)*0.001;
}

void StateSpaceProcessor::setParsedCircuit(CircuitParser circuitParser){
    mRv0 = circuitParser.getRv0();
    mNodeNames = circuitParser.getNodeNames();
    mPotNames = circuitParser.getPotNames();
    mSourceVoltages = circuitParser.getSourceVoltages();
}

void StateSpaceProcessor::updatePotValue(int idx, float value){
    int topIdx = idx*2;
    int botIdx = idx*2+1;
    mRv(topIdx,topIdx) = value*mRv0(topIdx,topIdx)+0.001;
    mRv(botIdx,botIdx) = (1-value)*mRv0(topIdx,topIdx)+0.001;
    updateRtMatrices();
    mNonlinearSolver->setK(mK);
}

float StateSpaceProcessor::process(float input){
    // update the input voltage.
    mSourceVoltages(0) = input;

    // Calculate P(n) = G*xn-1+H*un
    mP.noalias() = mG*mState;
    mP.noalias() += mH*mSourceVoltages;

    
    //Numerically solve p(n)+K*In-Vn
    mNonlinearSolver->solve(&mNonlinearVoltages,&mP);
    mNonlinearVoltages = mNonlinearSolver->getVoltages();
    mNonlinearCurrents = mNonlinearSolver->getCurrents();
    
    // Calculate the output
    mOutput.noalias() = mD*mState;
    mOutput.noalias() += mE*mSourceVoltages;
    mOutput.noalias() += mF*mNonlinearCurrents;
    float output = mOutput(0);
    
    // Update the state
    mState.noalias() = mA*mState;
    mState.noalias() += mB*mSourceVoltages;
    mState.noalias() += mC*mNonlinearCurrents;
    
    return output;
}

