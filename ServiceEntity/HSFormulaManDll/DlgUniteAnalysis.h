/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgUniteAnalysis.h
*	�ļ���ʶ��	"��������"������ģ��
*	ժ	  Ҫ��	���ﲻ��ʹ��,ȥ��---
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once

//#define		
#include	"resource.h"
#include	"BaseListCtrl.h"
#include "hsstructnew.h"
#include "ExpPublic.h"
//#include	"..\comm\stockmanager.h"


int CALLBACK UniteAnalyseCompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);

class CUniteLineData			//ÿһ�е����ݽṹ,����ʱ��
{
public:
	StockUserInfo	*m_pStockA;
	StockUserInfo	*m_pStockB;
	StockUserInfo	*m_pStockH;

public:
	CUniteLineData(StockUserInfo* StockA = NULL, 
		StockUserInfo* StockB = NULL, 
		StockUserInfo* StockH = NULL
		);
	~CUniteLineData();
	void Free();
	void Save(CFile *pFile, int nKeyIndex);
	BOOL Read(CString strFileName, CString strKeyIndex);
	StockUserInfo* CodeInfoToStockUserInfo(CodeInfo * pCodeInfo);
};

struct UniteColData			//ÿһ�е�����
{
	WORD	nID;
	CString	strColName;
	UniteColData()
	{
		nID = -1 ;//AU_CODE_ID;
	}
	void Serialize(CArchive& ar)
	{
		if( ar.IsStoring() )
		{
			ar << nID;			// ֤ȯ����
			ar << strColName;
		}
		else
		{
			ar >> nID;			// ֤ȯ����
			ar >> strColName;
		}
	}
};

struct DisplayLineData			//Ҫ��ʾ��������
{
	CStringArray	m_ayStrData;
	CUniteLineData* m_pUniteLineData;
	int				m_nCurIndex;
	DisplayLineData(CUniteLineData* pLine = NULL, int nSize = 0)
	{
		m_pUniteLineData = pLine;
		m_ayStrData.SetSize(nSize);
	}
	~DisplayLineData()
	{
		m_ayStrData.RemoveAll();
		if( m_pUniteLineData != NULL )
			m_pUniteLineData = NULL;
	}
	void CaseData(DisplayLineData* pLine)
	{
		CStringArray ayTemp;
		CUniteLineData* pTemp = pLine->m_pUniteLineData;
		int				nTemp = pLine->m_nCurIndex;

		ayTemp.Copy(m_ayStrData);
		m_ayStrData.RemoveAll();
		m_ayStrData.Copy(pLine->m_ayStrData);
		pLine->m_ayStrData.RemoveAll();
		pLine->m_ayStrData.Copy(ayTemp);

		pLine->m_pUniteLineData = m_pUniteLineData;
		m_pUniteLineData = pTemp;
		pLine->m_nCurIndex = m_nCurIndex;
		m_nCurIndex = nTemp;
	}
};

struct ListSortParam
{
	DisplayLineData**	pListData;
	int					nCurSel;
	BOOL				bSortFlag; //FALSE ���� TURE ��
	ListSortParam(DisplayLineData**pList = NULL, int nCur = 0, BOOL bFlag = FALSE)
	{
		pListData = pList;
		nCurSel = nCur;
		bSortFlag = bFlag;
	}
};

// CDlgUniteAnalysis dialog
class CDlgUniteAnalysis : public CDialog
{
	DECLARE_DYNAMIC(CDlgUniteAnalysis)

public:
	CDlgUniteAnalysis(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUniteAnalysis();

// Dialog Data
	enum { IDD = IDD_UNITE_ANALYSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	long OnMsgFun(WPARAM wp,LPARAM lp);

	DECLARE_MESSAGE_MAP()
public:
	CArray<CUniteLineData*, CUniteLineData*>		m_ayTabData;
	CArray<UniteColData*, UniteColData*>		m_ayColData;
	CArray<DisplayLineData*, DisplayLineData*>	m_ayDisplayData;

protected:
	int		m_nCurCol;
	HSOpenExpressUseData*	m_pData;
	CString					m_strStockIniFileName;
	CString					m_strColIniFileName;
	BYTE					m_bCurSortNum;
	BOOL					m_bCurSortFlag;				//FLASE ��, TRUE ��

public:
	void OnMenuMsg(UINT nID);
	void SaveStockIni();								//	��ȡ��Ʊ����
	void ReadStockIni();
	void SaveColIni();									//	��ȡ������
	void ReadColIni();									
	void ReDraw();										//	���»�����
	void ReDrawHandCtrl();								//	��ʼ����
	void ReCompute(int nCol);							//	���¼���nCol��,����������m_ayDisplayData
	void ComputeAll();							
	void DelColAt(int nColNum);							//ɾ��һ��
	void AppendCol(UniteColData * pUniteCOl);			//��һ��
	void ChageCol(UniteColData * pUniteCOl, int nCol = -1);	//�ı�һ������
	void DrawCurCol(int nCol);
	int	 IdToType(int nID);

	// �б�
	CBaseListCtrl m_ctrlList;
	virtual BOOL OnInitDialog();

	double	Compute(LPCSTR strCode, DWORD dwID);

protected:
	virtual void OnCancel();

public:
	CString GetNameByKey(UINT nKey);

	afx_msg void OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void PostNcDestroy();
public:
//	afx_msg void OnHdnItemdblclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
