%% Initialization
clear all

[audioInput, fs] = audioread('../../InputSounds/Sine_440_Amp_02_offset_04.wav');
[ndkModel, nodeNames, nonlinModels] = jsonToNdk('../Spice/OpampAmpStage.json', fs);

N = max(size(audioInput));
T = 1/fs;


a = 0.59; f = 100; vs = audioInput;
input = ones(ndkModel.numSources, N) .* ndkModel.sourceVoltages;
input(1, :) = vs;

%% Processing
[input, output, nonlinearCurrents, nonlinearVoltages, states] = ndkSolve( ...
          audioInput, ndkModel, [], [], [], [], []);
      
      
output = output - 1.9;
      
ndkPlot(input, output, nonlinearCurrents, nonlinearVoltages, states);
