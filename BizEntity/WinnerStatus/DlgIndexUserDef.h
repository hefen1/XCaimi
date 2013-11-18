#pragma once

#include "..\..\Common\hsstructnew.h"
#include "..\..\Common\DataSourceDefineEx.h"
#include "MyListBox.h"
#include "HSMacro.h"
#include "ConfigInterface.h"
// CDlgIndexUserDef �Ի���


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
	CDlgIndexUserDef(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgIndexUserDef();

// �Ի�������
	enum { IDD = IDD_INDEX_USERDEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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

	//��������
public:
	void   InsertList(CMyListBox& myListBox, CArray<showList>& myShowList ,StockUserInfo aStock);

//����
public:
	CArray<showList> m_oneList;   //��һ�й�Ʊ
	CArray<showList> m_twoList;   //�ڶ��й�Ʊ
	CArray<showList> m_threeList; //�����й�Ʊ
	CArray<showList> m_fourList;  //�����й�Ʊ

	BOOL m_threeShow; //��ǵ������Ƿ���ʾ
	BOOL m_fourShow;  //��ǵ������Ƿ���ʾ
	unsigned int m_scrollSpeed; //��¼����ʱ��

private:
	//listbox��Ա����
	CMyListBox m_myListBox1;
	CMyListBox m_myListBox2;
	CMyListBox m_myListBox3;
	CMyListBox m_myListBox4;
	int        m_nSelectIndex;
	
public:
	IDataSourceEx*	m_pDataSource;		// ��������ָ��
	IBlockManager*  m_pBlockManager;    // ������ָ��
};

