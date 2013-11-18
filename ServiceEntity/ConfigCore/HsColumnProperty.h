/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsColumnProperty.h
* ժ    Ҫ�����鱨��������ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-22
* ��	 ע��
*******************************************************************************/

#pragma once
#include "ConfigBase.h"
#include "ConfigStruct.h"
#include "ConfigInterface.h"

class CHsColumnProperty : public IHsColumnProperty
{
public:
	CHsColumnProperty();
	~CHsColumnProperty();

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

private:

	void InitAySys(); // ��ʼ�� ϵͳ��Tag���� ��ϵͳ��name������

	// ����strPath���ڵ���name����ΪstrName�ĺ��ӽڵ�
	TiXmlNode* GetNodeByName(CConfigBase* pConfig, CString strPath, CString strName);
	//��DispColumn�ļ��� ͨ��name����ֵ ���ҽڵ� ��:<stock>��<Fund>
	TiXmlNode* GetNodeByNameFromDisp(CString strName);
	//��ReportColumn�ļ��� ͨ��name����ֵ ���ҽڵ� ��:<StockQuote>��<StockQuoteEx>
	TiXmlNode* GetNodeByNameFromReport(CString strName);
	
	// ͨ���ڵ��Tagֵ��ȡ�ڵ��Name����ֵ �Ҳ����򷵻�strName
	CString GetGroupNameByTagName(CString strName);
	// ͨ���ڵ��Name����ֵ��ȡ�ڵ��Tagֵ �Ҳ����򷵻�strName
	CString GetTagNameByGroupName(CString strName);
	// ����strPath���ڵ��������ӽڵ�name����ֵ 
	long    GetGroupName(CConfigBase* pConfigBase, CString strPath, CStringArray& ayGroup);

	TableColProperty*	GetColumn(TiXmlNode* pNode);  	// ��ȡ������
//�ӿ�ʵ��
public:
	virtual TableColProperty*	GetColumn(int nID);  	// ��ȡ������
	virtual CString		GetDefDispColGroupName();		    // ��ȡĬ����ʾ��������
	virtual CString		GetDefaultColGroupName();		    // ��ȡĬ���������� ��ɾ���ȱ���
	virtual long		GetAllColumnGroupName(CStringArray& ayGroup);		// ��ȡ������������
	virtual CColPropArray*   GetColumnGroupProp(CString strGroupName);		// ��ȡָ���������������� 
	virtual BOOL             GetColumnGroupPropEx(CColPropIDIndex &mapProp, CString strGroupName);    // ����ͬ�� ����ֵ��ͬ
	virtual CString		GetFinanceGroupName();  			// ��ȡ����������������
	
	virtual long		GetAllDispColTemplName(CStringArray& ayGroup);	    // ��ȡ������ʾ��ģ������
	virtual RefreshDispTextProp* GetQuoteRefreshProp();   // ��ȡˢ�����Խṹ

	virtual CString		GetColumnName(int nID);       // ��ȡ������
	virtual long		GetColumnMask(int nID);       // ��ȡ������
	virtual long		GetColumnWidth(int nID);      // ��ȡ�п��

	//��ȡ��ʾ���еĸ����е����� ayGroup����������ColumnID��ż������ColumnWidth
	virtual long        GetDispColGroup(CString strGroup, CUIntArray*& pAyGroup);  
	virtual void		SetDispColGroup(CString strGroup, CUIntArray* pAyGroup);	// ������ʾ��������
	virtual BOOL		AddDispColGroup(CString strGroup, CUIntArray* pAyGroup);	// �����ʾ����
	virtual BOOL		DelDispColGroup(CString strGroup);		// ɾ����ʾ����
private:
	CConfigBase*  m_pReportColumnCfg;
	CConfigBase*  m_pDisplayColumnCfg;
	BOOL m_bInited;

	//����ͬһ���±������ֵ ����ͬһ�ڵ�
	CStringArray    m_aySysTagName;             // ϵͳ��Tag����
	CStringArray    m_aySysAttrName;            // ϵͳ��name������

	CMapWordToPtr   m_mapColIDIndex;			// ��ID������������ (TableColProperty*)
	CMapStringToPtr m_mapColPropGroup;          // ������(������) (CColPropArray*)
	CMapStringToPtr m_mapShowColGroup;          // ��ʾ���������   (CUIntArray*)
	RefreshDispTextProp m_sRefreshDispTextProp; // ���鱨��ˢ����ʽ�ṹ
};
