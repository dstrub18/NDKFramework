/*
  ==============================================================================

    NoNonlinearity.cpp
    Created: 19 Mar 2019 4:33:31pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "NoNonlinearity.h"
#include <Eigen>

NoNonlinearity::NoNonlinearity(){
    mCurrents = Eigen::MatrixXd::Zero(0, 1);
    mVoltages = Eigen::MatrixXd::Zero(0, 1);
}


void NoNonlinearity::solve(Eigen::MatrixXd* nonlinearVoltages, Eigen::MatrixXd* P) {

}
