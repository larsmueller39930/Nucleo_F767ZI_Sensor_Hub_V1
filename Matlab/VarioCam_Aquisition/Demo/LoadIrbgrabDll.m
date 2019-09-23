function Result=LoadIrbgrab(hObject,handles)
Result = false ;
if libisloaded ('irbgrablib')
    UnloadIrbgrabDll(hObject, handles)
end

if( exist(handles.irbgrabDllDir,'dir') ~= 7 )
    error('Error: directory %s not found', irbgrabDllDir)
    return;
end
if( exist(handles.irbgrabHeader,'file') ~=2 )
    error('Error: file %s not found', irbgrabHeader)
    return;
end

irbgrabDll = [handles.irbgrabDllDir,filesep,'irbgrab.dll'];
res = exist(irbgrabDll,'file') ;
% in Matlab 7 / 32 Bit you would get 3, in Matlab 2011 / 64 Bit 2 is given back 
if(( res ~=2 ) && ( res ~= 3 ))
    error('Error: file %s not found', irbgrabDll)
    return;
end

loadlibrary(irbgrabDll, handles.irbgrabHeader, 'alias' ,'irbgrablib' );

if libisloaded ('irbgrablib')
    disp('irbgrab.dll loaded')
    drawnow
    cd(handles.irbgrabDllDir);
    % get handle of calling MS Windows application 
end
Result = true ;
guidata(hObject, handles);





