/*
  ==============================================================================

    ConstantMatrices.h
    Created: 19 Mar 2019 4:32:40pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#ifndef CONSTANTMATRICES_H
#define CONSTANTMATRICES_H

#include <Eigen>
#include "CircuitParser.h"

class ConstantMatrices
{
private:
    int CAPACITOR = 1;
    int INDUCTOR = -1;
    int mOpenloadAmp = 100000;
    unsigned int mNumNonlinears;
    unsigned int mNumPotentiometers;
    Eigen::MatrixXd mA0, mB0, mC0, mD0, mE0, mF0, mG0, mH0, mK0;
    Eigen::MatrixXd mUo, mUx, mUn, mUu, mUxT, mUnT, mUuT, mS0inv, mGx, mQ, mZ;
    
public:
    
    ConstantMatrices(CircuitParser circuit, int fs);
    
    void calculateGx(Eigen::MatrixXd Xlc, Eigen::MatrixXd Z, int numReactives, int fs);
    void calculateS0inv(Eigen::MatrixXd Nu, Eigen::MatrixXd Nr, Eigen::MatrixXd Gr, Eigen::MatrixXd Nx, Eigen::MatrixXd NopaI, Eigen::MatrixXd NopaO, Eigen::MatrixXd ampFactor, int numNodes, int numSources);
    void calculateConstantMatrices(Eigen::MatrixXd Nv, Eigen::MatrixXd Nx, Eigen::MatrixXd No,
                                       Eigen::MatrixXd Nn, int numReactives, int numSources, int numOutputs,
                                       int numNodes);
    
    Eigen::MatrixXd getA0(){return mA0;};
    Eigen::MatrixXd getB0(){return mB0;};
    Eigen::MatrixXd getC0(){return mC0;};
    Eigen::MatrixXd getD0(){return mD0;};
    Eigen::MatrixXd getE0(){return mE0;};
    Eigen::MatrixXd getF0(){return mF0;};
    Eigen::MatrixXd getG0(){return mG0;};
    Eigen::MatrixXd getH0(){return mH0;};
    Eigen::MatrixXd getK0(){return mK0;};
    
    Eigen::MatrixXd getUo(){return mUo;};
    Eigen::MatrixXd getUx(){return mUx;};
    Eigen::MatrixXd getUn(){return mUn;};
    Eigen::MatrixXd getUu(){return mUu;};
    Eigen::MatrixXd getUxT(){return mUxT;};
    Eigen::MatrixXd getUnT(){return mUnT;};
    Eigen::MatrixXd getUuT(){return mUuT;};
    Eigen::MatrixXd getS0inv(){return mS0inv;};
    Eigen::MatrixXd getGx(){return mGx;};
    Eigen::MatrixXd getQ(){return mQ;};
    Eigen::MatrixXd getZ(){return mZ;};
    
    int getNumNonlinears(){return mNumNonlinears;}
    int getNumPotentiometers(){return mNumPotentiometers;};
};

#endif
