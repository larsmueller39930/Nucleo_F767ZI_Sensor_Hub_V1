function  Result=irbg_Grab( InfoIn, InfoOut)
refInfoIn = libpointer('GRABINFOIN', InfoIn);
refInfoOut = libpointer('GRABINFOOUT', InfoOut);

Result=calllib('irbgrablib', 'irbg_Grab', refInfoIn, refInfoOut) ;
