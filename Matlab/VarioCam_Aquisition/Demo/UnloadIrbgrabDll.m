function UnloadIrbgrabDll(hObject,handles)

global	glbConnected

if libisloaded ('irbgrablib') & glbConnected
	irbgrabDisconnect(hObject,handles)
end

if (libisloaded ('irbgrablib')) & (~glbConnected)
    unloadlibrary 'irbgrablib';
    disp('irbgrab.dll unloaded')
end


