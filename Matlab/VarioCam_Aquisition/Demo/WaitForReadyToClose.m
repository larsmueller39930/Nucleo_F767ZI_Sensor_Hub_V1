function WaitForReadyToClose(hTimer, event,hObject)

global glbGrabberRunning ;
global glbTimer ;

handles=guidata(hObject);
if ~glbGrabberRunning
    stop(glbTimer);
    delete (glbTimer);
    guidata(hObject, handles);
    irbgrabDisconnect(hObject,handles) ;
    UnloadIrbgrabDll ;
    delete( gcf );
end
