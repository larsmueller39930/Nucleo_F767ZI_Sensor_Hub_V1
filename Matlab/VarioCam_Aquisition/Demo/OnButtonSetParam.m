function OnButtonSetParam(hObject,handles)
parCode=get( handles.editParamCode, 'String' ) ;
parameter = str2num( parCode ) ;
% please refer to documentation 1.3.8. Constants for the Function 
% irbg_GetParam and irbg_SetParam

global  glbSourceId ;

%get parameter
ParamValue=0;
parStr=get( handles.editParamValue, 'String' ) ;
ParamValue=str2num( parStr ) ;

result = irbg_SetParam( glbSourceId, parameter, ParamValue );
if result<0 
  return 
end

guidata(hObject, handles);
drawnow

