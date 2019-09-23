function irbgrabImageAcq(hObject,handles, RawData)

global glbStoppReq
global glbGrabberRunning
global glbSourceId
%CHANGE--------------------------------------%
global glbSave
%CHANGE--------------------------------------%

InfoIn = libstruct( 'GRABINFOIN') ;
InfoOut = libstruct( 'GRABINFOOUT') ; 

% DATATYPE_BITMAP         1    // complete bitmap with header (54 Byte)
% DATATYPE_BITMAP32DATA   2    // Bitmap pixel Data (32Bit color) without header
% DATATYPE_IRBFRAME       3    // complete IRB-Frame with header
% DATATYPE_IRBDATA        4    // IRB-RawData (digital data - one Word per Pixel) without Header
% DATATYPE_TEMPERATURES   5    // temperatures (Single/float)

% fill InfoIn-structure
InfoIn.SrcId    = glbSourceId ;
if RawData == 1
    InfoIn.DataType = 4 ;
else
    InfoIn.DataType = 5 ;
end
%InfoIn.Buffer   = libpointer ;
InfoIn.Buffer = 0 ;
InfoIn.bufsize  = 0 ;
InfoIn.options  = 0 ;
InfoIn.timeout  = 2000 ;
TimeStampMS = uint32(0) ;
TimeStampUS = uint32(0) ;

%CHANGE--------------------------------------%
firstTrigger = 0;

