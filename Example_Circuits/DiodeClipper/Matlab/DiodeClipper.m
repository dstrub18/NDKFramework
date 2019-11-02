clear

%% Input parameters
ndk = '../Spice/DiodeClipper.json';
[input,fs] = audioread('../../InputSounds/Sine_440_Amp_07.wav');

%% Diode Parameters:
Is = 2.52e-9;
Vt = 0.045;
%% Newton-Raphson
nlFunction = @(v) [-Is*(exp(v(1)/Vt)-1); -Is*(exp(v(2)/Vt)-1)];
Jf = @(v) [-Is/Vt*exp(v(1)/Vt) 0;0 -Is/Vt*exp(v(2)/Vt)
                ];

tol = 0.1;
maxCount = 100;
initialGuess = [0;0];


%% Process
[ndkModel, nodeNames, nonlinearModels] = jsonToNdk(ndk, fs);
[input, output, nlCur, nlVolt, state] = ndkSolve(input, ndkModel, nlFunction, Jf, initialGuess, maxCount, tol);
ndkPlot(input, output, nlCur, nlVolt, state);