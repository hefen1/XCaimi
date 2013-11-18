#pragma once

#include "ConfigInterface.h"
#include "quotestuctdefine.h"
#include "datasourcedefineex.h"

#define QUOTETAB_NOTIFY					WM_USER + 200	// ��ǩҳ�ؼ�֪ͨ��Ϣ
#define QUOTETAB_SETTING				WM_USER + 201	// �����ǩҳ����ѡ��
#define QUOTETAB_ADDTEMP_MENUINDEX		WM_USER + 261	// ���һ����ʱ�˵���Ӧ��

#define ID_MENU_MARKET_BEGIN			100	// �л��г�����飩�˵�
#define ID_MENU_MARKET_END				ID_MENU_MARKET_BEGIN+1000
#define ID_MENU_BLOCK_BEGIN				ID_MENU_MARKET_END+1
#define ID_MENU_BLOCK_END				ID_MENU_BLOCK_BEGIN+1000
#define QUOTETAB_TABGROUPS_BEGIN		ID_MENU_BLOCK_END + 100	// �л���ǩҳ��
#define QUOTETAB_TABGROUPS_END			QUOTETAB_TABGROUPS_BEGIN + 50

#define CQuoteTab_Type_Item				0x00000000	// ��ͨ��ǩ
#define CQuoteTab_Type_Guide			0x10000000	// �򵼱�ǩ(���)
#define CQuoteTab_type_Extre			0x20000000	// ��չ��ǩ(�Ҳ�)

#define CQuoteTab_ALIGN_LEFT			0x00000010	// ��
#define CQuoteTab_ALIGN_TOP				0x00000020	// ��
#define CQuoteTab_ALIGN_RIGHT			0x00000030	// ��
#define CQuoteTab_ALIGN_BOTTOM			0x00000040	// ��

#define CQuoteTab_Show_Auto_TILE		0x00000100	// ��ǩ��ƽ��

#define TCIS_DISABLE					0x0004		// ��ǩҳ��ť������

class CQuoteTabCtrl;
struct HsTCItem
{
	UINT	mask;			// ��ǰ��ǩ�����Ի���˵��

	CRect   rc;				// ��ǰ��ǩ��Χ��ÿ��Draw��ʱ�򻮶���Χ
	CString pszText;		// ��ǩ����
	int		iImage;			// ��ǩͼ��(������������֮ǰ)
	LPARAM  lParam;			// ��ǩ����

	int		m_nSubImage;	// ��ǩ��ͼ��(������������֮��)
	int		m_nOtherMask;	// �رմ���ʱ���Զ�ɾ��tab��

	HsTCItem()
	{
		rc.SetRectEmpty();

		mask    = 0;
		iImage  = -1;
		lParam  = NULL;
		m_nSubImage = -1;
		m_nOtherMask = 0;
	}
	BOOL Draw(CDC* pDC,CRect& rect,int nMask,int nWidth = 0,int nTabType = 1);
	virtual BOOL Operator(CWnd* pParentWnd,int nMouse);

	void  Serialize(CArchive& ar,CWnd* pParent);
};

struct HsTCButton:public HsTCItem
{
	enum
	{
		TabPrev			= 0x0400, 
		TabNext			= 0x0800,

		TabTitle		= 0x1000, // ����
		TabClose		= 0x2000, // �ر�
		TabOpertor		= 0x4000, // ����(����)
		TabCode			= 0x8000, // ����

		TabNoIndexNoQH	= 0x100000,		// ���಻��ʾָ�� ����ʾ�ڻ�
		TabMaxWin		= 0x200000,		// ��󻯴���
		TabRestoreWin	= 0x400000,		// ��ԭ����
	};
	BOOL DrawButton(CDC* pDC, CRect& rect, int nMask, int nTabType = 1);
	BOOL Operator(CWnd* pParentWnd,int nMouse);
};

struct TabItemData
{
	UINT			m_nID;				// ��ǩID
	DWORD			m_dStyle;			// ��ǩ��ʽ
	PageViewInfo	m_PageInfo;			// ����ҳ����Ϣ
	CString			m_strColGroupName;	// �ֶη�������
	DWORD			m_lMenuID;

	TabItemData()
	{
		m_dStyle = 0;
		m_lMenuID = 0;
	}
	~TabItemData()
	{
	}
};

