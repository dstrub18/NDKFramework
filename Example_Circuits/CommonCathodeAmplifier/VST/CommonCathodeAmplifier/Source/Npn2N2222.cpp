/*
  ==============================================================================

    Npn2N2222.cpp
    Created: 14 May 2019 12:23:57pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "Npn2N2222.h"
#include <iostream>

Npn2N2222::Npn2N2222(){
    mVt = 0.02528;
    mIs = 1.16e-14;
    mBf = 200;
    mBr = 3;
    mNumberOfFunctions = 2;
    mCurrent = 0;
}

float Npn2N2222::calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx){
    if(functionIdx == 0){
        mCurrent = mIs*(1/mBf * (exp((voltages(voltageIdx+1)-voltages(voltageIdx))/mVt) - 1) + 1/mBr * (exp(-voltages(voltageIdx)/mVt) - 1));
    }else if (functionIdx == 1){
        mCurrent = -mIs*(-(exp(-voltages(voltageIdx)/mVt)-1) + (1+mBf)/mBf * (exp((voltages(voltageIdx+1)-voltages(voltageIdx))/mVt) - 1));
    }
    return mCurrent;
}
void Npn2N2222::calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx){
    (*jacobian)(voltageIdx,voltageIdx) = mIs/mVt*(-1/mBf * exp((voltages(voltageIdx+1)-voltages(voltageIdx))/mVt) - 1/mBr * exp(-voltages(voltageIdx)/mVt));
    (*jacobian)(voltageIdx,voltageIdx+1) = mIs/mVt*(1/mBf * exp((voltages(voltageIdx+1)-voltages(voltageIdx))/mVt));
    (*jacobian)(voltageIdx+1,voltageIdx) =mIs/mVt*(-exp(-voltages(voltageIdx)/mVt) + (1 + mBf)/mBf * exp((voltages(voltageIdx+1)-voltages(voltageIdx))/mVt));
    (*jacobian)(voltageIdx+1,voltageIdx+1) = mIs/mVt*(-(1 + mBf)/mBf * exp((voltages(voltageIdx+1)-voltages(voltageIdx))/mVt));
}
