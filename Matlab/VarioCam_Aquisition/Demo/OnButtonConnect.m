function OnButtonConnect(hObject,handles)
% at first disable buttons, so the user is pleased to be patient to wait for 
% the end of initialization process
set(handles.ButtonConnect,'Enable','off');
set(handles.ButtonClose,'Enable','off');
set(handles.ButtonGrab,'Enable','off');
drawnow

global  glbSourceId ;
global  glbConnected ;


if glbConnected
    irbgrabDisconnect(hObject,handles);
else
    irbgrabConnect(hObject,handles);
end

% we are ready ...
if glbConnected
    if get(handles.checkboxShowWindow, 'value') > 0
        showWnd = 1;
    else
        showWnd = -1;
    end
    irbg_ShowWindow( glbSourceId, showWnd ) ;    
   
    set(handles.checkboxShowWindow,'Enable','on');
    %CHANGE--------------------------------------%
    set(handles.checkboxSaveData,'Enable','on');
    %CHANGE--------------------------------------%
    
    set(handles.radiobuttonTemperature,'Enable','on');
    set(handles.radiobuttonRawdata,'Enable','on');
    set(handles.editFramerate,'Enable','inactive');
    set(handles.editParamValue,'Enable','on');
    set(handles.editParamCode,'Enable','off');
    set(handles.editParamCode,'String', '20');
    set(handles.popParameter,'Enable','on');
    set(handles.popParameter,'Enable','on');
    set(handles.editSendCommand,'Enable','on');
    set(handles.popParameter,'Enable','on');
    set(handles.popParameter,'Value',1);
    set(handles.ButtonConnect,'String','Disconnect');
    set(handles.ButtonGrab,'Enable','on');
    set(handles.ButtonConnect,'Enable','on');
    set(handles.ButtonGetParam,'Enable','on');
    set(handles.ButtonSetParam,'Enable','on');
else
    set(handles.checkboxShowWindow,'Enable','off');
    
    %CHANGE--------------------------------------%
    set(handles.checkboxSaveData,'Enable','off');  
    %CHANGE--------------------------------------%

    set(handles.popParameter,'Enable','off');
    set(handles.ButtonConnect,'String','Connect');
    set(handles.ButtonGrab,'Enable','off');
    set(handles.ButtonConnect,'Enable','on');
    set(handles.ButtonGetParam,'Enable','off');
    set(handles.ButtonSetParam,'Enable','off');
    
    set(handles.radiobuttonTemperature,'Enable','off');
    set(handles.radiobuttonRawdata,'Enable','off');
    set(handles.editFramerate,'Enable','off');
    set(handles.editParamValue,'Enable','off');
    set(handles.editParamCode,'Enable','off');
    set(handles.editSendCommand,'Enable','off');
end
set(handles.ButtonClose,'Enable','on');
guidata(hObject, handles);
drawnow

