/*
  ==============================================================================

    NewtonRaphson.cpp
    Created: 6 Apr 2019 5:05:48pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "NewtonRaphson.h"
#include <Eigen>
#include <iostream>

NewtonRaphson::NewtonRaphson(int numNonlinears, Eigen::MatrixXd K,
                             std::unique_ptr<std::vector<NonlinearComponent*>> nonlinearComponents){
    mNumNonlinears = numNonlinears;
    mVoltages = Eigen::MatrixXd::Zero(numNonlinears, 1);
    mCurrents = Eigen::MatrixXd::Zero(numNonlinears,1);
    //mStep = Eigen::MatrixXd::Ones(numNonlinears, 1);
    mK = K;
    mComponentsJacobian = Eigen::MatrixXd::Zero(numNonlinears,numNonlinears);
    mFunctionJacobian = Eigen::MatrixXd::Zero(numNonlinears,numNonlinears);
    mEye = Eigen::MatrixXd::Identity(numNonlinears, numNonlinears);
    mComponents = std::move(nonlinearComponents);
    mNumComponents = (int) mComponents->size();
    mNonlinearFunction = Eigen::MatrixXd::Zero(numNonlinears,1);
}

void NewtonRaphson::solve(Eigen::MatrixXd* nonlinearVoltages, Eigen::MatrixXd* P) {
    mP = *P;
    mStep = Eigen::MatrixXd::Ones(mNumNonlinears, 1);
    mCounter = 0;
    while(mCounter < mMaxIterations && mStep.squaredNorm() > mTolerance) {
        calculateUnknowns();
        mFunctionJacobian.noalias() = mK*mComponentsJacobian;
        mFunctionJacobian -= mEye;
        mNonlinearFunction.noalias() = mK*mCurrents;
        mNonlinearFunction += mP;
        mNonlinearFunction -= mVoltages;
        mStep.noalias() = mFunctionJacobian.inverse()*mNonlinearFunction;
        mVoltages -= mStep;
        mCounter++;
    }
    updateCurrents();
}

void NewtonRaphson::updateCurrents(){
     mVoltageIdx = 0;
    for(int n = 0; n<mNumComponents; n++){
        mNumFunctions = (*mComponents)[n]->getNumberOfFunctions();
        for(int m = 0; m<mNumFunctions;m++){
            mCurrents(m+mVoltageIdx) = (*mComponents)[n]->calculateCurrent(mVoltages,mVoltageIdx,m);
        }
        mVoltageIdx += mNumFunctions;
    }
}

void NewtonRaphson::calculateUnknowns(){
    mVoltageIdx = 0;
    for(int n = 0; n<mNumComponents; n++){
        mNumFunctions = (*mComponents)[n]->getNumberOfFunctions();
        (*mComponents)[n]->calculateJacobian(&mComponentsJacobian,mVoltages,mVoltageIdx);
        for(int m = 0; m<mNumFunctions;m++){
            mCurrents(m+mVoltageIdx) = (*mComponents)[n]->calculateCurrent(mVoltages,mVoltageIdx,m);
        }
        mVoltageIdx += mNumFunctions;
    }
}
