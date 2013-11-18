/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawQuoteTabCtrl.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ���鱨��Tabҳǩ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-04-01
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DiagramTab.h"
#include "DrawQuoteReportStruct.h"
#include "DataSourceDefineEx.h"
#include "ConfigInterface.h"
#include <afxtempl.h>
#include "QuoteLangMngInterFace.h"

#define CDrawQuoteTab_Type_Item		0x1000	// ��ͨ��ǩ
#define CDrawQuoteTab_Type_Guide	0x2000	// �򵼱�ǩ
#define CDrawQuoteTab_Type_Button	0x4000	// ��ť��ǩ

#define CDRAWTAB_ID_MENU_MARKET_BEGIN		100	// �л��г�����飩�˵�
#define CDRAWTAB_ID_MENU_MARKET_END			CDRAWTAB_ID_MENU_MARKET_BEGIN+1000 
#define CDRAWTAB_ID_MENU_BLOCK_BEGIN		CDRAWTAB_ID_MENU_MARKET_END+1
#define CDRAWTAB_ID_MENU_BLOCK_END			CDRAWTAB_ID_MENU_BLOCK_BEGIN+1000

#define CDRAWTAB_QUOTETAB_SETTING		    CDRAWTAB_ID_MENU_BLOCK_END + 1	// �����ǩҳ����ѡ��
#define CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN   CDRAWTAB_QUOTETAB_SETTING + 1	// �л���ǩҳ��
#define CDRAWTAB_QUOTETAB_TABGROUPS_END	    CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN + 50

class CDrawQuoteTabCtrl;
class CDrawQuoteTab
{
	friend CDrawQuoteTabCtrl;
public:	
	CDrawQuoteTab();
	~CDrawQuoteTab();
	enum
	{
		Tab_Pressed = 0x0001,
		Tab_Disabled = 0x0002,
	};

	BOOL DrawGuide(CDC* pDC, CRect& rect);
	BOOL DrawItem(CDC* pDC, CRect& rect, BOOL bPressed);
	BOOL DrawButton(CDC* pDC, CRect& rect, short nMask);

	PageInfo* GetPageInfo() {return &m_PageInfo;}
	CString   GetGroupName() {return m_strColGroupName;}
protected:	
	CString         m_strName;          // ����
	UINT			m_nID;				// ��ǩID
	long            m_lMenuMask;        // ���Ի���˵��
	PageInfo    	m_PageInfo;			// ����ҳ����Ϣ
	CString			m_strColGroupName;	// �ֶη�������
	DWORD			m_lMenuID;          // �˵�ID

	BOOL            m_bVisable;         // �Ƿ�ɼ� 
	CRect           m_rcTab;            // ��ǰ���������

private:
	static IHsColor*  m_pColor;         // ��ɫ�����ļ�ָ��
	static IHsFont*   m_pFont;          // ���������ļ�ָ��
};

//////////////////////////////////////////////////////////////////////////

interface IDrawQuoteTabNotify
{
public:
	virtual void  OnActiveTabChange(CDrawQuoteTab* pActiveItem) = 0;
	virtual void  TabInvalidate(CRect* pRect, BOOL bErase = TRUE) = 0;
	virtual CWnd* TabGetCWnd() = 0;
};

//////////////////////////////////////////////////////////////////////////

typedef CTypedPtrArray<CPtrArray, CDrawQuoteTab*> CDrawQuoteTabItemArray;

class CDrawQuoteTabCtrl
{
public:
	// ���ܰ�ť��
	enum 
	{ 
		TabFenLei  = 0x0001, // �����Ʊ
		TabGangGu  = 0x0002, // �۹�
		TabWaiPan  = 0x0004, // �ڻ�
		TabQiHuo   = 0x0008, // ����
		TabWaiHui  = 0x0010, // ���

		TabBlock   = 0x0020, // ���
		TabUsrDef  = 0x0040, // ��ѡ��
		TabSort    = 0x0080, // ����

		TabUsrDefine = 0x0100, // �Զ�����鵯���˵�,Ҳ���ǵ�ǰ�����������Զ���
		TabBourse    = 0x0200, // �Զ�����鵯���˵�,ָ��һ���г�

		//HsTCButton
		TabPrev = 0x0400, 
		TabNext = 0x0800,

