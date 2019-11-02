clear all

%% Input parameters
ndk = '../Spice/CommonEmitterAmplifier.json';
[input,fs] = audioread('../../InputSounds/Sine_50_Amp_1.wav');
%N = 40000;
%a = 0.2; f = 440; vs = a*(sin(2*pi*f/fs*(1:N)));


%% BJT Parameters:
Is = 1.16e-14;
Bf = 200;
Br = 3;
Vt = 0.02528;

%% Newton-Raphson
nlFunction = @(v) Is*[   
        1/Bf * (exp((v(2)-v(1))/Vt) - 1) + 1/Br * (exp(-v(1)/Vt) - 1);
        -(-(exp(-v(1)/Vt)-1) + (1+Bf)/Bf * (exp((v(2)-v(1))/Vt) - 1));];
Jf = @(v) Is/Vt*[ 
        -1/Bf * exp((v(2)-v(1))/Vt) - 1/Br * exp(-v(1)/Vt),     1/Bf * exp((v(2)-v(1))/Vt);
        -exp(-v(1)/Vt) + (1 + Bf)/Bf * exp((v(2)-v(1))/Vt),     -(1 + Bf)/Bf * exp((v(2)-v(1))/Vt)];

tol = 1e-5;
maxCount = 100;
initialGuess = [0;0];


%% Process
[ndkModel, nodeNames, nonlinearModels] = jsonToNdk(ndk, fs);
[input, output, nlCur, nlVolt, state] = ndkSolve(input, ndkModel, nlFunction, Jf, initialGuess, maxCount, tol);
ndkPlot(input, output, nlCur, nlVolt, state);