/*
  ==============================================================================

    Npn2N2222.h
    Created: 14 May 2019 12:23:57pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#pragma once
#include "NonlinearComponent.h"

class Npn2N2222 : public NonlinearComponent{
private:
    float mCurrent;
    float mJacobian;
    float mVt;
    float mIs;
    float mBf;
    float mBr;
    
    
public:
    
    Npn2N2222();
    ~Npn2N2222() = default;
    
    float calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx) override;
    void calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx) override;
    
};
