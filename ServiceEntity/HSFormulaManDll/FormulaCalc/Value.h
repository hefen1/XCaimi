/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	Value.h
*	文件标识：	
*	摘	  要：	公式系统将数据或指针封装成类的基类
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
***************************************************************/

#if !defined(AFX_VALOARE_H__5231D2C4_B344_11D1_866F_00002145DF63__INCLUDED_)
#define AFX_VALOARE_H__5231D2C4_B344_11D1_866F_00002145DF63__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ExpPublic.h"
#include "CompileInterFace.h"
// 查找时用到的MASK
#define FindInName     0x200 // 在文件名中查找
#define FindInDescribe 0x400 // 在描述中查找
#define FindInNotes    0x800 // 在注释中查找
#define FindInEidolon  0x1000 // 在参类精灵中查找
#define FindInSource   0x2000 // 在公式定义代码中查找
// 在所有里面FIND
#define FindInAll      (FindInName | FindInDescribe | FindInNotes | FindInEidolon | FindInSource)


/*************************************************************
*	类 名 称：	CValue
*	类 描 述：	公式系统将数据或指针封装成类的基类,这里各函数基本不实现,函数解释看要应子类
***************************************************************/
class BCGCONTROLBARDLLEXPORT CValue : public CObject  
{
public:
	DECLARE_DYNCREATE(CValue)
	
	virtual void   SetValue(void* value);
	
	virtual double GetValue(HSCurDataInfoStatus* pCurDataStatus = NULL);
	virtual BOOL   CalcValue(CLineProperty*& pRefData, int nStart,int nEnd,HSCurDataInfoStatus* pCurDataStatus = NULL){ return 0;}
	
	virtual void   UpdateValue(void);
	virtual void   Serialize(CArchive &ar);

	virtual class CExpression* GetExp() { return NULL; }
	virtual class CExpression* GetExpByCompile() { return NULL; }
	virtual UnionSelCondition* GetUnion() { return NULL; }
	virtual void    Free()  {}

	virtual void    Clean() {}
	virtual CString GetWholeName() { return ""; }

	CValue();
	virtual ~CValue();

public:
	virtual CString GetNotes()	{ return ""; };

	CValue *pNext;

protected:
	void*	m_value;

// style
protected:
	long	m_dStyle;
public:
	BOOL	IsStyle(long dStyle)		{ return (m_dStyle & dStyle); }
	void	AddStyle(long dStyle)		{ m_dStyle |= dStyle;         }
	void	RemoveStyle(long dStyle)	{ m_dStyle &= (~dStyle);      }
	void    SetStyle(long dStyle)		{ m_dStyle = dStyle;          }
	long    GetStyle(){return m_dStyle;}
public:
	virtual BOOL Find(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe) { return 0; }
};

#endif // !defined(AFX_VALOARE_H__5231D2C4_B344_11D1_866F_00002145DF63__INCLUDED_)
