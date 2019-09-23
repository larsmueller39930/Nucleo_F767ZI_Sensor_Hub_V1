function  Result=irbg_SendCommand( SourceId, Command, Answer, TimeOut)
Result=calllib('irbgrablib','irbg_SendCommand', SourceId, Command, Answer, TimeOut);
