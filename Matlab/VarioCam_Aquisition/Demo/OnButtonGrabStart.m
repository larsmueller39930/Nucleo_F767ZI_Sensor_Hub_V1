function OnButtonGrabStart(hObject,handles)

global  glbConnected ;
global  glbGrabberRunning ;

if (~glbGrabberRunning)
    drawnow
    if libisloaded ('irbgrablib') & glbConnected
        set(handles.ButtonGrab,'String','Stopp');
        set(handles.radiobuttonRawdata,'Enable', 'off' );
        set(handles.radiobuttonTemperature,'Enable', 'off' );
        RawData=get(handles.radiobuttonRawdata,'Value');
        guidata(hObject, handles);
        drawnow
        disp('Acquisition is running')
        glbGrabberRunning=true;
        irbgrabImageAcq(hObject,handles, RawData);
        glbGrabberRunning=false;        
        disp('Acquisition stopped')
        set(handles.ButtonGrab,'String','Grab');
        set(handles.radiobuttonRawdata,'Enable', 'on' );
        set(handles.radiobuttonTemperature,'Enable', 'on' );
    end
end
guidata(hObject, handles);
