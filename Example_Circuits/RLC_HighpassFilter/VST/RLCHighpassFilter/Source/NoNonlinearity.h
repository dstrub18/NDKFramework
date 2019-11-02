/*
  ==============================================================================

    NoNonlinearity.h
    Created: 19 Mar 2019 4:33:31pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#pragma once

#include "NonlinearSolver.h"
#include <Eigen>

class NoNonlinearity : public NonlinearSolver{
private:
    
public:
    NoNonlinearity();
    ~NoNonlinearity() = default;

    void solve(Eigen::MatrixXd* nonlinearVoltages, Eigen::MatrixXd* P);
};
