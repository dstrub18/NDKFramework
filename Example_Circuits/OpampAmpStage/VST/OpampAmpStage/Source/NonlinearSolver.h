/*
  ==============================================================================

    NonlinearSolver.h
    Created: 19 Mar 2019 4:33:02pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#pragma once
#include <Eigen>
#include "NonlinearComponent.h"

class NonlinearSolver {
protected:
    Eigen::MatrixXd mCurrents;
    Eigen::MatrixXd mVoltages;
    Eigen::MatrixXd mK;
    Eigen::MatrixXd mP;
    std::unique_ptr<std::vector<NonlinearComponent*>> mComponents;
    int mNumComponents;
    int mVoltageIdx = 0;
    
private:
    
public:
    virtual ~NonlinearSolver() = default;
    
    Eigen::MatrixXd getCurrents(){return mCurrents;};
    Eigen::MatrixXd getVoltages(){return mVoltages;};
    void setK(Eigen::MatrixXd K){mK = K;};
    
    virtual void solve(Eigen::MatrixXd* nonlinearVoltages, Eigen::MatrixXd* P) = 0;
    
};