KameraPath = 'D:\Studium\Masterthesis\Sensor_Data_Files\Kamera\MAT';
Kamera_filename = 'VarioCam_FinalTest_HeatUp_TSEP2_7A_Sens075_3';
Kamera_file = [KameraPath '\' Kamera_filename];

%CHANGE--------------------------------------%

if glbGrabberRunning
    result = irbg_Grab( InfoIn, InfoOut) ;
    if result ~= 1 
        return
    end
    imWidth=InfoOut.Width ; 
    imHeight=InfoOut.Height ;
    if RawData == 1
        bsz = imWidth * imHeight * 2 ; % int16 buffer !
        if bsz == InfoOut.bufsize
            Buffer=uint16(zeros(1,imWidth*imHeight));
        else
            disp('buffer size missmatch in irbgrabImageAcq') ;
        end
     else
        bsz = imWidth * imHeight * 4 ; % single buffer !
        if bsz == InfoOut.bufsize
            Buffer=single(zeros(1,imWidth*imHeight));
        else
            disp('buffer size missmatch in irbgrabImageAcq') ;
        end
    end
    
    pBuffer = libpointer('voidPtr',Buffer);
    InfoIn.Buffer = irbg_PointerToInteger( pBuffer ) ;
    InfoIn.bufsize = bsz ;
    % build an empty frame
    bild=reshape(Buffer,imWidth,imHeight);
    bild=rot90(bild,-1);
    hImage=imagesc(bild);
    glbStoppReq=false;
    tic;
    frameCnt = 0 ;
    frameCntOld = 0 ;
    InfoOut.Width=0 ;
    while ~glbStoppReq
        
        guidata(hObject, handles) ;
        drawnow ;
        result = irbg_Grab( InfoIn, InfoOut) ;
        if RawData == 1
            frame=uint16(get(pBuffer, 'value'));
        else
            frame=single(get(pBuffer, 'value'));
        end
        if result == 1 
            % Attention ! Matlab 7.0 R14 has a bug in its generic Dll interface
            % The conversion of double does not work, so the value of TimeStamp
            % with type double can not converted in the expected manner.
            % It seems, that type double is interpreted as a 4 byte type what 
            % leads to serios effects... 
            % OK, this is a work around to get the high resolution timestamp of the grabbed frame:
            pTimeStampMS = libpointer( 'int32Ptr', TimeStampMS) ;
            pTimeStampUS = libpointer( 'int32Ptr', TimeStampUS) ;
            irbg_TimeStampToIntegers( InfoOut.DummyTimestamp2, InfoOut.DummyTimestamp1 , pTimeStampMS, pTimeStampUS ) ;
            TimeStampMS = pTimeStampMS.Value ; % Milliseconds
            TimeStampUS = pTimeStampUS.Value ; % Microseconds
            
            frameCnt = frameCnt + 1 ;
            frame=reshape(frame,imWidth,imHeight);
            frame=rot90(frame,-1);
            frameMatrix=fliplr(frame);
            
            %CHANGE--------------------------------------%
            if InfoOut.Trigger == 1
                 
                 if firstTrigger == 0
                     
                     firstTrigger = 1;
                     triggerCnt = 1;
                     
                     mosfet1_data = frameMatrix(162:211, 250:299);
                     mosfet2_data = frameMatrix(262:311, 250:299);
                     mosfet3_data = frameMatrix(162:211, 370:419);
                     mosfet4_data = frameMatrix(262:311, 370:419);
                     
                     thermocouple_data = frameMatrix(315:320, 430:435);
                     thermocouple_data = diag(thermocouple_data);
                     thermistor_data = frameMatrix(300:303, 467:470);
                     thermistor_data = diag(thermistor_data);
                                           
                     mosfet1_perRowMean = mean(mosfet1_data);
                     mosfet1_totalMean = mean(mosfet1_perRowMean);
                     
                     mosfet2_perRowMean = mean(mosfet2_data);
                     mosfet2_totalMean = mean(mosfet2_perRowMean);
                     
                     mosfet3_perRowMean = mean(mosfet3_data);
                     mosfet3_totalMean = mean(mosfet3_perRowMean);
                     
                     mosfet4_perRowMean = mean(mosfet4_data);
                     mosfet4_totalMean = mean(mosfet4_perRowMean);
                     
                     thermocouple_perRowMean = mean(thermocouple_data);
                     thermocouple_totalMean = mean(thermocouple_perRowMean);
                     
                     thermistor_perRowMean = mean(thermistor_data);
                     thermistor_totalMean = mean(thermistor_perRowMean);
                                    
                     writeData(triggerCnt,:) = [mosfet1_totalMean mosfet2_totalMean mosfet3_totalMean mosfet4_totalMean thermocouple_totalMean thermistor_totalMean];
                            
                     triggerCnt = triggerCnt + 1;
                
                 else
     
                     mosfet1_data = frameMatrix(162:211, 250:299);
                     mosfet2_data = frameMatrix(262:311, 250:299);
                     mosfet3_data = frameMatrix(162:211, 370:419);
                     mosfet4_data = frameMatrix(262:311, 370:419);
                     
                     thermocouple_data = frameMatrix(315:320, 430:435);
                     thermocouple_data = diag(thermocouple_data);
                     thermistor_data = frameMatrix(300:303, 467:470);
                     thermistor_data = diag(thermistor_data);
                     
                     mosfet1_perRowMean = mean(mosfet1_data);
                     mosfet1_totalMean = mean(mosfet1_perRowMean);
                     
                     mosfet2_perRowMean = mean(mosfet2_data);
                     mosfet2_totalMean = mean(mosfet2_perRowMean);
                     
                     mosfet3_perRowMean = mean(mosfet3_data);
                     mosfet3_totalMean = mean(mosfet3_perRowMean);
                     
                     mosfet4_perRowMean = mean(mosfet4_data);
                     mosfet4_totalMean = mean(mosfet4_perRowMean);
                     
                     thermocouple_perRowMean = mean(thermocouple_data);
                     thermocouple_totalMean = mean(thermocouple_perRowMean);
                     
                     thermistor_perRowMean = mean(thermistor_data);
                     thermistor_totalMean = mean(thermistor_perRowMean);
                     
                     writeData(triggerCnt,:) = [mosfet1_totalMean mosfet2_totalMean mosfet3_totalMean mosfet4_totalMean thermocouple_totalMean thermistor_totalMean];
                     
                     triggerCnt = triggerCnt + 1;
                 end
                 %CHANGE--------------------------------------%

            end
            set(hImage,'Cdata',frameMatrix);
            
            %CHANGE--------------------------------------%
            
            %Indication Lines for aligning measurement objekt
%             x = [0 640]
%             y = [162 162]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [0 640]
%             y = [212 212]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [0 640]
%             y = [262 262]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [0 640]
%             y = [312 312]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [250 250]
%             y = [0 480]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [300 300]
%             y = [0 480]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [370 370]
%             y = [0 480]
%             line(x,y,'Color','red','Linewidth',1);
%             x = [420 420]
%             y = [0 480]
%             line(x,y,'Color','red','Linewidth',1);
%             
%             y = [315, 320]
%             x = [430, 435]
%             line(x,y,'Color','red','Linewidth',1);
%             y = [300, 303]
%             x = [467, 470]
%             line(x,y,'Color','red','Linewidth',1);
%             
            %CHANGE--------------------------------------%

        end
        if toc >= 1
            rate = frameCnt - frameCntOld ;
            tic ;
            frameCntOld = frameCnt ;
            set(handles.editFramerate,'String',rate);
        end
        
    end
end
guidata(hObject, handles);
 %CHANGE--------------------------------------%
if glbSave
    save(Kamera_file,'writeData');
end
 %CHANGE--------------------------------------%
clear Buffer


