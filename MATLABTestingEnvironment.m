%% System Input
% The input to the system can be a pre-recorded WAV on file or can be read
% from the computer sound card. Reading from the sound card allows the use
% of USB interfaces with jack or XLR cable inputs.

%% External Input. The input to the system is recorded live by the user.
% The wavrecord function will record using the computers internal
% microphone, a microphone connected to the computers microphone jack or
% any interface connected to the computers sound card.
%
% WARNING:
% To use this functionality please comment out the below code which uses a
% wav on the computers file system.

% fs = 44100; % Sampling Frequency.
% Seconds = 12; % Length of Recording in Seconds.
% n = fs*Seconds; % n will determine the size of the WAV recorded.
% In = wavrecord(n,fs); % The WAV file is recorded.

%% Input using WAV from file system. Input to the system is on file.
% The input from the system is a wav that is already on the computer file
% system. The sampling frequency of the wav is obtained from the wav by the
% wavread function. This allows the user to import their own existing wav
% file or to download a wav file and use this as the system input.
%
% WARNING:
% To use this functionality please comment out the above code which uses a
% an external real time input to record the wav.

[In fs] = wavread('C:\Users\Philip\Desktop\FYP\Wavs\GchDry.wav');

%% Processing. Input signal is processed.
%  Copy the code of your desired effect from the bottom of this m-file to
%  implement this effect.



%% Output from the system.
% Users can listen to the input and output files using the wavplay function
% in order to compare and contrast.
wavplay(In,fs);
wavplay(Out,fs);

figure;
subplot(2,1,1);
plot(In,'b');
xlabel('Samples')
ylabel('Amplitude')
title('Input Signal')
axis tight;
subplot(2,1,2);
plot(Out,'r');
xlabel('Samples')
ylabel('Amplitude')
title('Output Signal')
axis tight;

%% Delay code. Uses circular buffer to create echo/delay effect.
seconds = 1; % delay time
delaytime = fs*seconds; % set delay time
i = 1; % i used to move through buffer index
n = 1; % n used to move through input and output
gain = 0.5; % repeat gain
for(i = 1:delaytime) % fill buffer
    buffer(i) = In(n); 
    Out1(n) = buffer(i);
    i = i+1;
    n = n+1;
end
i = 1;        
for(n = n:(length(In))) % buffer is full
    Out1(n) = (buffer(i)*gain) + In(n);  % add new input signal to old data
                                        % with reduced amplitude
    buffer(i) = Out1(n);
    j = i+1;
    i = mod(j,delaytime)+1; % carry out modulus operation
    n = n + 1;
end

%% Distortion effect. Clips signal and increases signal gain.
Out1 = In;
n = 1;
for(n = n:length(In));
    if(Out1(n) > 0.1) % signal is clipped if in goes above 0.1
        Out1(n) = 0.1;
    end
    if(Out1(n) < -0.1) % signal is clipped if it goes above 0.1
        Out1(n) = -0.1;
    end
end
Out = Out1*3; % increase ouput volume


%% High pass filter. Create filter and filter input signal
[b,a] = butter(2,0.08,'high'); % second order high pass butterworth filter
Out = filter(b,a,In); % filter input with b and a coefficients
Out = Out*10; % increase amplitude of filtered signal to make up for loss
freqz(b,a); % show filter frequency response

%% Tremolo/amplitude modulation using
% sine wave with amplitude in range 0 - 1.0
x = sin(2*pi*[0:9999]/10000); % generate sine wave
x1 = x + 1; % scale sine wave for range 0 - 1.0
x2 = x1 / 2;

i = 1;
n = 1;
for(n = n:length(In));
    Out(n) = In(n) * x2(i); % multiply input by sine wave
    n = n + 1;
    i = i + 1;
    if(i == 10000)
        i = 1;
    end
end

%% WahWah code. Taken from paper:
%http://www.cs.cf.ac.uk/Dave/CM0268/PDF/10_CM0268_Audio_FX.pdf
% lower the damping factor the smaller the pass band
damp = 0.07;
% min and max centre cutoff frequency of variable bandpass filter
minf=500;
maxf=3000;
% wah frequency, how many Hz per second are cycled through
Fw = 2500; 
% change in centre frequency per sample (Hz)
%delta=0.1;
delta = Fw/fs;
%0.1 => at 44100 samples per second should mean  4.41kHz Fc shift per sec
% create triangle wave of centre frequency values
Fc=minf:delta:maxf;
while(length(Fc) < length(In) )
    Fc= [ Fc (maxf:-delta:minf) ];
    Fc= [ Fc (minf:delta:maxf) ];
end
% trim tri wave to size of input
Fc = Fc(1:length(In));
% difference equation coefficients
F1 = 2*sin((pi*Fc(1))/fs);  % must be recalculated each time Fc changes
Q1 = 2*damp;                % this dictates size of the pass bands
yh=zeros(size(In));          % create emptly out vectors
yb=zeros(size(In));
yl=zeros(size(In));
% first sample, to avoid referencing of negative signals
yh(1) = In(1);
yb(1) = F1*yh(1);
yl(1) = F1*yb(1);
% apply difference equation to the sample
for n=2:length(In),
    yh(n) = In(n) - yl(n-1) - Q1*yb(n-1);
    yb(n) = F1*yh(n) + yb(n-1);
    yl(n) = F1*yb(n) + yl(n-1); 
    F1 = 2*sin((pi*Fc(n))/fs);
end
%normalise
maxyb = max(abs(yb));
Out = (yb/maxyb)*3;