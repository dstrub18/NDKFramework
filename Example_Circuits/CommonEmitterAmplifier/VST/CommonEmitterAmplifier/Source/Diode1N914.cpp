/*
  ==============================================================================

    Diode.cpp
    Created: 6 Apr 2019 5:05:32pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include "Diode1N914.h"
#include <iostream>

Diode1N914::Diode1N914(){
    mVt = 0.045;
    mIs = 2.52e-9;
    mNumberOfFunctions = 1;
}

float Diode1N914::calculateCurrent(Eigen::MatrixXd voltages,int voltageIdx, int functionIdx){
    return -mIs*(exp(voltages(voltageIdx)/mVt)-1);
}
void Diode1N914::calculateJacobian(Eigen::MatrixXd *jacobian, Eigen::MatrixXd voltages,int voltageIdx){
    (*jacobian)(voltageIdx,voltageIdx) = -mIs/mVt*exp(voltages(voltageIdx)/mVt);
}