		TabTitle   = 0x1000, // ����
		TabClose   = 0x2000, // �ر�
		TabOpertor = 0x4000, // ����(����)
		TabCode    = 0x8000, // ����

		TabNoIndexNoQH = 0x100000,		// ���಻��ʾָ�� ����ʾ�ڻ�
		TabMaxWin      = 0x200000,		// ��󻯴���
		TabRestoreWin  = 0x400000,		// ��ԭ����
		//
		TabF10HideRight = 0x1000000,

		TabHide        = 0x80000000,    // ��ǰtab����
	};

	CDrawQuoteTabCtrl();
	~CDrawQuoteTabCtrl(void);

	void Create();           // ��ʼ������
	void SetRect(CRect rc);  // ���ô�С

	BOOL ChangeTabGroup(CString strGroup = _T(""), BOOL bForce = FALSE);	// �ı䵱ǰ��ǩ��
	CString GetDefaultTabGroupName();	            // ��ȡĬ�ϱ�ǩ������
	// ����������ȡ����Ϣ��ӱ�ǩҳ
	LONG AddTabItem(CString strName, UINT nID, long lMenuMask = 0, PageInfo* pageInfo = NULL, CString strGroupName = _T(""));
	int  FindTabItem(CString strName);  // ���ݱ�ǩ���Ʋ�ѯ��ǩ���
	// �ı䵱ǰ��ǩҳ
	BOOL ChangeCurItem(CString strName, BOOL bNotify = TRUE, short nMarkType = 0);
	BOOL ChangeCurItem(int nIndex, BOOL bNotify = TRUE); 	
	// ʹָ����ǩҳ�ɼ�
	int EnSureVisible(CString strName);
	int EnSureVisible(int nIndex);

	void OnScrollTCItem(int nCount); // �ƶ���ǩҳƫ��   nCount>0�����ƶ�nCount��  ==0���ƶ�  <0�����ƶ�nCount��
	BOOL RemoveTabItem(int nItem);   // ɾ��tabҳǩ
	
	void DrawTab(CDC *pDC); // �������б�ǩҳ
	BOOL MouseMove(UINT nFlags, CPoint point);
	BOOL MouseDown(UINT nFlags, CPoint point);

	// �Ƿ�ѡ��
	int  HitTest(CPoint point, int& nIndex);

	void SetNotify(IDrawQuoteTabNotify* pDrawTabNotify){m_pDrawTabNotify = pDrawTabNotify;}
protected:
	int  OpenBlockMenu(CPoint point, BOOL bSysBlock);              // �򿪰��˵�
	int  OpenSecondBlockMenu(CString strMarketName, CPoint point); // �򿪶��ΰ���г��˵�
	int  OpenAllMarketMenu(CPoint point);                      // ��ȫ�г��˵�
	int  OpenSettingMenu(CPoint point);                        // �����ò˵�

	IDataSourceEx*          m_pDataSource;      // ��������ָ��
	IDrawQuoteTabNotify*    m_pDrawTabNotify;   // �ص�����ָ�� 

	CString                 m_strCurGroupName;  // ��ǰ��ǩ��
	CDrawQuoteTabItemArray  m_ayGuideItem;      // ����ҳ
	CDrawQuoteTabItemArray  m_ayItem;           // ��ǩҳ
	CDrawQuoteTabItemArray  m_ayButtonItem;     // ��ťҳ
	
	CRect                   m_rcClient;         // �����С

	short		            m_nMouseActiveItem;	// ����ƶ�����ļ���
	short		            m_nCurItem;	        // ��ǰ������
	short                   m_nCurButton;       // ��ǰ���ť
	short		            m_nLeftOffset;	    // ƫ�Ƶĵ�һ��
	short		            m_nRightOffset;	    // ��ʾ�����һ��

	CStringArray            m_ayTempGroups;     // �����ʾ��

	static IHsColor*        m_pColor;           // ��ɫ���ýӿ�
	static IHsTabCfg*		m_iTab;				// ����ģ��ӿ�
	static IBlockManager*	m_iBlockManager;	// ������ӿ�
	static IQuoteLang*	    m_iQuoteLang;		// ������Դ�ӿ�
	static CMap<UINT,UINT,CString,CString> m_mapTempMenuIndex; // ��Ų˵�ID��Ӧ���г����߰������
};