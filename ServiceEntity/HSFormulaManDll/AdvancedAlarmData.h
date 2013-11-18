/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	AdvancedAlarmData.h
*	文件标识：	预警条件设置
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
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
	//本条件名称
	CString									m_strName;				

	//本条件预警的股票列表
	CArray<StockUserInfo*, StockUserInfo*>	m_ayStkList;			

	//预警板块名
	CString									m_strAlarmBlock;		

	//预警类型
	WORD									m_uAlarmType;			

	//预警次数计数
	int										m_nCount;

	//预警周期
	static int								m_nCycle;

	//预警声音文件
	CString									m_strSoundFileName;

	//条件
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
