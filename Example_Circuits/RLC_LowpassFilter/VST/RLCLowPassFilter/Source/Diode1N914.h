/*
  ==============================================================================

    Diode.h
    Created: 6 Apr 2019 5:05:32pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#pragma once
#include "NonlinearComponent.h"

class Diode1N914 : public NonlinearComponent{
private:
    float mVt;
    float mIs;
    
    
public:
    
    Diode1N914();
    ~Diode1N914() = default;
    
    float calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx) override;
    void calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx) override;
    
};
