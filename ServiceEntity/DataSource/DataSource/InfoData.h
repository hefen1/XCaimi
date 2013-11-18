#pragma once

//×ÊÑ¶»º´æ¶ÁÐ´Àà

class CInfoData
{
public:
	CInfoData(void);
	virtual ~CInfoData(void);

	char* Read(AskData* pAsk, long& lSize);
	void Write(void*, CodeInfo*);
	CString GetFilePath(TextMarkData* pData,  CodeInfo* pCode);
protected:
	

};
