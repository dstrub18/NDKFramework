/*
  ==============================================================================

    Triode12AX7.h
    Created: 16 May 2019 3:31:40pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "NonlinearComponent.h"

#pragma once
class Triode12AX7 : public NonlinearComponent{
private:
    float mCurrent;
    float mJacobian;
    float mEp;
    float mGam;
    float mG;
    float mGg;
    float mC;
    float mCg;
    float mMy;
    float mIg0;
    
    
public:
    
    Triode12AX7();
    ~Triode12AX7() = default;
    
    float calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx) override;
    void calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx) override;
    
};
