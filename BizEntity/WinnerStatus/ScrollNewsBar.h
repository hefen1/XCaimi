#pragma once

#include "ConfigInterface.h"
#include "WinnerApplication.h"

//跑马灯滚动风格
enum { ScrollUp=1,ScrollDown,ScrollLeft,ScrollRight,ScrollSpeedUp };

// 图片在list中的位置
#define BMP_TEXT_INDEX      0 
#define BMP_CAPTION_INDEX   1
#define BMP_TOOL_INDEX      2

// CScrollNewsBar 对话框
class CScrollNewsBar : public CDialog
{
	DECLARE_DYNAMIC(CScrollNewsBar)

public:
	CScrollNewsBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScrollNewsBar();
	
// 对话框数据
	enum { IDD = IDD_SCROLLNEWSBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//////////////////////右键菜单/////////////////////// 
	afx_msg void OnFloat();    //浮动显示
	afx_msg void OnMultiline();//多行显示
	afx_msg void OnSetting();  //高级设置
	//鼠标事件
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	int MouseDown(UINT nFlags, CPoint point); //自定义鼠标事件

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

//
protected:
	BOOL ReadCfg();      // 读配置文件
	void WriteCfg();     // 写配置文件
	void SetSize();      // 设置绘制区域

	void DrawMyText( CDC* pDC);   // 绘制滚动文字
	void DrawMyTool( CDC* pDC);

	void InitRect(CDC* pDC);      // 初始化显示区域
	void CalculateRect(CDC* pDC); // 计算滚动区域

	BOOL HitTest(CPoint point);       // 判断鼠标位置是否在绘制区域里面
	BOOL HitCaption(CPoint point);    // 判断鼠标位置是否在标题区域里面
	BOOL HitTool(CPoint point);       // 判断鼠标位置是否在工具区域里面
	void Scrolling( int param1 );
public:
	void DrawMyCaption( CDC* MemDC);//绘制标题
public:
	//跑马灯位置属性
	BOOL  bScrollFixed;       //跑马灯是否固定在状态栏上
	BOOL  bScrollMultiLine;   //跑马灯是否多行显示
	int   m_nShowScrollAlone; //跑马灯显示的方式 
    //跑马灯里面的布局
	CRect m_rectCaption;      //跑马灯标题矩形
	CRect m_rectText;         //跑马灯滚动矩形
	CRect m_rectTool;         //跑马灯工具栏矩形

	//状态栏指针
	CWnd* m_pStatuBar;
//私有变量
private:
	CMenu menu;
	CPoint lastPoint;  //记录鼠标拖动前的位置

	CStringArray m_sTitleArray;    //记录跑马灯所要显示内容的标题
	CStringArray m_sContentArray;  //记录跑马灯所要显示内容的内容
	CStringArray m_sUrlArray;      //记录跑马灯所要显示内容的链接

	CArray<CRect> m_showDataCRectArray;// 记录显示内容的位置
	int  m_nShowIndex;                 // 当前显示的起始信息的下标
	BOOL m_bIsInitRect;                // 是否进行过绘制矩形的初始化

	COLORREF m_clrBk;           // 滚动背景颜色
	COLORREF m_clrText;         // 滚动字体颜色
	int      m_nSpeed;          // 滚动速度
	CFont*   m_pFontText; // 文本字体

	CImageList   m_lstImage; // 图片资源
	int m_nIsUpDownRate;    // 标记涨跌幅是否显示
	int m_nIsTotal;         // 标记总额是否显示
	int m_nIsShowNewPirce;  // 标记成交价是否显示
	int m_nIsUpDown;        // 标记涨跌是否显示

	int      m_nClickWay;   //鼠标单击选中方式 0-单击 1-双击
	int	     m_nHorizonScrollWay;  //记录水平滚动方向
	int      m_nVerticalScrollWay; //记录垂直滚动方向

	long     m_nTimeID;     //滚动时钟
	long     m_nTimeIDMouse;//鼠标判断时钟

	IHsColor*      m_pColorCfg;    // 颜色读写接口
	IHsFont*       m_pFontCfg;     // 字体读写接口
	IHsScrollNews* m_pScrollCfg;   // 跑马灯配置读写接口
};


