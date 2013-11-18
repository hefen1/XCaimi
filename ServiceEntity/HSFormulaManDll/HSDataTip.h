//////////////////////////////////////////
//
#ifndef _HSDATATIP_H__
#define _HSDATATIP_H__

#include "ExportDll.h"

BCGCONTROLBARDLLEXPORT extern UINT TRACEBOX_BUTTON_EXPLOYER;
BCGCONTROLBARDLLEXPORT extern UINT TRACEBOX_BUTTON_PRE;
BCGCONTROLBARDLLEXPORT extern UINT TRACEBOX_BUTTON_NEXT;
BCGCONTROLBARDLLEXPORT extern UINT TRACEBOX_BUTTON_CLOSE;

BCGCONTROLBARDLLEXPORT extern UINT HS_OPEN_DATA_TIP_MSG; // 外部打开

#define CHSDataTip_ButMsg_Begin	0x1000
#define CHSDataTip_ButMsg_End		0x1100

#define CHSDataTip_Style_FuntionTip			0x00001 // 函数编辑提示

#define CHSDataTip_Style_FenshiTip			0x00002 // 时时信息地雷提示
#define CHSDataTip_Style_HistoryTip			0x00004 // 历史信息地雷提示

#define CHSDataTip_Style_ExpNotesTip		0x00008 // 公式注解提示
#define CHSDataTip_Style_ChuQuanTip			0x00010 // 除权提示

#define CHSDataTip_Style_TechTip	        0x00020 // 指标标记提示
#define CHSDataTip_Style_KLineTip		    0x00040 // 五彩k线标记提示
#define CHSDataTip_Style_MaiMaiTip		    0x00080 // 买入卖出标记提示
#define CHSDataTip_Style_SelStockTip	    0x00100 // 条件选股标记提示

#define CHSDataTip_Style_NotCloseTip	    0x10000 // 提示一直不关闭
#define CHSDataTip_Style_ClickOpenTip		0x20000 // 单击打开
#define CHSDataTip_Style_DlgOpenTip			0x40000 // dlg botton tip

#define CHSDataTip_Style_ButExployerTip		0x01000 // EXPLOYER
#define CHSDataTip_Style_ButPreTip			0x02000 // PRE
#define CHSDataTip_Style_ButNextTip			0x04000 // NEXT
#define CHSDataTip_Style_ButCloseTip		0x08000 // CLOSE
#define CHSDataTip_Style_IgnoreOriginPoint  0x100000   //不要求要两个点不相同

#define CHSDATATIP_STYLE_OPRNTIP_ATONCE		0x1000000 // 立刻打开TIP提示

// gbq add 20060207, 增加在选定点弹出方式
#define CHSDATATIP_STYLE_OPRNTIP_JUMP			0x2000000	// 弹出TIP提示, 指定当前位置而不是有该类获取
// 20060310, 增加弹出信息对齐方式
#define	CHSDATATIP_STYLE_JUMP_ALIGIN_LEFT		0x4000000
#define	CHSDATATIP_STYLE_JUMP_ALIGIN_RIGHT		0x8000000
#define	CHSDATATIP_STYLE_JUMP_ALIGIN_HCENTER	0x10000000
#define	CHSDATATIP_STYLE_JUMP_ALIGIN_TOP		0x20000000
#define	CHSDATATIP_STYLE_JUMP_ALIGIN_BOTTOM		0x40000000
#define	CHSDATATIP_STYLE_JUMP_ALIGIN_VCENTER	0x80000000
// gbq end

class CHSButton;
class BCGCONTROLBARDLLEXPORT CHSDataTip : public CWnd
{
	// Construction
public:
	CHSDataTip();
	DECLARE_DYNCREATE(CHSDataTip)

	// Attributes
public:
	void SetOffset(long x, long y) { m_offset = CPoint(x, y); }
	void SetOffset(CPoint offset)  { m_offset = offset; }

	// Operations
public:
	virtual BOOL Create(CWnd* pParentWnd,DWORD dStyle = 0);

