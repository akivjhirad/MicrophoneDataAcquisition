% NAME: Akiv Jhirad
% STUDENT NUMBER: 1207458
% MACID: jhiradas

%%%%%%%%%%%COE2DP4 FINAL PROJECT%%%%%%%%%%%%%%%%%%%%%%


s = serial('COM8');     %choose COM port
set(s, 'Baudrate', 9600);   %sets the Baudrate to the same thing as the Esduino
s.Terminator = 'CR';      %define terminator sent by Esduino
fopen(s);   %open the port

i = 0;
lHandle = line(nan, nan);

while (1)    
    i = i+1;        %increment x-axis
    X = get(lHandle, 'XData');  %define x-axis as type XData
    Y = get(lHandle, 'YData');  %define y-axis as type YData
    
    X = [X i];      % store X axis value
    micOutput = fscanf(s);  % read serial data as char
    micOutput = str2num(micOutput); %convert char to int for plot
    micOutput = (micOutput / 4095) * 5; %normalize data to 0 - 5 volts
    Y = [Y micOutput];  % store data in y-axis
    
    set(lHandle, 'XData', X, 'YData', Y);   %create plot
    drawnow;    %show plot

end

fclose(s);  %close the port
delete(s);  
clear s;