%{
    [Sensor_Data_Eval.m]
    [Matlab script that hold the defines for the Sensor_Data_Eval script],

    Author:               Lars Mueller
    LastChangedRevision:  0.0
    LastChangedDate:      21.09.19

    Change Record:
    Version | Date      | Author        | Section affected
    -------------------------------------------------------
    0.0 <- Initial Development Version 
%}

clear; clc;

%Defines
PLOT_OPTION_TYPE_SINGLE_RAW      = 0; 
PLOT_OPTION_TYPE_AVERAGED        = 1;
PLOT_OPTION_TYPE_FILTERED        = 2;

TIME_OPTION_SCALE_MS             = 0;
TIME_OPTION_SCALE_S              = 1;
TIME_OPTION_SCALE_MIN            = 2;
TIME_OPTION_SCALE_HOURS          = 3;

MOSFET1                          = 1;
MOSFET2                          = 2;
MOSFET3                          = 3;    
MOSFET4                          = 4;

TRUE                             = 1;
FALSE                            = 0;
