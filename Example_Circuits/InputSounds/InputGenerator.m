%% Global variables
clear variables
fs = 44100;
silencePad = zeros(2*fs,1); % 2 seconds of silence
N = 2*fs; % Signal length
%% Noise generation
noise = (rand(N,1)-0.5)*0.5;
audiowrite('noise_Amp_025.wav',[silencePad;noise],fs,'BitsPerSample',24);

%% Stationary Sine generation
amplitudes = [0.2,0.5,0.7,1];
f = 440;
sine= a*(sin(2*pi*f/fs*(1:N)));
for n = 1: max(size(amplitudes))
     a = (amplitudes(n));
     sine= a*(sin(2*pi*f/fs*(1:N)));
     audiowrite(strcat('Sine_440_Amp_',num2str(amplitudes(n)),'.wav'),...
         [silencePad;sine.'],fs, 'BitsPerSample',24);
end

%% Sine Sweep generation

t = 0:1/fs:10;
phi = 90; %Phase shift in degrees. Chirp makes a cosine by default.
sweep = 0.1*(chirp(t,20,10,20000,'logarithmic',phi));
plot(sweep);
audiowrite('Sweep_20_20000_10s_Amp_01.wav',[silencePad;sweep.'],fs,'BitsPerSample',24);

%% Op-amp offset Sine
clear variables
fs = 44100;
N = 2*fs; % Signal length
silencePad = zeros(2*fs,1); % 2 seconds of silence
f = 440;
a = 0.2;
sine= 0.4+a*(sin(2*pi*f/fs*(1:N)));
silencePad(:) = silencePad(:)+0.4;
plot([silencePad;sine.'])

audiowrite('Sine_440_Amp_02_offset_04.wav',[silencePad;sine.'],fs,'BitsPerSample',24)