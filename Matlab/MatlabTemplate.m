clear

%% Input parameters
file = "../DiodeClipper/DiodeClipper.json";
fs = 44100;
N = 40000;
a = 1; 
f = 6000; 
vs = a*(sin(2*pi*f/fs*(1:N)));

%% BJT Parameters:
Is = 1.16e-14;
Bf = 200;
Br = 3;
Vt = 0.02528;

%% Newton-Raphson
nlFunction = @(v) Is*[   
        1/Bf * (exp((v(2)-v(1))/Vt) - 1) + 1/Br * (exp(-v(1)/Vt) - 1);
        -(-(exp(-v(1)/Vt)-1) + (1+Bf)/Bf * (exp((v(2)-v(1))/Vt) - 1));
        1/Bf * (exp((v(4)-v(3))/Vt) - 1) + 1/Br * (exp(-v(3)/Vt) - 1);
        -(-(exp(-v(3)/Vt)-1) + (1+Bf)/Bf * (exp((v(4)-v(3))/Vt) - 1));
    ];
Jf = @(v) Is/Vt*[ 
        -1/Bf * exp((v(2)-v(1))/Vt) - 1/Br * exp(-v(1)/Vt),     1/Bf * exp((v(2)-v(1))/Vt),             0,  0;
        -exp(-v(1)/Vt) + (1 + Bf)/Bf * exp((v(2)-v(1))/Vt),     -(1 + Bf)/Bf * exp((v(2)-v(1))/Vt),     0,  0;
        0,  0,      -1/Bf * exp((v(4)-v(3))/Vt) - 1/Br * exp(-v(3)/Vt),        1/Bf * exp((v(4)-v(3))/Vt);
        0,  0,      -exp(-v(3)/Vt) + (1 + Bf)/Bf * exp((v(4)-v(3))/Vt),        -(1 + Bf)/Bf * exp((v(4)-v(3))/Vt)
    ];

tol = 0.1;
maxCount = 100;
initialGuess = [0;0;0;0];


%% Process
[ndkModel, nodeNames, nonlinearModels] = jsonToNdk(file, fs);
[input, output, nlCur, nlVolt, state] = ndkSolve(vs, ndkModel, nlFunction, Jf, initialGuess, maxCount, tol, 1);
ndkPlot(input, output, nlCur, nlVolt, state);