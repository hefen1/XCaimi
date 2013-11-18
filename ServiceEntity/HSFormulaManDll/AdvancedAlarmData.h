/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	AdvancedAlarmData.h
*	�ļ���ʶ��	Ԥ����������
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#include "hsstructnew.h"
#include "ExpPublic.h"
#pragma once
#define	HS_Block	0x0001
#define HS_Avi		0x0002
#define HS_Box		0x0004
#define HS_NoBreak 0x0008
#define HS_T0		0x0010
#define HS_Jg		0x0020
#define HS_Ts		0x0040
#define HS_ALARM_ON 0x8000
#define HS_ALARM_ENABLE	0x4000

class CAdvancedAlarmData
{
public:
	CAdvancedAlarmData(void);
	~CAdvancedAlarmData(void);

public:
	//����������
	CString									m_strName;				

	//������Ԥ���Ĺ�Ʊ�б�
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList;			

	//Ԥ�������
	CString									m_strAlarmBlock;		

	//Ԥ������
	WORD									m_uAlarmType;			

	//Ԥ����������
	int										m_nCount;

	//Ԥ������
	static int								m_nCycle;

	//Ԥ�������ļ�
	CString									m_strSoundFileName;

	//����
	UnionSelCondition*								m_pUnionCondition;
	SingleSelCondition*							m_pSingleSel;
	BOOL									m_SingleOrUnion;
		
public:
	BOOL SetCondition(SingleSelCondition* sSingleSel);
	BOOL SetUnionCondition(UnionSelCondition* UnionSel);
	BOOL AddStock(StockUserInfo* pData);
	BOOL DelStock(int nIndex);
	BOOL CopyStkTo(CArray<StockUserInfo*, StockUserInfo*>* payStkList);
	BOOL CopyStk(CArray<StockUserInfo*, StockUserInfo*>* payStkList);
	void Init();
	void Free();
	void Serialize(CArchive& ar);
	void SetBlockName(CString block);
	void AddCount(int nSize = 1);
	int	 GetStkCount();
	void RemoveAllStk();
};
