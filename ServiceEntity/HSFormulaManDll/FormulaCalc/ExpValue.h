/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExpValue.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	�����˹�ʽ����İ�װ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
***************************************************************/

#if !defined(AFX_EXPVALUE_H__AC8E70D5_333D_11D2_89A8_00002145DF63__INCLUDED_)
#define AFX_EXPVALUE_H__AC8E70D5_333D_11D2_89A8_00002145DF63__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxmt.h>

/*#include "..\comm\HSexportbase.h"*/

#include "Value.h"
#include "FxjFormulaStruct.h"
#include "CompileInterFace.h"
//#include "UnionSelCondition.h"

/*************************************************************
*	�� �� �ƣ�	CExpValue
*	�� �� ����	��ʽ����İ�װ��
***************************************************************/
class BCGCONTROLBARDLLEXPORT CExpValue : public CValue  
{
	DECLARE_DYNCREATE(CExpValue)

public:

	/******************************************************************
	*	�������ܣ�	���±�������װ�Ĺ�ʽ
	*	����������	void
	*	�� �� ֵ��	void  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual void   UpdateValue(void);
	

	/******************************************************************
	*	�������ܣ�	����װ�Ĺ�ʽ���м���
	*	����������	HSCurDataInfoStatus* pCurDataStatus = NULL : [out] ������
	*	�� �� ֵ��	double : ����hx_DefMinValues��ʾ���㲻�ɹ�, 
							 ����hx_ReturnPointValues��ʾ����ɹ�,�������pCurDataStatus��,
							 ���ع������ʾ����Ϊ������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual double GetValue(HSCurDataInfoStatus* pCurDataStatus = NULL);

	/******************************************************************
	*	�������ܣ�	����װ�Ĺ�ʽ���м���,������������������(��m_pRefCalcValuesָ��ʵ��)
	*	����������	CLineProperty*& pRefData: [out] ���������
	*				int nStart				: [in] ���㿪ʼλ��
	*				int nEnd				: [in] �������λ��
	*				HSCurDataInfoStatus* pCurDataStatus = NULL: Ŀǰû��
	*	�� �� ֵ��	BOOL  : �����Ƿ�ɹ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	//******************************������litao����*****************************************
	virtual BOOL   CalcValue(CLineProperty*& pRefData,int nStart,int nEnd,HSCurDataInfoStatus* pCurDataStatus = NULL,
		bool bEnd = false,CArray<CLineProperty*,CLineProperty*>* pAyData=NULL);
	//******************************������litao����*****************************************


	/******************************************************************
	*	�������ܣ�	����װ��ʽ�Ĺ�ʽ�����ڲ���ͼ��ʽ�б��ɾ��,�Լ���������ý���Ͳ����б��ɾ��
	*	����������	���ޣ�
	*	�� �� ֵ��	void	  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual void	   Free();
	// ��������װ�Ĺ�ʽ
	class CExpression* GetExp();
	// �������Ϲ�ʽ
	virtual UnionSelCondition*  GetUnion();
	// ���ر����Ĺ�ʽ
	class CExpression* GetExpByCompile();

	// �õ���ʽ�����ϵ���������(��ʽ����������ǰ������˹�ʽ����������ͺ�)
	virtual CString GetWholeName();

	// ��ʼ��
	CExpValue();
	// ���ռ�õ���Դ
	virtual ~CExpValue();
	
	// ���л�,����
	virtual void   Serialize(CArchive &ar);
	virtual void   Serialize(FxjExpress*  pFxjExpress);
	virtual void   Serialize(CExpression* pExpress);

	// ������������
	enum ReNameState { Fail/*��������ȷ*/,Equal/*ͬ��*/,Exist/*�����Ѵ���*/,SysExist/*�����Ѵ�����ȫ�ֵ��ⲿ����ʽ��*/,NoFind/*�ɹ�ʽ����ȫ�ֵ��ⲿ����ʽ�б���*/,Succ /*�ɹ�*/};

	/******************************************************************
	*	�������ܣ�	��������װ�Ĺ�ʽ��
	*	����������	CString strNewName: [in] �µĹ�ʽ��
	*				CMapVariabile* pExternExpression: [in] ��ʽ���ڵı�
	*	�� �� ֵ��	int : �������(enum ReNameState)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	int ReName( CString strNewName,CMapVariabile* pExternExpression );

	void RemoveAllNo(int iCompExec);

protected:
	CTValues<void>		m_pRefCalcValues; // ���ʽ�Ѽ���Ľ��(��Ҫ���ڼ�ӱ��ʽ)
	CMapVariabile*		m_pParam;		  // ��ز�����
										/* ���򣺲���1 -> ͼ�λ��Ʒ�ʽ(ȴʡΪ����)
												 ����2 -> ͼ�λ�����ɫ(ȴʡΪ�Զ�ȡ)
												 ����3 -> ͼ�λ��Ƶ��߷��(ȴʡΪ��)
												 ����4 -> ͼ�λ��Ƶ��߿��(ȴʡΪ�Զ�ȡ)											
										*/

