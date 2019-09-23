function irbGrabGetSources(hObject,handles)

% so user is pleased to be patient to wait for the end of initialization
% process
set(handles.ButtonConnect,'Enable','inactive');
set(handles.ButtonClose,'Enable','inactive');
set(handles.ButtonGrab,'Enable','inactive');
drawnow

CharBuffer = blanks(256);
pCharBuffer = libpointer('voidPtr',[uint8(CharBuffer) 0]);

SourceCnt=0;
pSourceCnt = libpointer( 'int32Ptr', SourceCnt) ;

result=irbg_GetSources(pCharBuffer, pSourceCnt );
if result<0 
    return 
end
SourceCnt = pSourceCnt.Value ;
if SourceCnt<1
    return
end
Sources = char(pCharBuffer.value) ; 
remainder = Sources;
SrcEntries = '';

for i=1:SourceCnt
  [chopped,remainder] = strtok(remainder);
  SrcEntries = strvcat(SrcEntries, chopped);
end

set( handles.popSources, 'String', SrcEntries ) ;
set(handles.ButtonClose,'Enable','on');
set(handles.ButtonConnect,'Enable','on');
guidata(hObject, handles);
drawnow

