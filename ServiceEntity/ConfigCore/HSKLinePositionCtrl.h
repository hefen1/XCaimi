#pragma once
/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsKLineTab.h
* ժ    Ҫ��K��tab����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2011-5-14
* ��	 ע��
*******************************************************************************/
#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHSLinePositionCtrl : public IHSKLinePosition
{
protected:
	BOOL			m_bInited;
	CConfigBase*	m_pConfigBase;
	int				m_nCurrentPos;
public:
	CHSLinePositionCtrl(void);
	~CHSLinePositionCtrl(void);
	void Init();
	BOOL IsInit() {return m_bInited;}
	BOOL Release();
//interface
	virtual int			GetTechLines(int nType, CStringArray& ayStrRet, BOOL bResearch = FALSE);//��ȡλ�ÿ���ǰ������ͼԪ
	virtual float		GetPosCtrolRatio(int nType);										//��ȡλ�ÿ��Ƶı���
	virtual int			SetTechLines(int nType, CStringArray& strKLIneName, 
							BOOL bFirst = FALSE, BOOL bLast = FALSE);//д��ͼԪ
	virtual int			SetPosCtrolRatio(int nType, float fRatio);							//д��λ�ÿ���

};