	void Set(CPoint point, const CString& title = "无!",
		const CString& tip = "显示空!",
		int nWidChar = 25, int nRow = 3,
		CWnd* pWnd = NULL,
		DWORD dStyle = 0);

	void Hide( );

	// class operations
public:
	static void SetDelay(short delay)			 { _delay = delay; }
	static void SetMaxDispRow(short nMaxDispRow) { m_nMaxDispRow = nMaxDispRow; }

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHSDataTip)
public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CHSDataTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHSDataTip)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	long OpenDataTipMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()


protected:
	void Display( );

protected:
	CPoint  m_offset;
	CPoint  m_origin;

	BOOL    m_ready;

	UINT    m_timer;
	UINT    m_Killtimer;

	BOOL	m_bTipLocked;

protected:
	// Attributes
	CString		  m_Title;

	CString		  m_Text;
	CRect		  m_TextRect;

	CSize		  m_WinSize;

	// font
	CFont*		  m_pTitleFont;
	CFont*		  m_pFont;

	void  SetDispTextSize(CString text,int nWidChar = 25,int nRow = 3);

	// wave	
protected:
	BOOL	PlayPopSound();
	BOOL	m_bSoundOn;
	CString m_strSoundFile;
public:
	void SetPopupSound(BOOL bSoundOn,CString strSoundFile)
	{
		m_bSoundOn = bSoundOn;
		m_strSoundFile = strSoundFile;
	}

	//
protected:
	static void RegisterWnd( );
	static void Initialise( );

	static BOOL  _registered;
	static short _delay;
	static short m_nMaxDispRow;
	static short _count;
	static short m_nHidedelay;

	// keyboard hook
	static LRESULT CALLBACK KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam);	
	static HHOOK _hookProc;

	// mouse hook
	static LRESULT CALLBACK MouseProcCallback(int code, WPARAM wParam, LPARAM lParam);
	static HHOOK _hookMouseProc;

	//
	static CHSDataTip* _current;

	//add by lxqi 20090617	读配置判断是否记录用户鼠标和键盘操作
	static BOOL m_bIsRecordOpr;
protected:
	CEdit*	m_pEdit;
	DWORD   m_dStyle;

public:
	void  SetStyle(DWORD dStyle)    { m_dStyle = dStyle; } 
	void  AddStyle(DWORD dStyle)    { m_dStyle |= dStyle; }
	void  RemoveStyle(DWORD dStyle) { m_dStyle &= ~dStyle; }
	BOOL  IsStyle(DWORD dStyle)     { return ((m_dStyle & dStyle) == dStyle); }

public:
	virtual void SetCurFontClr(CFont* pFont,CFont*	pTitleFont,
		COLORREF*      BackColorRef	     ,
		COLORREF*      TextColorRef	     ,
		COLORREF*      TitleBackColorRef ,
		COLORREF*      TitleTextColorRef );

	//virtual void SetSize();
	virtual void SetSize(CPoint pt = (0,0));	// gbq modify 20060313

public: // != NULL
	virtual COLORREF* GetBkColor();
	virtual COLORREF* GetFgColor();
	virtual COLORREF* GetBkTitleColor();
	virtual COLORREF* GetFgTitleColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


protected:
	CMapWordToOb  m_ayBut;

public:
	CHSButton*  GetButton(WORD key);
	virtual void AddButton(WORD key);
	virtual void RemoveAllButton();

	virtual void OnButtonMsg(UINT nID);

	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	///CPoint m_oldPoint;
	//CRect m_saveRect;
};

extern CHSDataTip* HSGetTipWnd();

extern long HSOpenTip(CPoint point,CString strTitle,CString strDisp,
					   DWORD dwMask = 0,
					   CWnd* pWnd   = NULL,
					   void* pData  = NULL);

extern void HSSetTipFontClr(CFont* pFont,CFont*	pTitleFont,
							 COLORREF*      BackColorRef	     ,
							 COLORREF*      TextColorRef	     ,
							 COLORREF*      TitleBackColorRef ,
							 COLORREF*      TitleTextColorRef );

#endif