struct TempMenuIndex		// ��ʱ�˵���Ӧ��
{
	UINT m_nID;
	CString m_strMenuText;
};

// CQuoteTabCtrl

class CQuoteTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CQuoteTabCtrl)

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

public:
	CQuoteTabCtrl(IDataSourceEx* pDataSource, long nDataSourceID);
	virtual ~CQuoteTabCtrl();

	// ����������ȡ����Ϣ��ӱ�ǩҳ
	LONG AddTabItem(CString strName, UINT nID, DWORD dStyle = 0, PageViewInfo* pageInfo = NULL, CString strGroupName = _T(""));
	
	// �����ǩ��
	LONG InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask = 0);
	// ɾ����ǩ��
	BOOL DeleteItem(int nItem);
	// ��ȡ��ǩ��
	HsTCItem* GetItem(int nItem);
	// ��ȡ��ǩ�����
	LPARAM GetItemParam(int nItem);

	// ���뵼����
	LONG InsertGuideItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask = 0);

	// �����Ҳఴť
	LONG InsertButtonItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask = 0);

	BOOL ChangeCurItem(int nIndex, BOOL bNotify = TRUE);
	BOOL ChangeCurItem(CString strName, BOOL bNotify = TRUE, short nMarkType = 0);

	// ���ݱ�ǩ���Ʋ�ѯ��ǩ���
	int FindTabItem(CString strName);

	// ʹָ����ǩҳ�ɼ�
	int EnSureVisible(CString strName);
	int EnSureVisible(int nIndex);
	// �ı䵱ǰ��ǩ��
	BOOL ChangeTabGroup(CString strGroup = _T(""));
	// ��ȡĬ�ϱ�ǩ������
	CString GetDefaultTabGroupName();

	// �������б�ǩҳ
	void DrawTab(CDC *pDC);

	// �Ƿ�ѡ��
	int  HitTest(CArray<HsTCItem*,HsTCItem*>& ay,CPoint point);
	int  HitTest(CArray<HsTCButton*,HsTCButton*>& ay,CPoint point);

	// ������ʽ����
	void  SetStyle(DWORD dStyle)    { m_nStyle = dStyle; } 
	void  AddStyle(DWORD dStyle)    { m_nStyle |= dStyle; }
	void  RemoveStyle(DWORD dStyle) { m_nStyle &= ~dStyle; }
	BOOL  IsStyle(DWORD dStyle)     { return ((m_nStyle & dStyle) == dStyle); }

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CArray<HsTCItem*,HsTCItem*> m_ayGuideItem;   // ����
	CArray<HsTCItem*,HsTCItem*> m_ayItem;        // tab ��
	CArray<HsTCButton*,HsTCButton*> m_ayButtonItem;  // �ұ߰�ť��

	HsTCItem	m_LeftArrow;
	HsTCItem	m_RightArrow;

	short		m_nMouseActiveItem;		// ����ƶ�����ļ���
	short		m_nCurButton;			// ��ǰ���ť
	short		m_nLeftOffset;			// ƫ�Ƶĵ�һ��
	short		m_nRightOffset;			// ��ʾ�����һ��

	CString		m_strCurGroupName;			// ��ǰ��ǩ������
	long		m_nStyle;
	UINT		m_nTimerID;

	CStringArray m_ayTempGroups;


public:
	short		m_nCurItem;				// ��ǰ������

	static IHsTabCfg*		m_iTab;				// ����ģ��ӿ�
	static IHsColor*        m_iColor;           // ��ɫģ��ӿ�
	static IHsFont*			m_iFont;            // ����ģ��ӿ�
	static IBlockManager*	m_iBlockManager;	// ������ӿ�

	static IDataSourceEx*		m_pDataSource;				// ��������ӿ�ָ��
	static long				m_nDataSourceID;			// ��������ID

	static CMap<UINT,UINT,CString,CString> m_mapTempMenuIndex;
	// ��Ϣ������
public:
	void OnScrollTCItem(int nCount);			// �ƶ���ǩҳƫ��   nCount>0�����ƶ�nCount��  ==0���ƶ�  <0�����ƶ�nCount��
	void OnButtonClickMsg(UINT nID);
	void OnTabSetting();
	void OnGuideItemMarketMenuMsg(UINT nID);
	void OnGuideItemBlockMenuMsg(UINT nID);
	void OnTabGroupChangeMenuMsg(UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


