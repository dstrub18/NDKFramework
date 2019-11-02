clear variables
[audioInput,fs] = audioread('../../InputSounds/noise_Amp_025.wav');
ndk = jsonToNdk('../Spice/RLCHighpassFilter.json',fs);
[input,output,nonlinearCurrents,nonlinearVoltages,states] = ndkSolve(audioInput,ndk,[],[],0,10,1);
ndkPlot(input, output, nonlinearCurrents, nonlinearVoltages, states);