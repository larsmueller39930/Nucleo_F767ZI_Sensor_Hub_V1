%{
    [Sensor_Data_Eval.m]
    [Matlab script that evaluate the sensor data and create plots],

    Author:               Lars Mueller
    LastChangedRevision:  0.5
    LastChangedDate:      21.09.19

    Change Record:
    Version | Date      | Author         | Section affected
    -------------------------------------------------------
    1.0 <- First Release Version 
    1.0     | 21.09.19  | Lars Mueller   | Clean up for first version
    0.5     | 21.09.19  | Lars Mueller    | Added Save Plot Functionality
    0.4     | 12.09.19  | Lars Mueller    | Added TSEP Calibration
    0.3     | 06.09.19  | Lars Mueller    | Added averaging and filter
    0.2     | 06.09.19  | Lars Mueller    | Added evaluation for multiple
                                           meausruments
    0.1     | 01.09.19  | Lars Mueller    | Added option parameter 
    0.0 <- Initial Development Version 
%}

clear; clc;

%Defines
Sensor_Data_Eval_Defines;

%Plot Options
Options_Plot_Thermocouple        = TRUE;
Options_Plot_Thermistor          = TRUE;
Options_Plot_TSEP                = TRUE;
Options_Plot_Type                = PLOT_OPTION_TYPE_FILTERED;

Options_Plot_Camera              = TRUE;
Options_Plot_Calibration         = TRUE;

Options_Save_Plots               = TRUE;

%Process Options
Options_VarioCamActive           = TRUE;
Options_CalculateTSEPCalibration = TRUE;
Options_MOSFETforCalibration     = MOSFET2;

Options_ActiveMeasurementHub     = [3];
Options_ActiveMeasurementCam     = 3;

Options_Time_Scale               = TIME_OPTION_SCALE_S;

