function hGrabber=irbgrabDisconnect(hObject,handles)

global  glbStoppReq;
global  glbGrabberRunning;
global  glbConnected;
global  glbSourceId;

if libisloaded ('irbgrablib') & glbConnected
    disp('close connection ...')
    drawnow
    if ~glbGrabberRunning
        irbg_CloseSource( glbSourceId );
        disp('disconnected')
        glbConnected=false;
    else
        glbStoppReq=true;
    end
end
guidata(hObject, handles);

