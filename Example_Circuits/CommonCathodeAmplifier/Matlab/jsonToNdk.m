function [ndkModel, nodeNames, nonlinModels] = jsonToNdk(filename, samplingRate)

T = 1/samplingRate;
j = jsondecode(fileread(filename));

Nr = j.Nr;
Nx = j.Nx;
Nv = j.Nv;
No = j.No;
Nn = j.Nn;
Nu = j.Nu;
NopaI = j.Nopai;
NopaO = j.Nopao;
Gr = j.Gr;
Xlc = j.Xlc;
Xopa = 100000;
NopaI = NopaI.*Xopa;
NuO = [Nu; NopaO];
NuI = [Nu; NopaO+NopaI];
Z = j.Z;
Rv = j.Rv0;
ndkModel.sourceVoltages = j.sourceVoltages;

ndkModel.numNodes = j.numNodes;
ndkModel.numResistors = j.numResistors;
ndkModel.numReactives = j.numReactives;
ndkModel.numInputs = j.numInputs;
ndkModel.numSources = j.numSources;
ndkModel.numPotentiometers = j.numPotentiometers;
ndkModel.numOutputs = j.numOutputs;
ndkModel.numNonlinears = j.numNonlinears;

nodeNames = j.nodeNames;
nonlinModels = j.nonlinearModels;

% Reformatting empty matrices from json as 
if isempty(Nr); Nr = zeros(ndkModel.numResistors, ndkModel.numNodes); end
if isempty(Nx); Nx = zeros(ndkModel.numReactives, ndkModel.numNodes); end
if isempty(Nv); Nv = zeros(ndkModel.numPotentiometers, ndkModel.numNodes); end
if isempty(No); No = zeros(ndkModel.numOutputs, ndkModel.numNodes); end
if isempty(Nn); Nn = zeros(ndkModel.numNonlinears, ndkModel.numNodes); end
if isempty(Nu); Nu = zeros(ndkModel.numSources, ndkModel.numNodes); end
if isempty(Gr); Gr = zeros(ndkModel.numResistors, ndkModel.numResistors); end
if isempty(Z); Z = zeros(ndkModel.numReactives, ndkModel.numReactives); end
if isempty(Rv); Rv = zeros(ndkModel.numPotentiometers, ndkModel.numPotentiometers); end

Gx = zeros(ndkModel.numReactives);
for i = 1:ndkModel.numReactives
    if Z(i, i) == 1
        Gx(i, i) = 2 * Xlc(i, i) / T;
    elseif Z(i, i) == -1
        Gx(i, i) = T / (2 * Xlc(i,i));
    end
end

ndkModel.Theta = Nr'*Gr*Nr + Nx'*Gx*Nx + Nv'*inv(Rv)*Nv;
ndkModel.S = [ndkModel.Theta,  NuO';
         NuI,     zeros(ndkModel.numSources)];
ndkModel.SInv = inv(ndkModel.S);

ndkModel.NxPadded = [Nx zeros(ndkModel.numReactives,ndkModel.numSources)];
ndkModel.NnPadded = [Nn zeros(ndkModel.numNonlinears,ndkModel.numSources)];
ndkModel.NoPadded = [No zeros(ndkModel.numOutputs,ndkModel.numSources)];
ndkModel.eyePadded = [zeros(ndkModel.numSources,ndkModel.numNodes) eye(ndkModel.numSources)];

ndkModel.A = 2*Z*Gx * ndkModel.NxPadded * ndkModel.SInv * ndkModel.NxPadded' - Z;
ndkModel.B = 2*Z*Gx * ndkModel.NxPadded * ndkModel.SInv * ndkModel.eyePadded';
ndkModel.C = 2*Z*Gx * ndkModel.NxPadded * ndkModel.SInv * ndkModel.NnPadded';
ndkModel.D = ndkModel.NoPadded * ndkModel.SInv * ndkModel.NxPadded';
ndkModel.E = ndkModel.NoPadded * ndkModel.SInv * ndkModel.eyePadded';
ndkModel.F = ndkModel.NoPadded * ndkModel.SInv * ndkModel.NnPadded';
ndkModel.G = ndkModel.NnPadded * ndkModel.SInv * ndkModel.NxPadded';
ndkModel.H = ndkModel.NnPadded * ndkModel.SInv * ndkModel.eyePadded';
ndkModel.K = ndkModel.NnPadded * ndkModel.SInv * ndkModel.NnPadded';