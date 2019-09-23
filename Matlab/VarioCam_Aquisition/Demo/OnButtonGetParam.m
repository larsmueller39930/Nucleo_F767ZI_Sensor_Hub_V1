function OnButtonGetParam(hObject,handles)
parCode=get( handles.editParamCode, 'String' ) ;
parameter = str2num( parCode ) ;
% please refer to documentation 1.3.8. Constants for the Function 
% irbg_GetParam and irbg_SetParam

global  glbSourceId ;

%get parameter
ParamValue=0;
pParamValue = libpointer( 'doublePtr', ParamValue ) ;
result = irbg_GetParam( glbSourceId, parameter, pParamValue );
if result<0 
  return 
end
ParamValue = pParamValue.Value ;
set( handles.editParamValue, 'String', num2str(ParamValue,3) ) ;

%disp(int2str(parameter) ) ;
guidata(hObject, handles);
drawnow

