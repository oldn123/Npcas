#pragma once
class asmfunc
{
public:
	asmfunc(void);
	~asmfunc(void);
};

void ConvertEndByte(unsigned char * pData, int nLen);

void SetDicDataPath(const char *);

int fun_7ff72ebd4f60_readDic(int nRdx);