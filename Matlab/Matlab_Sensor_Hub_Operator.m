%{
    [Matlab_Sensor_Hub_Operator.m]
    [Matlab Operator script that communicates with the Sensor Hub],

    Author:               Lars Mueller
    LastChangedRevision:  1.0
    LastChangedDate:      21.09.19

    Change Record:
    Version | Date      | Author         | Section affected
    -------------------------------------------------------
    1.0 <- First Release Version 
    1.0     | 21.09.19  | Lars Mueller   | Clean up for first version
    0.4     | 02.09.19  | Lars Mueller   | Add dataSize variable to stop
                                           control readout time                                     
    0.3     | 26.08.19  | Lars Mueller   | Reduce computing time of readOut while loop 
    0.2     | 14.06.19  | Lars Mueller   | Added preliminary xml code
    0.1     | 02.06.19  | Lars Mueller   | Added additional serialfunctions
  
    0.0 <- Initial Development Version 
%}

clear; clc;

%Run Options
Options_UseXMLSensorFile   = 0;
Options_UseFinalBeepSound  = 1;

% Time for Measurement t_readout = t_cycle * dataSize (t_cycle ca. 5ms)

% -----------------------NOTIFICATION:----------------------------------------
% Currently the dataSize has to be changed manually in the Stm32 SensorHub
% code when editing this number!
dataSize                   = 100000;
% -----------------------NOTIFICATION:----------------------------------------

%Setting up the serial connection
avaible_comports= seriallist();

openport = instrfind('Port','COM7');
if (~isempty(openport))
    fclose(openport);
    delete(openport);
end

%Create new serial object for port 7
serial_interface = serial("COM7");

%Use the same settings as in the Nucleo board
set(serial_interface,'BaudRate',256000,'DataBits',8,'StopBits',1);
set(serial_interface,'Parity','none','Timeout', 10);
set(serial_interface,'InputBufferSize',4000);

%Open the port
fopen(serial_interface);
while (serial_interface.Status ~= "open")
end

CSV_path = 'D:\Studium\Masterthesis\Sensor_Data_Files\SensorHub\CSV';
CSV_filename = 'SensorHub_V1Final_HeatUp_TSEP2_7A_Sens075_3.csv';
CSV_file = [CSV_path '\' CSV_filename];

MAT_path = 'D:\Studium\Masterthesis\Sensor_Data_Files\SensorHub\MAT';
MAT_filename = 'SensorHub_V1Final_HeatUp_TSEP2_7A_Sens075_3.mat';
MAT_file = [MAT_path '\' MAT_filename];

%Initialize the data structures to improve performance%
cnt = 1;
rawData = zeros(dataSize,20);
writeData = zeros(dataSize,8);

%{
    %Command Description:

    %COMMAND:001 Start data aquisition
    %COMMAND:002 Send number of data points
    
%}

%Give the number of data points to the nucleo
%fprintf(serial_interface, "COMMAND:002,010000");

%Start the data aquistion%
fprintf(serial_interface, "COMMAND:001,000000");

%Read raw data over the serial interface until dataSize is reached%
while(1)
     
    rawData(cnt,:) = fread(serial_interface,[20,1]);
    cnt = cnt + 1;
    
    if(cnt > dataSize)
        break
    end
       
end

%Convert the raw data to corresponding temperature in °C %
for i = 1:dataSize

    sensValue = rawData(i,:);
    
    temp_sens1 = (sensValue(6) * 16.0) + (sensValue(7) / 16.0); 
    time_sens1 = [sensValue(1) sensValue(2) sensValue(3) (  bitshift(sensValue(4),0) +  bitshift(sensValue(5),8) )]
    
    %SENSOR 2 currently not active!
    %temp_sens2 = bitshift(sensValue(8),24) + bitshift(sensValue(9),16) + bitshift(sensValue(10),8)/1000.0 + bitshift(sensValue(11),0)/1000;
    %time_sens2 = [sensValue(12) sensValue(13) sensValue(14) ( bitshift(sensValue(15),0) +  bitshift(sensValue(16),8) ) ];

    adc_rth = bitshift(sensValue(12),24) + bitshift(sensValue(13),16) + bitshift(sensValue(14),8) + bitshift(sensValue(15),0); 
    r_th = 10000.0 * ( ( 1 - adc_rth / 4096.0) / (adc_rth / 4096.0) );
    temp_sens3 =  ( 1 / ( (1/298.15) - (log(10000.0) - log(r_th)) / 3450.0 ) ) - 273.15 

    adc_fw = bitshift(sensValue(16),24) + bitshift(sensValue(17),16) + bitshift(sensValue(18),8) + bitshift(sensValue(19),0);
    temp_sens4 = (adc_fw / 4096.0) * 3.31;
   
    temp_sens5 = sensValue(20);
    
    writeData(i,:) = [time_sens1 temp_sens1 temp_sens3 temp_sens4 temp_sens5]; 
    sensValue = 0;
end

%Indication that the measurement is complete
if Options_UseFinalBeepSound
    res = 22050;
    len = 0.1 * res;
    hz = 320;
    sound( sin( hz*(2*pi*(0:len)/res) ), res);
end

%Save the data to the given location as mat and csv file
save(MAT_file, 'writeData');
dlmwrite(CSV_file,writeData ,'delimiter','\t');

%Close the serial port and delete the serial object
fclose(serial_interface);
delete(serial_interface);
                