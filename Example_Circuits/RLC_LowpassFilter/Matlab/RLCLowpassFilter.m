clear variables
[input,fs] = audioread('../../InputSounds/noise_Amp_025.wav');
ndk = jsonToNdk('../Spice/RLCLowpassFilter.json',fs);
[input,output,nonlinearCurrents,nonlinearVoltages,states] = ndkSolve(input,ndk,[],[],0,10,1);
ndkPlot(input, output, nonlinearCurrents, nonlinearVoltages, states);

