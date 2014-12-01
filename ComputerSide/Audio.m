%% Serial Port Configuration and Initialization

%% Initialize program and USB port
if(exist('M2usb', 'var'))
  fclose(M2usb);
else
 delete(instrfindall);
end

close all;
clear all;

%----> for ***MAC***
%{a
M2usb = serial('/dev/tty.usbmodem411','Baudrate',9600);
fopen(M2usb);       % Open up the port to the M2 microcontroller.
flushinput(M2usb);  % Remove anything extranneous that may be in the buffer.
%}

% M2 Serial port interaction examples
% data = fgets(M2usb);                      % Read data from M2
% fprintf(data);                            % Display read data

% packet = [R3_ADDRESS, COMMAND_BIT, COMMAND_BIT];
% flushinput(M2usb);                % Flush any strange things in the buffer
% fwrite(M2usb,packet);             % Write packet to m2
% pause(1.0);                       % Make a pause because of serial init
% data = fgetl(M2usb);              % Get response from m2
        
        
%% Initialization
%  serial
%  s = serial('/dev/tty.usbmodem1421');%matlab serial port
%  fopen(s);


SamplesPerFrame = 300;
SampleRate = 1000;

FReader = dsp.AudioRecorder('SampleRate', SampleRate, 'SamplesPerFrame',SamplesPerFrame,...
    'OutputDataType','double');

hts1 = dsp.TimeScope('SampleRate', FReader.SampleRate,'TimeSpan', 30,'YLimits',[-2,2]);

% Meaner = dsp.Mean();


%% Stream
tic;
while ~isDone(FReader)
    % Read frame from file
    % audioIn = step(FReader);
     
    % Trivial algorithm, scale input audio
    % audioOut = 0.8*audioIn;
    
    for ii = 1:SampleRate
        % Analog to Digital conversion
        x = step(FReader);      % 300 by 2 matrix. Left and right mic.
        
        % Pass x value to the drawing (Time Scope)
        step(hts1, x);
        Sound_mean = mean(x, 2);
        Sound_mean = abs(mean(Sound_mean) * 100000);
        disp(Sound_mean);
        if(Sound_mean > 70)
        %{a
            packet = 'e';
            disp('Threshold 1 Surpassed');
            fwrite(M2usb,packet);             % Write packet to m2
            wrote = TRUE;
        elseif(Sound_mean > 30)
            packet = 'g';
            disp('Threshold 2 Surpassed');
            fwrite(M2usb,packet);             % Write packet to m2
            wrote = TRUE;
        %}
        end
        if wrote
           wrote = FALSE;
           pause(0.1);
        end
    end
    release(hts1); 
    
end