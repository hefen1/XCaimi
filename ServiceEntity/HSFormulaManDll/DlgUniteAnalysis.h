/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgUniteAnalysis.h
*	文件标识：	"联动分析"功能主模块
*	摘	  要：	这里不再使用,去除---
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
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

class CUniteLineData			//每一行的数据结构,计算时用
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

struct UniteColData			//每一列的数据
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
			ar << nID;			// 证券类型
			ar << strColName;
		}
		else
		{
			ar >> nID;			// 证券类型
			ar >> strColName;
		}
	}
};

struct DisplayLineData			//要显示的行数据
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
	BOOL				bSortFlag; //FALSE 减， TURE 增
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
	BOOL					m_bCurSortFlag;				//FLASE 减, TRUE 增

public:
	void OnMenuMsg(UINT nID);
	void SaveStockIni();								//	存取股票配置
	void ReadStockIni();
	void SaveColIni();									//	存取列设置
	void ReadColIni();									
	void ReDraw();										//	重新画数据
	void ReDrawHandCtrl();								//	初始化列
	void ReCompute(int nCol);							//	重新计算nCol列,计算结果放在m_ayDisplayData
	void ComputeAll();							
	void DelColAt(int nColNum);							//删除一列
	void AppendCol(UniteColData * pUniteCOl);			//加一列
	void ChageCol(UniteColData * pUniteCOl, int nCol = -1);	//改变一列属性
	void DrawCurCol(int nCol);
	int	 IdToType(int nID);

	// 列表
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
