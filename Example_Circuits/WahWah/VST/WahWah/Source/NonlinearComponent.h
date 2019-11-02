/*
  ==============================================================================

    NonlinearComponent.h
    Created: 6 Apr 2019 5:05:03pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#pragma once

#pragma once
#include <Eigen>

class NonlinearComponent {
    
protected:
    int mNumberOfFunctions;
    
public:
    
    virtual ~NonlinearComponent() = default;
    
    virtual float calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx) = 0;
    
    virtual void calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx) = 0;
    
    int getNumberOfFunctions(){return mNumberOfFunctions;};
    
};