public:
	// ����&m_pRefCalcValues
	CTValues<void>*	GetRefCalcValues();
	// ����m_pParam// ��ز�����
	CMapVariabile*  GetParam();		 	
	// ��һ���������ز�����
	void			AddParam(CValue* pExpress);
	CString	 m_LineName;       //��ǰ��ֵ���ı�ʶ
	bool	 m_InWhile;		   //��ǰ��ֵ����Ƿ���ѭ����	
	bool     m_CurExecInWhile;  //��ǰִ������Ƿ���ѭ�����ˣ���ֹѭ���ڵ������ѭ�����ֱ����á���Ϊwhile������ѭ�������֮ǰ��
	int		 m_WhileLevel;		//��ǰ��������ǵڼ����WHILEѭ��
	int		 m_CurExecWhileLevel;		//��ǰ��������ǵڼ����WHILEѭ��

public:
	void SetMaxObjNo(CString key,int iCompExec);

public:
	// ���ռ�õ���Դ
	void Clean();

public:
	/******************************************************************
	*	�������ܣ�	�����Ĺ�ʽ��,��ʽ������ʽ������˵�����Ƿ���strKey�Ӵ�.���ҹ�ʽʱ��.
	*	����������	CString strKey	: [in] Ҫ���ҵ�����
	*				CString& strRet	: [out] ����ҵ�,�򷵻غ������������Ϣ(����,˵������"-"�Ÿ���)
	*				DWORD dMask = FindInName | FindInDescribe : [in] ���ҵķ�Χ
	*	�� �� ֵ��	BOOL : �ڹ涨��Χ���ҵ��ִ��򷵻�TRUE,���򷵻�FALSE
	*	����������	CExpression::IsFind()
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	virtual BOOL Find(CString strKey,CString& strRet,DWORD dMask = FindInName | FindInDescribe);

	/******************************************************************
	*	�������ܣ�	����ȫ�ֵ�ָ��ȽϷ�ʽ������, С�ڣ� ����...��
	*	����������	int& nCount: [out] �ȽϷ�ʽ�ĸ���
	*	�� �� ֵ��	char** : �ȽϷ�ʽ���ı�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	static char** GetConditionStr(int& nCount);
};

extern void    AddPeriodNameToWnd(CComboBox* pBox);
extern int     GetPeriodNumByName(CString strPeriodName);
extern CString GetPeriodStr(WORD dPeriod,int* nPos = NULL);

CMap<CString,LPCTSTR,LPEXECOBJNO,LPEXECOBJNO> m_pCompNo; // ִ��ʱ��ʶ�����к�
CMap<CString,LPCTSTR,LPEXECOBJNO,LPEXECOBJNO> m_pExecNo; // ִ��ʱ��ʶ�����к�
#endif // !defined(AFX_EXPVALUE_H__AC8E70D5_333D_11D2_89A8_00002145DF63__INCLUDED_)
