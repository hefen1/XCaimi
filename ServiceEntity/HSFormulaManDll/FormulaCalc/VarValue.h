/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	VarValue.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��������ͨ����(����,double)�İ�װ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/

#if !defined(AFX_VARVALUE_H__AC8E70D6_333D_11D2_89A8_00002145DF63__INCLUDED_)
#define AFX_VARVALUE_H__AC8E70D6_333D_11D2_89A8_00002145DF63__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/*#include "..\comm\HSexportbase.h"*/
#include "FxjFormulaStruct.h"
#include "Value.h"

/*************************************************************
*	�� �� �ƣ�	CNumericValue
*	�� �� ����	��ͨ����(����,double)�İ�װ��
***************************************************************/
class CNumericValue : public CValue  
{
	DECLARE_DYNCREATE(CNumericValue)

public:
	void			SetValueOnly(double value);//yulx add �������װ��ֵ(��ɾ��ԭ���Ŀռ�)
	/******************************************************************
	*	�������ܣ�	�������װ��ֵ(��ɾ��ԭ���Ŀռ䣬���·�����ٸ�ֵ)
	*	����������	double value : [in] ��ֵ
	*	�� �� ֵ��	void		  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	void		    SetValue(double value);

	/******************************************************************
	*	�������ܣ�	���ذ�װ��ֵ
	*	����������	HSCurDataInfoStatus* pCurDataStatus = NULL: [in] ����
	*	�� �� ֵ��	double  : ���ص�ֵ((dobule*)m_value);
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��16��
	*	�޸�˵����	
	*****************************************************************/
	virtual double  GetValue(HSCurDataInfoStatus* pCurDataStatus = NULL);

	// �������ֵ
	void		    SetMaxValue(double value);
	// �õ����ֵ
	virtual double  GetMaxValue(void);

	// ������Сֵ
	void		    SetMinValue(double value);
	// �õ���Сֵ
	virtual double  GetMinValue(void);

	// ���ò��Բ���
	void		    SetTestStepValue(double value);
	// �õ����Բ���
	virtual double  GetTestStepValue(void);

	// ���ö�����װ�����ݵ�����
	void		    SetDescribe(CString strDescribe);
	// �õ�������װ�����ݵ�����
	virtual CString GetDescribe();

	// ��ʼ��
	CNumericValue();
	// �����ռ��Դ
	virtual ~CNumericValue();

	// ���л�
	virtual void   Serialize(CArchive &ar);
	// �ӷ����ҹ�ʽ��ʽת��
	virtual void   Serialize(FxjParam* pParam);
	// COPY����,�����ڴ治����
	virtual void   Copy(CNumericValue* pRefValue);

protected:
	// ���ֵ
	void*	 m_pMax;
	// ��Сֵ
	void*	 m_pMin;
	// ����ϵͳ���Բ���
	void*    m_pTestStep; 
	// ע��
	CString* m_pDescribe; 
};

#endif // !defined(AFX_VARVALUE_H__AC8E70D6_333D_11D2_89A8_00002145DF63__INCLUDED_)
