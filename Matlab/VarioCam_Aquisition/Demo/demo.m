function varargout = demo(varargin)
%DEMO M-file for demo.fig
%      DEMO, by itself, creates a new DEMO or raises the existing
%      singleton*.
%
%      H = DEMO returns the handle to a new DEMO or the handle to
%      the existing singleton*.
%
%      DEMO('Property','Value',...) creates a new DEMO using the
%      given property value pairs. Unrecognized properties are passed via
%      varargin to demo_OpeningFcn.  This calling syntax producgres a
%      warning when there is an existing singleton*.
%
%      DEMO('CALLBACK') and DEMO('CALLBACK',hObject,...) call the
%      local function named CALLBACK in DEMO.M with the given input
%      arguments.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help demo

% Last Modified by GUIDE v2.5 14-Aug-2019 09:21:23

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @demo_OpeningFcn, ...
                   'gui_OutputFcn',  @demo_OutputFcn, ...
                   'gui_LayoutFcn',  [], ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
   gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before demo is made visible.
function demo_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   unrecognized PropertyName/PropertyValue pairs from the
%            command line (see VARARGIN)

% Choose default command line output for demo
handles.output = hObject;

global  glbSourceId ;
global  glbConnected ;
global  glbStoppReq ;
global  glbGrabberRunning ;
global  glbTimer ;
%CHANGE--------------------------------------%
global  glbSave;

glbSave = 0;
%CHANGE--------------------------------------%
glbStoppReq = false;
glbSourceId=-1 ;
glbConnected=false ;
glbGrabberRunning=false; 
handles.curWorkDir=pwd;

curWorkDir=pwd;
addpath(curWorkDir);
relPath=['..',filesep,'..', filesep];
cd(relPath);
handles.irbgrabDllDir=pwd;
addpath(pwd) ;

cd(curWorkDir);

%Path auf ..\Irbgrabfunctions erweitern
cd(['..',filesep,'Irbgrabfunctions']) ; 
addpath(pwd);
handles.irbgrabHeader=[pwd,filesep,'IRBGrab.h'];
cd(curWorkDir);

handles.curWorkDir=curWorkDir;
handles.Connected=false;

LoadIrbgrabDll(hObject, handles);
irbgrabGetSources(hObject,handles);

% Update handles structure
guidata(hObject, handles);

% move and resize figure object
% set(hObject,'Position',[7 35 357 496]);
set(handles.ButtonGrab,'Enable','off');
set(handles.editParamCode,'String', '20');
% UIWAIT makes demo wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = demo_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



% --- Executes on button press in ButtonConnect.
function ButtonConnect_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%if get(hObject,'Value')
if handles.Connected    
    OnButtonDisconnect(hObject,handles)
else
    OnButtonConnect(hObject,handles)    
end


% --- Executes on button press in ButtonGrab.
function ButtonGrab_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonGrab (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of ButtonGrab
global glbStoppReq
global glbGrabberRunning

if get(hObject,'Value')
    OnButtonGrabStart(hObject,handles)
else
    if glbGrabberRunning
        glbStoppReq=true;
        guidata(hObject, handles);
    end
end



% --- Executes on button press in ButtonClose.
function ButtonClose_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonClose (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

OnCloseReq(hObject,handles);
cd(handles.curWorkDir);

function FigureClose_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonClose (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

OnCloseReq(hObject,handles);
cd(handles.curWorkDir);


function editSendCommand_Callback(hObject, eventdata, handles)
% hObject    handle to editSendCommand (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global glbConnected ;
global glbSourceId

libstate=libisloaded ('irbgrablib');
if libstate == false
    OnButtonConnect(hObject,handles);
end
if glbConnected
    command=get(hObject,'String');
%    answer=uint8(zeros(1,100));
%    pBuffer = libpointer('voidPtr',answer);    

    Buffer = blanks(256);
    pBuffer = libpointer('voidPtr',[uint8(Buffer) 0]);    
    

    irbg_SendCommand( glbSourceId , command, pBuffer, 2000 );
    answer=char(get(pBuffer,'value'));
    set(handles.editAnswer,'String',answer);
end
if libstate == false
    OnButtonDisconnect(hObject,handles);
end


% --- Executes on button press in checkboxShowWindow.
function checkboxShowWindow_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxShowWindow (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxShowWindow

global glbConnected
global glbSourceId

if glbConnected
    if get(hObject,'value') > 0
        showWnd = 1;
    else
        showWnd = -1;
    end
    irbg_ShowWindow( glbSourceId, showWnd ) ;
end


% --- Executes on button press in ButtonGetParam.
function ButtonGetParam_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonGetParam (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
OnButtonGetParam(hObject,handles);


% --- Executes on button press in ButtonSetParam.
function ButtonSetParam_Callback(hObject, eventdata, handles)
% hObject    handle to ButtonSetParam (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
OnButtonSetParam(hObject,handles);



% --- Executes during object creation, after setting all properties.
function editParamCode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editParamCode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



% --- Executes on selection change in popParameter.
function popParameter_Callback(hObject, eventdata, handles)
% hObject    handle to popParameter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns popParameter contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popParameter
selentry = get( handles.popParameter, 'String' ) ;
index = get( handles.popParameter, 'Value' ) ;
len = size(selentry) ;
if index == len(1) % entry 'Other'
    set(handles.editParamCode, 'String', '' );    
    set(handles.editParamCode, 'Enable', 'on' );    
else
    set(handles.editParamCode, 'Enable', 'off' );   
    index=get( handles.popParameter, 'Value' ) ;
    % please refer to documentation 1.3.8. Constants for the Function 
    % irbg_GetParam and irbg_SetParam

    % simple mapping of index to parameter (it this case it works that way)
    ParameterCode = index + 20 -1 ;
    set(handles.editParamCode, 'String', num2str(ParameterCode,3) )
end


%CHANGE--------------------------------------%

% --- Executes on button press in checkboxShowWindow.
function checkboxSaveData_Callback(hObject, eventdata, handles)
% hObject    handle to checkboxShowWindow (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkboxShowWindow

global glbConnected
global glbSave

if glbConnected
   if get(hObject,'Value') > 0
        glbSave = 1; 
   else
       glbSave = 0;
   end
  
end


%CHANGE--------------------------------------%


