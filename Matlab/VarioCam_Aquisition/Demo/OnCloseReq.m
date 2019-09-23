function OnCloseReq(hObject,handles)

global glbStoppReq
global glbGrabberRunning
global glbTimer

set(handles.ButtonConnect,'Enable','inactive');
set(handles.ButtonGrab,'Enable','off');
if glbGrabberRunning
    glbStoppReq=true;
end
glbTimer=timer('TasksToExecute',30,'Period',0.5,'ExecutionMode','fixedSpacing');
set(glbTimer,'TimerFcn',{'WaitForReadyToClose',hObject});
guidata(hObject, handles);
start(glbTimer);
