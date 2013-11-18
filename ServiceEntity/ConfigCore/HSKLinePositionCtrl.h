#pragma once
/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsKLineTab.h
* 摘    要：K线tab配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2011-5-14
* 备	 注：
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
	virtual int			GetTechLines(int nType, CStringArray& ayStrRet, BOOL bResearch = FALSE);//获取位置控制前的所有图元
	virtual float		GetPosCtrolRatio(int nType);										//获取位置控制的比例
	virtual int			SetTechLines(int nType, CStringArray& strKLIneName, 
							BOOL bFirst = FALSE, BOOL bLast = FALSE);//写入图元
	virtual int			SetPosCtrolRatio(int nType, float fRatio);							//写入位置控制

};