/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	Value.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��ʽϵͳ�����ݻ�ָ���װ����Ļ���
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
***************************************************************/

#if !defined(AFX_VALOARE_H__5231D2C4_B344_11D1_866F_00002145DF63__INCLUDED_)
#define AFX_VALOARE_H__5231D2C4_B344_11D1_866F_00002145DF63__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ExpPublic.h"
#include "CompileInterFace.h"
// ����ʱ�õ���MASK
#define FindInName     0x200 // ���ļ����в���
#define FindInDescribe 0x400 // �������в���
#define FindInNotes    0x800 // ��ע���в���
#define FindInEidolon  0x1000 // �ڲ��ྫ���в���
#define FindInSource   0x2000 // �ڹ�ʽ��������в���
// ����������FIND
#define FindInAll      (FindInName | FindInDescribe | FindInNotes | FindInEidolon | FindInSource)


/*************************************************************
*	�� �� �ƣ�	CValue
*	�� �� ����	��ʽϵͳ�����ݻ�ָ���װ����Ļ���,���������������ʵ��,�������Ϳ�ҪӦ����
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
