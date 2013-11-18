/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsQuoteSys.h
* ժ    Ҫ������ϵͳ����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-29
* ��	 ע��
*******************************************************************************/
#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"



class CHsQuoteSys : public IHsQuoteSys
{
public:
	CHsQuoteSys(void);
	~CHsQuoteSys(void);
	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

// ʵ�ֽӿ�
public:
	virtual BOOL IsSupportLevel2(); // �Ƿ�֧��Level2
	virtual BOOL IsSupportFuture();//�Ƿ�֧���ڻ�����
	virtual CString GetQuoteServerInitMarket();
	virtual CString GetFutureServerInitMarket();

private:
	BOOL m_bInited;
	CConfigBase* m_pConfigBase;
};
