/*
  ==============================================================================

    ConstantMatrices.cpp
    Created: 19 Mar 2019 4:32:40pm
    Author:  Rasmus Kürstein

  ==============================================================================
*/

#include <iostream>
#include "ConstantMatrices.h"

ConstantMatrices::ConstantMatrices(CircuitParser circuit, int fs){
    int numReactives = circuit.getNumReactives();
    int numSources = circuit.getNumSources();
    int numOutputs = circuit.getNumOutputs();
    int numNodes = circuit.getNumNodes();
    mNumNonlinears = circuit.getNumNonlinears();
    mNumPotentiometers = circuit.getNumPotentiometers();
    int numOpamps = circuit.getNumOpamps();

    Eigen::MatrixXd Nr = circuit.getNr();
    Eigen::MatrixXd Nx = circuit.getNx();
    Eigen::MatrixXd Nv = circuit.getNv();
    Eigen::MatrixXd Nu = circuit.getNu();
    Eigen::MatrixXd Nn = circuit.getNn();
    Eigen::MatrixXd No = circuit.getNo();
    Eigen::MatrixXd Gr = circuit.getGr();
    Eigen::MatrixXd Xlc = circuit.getXlc();
    Eigen::MatrixXd Nopai = circuit.getNopai();
    Eigen::MatrixXd Nopao = circuit.getNopao();
    Eigen::MatrixXd ampFactor = Eigen::MatrixXd::Identity(numOpamps,numOpamps)*mOpenloadAmp;
    mZ = circuit.getZ();

    calculateGx(Xlc, mZ, numReactives, fs);

    calculateS0inv(Nu, Nr, Gr, Nx, Nopai, Nopao, ampFactor, numNodes, numSources);
    calculateConstantMatrices(Nv, Nx, No, Nn, numReactives, numSources, numOutputs, numNodes);

}

void ConstantMatrices::calculateGx(Eigen::MatrixXd Xlc, Eigen::MatrixXd Z, int numReactives, int fs){
    mGx = Eigen::MatrixXd::Zero(numReactives,numReactives);
    float T = 1/float(fs);
    for(int i=0; i<=numReactives-1; i++){
        if(Z(i,i)==CAPACITOR){
            float C = Xlc(i,i);
            mGx(i,i) = 2*C/T;
        }else if(Z(i,i)==INDUCTOR){
            float L = Xlc(i,i);
            mGx(i,i) = T/(2*L);
        }
    }
    
}

void ConstantMatrices::calculateS0inv(Eigen::MatrixXd Nu, Eigen::MatrixXd Nr, Eigen::MatrixXd Gr, Eigen::MatrixXd Nx, Eigen::MatrixXd Nopai, Eigen::MatrixXd Nopao, Eigen::MatrixXd ampFactor, int numNodes, int numSources){
    Eigen::MatrixXd Theta = Nr.transpose()*Gr*Nr+Nx.transpose()*mGx*Nx;
    Eigen::MatrixXd S0 = Eigen::MatrixXd::Zero(numNodes+numSources, numNodes+numSources);
    S0.topRows(numNodes).leftCols(numNodes) = Theta;
    Eigen::MatrixXd NuI(Nu.rows()+Nopao.rows(), Nu.cols());
    NuI << Nu, Nopao+ampFactor*Nopai;
    
    S0.bottomRows(numSources).leftCols(numNodes) = NuI ;
    Eigen::MatrixXd NuO(Nu.rows()+Nopao.rows(), Nu.cols());
    NuO << Nu, Nopao;
    
    S0.topRows(numNodes).rightCols(numSources) = NuO.transpose();
    S0.bottomRows(numSources).rightCols(numSources) = Eigen::MatrixXd::Zero(numSources, numSources);
    
    mS0inv = S0.inverse();
}

void ConstantMatrices::calculateConstantMatrices(Eigen::MatrixXd Nv, Eigen::MatrixXd Nx, Eigen::MatrixXd No,
                                                 Eigen::MatrixXd Nn, int numReactives, int numSources, int numOutputs,
                                                 int numNodes) {

    Eigen::MatrixXd NvPadded = Eigen::MatrixXd::Zero(mNumPotentiometers,numNodes+numSources);
    if(mNumPotentiometers!=0){
        NvPadded.leftCols(numNodes) = Nv;
    }
    Eigen::MatrixXd  NxPadded = Eigen::MatrixXd::Zero(numReactives,numNodes+numSources);
    if(numReactives!=0){
        NxPadded.leftCols(numNodes) = Nx;
        }
    Eigen::MatrixXd NoPadded = Eigen::MatrixXd::Zero(numOutputs,numNodes+numSources);
    if(numOutputs!=0){
        NoPadded.leftCols(numNodes) = No;
    }

    Eigen::MatrixXd NnPadded = Eigen::MatrixXd::Zero(mNumNonlinears,numNodes+numSources);
    if(mNumNonlinears!=0){
        NnPadded.leftCols(numNodes) = Nn;
    }
    Eigen::MatrixXd eyePadded = Eigen::MatrixXd::Zero(numSources,numNodes+numSources);
    eyePadded.rightCols(numSources) = Eigen::MatrixXd::Identity(numSources,numSources);
    
    
    mA0 = 2*mZ*mGx*NxPadded*mS0inv*NxPadded.transpose()-mZ;
    mB0 = 2*mZ*mGx*NxPadded*mS0inv*eyePadded.transpose();
    mC0 = 2*mZ*mGx*NxPadded*mS0inv*NnPadded.transpose();
    mD0 = NoPadded*mS0inv*NxPadded.transpose();
    mE0 = NoPadded*mS0inv*eyePadded.transpose();
    mF0 = NoPadded*mS0inv*NnPadded.transpose();
    mG0 = NnPadded*mS0inv*NxPadded.transpose();
    mH0 = NnPadded*mS0inv*eyePadded.transpose();
    mK0 = NnPadded*mS0inv*NnPadded.transpose();
    mUo = NoPadded*mS0inv*NvPadded.transpose();
    mUx = NxPadded*mS0inv*NvPadded.transpose();
    mUn = NnPadded*mS0inv*NvPadded.transpose();
    mUu = eyePadded*mS0inv*NvPadded.transpose();
    mUxT = mUx.transpose();
    mUnT = mUn.transpose();
    mUuT = mUu.transpose();
    mQ = NvPadded*mS0inv*NvPadded.transpose();
}
