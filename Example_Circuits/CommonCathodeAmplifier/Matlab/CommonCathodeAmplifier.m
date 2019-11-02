clear all

%% Input parameters
ndk = '../Spice/CommonCathodeAmplifier.json';
[input,fs] = audioread('../../InputSounds/Sine_440_Amp_1.wav');

%% BJT Parameters:
%{
ep = 1.314;
gm = 1.26;
G = 2.242e-3;
Gg = 6.177e-4;
C = 3.40;
Cg = 9.901;
my = 103.2;
Ig0 = 8.025e-8;
%}
%{
ep = 1.358;
gm = 1.28;
G = 2.173e-3;
Gg = 5.911e-4;
C = 3.19;
Cg = 11.76;
my = 100.2;
Ig0 = 4.527e-8;
%}
ep = 1.156;
gm = 1.349;
G = 1.371e-3;
Gg = 3.263e-4;
C = 4.56;
Cg = 11.99;
my = 86.9;
Ig0 = 3.917e-8;
%% Newton-Raphson
nlFunction =@(v) [ 
    -Gg*(1/Cg*log(1+exp(Cg*(v(2)-v(1)))))^ep-Ig0;
    -G*(1/C*log(1+exp(C*(1/my*v(2)+v(2)-v(1)))))^gm
    ];
Jf = @(v) [
    Gg*ep*exp(Cg*(v(2)-v(1)))*(1/Cg*log(exp(Cg*(v(2)-v(1)))+1))^(ep-1)/(exp(Cg*(v(2)-v(1)))+1), ...
    -Gg*ep*exp(Cg*(v(2)-v(1)))*(1/Cg*log(exp(Cg*(v(2)-v(1)))+1))^(ep-1)/(exp(Cg*(v(2)-v(1)))+1);
    % Second row
     G*gm*exp(C*(v(2)/my+v(2)-v(1)))*(1/C*log(1+exp(C*(1/my*v(2)+v(2)-v(1)))))^(gm-1)*1/(exp(C*(v(2)/my+v(2)-v(1)))+1), ...
    -G*gm*(1/my+1)*exp(C*(v(2)/my+v(2)-v(1)))*(1/C*log(1+exp(C*(1/my*v(2)+v(2)-v(1)))))^(gm-1)*1/(exp(C*(v(2)/my+v(2)-v(1)))+1)
    ];

tol = 1e-5;
maxCount = 100;
initialGuess = [0;0];


%% Process
[ndkModel, nodeNames, nonlinearModels] = jsonToNdk(ndk, fs);
[input, output, nlCur, nlVolt, state] = ndkSolve(input, ndkModel, nlFunction, Jf, initialGuess, maxCount, tol);
ndkPlot(input, output, nlCur, nlVolt, state);