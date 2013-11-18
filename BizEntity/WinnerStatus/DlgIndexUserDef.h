#pragma once

#include "..\..\Common\hsstructnew.h"
#include "..\..\Common\DataSourceDefineEx.h"
#include "MyListBox.h"
#include "HSMacro.h"
#include "ConfigInterface.h"
// CDlgIndexUserDef 对话框


struct showList
{
	CodeInfo myCodeInfo;
	char     myStockName[strStructLenth];

	void SetEmpty()
	{
		myCodeInfo.SetEmpty();
		memset(myStockName, 0, strStructLenth);
	}
};

class CDlgIndexUserDef : public CDialog
{
	DECLARE_DYNAMIC(CDlgIndexUserDef)

public:
	CDlgIndexUserDef(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgIndexUserDef();

// 对话框数据
	enum { IDD = IDD_INDEX_USERDEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnLbnSelchangeList4();
	
	afx_msg void OnLbnSetfocusList1();
	afx_msg void OnLbnSetfocusList2();
	afx_msg void OnLbnSetfocusList3();
	afx_msg void OnLbnSetfocusList4();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	DECLARE_MESSAGE_MAP()

	//公共函数
public:
	void   InsertList(CMyListBox& myListBox, CArray<showList>& myShowList ,StockUserInfo aStock);

//属性
public:
	CArray<showList> m_oneList;   //第一列股票
	CArray<showList> m_twoList;   //第二列股票
	CArray<showList> m_threeList; //第三列股票
	CArray<showList> m_fourList;  //第四列股票

	BOOL m_threeShow; //标记第三列是否显示
	BOOL m_fourShow;  //标记第四列是否显示
	unsigned int m_scrollSpeed; //记录翻滚时间

private:
	//listbox成员变量
	CMyListBox m_myListBox1;
	CMyListBox m_myListBox2;
	CMyListBox m_myListBox3;
	CMyListBox m_myListBox4;
	int        m_nSelectIndex;
	
public:
	IDataSourceEx*	m_pDataSource;		// 数据引擎指针
	IBlockManager*  m_pBlockManager;    // 板块管理指针
};

