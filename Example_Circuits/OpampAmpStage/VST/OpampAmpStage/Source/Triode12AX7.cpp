/*
  ==============================================================================

    Triode12AX7.cpp
    Created: 16 May 2019 3:31:40pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "Triode12AX7.h"

Triode12AX7::Triode12AX7(){
    mEp = 1.358;
    mGam = 1.28;
    mG = 2.173e-3;
    mGg = 5.911e-4;
    mC = 3.19;
    mCg = 11.76;
    mMy = 100.2;
    mIg0 = 4.527e-8;
    
    mNumberOfFunctions = 2;
    mCurrent = 0;
}

float Triode12AX7::calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx){
    if(functionIdx == 0){
        mCurrent = -mGg*std::pow((1/mCg*log(1+exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx))))),mEp)-mIg0;
    }else if (functionIdx == 1){
        mCurrent = -mG*std::pow((1/mC*log(1+exp(mC*(1/mMy*voltages(voltageIdx+1)+voltages(voltageIdx+1)-voltages(voltageIdx))))),mGam);
    }
    return mCurrent;
}
void Triode12AX7::calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx){
    (*jacobian)(voltageIdx,voltageIdx) = mGg*mEp*exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx)))*std::pow((1/mCg*log(exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx)))+1)),mEp-1)/(exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx)))+1);
    (*jacobian)(voltageIdx,voltageIdx+1) = -mGg*mEp*exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx)))*std::pow((1/mCg*log(exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx)))+1)),mEp-1)/(exp(mCg*(voltages(voltageIdx+1)-voltages(voltageIdx)))+1);
    (*jacobian)(voltageIdx+1,voltageIdx) = mG*mGam*(1/mMy+1)*exp(mC*(voltages(voltageIdx+1)/mMy+voltages(voltageIdx+1)-voltages(voltageIdx)))*std::pow((1/mC*log(1+exp(mC*(1/mMy*voltages(voltageIdx+1)+voltages(voltageIdx+1)-voltages(voltageIdx))))),mGam-1)*1/(exp(mC*(voltages(voltageIdx+1)/mMy+voltages(voltageIdx+1)-voltages(voltageIdx)))+1);
    (*jacobian)(voltageIdx+1,voltageIdx+1) = -mG*mGam*exp(mC*(voltages(voltageIdx+1)/mMy+voltages(voltageIdx+1)-voltages(voltageIdx)))*std::pow((1/mC*log(1+exp(mC*(1/mMy*voltages(voltageIdx+1)+voltages(voltageIdx+1)-voltages(voltageIdx))))),mGam-1)*1/(exp(mC*(voltages(voltageIdx+1)/mMy+voltages(voltageIdx+1)-voltages(voltageIdx)))+1);
}

