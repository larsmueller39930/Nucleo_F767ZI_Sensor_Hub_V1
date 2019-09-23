

#define IRBDLL_NO_ERROR 		1
#define IRBDLL_CHANGED_PARAMS   	2
#define IRBDLL_RET_ERROR       		-1
#define IRBDLL_NO_IMAGE        		-2
#define IRBDLL_NO_IMAGE_CHANGED_PARAMS  -3
#define IRBDLL_NODLL_XCAM     		-4
#define IRBDLL_NODLL_VIRTGRAB 		-5
#define IRBDLL_ERROR_TIMEOUT  		-6

#define IRBDLL_TRIGGED		1
#define IRBDLL_TRIGGED_NONE	0

#define IRBDLL_HEADER_NONE  	0
#define IRBDLL_HEADER_IRB100   	1
#define IRBDLL_HEADER_IRB101   	2
#define IRBDLL_HEADER_BMP      	3


#define DATATYPE_BITMAP         1    // komplettes Bitmap inklusive Header (54 Byte)
#define DATATYPE_BITMAP32DATA   2    // Bitmap Pixel Daten (32Bit Farbtiefe) ohne Header
#define DATATYPE_IRBFRAME       3    // komplettes IRB-Frame inklusive Header
#define DATATYPE_IRBDATA        4    // IRB-Daten (DigitalDaten - ein Word pro Pixel) ohne Header
#define DATATYPE_TEMPERATURES   5    // Temperatur-Daten (Single-Werte)


struct GRABINFOIN
{
  int   SrcId;
  int	DataType;
  int   Buffer; // eigentl. ein Pointer
  int   bufsize;
  int	options;
  int   timeout;
  int   reserved1;
  int	reserved2;	
} ; 


struct GRABINFOOUT
{
  int 	 Width;
  int 	 height;	
  int 	 SrcId;
  int	 DataType;
  void*  Buffer; 
  int	 bufsize;
  int 	 ImgCounter;
  int	 headsize;
  int 	 headversion;
  int    Trigger;
  int 	 CombinedCnt;
  //irb-Sezifisch
  float  irbmin;
  float  irbavg;
  float  irbmax;
  float  irbstddev;
  int    DummyTimestamp1; // DummyTimestamp1 and DummyTimestamp2 are placeholder for type a high resolution timestamp of type double
  int    DummyTimestamp2; // (in Pascal/Delphi type TDateTime). To get a useful value we have to interprete both values together as double and so on.
  short  wYear ;          // To do that use function irbg_TimestampToIntegers
  short  wMonth ;
  short  wDayOfWeek ;
  short  wDay ;
  short  wHour ;
  short  wMinute ;
  short  wSecond ;
  short  wMilliseconds ; // not exact. For exact milliseconds use timestamp
  
//  _Systemtime  SystemTime;   //16Byte
  int   reserved1;
  int	reserved2;	
  int   reserved3;
  int	reserved4;	
  int   reserved5;
  int	reserved6;	
  int   reserved7;
  int	reserved8;	
  int   reserved9;
  int	reserved10;	
  int	reserved11;	
} ;

int _stdcall TimeStampToIntegers( int TimeStamp1, int TimeStamp2, int* TimeStampMS, int* TimeStampUS ) ;
int _stdcall irbg_AcqInterval_uSecs(int uSecs);
int _stdcall PointerToInteger(void* p);
int _stdcall irbg_GetSources(void* CharBuf,int* pSrcCount);
int _stdcall irbg_InitSource(int SrcNr);
int _stdcall irbg_CloseSource(int SrcNr);
int _stdcall irbg_SendCommand( int SrcNr,const char* Cmd,void* Answer,int TimeOut);
int _stdcall irbg_showWindow(int SrcNr, int mode) ;
int _stdcall irbg_GrabIRBLUT(int SrcNr, void* pbuf);
int _stdcall irbg_RegisterWndMsgNewPict(int SrcNr,unsigned long WndHandle, unsigned long MsgNr);
//int _stdcall irbg_Grab(struct GRABINFOIN* infoin, struct GRABINFOOUT* infoout);
int _stdcall irbg_Grab(void* infoin, void* infoout);
int _stdcall irbg_GetParam(int SrcID, int Param, double* Value);
int _stdcall irbg_SetParam(int SrcID, int Param, double  Value);



