/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	FunExpresion.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	������ȫ��ϵͳ�����İ�װ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/
#if !defined(AFX_FUNEXPRESION_H__2E4DE2C5_A80A_11D4_A128_00E09833EFFA__INCLUDED_)
#define AFX_FUNEXPRESION_H__2E4DE2C5_A80A_11D4_A128_00E09833EFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunExpresion.h : header file
//

/*#include "..\comm\HSexportbase.h"*/

#include "Value.h"
#include "SystemFun.h"


#define FILENAME_HS_FUNCTION_HELP_CFG	"FunctionHelp.cfg"  // ��ʽ���������ļ�

/*************************************************************
*	�� �� �ƣ�	CFunExpresion
*	�� �� ����	ϵͳ����ָ���װ��
***************************************************************/
class CExpression;
class BCGCONTROLBARDLLEXPORT CFunExpresion : public CValue
{
	DECLARE_DYNCREATE(CFunExpresion)
public:
	// �����������
	virtual void    SetValue(void* value);
	virtual double  GetValue(void);
	virtual void    UpdateValue(void);
	// ���õ�ǰ������������
	virtual BOOL    SetParams(short nParams);
	// ���ú�����
	virtual BOOL    SetFunName(CString* pFunName);	
	// �õ���������
	short			GetParams()	  { return m_nParams;   }
	// �õ�����ID
	unsigned short  GetOperator() { return m_nOperator; }
	// �õ�������
	char*		    GetFunName();
	// ���ú����÷�˵��
	virtual void    SetNotes();
	// �õ������÷�˵��
	CString			GetNotes();

	// �õ���������
	CString		    GetDescribe();
	// ���ú�������
	void			SetDescribe();
	
	// ���ú�������,ͬʱ���ؾ�����
	FunctionType    SetFunType(FunctionType nType);
	// �õ���������
	FunctionType    GetFunType() { return m_nFunType; }
	// ���������ж�
	BOOL		    IsFunType(FunctionType nType) { return (m_nFunType == nType); }


	/******************************************************************
	*	�������ܣ�	ִ�з�װ��ϵͳ����
	*	����������	CExpression* pCompile	: [in] ��ʽ����,��Ҫ��Ϊ��ͨ�����õ�����Դ
	*				FormulaGeneralParam* ayVars : [in] �����б�
	*				HSCurDataInfoStatus* pCurDataStatus: [out] ���������
	*				double& dValue : [out] ����hx_DefMinValues��ʾ���㲻�ɹ�, 
										   ����hx_ReturnPointValues��ʾ����ɹ�,�������pCurDataStatus��,
										   ����0,һ���ʾ��Ӧ��ϵͳ����ûʵ��,
										   ���ع������ʾ����Ϊ������
	*	�� �� ֵ��	int : ���������Ϣ,Ϊ0��ʾ��ȷ
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	int				Evaluate(CExpression* pCompile,FormulaGeneralParam* ayVars,
							 HSCurDataInfoStatus* pCurDataStatus,double& dValue);

	CFunExpresion();
	CFunExpresion(unsigned short nOperator,
				  CString pFunName,short nParams,COMPILEFUNCTION curFn);

	// ���ռ�õ�˽�˿ռ�
	virtual ~CFunExpresion();
public:
	// �õ����巽ʽ(���뺯��,TIP��ʾ�����ʱ��)��������ļ���û����,��õ��ľ���SUM( , )��ʽ�İ���;
	int		GetFunDefMode(CString& strFun);
	// �õ��������巽ʽ(����һ�º���,TIP��ʾ�����ʱ��),��������ļ���û����,��õ��ľ��ǿ�;
	CString GetFunDefMode();

public:
	// ����Ŀǰ����
	BOOL EvaluateFunction(FormulaGeneralParam* ayVars); // 

protected:
	unsigned short m_nOperator;			// ����
	FunctionType   m_nFunType;          // ������������

	CString*	   m_pFunName;			// ��������
	short		   m_nParams;			// ��������

	CString*	   m_pNotes;		    // ����ע��
	CString*  	   m_strDescribe;	    // ��������

	COMPILEFUNCTION	   m_curFn;			// ����ָ��

public:
	// ����Ŀǰ����
	virtual void Serialize(CArchive& ar);

public:
	/******************************************************************
	*	�������ܣ�	��������,����������������˵�����Ƿ���strKey�Ӵ�.���ҹ�ʽʱ��.
	*	����������	CString strKey	: [in] Ҫ���ҵ�����
	*				CString& strRet	: [out] ����ҵ�,�򷵻غ������������Ϣ(����,˵������"-"�Ÿ���)
	*				DWORD dMask = FindInName | FindInDescribe : [in] ���ҵķ�Χ
	*	�� �� ֵ��	BOOL : �ڹ涨��Χ���ҵ��ִ��򷵻�TRUE,���򷵻�FALSE
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual BOOL Find(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe);

public:
	// ���ط�װ��ϵͳ����
	COMPILEFUNCTION	 GetCurFun()  { return m_curFn; }
};
// ȫ�ֵ�ϵͳ���������(���������ж���Ĺ�ʽ�ײ�ϵͳ����)
extern CSystemFun* g_pSystemFun;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNEXPRESION_H__2E4DE2C5_A80A_11D4_A128_00E09833EFFA__INCLUDED_)