CSV_path = 'D:\Studium\Masterthesis\Sensor_Data_Files\SensorHub\CSV';
CSV_filename = 'SensorHub_V1Final_HeatUp_TSEP2_7A_Sens075';
CSV_file = [CSV_path '\' CSV_filename];

MAT_path = 'D:\Studium\Masterthesis\Sensor_Data_Files\SensorHub\MAT';
MAT_filename = 'SensorHub_V1Final_HeatUp_TSEP2_7A_Sens075';
MAT_file = [MAT_path '\' MAT_filename];

CameraPath = 'D:\Studium\Masterthesis\Sensor_Data_Files\Kamera\MAT';
Camera_filename = 'VarioCam_FinalTest_HeatUp_TSEP2_7A_Sens075';
Camera_file = [CameraPath '\' Camera_filename];

PLOT_path = 'D:\Studium\Masterthesis\Sensor_Data_Files\Plots';

%Read out all files for specific measurement parameters
for idx = Options_ActiveMeasurementHub
    fileNumber = num2str(idx);
    Mat_file_idx = [MAT_file '_' fileNumber];
    temp = load(Mat_file_idx);
    TempData_raw(idx,:,:) = temp.writeData;

    %Calculate time in ms
    hours       =   TempData_raw(idx,:,1);
    minutes     =   TempData_raw(idx,:,2);
    seconds     =   TempData_raw(idx,:,3);
    subseconds  =   TempData_raw(idx,:,4); %RTC is a count down timer!

    time_raw = hours * (3600 * 1000) + minutes * (60 * 1000) + seconds * 1000 + (1000 - subseconds);

    %Seperate Temperature
    TempData = TempData_raw;
    temp1(idx,:) = TempData_raw(idx,:,5);
    temp2(idx,:) = TempData_raw(idx,:,6);
    temp3(idx,:) = TempData_raw(idx,:,7);
    temp4(idx,:) = TempData_raw(idx,:,8);

    %Calculate Voltage to Temperature for VF%
    %     a1= -94.073; %Values from the CREE SiC-Body Diode model
    %     a0 = 360.51;
    
    
    %temp3(idx,:) = temp3(idx,:) * a1 + a0;

    %Change the time units of x axis
    %Case 0: ms
    %Case 1: s
    %Case 2: min
    %Case 3: h

    switch Options_Time_Scale
        case TIME_OPTION_SCALE_MS
       %Leave it to ms
            time(idx,:) = time_raw;
        case TIME_OPTION_SCALE_S
            time(idx,:) = time_raw*1.0e-3;
        case TIME_OPTION_SCALE_MIN
            time(idx,:)= time_raw*1.0e-3;
            time(idx,:) = time(idx,:)/60;
        case TIME_OPTION_SCALE_HOURS
            time(idx,:) = time_raw*1.0e-3;
            time(idx,:) = time(idx,:)/(60*60);
        otherwise
            time(idx,:) = time_raw;
            %Default: Leave it to ms
    end
end 

idx = Options_ActiveMeasurementCam;

fileNumber = num2str(idx);

Camera_file_idx = [Camera_file '_' fileNumber];
temp = load(Camera_file_idx);
TempData_Camera = temp.writeData;


%-------------Average Data-----------------%

temp1_average = mean(temp1,1);
temp2_average = mean(temp2,1);
temp3_average = mean(temp3,1);
temp4_average = mean(temp4,1);
    

%-------------Filter Data-----------------%

for idx = Options_ActiveMeasurementHub
    windowSize = 5;
    b = (1 / windowSize) * ones(1,windowSize);
    a = 1;

    temp1_filter(idx,:) = filtfilt(b,a,temp1(idx,:));
    temp2_filter(idx,:) = filtfilt(b,a,temp2(idx,:));
    temp3_filter(idx,:) = filtfilt(b,a,temp3(idx,:));
    temp4_filter(idx,:) = filtfilt(b,a,temp4(idx,:));

end  
%-------------Process Data-----------------%

if Options_VarioCamActive
   
    %CameraData
    %     TempData_Mosfet1 = TempData_Camera(1,:);
    %     TempData_Mosfet2 = TempData_Camera(2,:);
    %     TempData_Mosfet3 = TempData_Camera(3,:);
    %     TempData_Mosfet4 = TempData_Camera(4,:);
    %     TempData_Thermocouple = TempData_Camera(5,:);
    %     TempData_Thermistor = TempData_Camera(6,:);
    
    %Calculate the temperature in celsius
    TempData_Camera = TempData_Camera-273.15;

    %Check if there is some non-valid data (temp. value = 0) in the data
   
    idx_valid = TempData_Camera(:,1) > 0;
    
    TempData_CameraValid = TempData_Camera(idx_valid,:);
    
    %Extract the time stamps where a trigger was send 
    %The SensorHub puts 0 for no trigger and 255 for trigger in each cycle
    idx_valid = temp4(Options_ActiveMeasurementCam,:) == 255;
    time_Camera = time(Options_ActiveMeasurementCam,idx_valid);
    
    %The camera sometimes miss triggers so the time and temperature data
    %have to be aligned
    if(size(time_Camera,2) < size(TempData_CameraValid,1))
        endidx = size(time_Camera,2);
        TempData_CameraValid = TempData_CameraValid(1:endidx);
        % temp3_filter_calib = temp3_filter_calib(1:endix);
        temp3_calib = temp3_calib(1:endix);
    else
        endidx = size(TempData_CameraValid,1);
        time_Camera = time_Camera(1:endidx);     
    end
    
    if Options_CalculateTSEPCalibration
        %-------------Calirbation Data-----------------%
        
        idx_valid = temp4(Options_ActiveMeasurementCam,:) == 255;
        
        %temp3_filter_calib = temp3_filter(idx_valid); %Extract data for calibration curve
        temp3_calib = temp3(Options_ActiveMeasurementCam,idx_valid);
        endidx = size(time_Camera,2);
        temp3_calib = temp3_calib(1:endidx);
        
        p = polyfit(temp3_calib.', TempData_CameraValid(:,Options_MOSFETforCalibration),1);
        temp3_calibResult = temp3_calib*p(1) + p(2);
        
    end
end


%-------------PLOT Data-----------------%

%-------------Single Raw Data-----------------%
if Options_Plot_Type == PLOT_OPTION_TYPE_SINGLE_RAW
    %Plot the Thermocouple Temperature curve
    if Options_Plot_Thermocouple 
        figure();
        legendCell = cell(size(Options_ActiveMeasurementHub,2),1);
        for idx = Options_ActiveMeasurementHub
            plot(time(idx,:),temp1(idx,:)); hold on;
            legendCell{idx} = ['Measurement ' num2str(idx)];
        end
        title('Thermocouple Temperature RawData');
        xlabel('Time in s');
        ylabel('Temperature in °C');
        legendCell = legendCell(~cellfun('isempty',legendCell));
        legend(legendCell,'Location','southeast');
        if Options_Save_Plots
            plotName = [Camera_filename '_Themocouple_Temperature_Raw' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end

    %Plot the Thermistor Temperature curve
    if Options_Plot_Thermistor 
        figure();
        legendCell = cell(size(Options_ActiveMeasurementHub,2),1);
        for idx = Options_ActiveMeasurementHub
            plot(time(idx,:),temp2(idx,:)); hold on;
            legendCell{idx} = ['Measurement ' num2str(idx)];
        end
        title('Thermistor Temperature RawData');
        xlabel('Time in s');
        ylabel('Temperature in °C');
        legendCell = legendCell(~cellfun('isempty',legendCell));
        legend(legendCell,'Location','southeast');
        if Options_Save_Plots
            plotName = [Camera_filename '_Thermistor_Temperature_Raw' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end

    %Plot the TSEP Temperature curve
    if Options_Plot_TSEP 
        figure(); 
        legendCell = cell(size(Options_ActiveMeasurementHub,2),1);
        for idx = Options_ActiveMeasurementHub       
            plot(time(idx,:),temp3(idx,:)); hold on;
            legendCell{idx} = ['Measurement ' num2str(idx)];
        end
        title('TSEP V_F Temperature RawData');
        xlabel('Time in s');
        ylabel('Forward Voltage in V');
        legendCell = legendCell(~cellfun('isempty',legendCell));
        legend(legendCell,'Location','northeast');
        if Options_Save_Plots
            plotName = [Camera_filename '_TSEP_V_F_Temperature_Raw' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end   
end

%-------------Averaged Data-----------------%
if Options_Plot_Type == PLOT_OPTION_TYPE_AVERAGED
    %Plot the Thermocouple Temperature curve
    
   
    if Options_Plot_Thermocouple 
        figure();
        plot(time(1,:),temp1_average); hold on;
        title(['Thermocouple Temperature Average of '  num2str(size(Options_ActiveMeasurementHub,2)) ' Measurements']);
        xlabel('Time in s');
        ylabel('Temperature in °C');
         if Options_Save_Plots
            plotName = [Camera_filename '_Thermocouple_Temperature_Averaged' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end

    %Plot the Thermistor Temperature curve
    if Options_Plot_Thermistor 
        figure();
        plot(time(1,:),temp2_average); hold on;
        title(['Thermistor Temperature Average of '  num2str(size(Options_ActiveMeasurementHub,2)) ' Measurements']);
        xlabel('Time in s');
        ylabel('Temperature in °C');
        if Options_Save_Plots
            plotName = [Camera_filename '_Thermistor_Temperature_Averaged' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end

    %Plot the TSEP Temperature curve
    if Options_Plot_TSEP 
        figure(); 
        plot(time(1,:),temp3_average); hold on;
        title(['TSEP V_F Temperature Average of '  num2str(size(Options_ActiveMeasurementHub,2)) ' Measurements']);
        xlabel('Time in s');
        ylabel('Temperature in °C');
        if Options_Save_Plots
            plotName = [Camera_filename '_TSEP_V_F_Temperature_Averaged' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end
end

%-------------Filtered Data-----------------%
if Options_Plot_Type == PLOT_OPTION_TYPE_FILTERED
    %Plot the Thermocouple Temperature curve
    if Options_Plot_Thermocouple 
        figure();
        legendCell = cell(size(Options_ActiveMeasurementHub,2),1);
        for idx = Options_ActiveMeasurementHub
            plot(time(idx,:),temp1_filter(idx,:)); hold on;
             legendCell{idx} = ['Measurement ' num2str(idx)];
             
        end
        title('Thermocouple Temperature Filtered');
        xlabel('Time in s');
        ylabel('Temperature in °C');
        legendCell = legendCell(~cellfun('isempty',legendCell));
        legend(legendCell,'Location','southeast');
        if Options_Save_Plots
            plotName = [Camera_filename '_Thermocouple_Temperature_Filtered' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end

    %Plot the Thermistor Temperature curve
    if Options_Plot_Thermistor 
        figure();
        legendCell = cell(size(Options_ActiveMeasurementHub,2),1);
        for idx = Options_ActiveMeasurementHub
            plot(time(idx,:),temp2_filter(idx,:)); hold on;
            legendCell{idx} = ['Measurement ' num2str(idx)];
        end
        title('Thermistor Temperature Filtered');
        xlabel('Time in s');
        ylabel('Temperature in °C');
        legendCell = legendCell(~cellfun('isempty',legendCell));
        legend(legendCell,'Location','southeast');
        if Options_Save_Plots
            plotName = [Camera_filename '_Thermistor_Temperature_Filtered' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
    end

    %Plot the TSEP Temperature curve
    if Options_Plot_TSEP 
        figure(); 
        legendCell = cell(size(Options_ActiveMeasurementHub,2),1);
        for idx = Options_ActiveMeasurementHub 
            plot(time(idx,:),temp3_filter(idx,:)); hold on;
            legendCell{idx} = ['Measurement ' num2str(idx)];
        end
        title('TSEP V_F Temperature Filtered');
        xlabel('Time in s');
        ylabel('Temperature in °C');
        legendCell = legendCell(~cellfun('isempty',legendCell));
        legend(legendCell,'Location','northeast');
        if Options_Save_Plots
            plotName = [Camera_filename '_TSEP_V_F_Temperature_Filtered' ];
            plotFile = [PLOT_path '\SensorHub\' plotName];
            savefig(plotFile);
            saveas(gcf,plotFile,'png');
        end
       % legend('GateVoltage -4V', 'GateVoltage -3V', 'GateVoltage -2V', 'GateVoltage -1V', 'GateVoltage 0V','Location','northeastoutside');
    end
end

%-------------Thermocam Plots -----------------%

%Plot the Camera Temperature curve
if Options_Plot_Camera
    figure();
    plot(time_Camera, TempData_CameraValid);  hold on;
    title('Variocam Temperature');
    xlabel('Time in s');
    ylabel('Temperature in °C');
    legend('SIC-MOSFET1','SIC-MOSFET2','SIC-MOSFET3','SIC-MOSFET4','Thermocouple CameraValue','Thermisotor CameraValue','Location', 'eastoutside');
    if Options_Save_Plots
        plotName = [Camera_filename '_Variocam_Temperature' ];
        plotFile = [PLOT_path '\Camera\' plotName];
        savefig(plotFile);
        saveas(gcf,plotFile,'png');
    end
end

%-------------Calibration Plots -----------------%
if Options_Plot_Calibration
    figure();
    plot(TempData_CameraValid(:,Options_MOSFETforCalibration),temp3_calib); hold on;
    %plot(time,temp3_filter);
    title('Calibration Curve for TSEP');
    if Options_Save_Plots 
        plotName = [Camera_filename '_TSEPCalibrationCurve' ];
        plotFile = [PLOT_path '\Calibration\' plotName];
        savefig(plotFile);  
        saveas(gcf,plotFile,'png');
    end
     
    figure();
    plot(time_Camera,temp3_calibResult); hold on;
    title('Result of Calibration for TSEP');
    legend('TSEP calibrated','Location','southeast');
    if Options_Save_Plots       
        plotName = [Camera_filename '_TSEPCalibrationResult' ];
        plotFile = [PLOT_path '\Calibration\' plotName];
        savefig(plotFile);
        saveas(gcf,plotFile,'png');
    end
end

%END OF SCRIPT


