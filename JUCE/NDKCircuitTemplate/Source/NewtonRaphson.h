/*
  ==============================================================================

    NewtonRaphson.h
    Created: 6 Apr 2019 5:05:48pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#pragma once

#include "NonlinearSolver.h"
#include "NonlinearComponent.h"
#include <Eigen>


class NewtonRaphson : public NonlinearSolver{
private:
    int mNumNonlinears;
    int mNumFunctions;
    int mMaxIterations = 100;
    int mCounter = 0;
    float mTolerance = 1e-5;
    
    Eigen::MatrixXd mEye;
    Eigen::MatrixXd mComponentsJacobian;
    Eigen::MatrixXd mFunctionJacobian;
    Eigen::MatrixXd mStep;
    Eigen::MatrixXd mNonlinearFunction;
    
public:
    NewtonRaphson(int numNonlinears, Eigen::MatrixXd K,
                  std::unique_ptr<std::vector<NonlinearComponent*>> nonlinearComponents);
    ~NewtonRaphson() = default;
    
    void solve(Eigen::MatrixXd* nonlinearVoltages, Eigen::MatrixXd* P) override;
    
    void calculateUnknowns();
    void updateCurrents();
};
