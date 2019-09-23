function irbgrabConnect(hObject,handles)

global  glbSourceId ;
global  glbConnected ;


if libisloaded ('irbgrablib')
    disp('If there is no camera please wait kindly for timeout...')
    drawnow
    cd(handles.irbgrabDllDir);
  
    glbSourceId=get( handles.popSources, 'Value' ) -1 ;
    
    result=irbg_InitSource( glbSourceId );
    if (result<0)
        disp('initialize camera failed');
    else
        glbConnected=true ;
    end
end
guidata(hObject, handles);



