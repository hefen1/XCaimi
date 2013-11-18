// StatuBar.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "StatuBar.h"
#include "DlgIndexUserDef.h"
#include "Memdc.h"
#include "..\..\Common\WinnerApplication.h"
#include "QuoteLangMngInterFace.h"
#include "..\..\Resource\QuoteLang\Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CStatuBar 对话框

IMPLEMENT_DYNAMIC(CStatuBar, CHSBizDialog)

CStatuBar::CStatuBar(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CStatuBar::IDD, pParent)
{

	//初始化一些信息 
	m_rectScrollFloat.SetRectEmpty();
	m_nShowIndex = 0;
 	m_ayConnectNotify.RemoveAll();
	m_lServerTime = 0;
	m_dwTick = 0;
// 	InitializeCriticalSection(&m_lock); 

	m_clrUp = RGB(0,0,0);       // 上涨颜色
	m_clrDown = RGB(0,0,0);     // 下跌颜色
	m_clrEqual = RGB(0,0,0);    // 平盘颜色
	m_clrCodeName = RGB(0,0,0); // 代码名字颜色
	m_pStatusFont = NULL;       // 状态栏指数字体
}

CStatuBar::~CStatuBar()
{
	if (m_lstCommStatusImage.GetSafeHandle())
		m_lstCommStatusImage.DeleteImageList();

	if (m_bmpRedFlag.GetSafeHandle())
		m_bmpRedFlag.DeleteObject();


}

void CStatuBar::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatuBar,CHSBizDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CStatuBar 消息处理程序

BOOL CStatuBar::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();
	return TRUE;
}

BOOL CStatuBar::OnInit()
{
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	if(m_pDataSource == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载IDataSource失败！");
			return FALSE;
	}
	m_pScrollCfg = (IHsScrollNews*)CWinnerApplication::GetObject(OBJ_SCROLLNEW);
	if(m_pScrollCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载IScrollNewCfg失败！");
		return FALSE;
	}
	m_pCommConf = (IHsCommunication*)CWinnerApplication::GetObject(OBJ_COMM);
	if (m_pCommConf == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载ICommunication失败！");
		return FALSE;
	}
	m_pBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);
	if(m_pBlockManager == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载IBlockManager失败！");
		return FALSE;
	}
	m_pColorCfg = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	if (m_pColorCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载IHsColor失败！");
		return FALSE;
	}
	m_pFontCfg = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	if (m_pColorCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载IHsFont失败！");
		return FALSE;
	}
	m_pTradeCore = (ITradeCoreObj *)CWinnerApplication::GetObject(OBJ_TRADECORE);
	if (m_pTradeCore == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll加载ITradeCoreObj失败！");
		return FALSE;
	}
	if (m_pDataSource && !IsBadReadPtr(m_pDataSource,1))
	{
		m_nDataID = m_pDataSource->HSDataSource_DataSouceInit(this,NULL);
	}
	else
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"WinnerStatus.Dll数据引擎初始化失败！");
		return FALSE;
	}

	IAccountMng *pMng = m_pTradeCore->GetAccountMng();
	if(pMng)
		pMng->RegAccountChange(this); //向账号管理器注册消息回调函数

	//////////////////////////////////////////////////////////////////////////
	// 加载图片资源
	IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	// 通讯状态
	HANDLE handle = pQuoteLang->LoadHImage(IDB_COMM_STATUS, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (handle)
	{
		CBitmap bit;
		bit.Attach((HBITMAP)handle);
		m_lstCommStatusImage.Create(16, 16, ILC_COLOR24 | ILC_MASK,3,3);
		BOOL bRet = m_lstCommStatusImage.Add(&bit, RGB(255,0,255));
		bit.DeleteObject();
	}
	// 红绿均图片
	handle = pQuoteLang->LoadHImage(IDB_RGBFLAG, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (handle)
	{
		m_bmpRedFlag.Attach((HBITMAP)handle);
	}
	//////////////////////////////////////////////////////////////////////////

	if(FALSE == ReadCfg())
		return FALSE;

	RequestRealTimeData(TRUE);
	GetServerTime();
	if(m_scrollSpeed != 0)
		m_nScrollIndexID = this->SetTimer((long)this+1,m_scrollSpeed*1000,NULL);

	return TRUE;
}

BOOL CStatuBar::ReadCfg()
{

	if (m_pScrollCfg == NULL || ::IsBadReadPtr(m_pScrollCfg, 1))
	{
		return FALSE;
	}
	
	//从配置文件中读取
	m_threeShow = m_pScrollCfg->IsDispThirdIndex();
	m_fourShow = m_pScrollCfg->IsDispFourthIndex();
	m_scrollSpeed = m_pScrollCfg->GetStatusScrollSpeed();
	m_strStatusName = m_pScrollCfg->GetStatusName();

	m_nIsUpDownRate   = m_pScrollCfg->GetUpDownRate();
	m_nIsTotal        = m_pScrollCfg->GetTotal();
	m_nIsShowNewPirce = m_pScrollCfg->GetShowNewPirce();
	m_nIsUpDown       = m_pScrollCfg->GetUpDown();


	CStringArray stockList;
	StatuBarData temp;

	//读取用户保存的列表信息
	stockList.RemoveAll();
	m_pScrollCfg->GetFirstListStockInfo(stockList);
	for(int i=0; i<stockList.GetCount(); i++)
	{
		memset(&temp, 0, sizeof(StatuBarData));
		strncpy(temp.m_stock.m_ciStockCode.m_cCode,stockList.GetAt(i),min(sizeof(temp.m_stock.m_ciStockCode.m_cCode),stockList.GetAt(i).GetLength()+1));
		i++;
		temp.m_stock.m_ciStockCode.m_cCodeType = atoi(stockList.GetAt(i));
		m_oneList.Add(temp);
	}

	stockList.RemoveAll();
	m_pScrollCfg->GetSecondListStockInfo(stockList);
	for(int i=0; i<stockList.GetCount(); i++)
	{
		memset(&temp, 0, sizeof(StatuBarData));
		strncpy(temp.m_stock.m_ciStockCode.m_cCode,stockList.GetAt(i),min(sizeof(temp.m_stock.m_ciStockCode.m_cCode),stockList.GetAt(i).GetLength()+1));
		i++;
		temp.m_stock.m_ciStockCode.m_cCodeType = atoi(stockList.GetAt(i));
		m_twoList.Add(temp);
	}

	stockList.RemoveAll();
	m_pScrollCfg->GetThirdListStockInfo(stockList);
	for(int i=0; i<stockList.GetCount(); i++)
	{
		memset(&temp, 0, sizeof(StatuBarData));
		strncpy(temp.m_stock.m_ciStockCode.m_cCode,stockList.GetAt(i),min(sizeof(temp.m_stock.m_ciStockCode.m_cCode),stockList.GetAt(i).GetLength()+1));
		i++;
		temp.m_stock.m_ciStockCode.m_cCodeType = atoi(stockList.GetAt(i));
		m_threeList.Add(temp);
	}

	stockList.RemoveAll();
	m_pScrollCfg->GetFourthListStockInfo(stockList);
	for(int i=0; i<stockList.GetCount(); i++)
	{
		memset(&temp, 0, sizeof(StatuBarData));
		strncpy(temp.m_stock.m_ciStockCode.m_cCode,stockList.GetAt(i), min(sizeof(temp.m_stock.m_ciStockCode.m_cCode), stockList.GetAt(i).GetLength()+1));
		i++;
		temp.m_stock.m_ciStockCode.m_cCodeType = atoi(stockList.GetAt(i));
		m_fourList.Add(temp);
	}
	m_pScrollCfg->GetStockNewName(m_newNameList);

	BOOL bRet = FALSE;
	bRet = ReadColor();
	bRet = ReadFont();

	return bRet;
}

BOOL CStatuBar::ReadColor()
{
	if (m_pColorCfg == NULL || ::IsBadReadPtr(m_pColorCfg, 1))
	{
		return FALSE;
	}

	m_clrUp   = m_pColorCfg->GetUpColor();
	m_clrDown = m_pColorCfg->GetDownColor();
	m_clrEqual    = m_pColorCfg->GetEqualColor();
	m_clrCodeName = m_pColorCfg->GetFixedNameColor();

	return TRUE;
}

BOOL CStatuBar::ReadFont()
{
	if (m_pFontCfg == NULL || ::IsBadReadPtr(m_pFontCfg, 1))
	{
		return FALSE;
	}

	m_pStatusFont = m_pFontCfg->GetStatusFontProt();

	return TRUE;
}

void CStatuBar::WriteCfg()
{
 	m_pScrollCfg->SaveToFile();
}

/////////////////////////////////以下是实现账号管理器回调///////////////////////////////////////////
////
void CStatuBar::OnAccountChange(IAccount* lpValue, EAccountChange nChangeType)
{
	m_ayConnectNotify.RemoveAll();
	InvalidateRect(&m_rectProgress,FALSE);

}
void CStatuBar::OnAccountStatus(IAccount* lpValue) 
{
	m_ayConnectNotify.RemoveAll();
	InvalidateRect(&m_rectProgress,FALSE);
}
////

void CStatuBar::SwitchTypeToString(int nType, CString &strServer,BOOL bSwitch)
{
	if(bSwitch)
	{
		switch (nType)
		{
		case atTrade:
			strServer = "交易";
			break;
		case atLevel1:
			strServer = "行情Level1";
			break;
		case atLevel2:
			strServer = "行情Level2";
			break;
		case atInfo:
			strServer = "资讯";
			break;
		case atCenter:
			strServer = "中心端";
			break;
		default:
			strServer.Empty();
			break;
		}
	}
	else
	{	
		switch (nType)
		{
		case CEV_Connect_HQ:
			strServer = "行情Level1";
			break;
		case CEV_Connect_LEVEL2:
			strServer = "行情Level2";
			break;
		case CEV_Connect_JY:
			strServer = "交易";
			break;
		case CEV_Connect_ZX:
			strServer = "咨询";
			break;
		case CEV_Connect_CENTER:
			strServer = "中心端";
			break;
		default:
			strServer.Empty();
			break;
		}
	}

}


////
void CStatuBar::GetAccountMng()
{
	//从账号管理器里面读取当前的连接状态
	IAccountMng *pMng = m_pTradeCore->GetAccountMng();
	if(pMng)
	{
		m_ayConnectNotify.RemoveAll();
		int nAccountlenth = pMng->GetCurrentAccountCount();
		for(int i=0; i<nAccountlenth; i++)
		{
			IAccount* pAccount = pMng->GetCurrentAccountByIndex(i);
			if(pAccount)
				AddAccountMng(pAccount); //添加账号信息
		}	
	}
}
void CStatuBar::AddAccountMng(IAccount* pAccount)
{
	if(pAccount == NULL)
		return;

	CString str;
	SwitchTypeToString(pAccount->GetAccountType(),str,TRUE); //获取类型
	if(!str.IsEmpty())
		m_ayConnectNotify.Add(str);
	else
	{
		m_ayConnectNotify.Add("Error");
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Status_Log_Name,"从账号管理器里面读取当前的连接状态失败！");
	}

	str = pAccount->GetJrName();
	m_ayConnectNotify.Add(str);

	if(pAccount->IsConnected())  //获取连接状态
		m_ayConnectNotify.Add(Comm_OK);
	else
		m_ayConnectNotify.Add(Comm_Error);

}
int CStatuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CHSBizDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_lRed_Green_Panel = Red_Green_Panel;

	m_nTimeIndexID = this->SetTimer((long)this,1000,NULL);
	
	m_scrollNewsBarWnd.Create(IDD_SCROLLNEWSBAR,this);
	if(!::IsWindow(m_scrollNewsBarWnd.m_hWnd))
		return -1;
	m_scrollNewsBarWnd.m_pStatuBar = this;//状态栏指针让跑马灯记录

	m_scrollFrame.Create(IDD_SCROLLFRAME,NULL);
	if(!::IsWindow(m_scrollFrame.m_hWnd))
		return -1;
	m_scrollFrame.m_pScrollWnd = &m_scrollNewsBarWnd;//将跑马灯对象地址给跑马灯框架	

	m_tip.Create(this);
	return 0;
}

void CStatuBar::OnSize(UINT nType, int cx, int cy)
{
//	TRACE("跑马灯 OnSize 函数执行 \n");
	CHSBizDialog::OnSize(nType, cx, cy);
	SetSize();
	MoveScrollNewsBarWnd();
}

void CStatuBar::SetSize()
{
	//指数条
	CClientDC dc(this);
	//获得状态栏字体 这里就直接赋值了
	CFont m_font;
	m_font.CreatePointFont(16,"Fixedsys",NULL);
	CFont* pOldFont = dc.SelectObject(&m_font);
	CSize size = dc.GetTextExtent("A");
	int nOneCharWidth = size.cx; //获得一个字符的长度
	dc.SelectObject(pOldFont);
	
	CRect rect;
	this->GetClientRect(rect);
	//获得状态栏显示名称的矩形
	m_rectHomeUrl = rect;
	m_rectHomeUrl.right = nOneCharWidth * m_strStatusName.GetLength() + 4;
	rect.left           = m_rectHomeUrl.right;

	//指数矩形
	m_rectIndex = rect;
	if (m_scrollNewsBarWnd.bScrollFixed)  //如果是固定显示 
	{
		if(1 == m_scrollNewsBarWnd.m_nShowScrollAlone) //状态栏在下方显示
		{
			m_rectIndex.top = rect.Height()/2;
		}
		else if (2 == m_scrollNewsBarWnd.m_nShowScrollAlone) //状态栏在上方显示
		{
			m_rectIndex.bottom = rect.Height()/2;
		}
	}
	//上下各留取4个像素
	m_rectIndex.top += 4;
	m_rectIndex.right = m_rectIndex.left;	//指数矩形右边到达偏移量

	//时间矩形
	m_rectTimePanel = rect;
	if (m_scrollNewsBarWnd.bScrollFixed) //跑马灯固定显示
	{
		if(m_scrollNewsBarWnd.m_nShowScrollAlone) //跑马灯不与指数一起显示
			m_rectTimePanel.top = rect.Height()/2;
	}
	//上下各留取4个像素
	m_rectTimePanel.top += 4;
	m_rectTimePanel.left = rect.right - nOneCharWidth*10;//时间矩形左边起始位置位于整个框架矩形右边倒数10个字符长度

	
	//通讯窗口矩形
	m_rectProgress = rect;
	if (m_scrollNewsBarWnd.bScrollFixed) //跑马灯固定显示
	{
		if(m_scrollNewsBarWnd.m_nShowScrollAlone) //跑马灯不与指数一起显示
			m_rectProgress.top = rect.Height()/2;
	}
	//上下各留4个像素
	m_rectProgress.top += 4;
	m_rectProgress.right = m_rectTimePanel.left; //通讯窗口显示位置就在时间矩形的左边
	m_rectProgress.left = m_rectProgress.right - Transmited_Process;
	

	//红绿均矩形
	m_rectRedGreen = rect;
	if (m_scrollNewsBarWnd.bScrollFixed) //跑马灯固定显示
	{
		if(m_scrollNewsBarWnd.m_nShowScrollAlone) //跑马灯不与指数一起显示
			m_rectRedGreen.top = rect.Height()/2;	
	}
	//上下各留取4个像素
	m_rectRedGreen.top += 4;
	m_rectRedGreen.right = m_rectProgress.left; //红绿均显示位置就在通讯矩形的左边
	m_rectRedGreen.left = m_rectRedGreen.right - m_lRed_Green_Panel + 1;
	if(0 == m_scrollNewsBarWnd.m_nShowScrollAlone) //如果跑马灯显示同一行 则红绿均不显示
	{
		m_rectRedGreen.left = m_rectRedGreen.right;
	}	

	//跑马灯矩形
	m_rectScroll = rect;
	if (m_scrollNewsBarWnd.bScrollFixed)//跑马灯固定显示
	{
		if(0 == m_scrollNewsBarWnd.m_nShowScrollAlone)//跑马灯与状态栏同一行
		{//跑马灯左起指数 右到通讯状态窗口 红绿均不显示
			m_rectScroll.left  = m_rectIndex.right;
			m_rectScroll.right = m_rectProgress.left;
			m_rectScroll.top    += 2;
			m_rectScroll.bottom -= 2;
		}
		else if (1 == m_scrollNewsBarWnd.m_nShowScrollAlone)//跑马灯在状态栏上面
		{
			m_rectScroll.top += 2;
			m_rectScroll.bottom = rect.Height()/2 + 1;
		}
		else//跑马灯在状态栏下面
		{//跑马灯左起状态名称 右到 红绿均
			m_rectScroll.top = rect.Height()/2 + 2;
			m_rectScroll.right = m_rectRedGreen.left;
			m_rectScroll.bottom -= 1;
		}
		m_rectScroll.top += 1;
	}
	else//跑马灯浮动显示
	{
		if(m_rectScrollFloat.IsRectEmpty())//如果数值为空 说明用户还没有使用过 设为默认值
		{
			
			if(0 == m_scrollNewsBarWnd.bScrollMultiLine) //跑马灯单行显示 
			{
				if(m_rectScrollFloat.IsRectEmpty())//如果用户没有自定义位置
				{
					m_rectScrollFloat.left   = ::GetSystemMetrics(SM_CXSCREEN) / 4;
					m_rectScrollFloat.top    = ::GetSystemMetrics(SM_CXSCREEN) / 4;
					m_rectScrollFloat.right  = m_rectScrollFloat.left + SCROLL_FLOAT_WIDTH;
					m_rectScrollFloat.bottom = m_rectScrollFloat.top  + SCROLL_FLOAT_HEIGHT + 6;
		
				}
			}
			else //多行显示 其高度有一个固定值
			{
				if(m_rectScrollFloat.IsRectEmpty()) //如果用户没有自定义位置
				{
					m_rectScrollFloat.left   = ::GetSystemMetrics(SM_CXSCREEN) / 4;
					m_rectScrollFloat.top    = ::GetSystemMetrics(SM_CXSCREEN) / 4;
					m_rectScrollFloat.right  = m_rectScrollFloat.left + SCROLL_FLOAT_WIDTH;
					m_rectScrollFloat.bottom = m_rectScrollFloat.top  + SCROLL_FLOAT_HEIGHT * 6;
				}
			}
		}
		else
		{	
		    m_rectScrollFloat = m_scrollFrame.getPosition();//当跑马灯不是固定在状态栏上 记录在窗口的位置，大小
			if(m_scrollNewsBarWnd.bScrollMultiLine)
			{//如果是多行 
				m_rectScrollFloat.bottom = m_rectScrollFloat.top  + SCROLL_FLOAT_HEIGHT * 6;
			}
			else
			{//如果是单行		  
				m_rectScrollFloat.bottom = m_rectScrollFloat.top  + SCROLL_FLOAT_HEIGHT + 6;
			}	
		}		
	}
}

void CStatuBar::MoveScrollNewsBarWnd()
{//通过跑马灯成员函数获得 跑马灯是否固定在状态栏上

	m_scrollNewsBarWnd.ShowWindow(SW_HIDE);
	m_scrollFrame.ShowWindow(SW_HIDE);

	if(m_scrollNewsBarWnd.bScrollFixed)
	{//固定显示

		m_scrollNewsBarWnd.SetParent(this);
		m_scrollNewsBarWnd.MoveWindow(m_rectScroll);
		if(0 == m_scrollNewsBarWnd.m_nShowScrollAlone)
		{
			ILayoutWindow *pLayoutWnd = GetLayoutWindow();
			if(pLayoutWnd)
			{
				pLayoutWnd->ChangeSize(0,STATUE_HEIGHT/2);
			}
		}
		else
		{
			ILayoutWindow *pLayoutWnd = GetLayoutWindow();
			if(pLayoutWnd)
			{
				pLayoutWnd->ChangeSize(0,STATUE_HEIGHT);
			}
		}

		m_scrollNewsBarWnd.ShowWindow(SW_SHOW);
		m_scrollFrame.ShowWindow(SW_HIDE);

	}
	else
	{//浮动显示

		m_scrollNewsBarWnd.SetParent(&m_scrollFrame);
		m_scrollFrame.MoveWindow(m_rectScrollFloat);
		CRect rect;
		m_scrollFrame.GetClientRect(&rect);
		m_scrollNewsBarWnd.MoveWindow(rect);

		ILayoutWindow *pLayoutWnd = GetLayoutWindow();
		if(pLayoutWnd)
		{
			pLayoutWnd->ChangeSize(0,STATUE_HEIGHT/2);
		}

		m_scrollNewsBarWnd.ShowWindow(SW_SHOW);
		m_scrollFrame.ShowWindow(SW_SHOW);
	}
}

void CStatuBar::OnPaint()
{
//	TRACE("跑马灯 OnPaint 函数执行 \n");
	CPaintDC dc(this); 
	CMemdc memDC(&dc);

	int nOldMode = memDC.SetBkMode(TRANSPARENT);
	//////////////////////////////////////////////////////////////////////////
	// 画背景
	CRect rect;
	this->GetClientRect(&rect);
	FillDockBar(&memDC,rect);

	//////////////////////////////////////////////////////////////////////////
	// 画边框
	CPen GrayPen(PS_SOLID,1,RGB(85,85,85));
	CPen *oldPen = NULL;
	COLORREF fmcolor = RGB(0,0,0);
	COLORREF cl = GetSysColor(COLOR_3DSHADOW);

	oldPen = memDC.SelectObject(&GrayPen);
	memDC.MoveTo(rect.left, rect.bottom-1);
	memDC.LineTo(rect.right,rect.bottom-1);

	memDC.MoveTo(rect.left, rect.top);
	memDC.LineTo(rect.right,rect.top);
	memDC.MoveTo(rect.left, rect.top+1);
	memDC.LineTo(rect.right,rect.top+1);

	rect.DeflateRect(0,2,0,0);
	memDC.Draw3dRect(&rect,fmcolor,fmcolor);
	memDC.SelectObject(oldPen);

	//////////////////////////////////////////////////////////////////////////

	DrawStatusName(&memDC); //绘制状态栏名称
	DrawIndexPanel(&memDC); //绘制指数
	DrawProgress(&memDC);   //通讯状态
	DrawRedGreenLine(&memDC); //绘制红绿灯
	if(m_scrollNewsBarWnd.bScrollFixed && m_scrollNewsBarWnd.m_nShowScrollAlone != 0)//跑马灯固定显示平且不与状态栏同行显示
	{
		GetClientRect(&rect);
		
		CPen BlackPen(PS_SOLID,1,RGB(0,0,0));
		oldPen = memDC.SelectObject(&BlackPen);
		memDC.MoveTo(rect.left+m_rectHomeUrl.Width(), rect.Height()/2+2);
		memDC.LineTo(rect.right, rect.Height()/2+2);

		memDC.SelectObject(&GrayPen);
		memDC.MoveTo(rect.left+m_rectHomeUrl.Width(), rect.Height()/2+1);
		memDC.LineTo(rect.right, rect.Height()/2+1);
		memDC.SelectObject(oldPen);

		BlackPen.DeleteObject();
	}

	DrawTimePanel(&memDC);	//时间
	
	memDC->SetBkMode(nOldMode);
	GrayPen.DeleteObject();
}

void CStatuBar::FillDockBar(CDC* dc, CRect& rc)
{
	//现在采用直接填充颜色
	COLORREF clrOld = dc->GetBkColor();
	dc->FillSolidRect(rc, STATUE_BKCOLOR);
	dc->SetBkColor(clrOld);
}

void CStatuBar::DrawIndexPanel( CDC* dc )
{
	GetShowString();

	int nOldMode = dc->SetBkMode(TRANSPARENT);
	
	CFont* oldFont = dc->SelectObject(m_pStatusFont);
	CSize oneWordSize = dc->GetTextExtent("A");
	oneWordSize.cx /= 2;

	COLORREF oldColor;
	COLORREF crl;
	CRect rect = m_rectIndex;
	rect.right = rect.left;

	for(int i=0; i<INDEX_MAX_COUNT; i++)
	{
		m_nIndexOff[i] = 0;
		if(m_indexString[i].trend == -2)
			continue;

		//股票名称
		CSize sz = dc->GetTextExtent(m_indexString[i].name);
		rect.left  = rect.right;
		rect.right = rect.left + sz.cx;

		crl = m_clrCodeName;
		oldColor = dc->SetTextColor(crl);
		dc->DrawText(m_indexString[i].name,rect,DT_VCENTER|DT_SINGLELINE);

		//获取颜色
		if(m_indexString[i].trend == -1) //跌
		{
			dc->SetTextColor(m_clrDown);
		}
		else if(m_indexString[i].trend == 0)//平
		{
			dc->SetTextColor(m_clrEqual);		
		}
		else if(m_indexString[i].trend == 1)//涨
		{
			dc->SetTextColor(m_clrUp);	
		}

		//现价
		if(m_nIsShowNewPirce == 1)
		{
			sz = dc->GetTextExtent(m_indexString[i].nowPrice);
			rect.left  = rect.right;
			rect.right = rect.left + sz.cx;
			dc->DrawText(m_indexString[i].nowPrice,rect,DT_VCENTER|DT_SINGLELINE);
		}


		//涨跌之前的符号
		if(m_nIsUpDown == 1)
		{
			rect.left  = rect.right + oneWordSize.cx;
			rect.right = rect.left  + SCROLL_FLOAT_HEIGHT/4;
			CRect arrowRect = rect;
			arrowRect.top += arrowRect.Height()/8;
			if(m_indexString[i].trend == -1) //跌
			{
				CGeneralHelper::DrawArrow(dc, arrowRect, m_clrDown, FALSE);
			}
			else if(m_indexString[i].trend == 0)//平
			{
			}
			else if(m_indexString[i].trend == 1)//涨
			{
				CGeneralHelper::DrawArrow(dc, arrowRect, m_clrUp, TRUE);
			}

			//涨跌
			rect.right += oneWordSize.cx;
			sz = dc->GetTextExtent(m_indexString[i].ricePrice);
			rect.left  = rect.right;
			rect.right = rect.left + sz.cx;
			dc->DrawText(m_indexString[i].ricePrice, rect, DT_VCENTER|DT_SINGLELINE);
		}

		//涨幅
		if(m_nIsUpDownRate == 1)
		{
			sz = dc->GetTextExtent(m_indexString[i].riceRatio);
			rect.left  = rect.right + oneWordSize.cx;
			rect.right = rect.left + sz.cx;
			dc->DrawText(m_indexString[i].riceRatio, rect, DT_VCENTER|DT_SINGLELINE);
		}

		//总金额
		if(m_nIsTotal == 1)
		{
			sz = dc->GetTextExtent(m_indexString[i].totalMoney);
			rect.left  = rect.right + oneWordSize.cx;
			rect.right = rect.left + sz.cx;
			dc->SetTextColor(crl);
			dc->DrawText(m_indexString[i].totalMoney, rect, DT_VCENTER|DT_SINGLELINE);
		}

		rect.right += oneWordSize.cx;
		CPen GrayPen(PS_SOLID,1,RGB(85,85,85));
		CPen BlackPen(PS_SOLID,1,RGB(0,0,0));
		CPen *oldPen = dc->SelectObject(&GrayPen);
		dc->MoveTo(rect.right,rect.top+3);
		dc->LineTo(rect.right,rect.bottom-3);

		dc->SelectObject(&BlackPen);
		dc->MoveTo(rect.right+1,rect.top+3);
		dc->LineTo(rect.right+1,rect.bottom-3);
		dc->SelectObject(oldPen);

		m_nIndexOff[i] = rect.right; //记录相对左边的偏移位置

		rect.right += oneWordSize.cx;
		dc->SetTextColor(oldColor);
		GrayPen.DeleteObject();
		BlackPen.DeleteObject();
	}

	m_rectIndex.right = rect.right;
	dc->SetBkMode(nOldMode);
	dc->SelectObject(oldFont);

	if(!m_scrollNewsBarWnd.bScrollFixed && m_scrollNewsBarWnd.bScrollMultiLine)
	{
		//m_scrollNewsBarWnd.Invalidate(FALSE);
		CClientDC dc(&m_scrollNewsBarWnd);
		m_scrollNewsBarWnd.DrawMyCaption(&dc);
	}
	if(0 == m_scrollNewsBarWnd.m_nShowScrollAlone && m_scrollNewsBarWnd.bScrollFixed)
	{
		m_rectScroll.left = m_rectIndex.right;
		m_scrollNewsBarWnd.MoveWindow(m_rectScroll);
	}
}

void CStatuBar::DrawStatusName(CDC* dc)
{
	COLORREF oldCrl = dc->SetTextColor(m_clrCodeName);
	dc->DrawText(m_strStatusName, m_rectHomeUrl, DT_VCENTER|DT_SINGLELINE);
	dc->SetTextColor(oldCrl);
}

void CStatuBar::DrawProgress(CDC* dc)
{
	//////////////////////////////////////////////////////////////////////////
	// 获取通讯状态
	int nStatus = 0;
	if(m_ayConnectNotify.GetCount() == 0)
		GetAccountMng();

	int nCount = m_ayConnectNotify.GetCount();
	for(int i=0; i<nCount; i=i+3)
	{
		if(m_ayConnectNotify.GetAt(i+2).Compare(Comm_Error) == 0)
			nStatus++;
	}
	//////////////////////////////////////////////////////////////////////////
	// 画图标
	int nBmpIndex(0);
	if(nStatus == 0)//通讯状态很好
		nBmpIndex = 0;
	else if(nStatus < nCount/3) //通讯状态一般
		nBmpIndex = 1;
	else
		nBmpIndex = 2;
	m_lstCommStatusImage.Draw(dc, nBmpIndex, CPoint(m_rectProgress.left, m_rectProgress.top), ILD_TRANSPARENT);

}

void CStatuBar::DrawTimePanel( CDC* dc )
{
	int oldMode = dc->SetBkMode(TRANSPARENT);
	COLORREF oldClr = dc->SetTextColor(RGB(255,255,255));
	dc->DrawText(m_strTime,m_rectTimePanel,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	dc->SetBkMode(oldMode);
	dc->SetTextColor(oldClr);
}

void CStatuBar::DrawRedGreenLine(CDC* pDC)
{
	//如果红绿灯大小为0 则不绘制红绿灯
	if(m_rectRedGreen.left == m_rectRedGreen.right)
		return;

	CRect rcSH;
	rcSH = m_rectRedGreen;
	//rcSH.top += 3;
	rcSH.bottom = rcSH.top + 5;
	rcSH.left += 4;
	rcSH.right = rcSH.left + (m_lRed_Green_Panel - 12) / 2 - 2;

	CRect  rect1 = rcSH;
	rect1.InflateRect( 2,1);
	CBrush brush1;   // Must initialize!
	brush1.CreateSolidBrush(0);//RGB(128,128,128));   // Blue brush.
	pDC->FillRect( rect1,&brush1);
	//pDC->FillSolidRect(rect1,0);//RGB(128,128,128));

	short nRedWidth = CalWidth(m_TSHIndex.m_nTotalStock,m_TSHIndex.m_nRiseCount,rcSH.Width());
	if( nRedWidth < 0 )
		nRedWidth = 0;
	pDC->FillSolidRect( rcSH.left, rcSH.top, nRedWidth, rcSH.Height(), RGB(255,0,0) );

	short nWhiteWidth = CalWidth(m_TSHIndex.m_nTotalStock,m_TSHIndex.m_nTotalStock - m_TSHIndex.m_nRiseCount - m_TSHIndex.m_nFallCount,rcSH.Width());
	if( nWhiteWidth < 0 )
		nWhiteWidth = 0;
	pDC->FillSolidRect( rcSH.left + nRedWidth, rcSH.top, nWhiteWidth, rcSH.Height(), RGB(255,255,255) );

	short nGreendWidth = rcSH.Width() - nRedWidth - nWhiteWidth;
	if( nGreendWidth < 0 )
		nGreendWidth = 0;
	pDC->FillSolidRect( rcSH.left + nRedWidth + nWhiteWidth, rcSH.top, nGreendWidth, rcSH.Height(), RGB(0, 255, 0));

	CRect rcSZ;
	rcSZ.top  = rcSH.top;
	rcSZ.left = rcSH.right + 6;
	rcSZ.bottom = rcSZ.top + 5;
	rcSZ.right  = rcSZ.left + (m_lRed_Green_Panel - 12) / 2;

	rect1 = rcSZ;
	rect1.InflateRect( 2,1);
	pDC->FillRect( rect1,&brush1);

	nRedWidth = CalWidth(m_TSZIndex.m_nTotalStock,m_TSZIndex.m_nRiseCount,rcSZ.Width());
	if( nRedWidth < 0 )
		nRedWidth = 0;
	pDC->FillSolidRect( rcSZ.left, rcSZ.top, nRedWidth, rcSZ.Height(), RGB(255,0,0) );

	nWhiteWidth = CalWidth(m_TSZIndex.m_nTotalStock,m_TSZIndex.m_nTotalStock - m_TSZIndex.m_nRiseCount - m_TSZIndex.m_nFallCount,rcSZ.Width());
	if( nWhiteWidth < 0 )
		nWhiteWidth = 0;
	pDC->FillSolidRect( rcSZ.left + nRedWidth, rcSZ.top, nWhiteWidth, rcSZ.Height(), RGB(255,255,255) );

	nGreendWidth = rcSZ.Width() - nRedWidth - nWhiteWidth;
	if( nGreendWidth < 0 )
		nGreendWidth = 0;
	pDC->FillSolidRect( rcSZ.left + nRedWidth + nWhiteWidth, rcSZ.top, nGreendWidth, rcSZ.Height(), RGB(0, 255, 0) );

	CRect rc  = rcSH;
	rc.top    = rcSH.bottom + 2;
	rc.bottom = rc.top + 6;
	rc.right  = rc.left + 5;

	CRect rcDraw = rc;

	for( int i = 0; i < 8; i++ )
	{
		DrawRedGreenFlag( pDC, rcDraw, DrawRGRectOffset(m_TSHIndex.m_nFlags[i]) );
		rcDraw.OffsetRect( 7, 0 );
	}

	rc  = rcSZ;
	rc.top    = rcSZ.bottom + 2;
	rc.bottom = rc.top + 6;
	rc.right  = rc.left + 5;
	rcDraw = rc;

	for(int i = 0; i < 8; i++ )
	{
		DrawRedGreenFlag( pDC, rcDraw, DrawRGRectOffset(m_TSZIndex.m_nFlags[i]) );
		rcDraw.OffsetRect( 7, 0 );
	}

	brush1.DeleteObject();
}

void CStatuBar::DrawRedGreenFlag( CDC* pDC, CRect rc, short offset )
{
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);

	CBitmap *pOldmap = dcCompatible.SelectObject(&m_bmpRedFlag);
	pDC->BitBlt(rc.left , rc.top, 6, 6, &dcCompatible, offset ,0,SRCCOPY);

	dcCompatible.SelectObject(pOldmap);
	dcCompatible.DeleteDC();
}

short CStatuBar::DrawRGRectOffset(short flags )
{
	if( flags == 1 ) return 0;
	if( flags == 2 ) return 6;
	if( flags == 3 ) return 12;
	if( flags == 4 ) return 18;
	if( flags == 5 ) return 24;
	if( flags == 6 ) return 30;

	return -1;
}


short CStatuBar::CalWidth(short total, short value,short width)
{
	if( total == 0 )
		return 0;
	return (short)(width * (float)value / total);
}

void CStatuBar::GetShowString()
{
	int i=0;
	int nIndex=0;

	for(int j=0; j<INDEX_MAX_COUNT; j++)
		m_indexString[j].trend = -2;

	if(m_oneList.GetCount() !=0)
	{
		nIndex = m_nShowIndex%m_oneList.GetCount();
		UpdateDataEntry(&m_oneList.GetAt(nIndex),i);
		i++;
	}
	if(m_twoList.GetCount() != 0)
	{
		nIndex = m_nShowIndex%m_twoList.GetCount();
		UpdateDataEntry(&m_twoList.GetAt(nIndex),i);
		i++;

	}
	if(m_threeShow && m_threeList.GetCount() != 0)
	{
		nIndex = m_nShowIndex%m_threeList.GetCount();
		UpdateDataEntry(&m_threeList.GetAt(nIndex),i);
		i++;
	}
	if(m_fourShow && m_fourList.GetCount() != 0)
	{
		nIndex = m_nShowIndex%m_fourList.GetCount();
		UpdateDataEntry(&m_fourList.GetAt(nIndex),i);
		i++;
	}
}

void CStatuBar::ReplaceName(char* name, char* code)
{
	
	for(int i=0; i<m_newNameList.GetCount(); i++)
	{
		CString temp = m_newNameList.GetAt(i);
		i++;
		if(0 == strncmp(code,temp,temp.GetLength()))
		{
			temp = m_newNameList.GetAt(i);
			strncpy(name, temp, temp.GetLength()+1);
			break;
		}
	}
}

void CStatuBar::OnTimer(UINT_PTR nIDEvent)
{
	if (m_nTimeIndexID == nIDEvent)
	{//时间显示
		if (m_dwTick != 0)
		{
			DWORD dwNowTick = ::GetTickCount();
			DWORD dwSpace = dwNowTick - m_dwTick;
			DWORD dwNowTime = m_lServerTime + dwSpace / 1000;
			CTime time(dwNowTime);
			m_strTime = time.Format("%H:%M:%S");
		}
		else
		{
			CTime time = CTime::GetCurrentTime();
			m_strTime = time.Format("%H:%M:%S");
		}
//		TRACE("跑马灯定时器函数 当前服务器时间为 %s \n", m_strTime);
		this->InvalidateRect(m_rectTimePanel,FALSE);
	}
	else if(m_nScrollIndexID == nIDEvent)
	{//指数条显示

		m_nShowIndex++;	
		CRect rc;
		GetClientRect(&rc);
		CRect test;
		test = m_rectIndex;
		test.right = rc.right;
		this->InvalidateRect(test,FALSE);
	}
}


void CStatuBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rc = m_rectIndex;
	rc.right = rc.left;
	int i(0);
	for( ; i<INDEX_MAX_COUNT; i++)
	{
		rc.left = rc.right;
		rc.right = m_nIndexOff[i];
		if(rc.PtInRect(point))
			break;
	}

	CArray<StatuBarData>* pAy = NULL;
	switch (i)
	{
	case 0: 
		pAy = &m_oneList;
		break;
	case 1:
		pAy = &m_twoList;
		break;
	case 2:
		pAy = &m_threeList;
		break;
	case 3:
		pAy = &m_fourList;
		break;
	default:
		pAy = NULL;
		break;
	}

	if (pAy && !::IsBadReadPtr(pAy, 1))
	{
		if (pAy->GetCount() != 0)
		{
			int nIndex = m_nShowIndex%pAy->GetCount();
			StockUserInfo* pStock = &pAy->GetAt(nIndex).m_stock;
			CString strMarkeType;
			strMarkeType.Format("%d",pStock->m_ciStockCode.m_cCodeType);

			CLinkageParam param;
			strMarkeType.Format("%d",pStock->m_ciStockCode.m_cCodeType);
			param.AddParam(PARAM_CODE_CHANGE_MARK_TYPE,strMarkeType);
			param.AddParam(PARAM_CODE_CHANGE_STOCK_CODE,pStock->m_ciStockCode.GetCode());
			SendLinkageMsg(QUOTE_MESSAGE_CODE_CHANGE,&param);
		}
	}
	
}

void CStatuBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rc = m_rectIndex;
	for(int i=INDEX_MAX_COUNT-1; i>0; i--)
	{
		if(m_nIndexOff[i] !=0)
		{
			rc.right = m_nIndexOff[i];
			break;
		}
	}

	if(rc.PtInRect(point))
	{//右键选中指数条上的指数
		if(m_nScrollIndexID !=0)
		{
			KillTimer(m_nScrollIndexID);
			m_nScrollIndexID=0;
		}
		
		//交换数据
		CDlgIndexUserDef dlg;
		showList listTemp;
		StockUserInfo stockTemp;

		for(int i=0; i<m_oneList.GetCount(); i++)
		{
			stockTemp.SetEmpty();
			stockTemp = m_oneList.GetAt(i).m_stock;

			listTemp.SetEmpty();
			listTemp.myCodeInfo = stockTemp.m_ciStockCode;
			strncpy(listTemp.myStockName, stockTemp.m_cStockName, min(strStructLenth,strlen(stockTemp.m_cStockName)+1));
			
			dlg.m_oneList.Add(listTemp);
		}
		for(int i=0; i<m_twoList.GetCount(); i++)
		{
			stockTemp.SetEmpty();
			stockTemp = m_twoList.GetAt(i).m_stock;

			listTemp.SetEmpty();
			listTemp.myCodeInfo = stockTemp.m_ciStockCode;
			strncpy(listTemp.myStockName, stockTemp.m_cStockName, min(strStructLenth, strlen(stockTemp.m_cStockName)+1));

			dlg.m_twoList.Add(listTemp);
		}
		for(int i=0; i<m_threeList.GetCount(); i++)
		{
			stockTemp.SetEmpty();
			stockTemp = m_threeList.GetAt(i).m_stock;

			listTemp.SetEmpty();
			listTemp.myCodeInfo = stockTemp.m_ciStockCode;
			strncpy(listTemp.myStockName, stockTemp.m_cStockName, min(strStructLenth, strlen(stockTemp.m_cStockName)+1));

			dlg.m_threeList.Add(listTemp);
		}
		for(int i=0; i<m_fourList.GetCount(); i++)
		{
			stockTemp.SetEmpty();
			stockTemp = m_fourList.GetAt(i).m_stock;

			listTemp.SetEmpty();
			listTemp.myCodeInfo = stockTemp.m_ciStockCode;
			strncpy(listTemp.myStockName, stockTemp.m_cStockName, min(strStructLenth, strlen(stockTemp.m_cStockName)));

			dlg.m_fourList.Add(listTemp);
		}

		dlg.m_threeShow = m_threeShow;
		dlg.m_fourShow = m_fourShow;
		dlg.m_scrollSpeed = m_scrollSpeed;
		dlg.m_pDataSource = m_pDataSource;
		dlg.m_pBlockManager = m_pBlockManager;

		if(IDOK == dlg.DoModal())
		{
			StatuBarData temp;
			CStringArray  ayList;
			CString       str;

			///////////////////////////////第一列/////////////////////////////////////////
			BOOL bRet = FALSE;
			if (m_oneList.GetCount() == dlg.m_oneList.GetCount())
			{
				for (int i = 0; i < m_oneList.GetCount(); i++)
				{
					if ( !m_oneList.GetAt(i).m_stock.m_ciStockCode.CompareCode(&dlg.m_oneList.GetAt(i).myCodeInfo) )
					{
						bRet = TRUE;
						break;
					}
				}
			}
			else
				bRet = TRUE;
			if (bRet)
			{
				m_oneList.RemoveAll();	
				ayList.RemoveAll();
				for(int i=0; i<dlg.m_oneList.GetCount(); i++)
				{
					temp.SetEmpty();
					temp.m_stock.m_ciStockCode = dlg.m_oneList.GetAt(i).myCodeInfo;
					m_oneList.Add(temp);

					str = temp.m_stock.m_ciStockCode.GetCode();
					ayList.Add(str);

					str.Format("%d", temp.m_stock.m_ciStockCode.m_cCodeType);
					ayList.Add(str);
				}
				m_pScrollCfg->SetFirstListStockInfo(ayList);
			}

			///////////////////////////////第二列/////////////////////////////////////////
			bRet = FALSE;
			if (m_twoList.GetCount() == dlg.m_twoList.GetCount())
			{
				for (int i = 0; i < m_twoList.GetCount(); i++)
				{
					if ( !m_twoList.GetAt(i).m_stock.m_ciStockCode.CompareCode(&dlg.m_twoList.GetAt(i).myCodeInfo))
					{
						bRet = TRUE;
						break;
					}
				}
			}
			else
				bRet = TRUE;
			if (bRet)
			{
				m_twoList.RemoveAll();
				ayList.RemoveAll();			
				for(int i=0; i<dlg.m_twoList.GetCount(); i++)
				{
					temp.SetEmpty();
					temp.m_stock.m_ciStockCode = dlg.m_twoList.GetAt(i).myCodeInfo;
					m_twoList.Add(temp);

					str = temp.m_stock.m_ciStockCode.GetCode();
					ayList.Add(str);

					str.Format("%d", temp.m_stock.m_ciStockCode.m_cCodeType);
					ayList.Add(str);
				}
				m_pScrollCfg->SetSecondListStockInfo(ayList);
			}

			///////////////////////////////第三列/////////////////////////////////////////
			bRet = FALSE;
			
			if (m_threeList.GetCount() == dlg.m_threeList.GetCount())
			{
				for (int i = 0; i < m_threeList.GetCount(); i++)
				{
					if ( !m_threeList.GetAt(i).m_stock.m_ciStockCode.CompareCode(&dlg.m_threeList.GetAt(i).myCodeInfo))
					{
						bRet = TRUE;
						break;
					}
				}
			}
			else
				bRet = TRUE;
			if (bRet)
			{
				m_threeList.RemoveAll();
			    ayList.RemoveAll();
				for(int i=0; i<dlg.m_threeList.GetCount(); i++)
				{
					temp.SetEmpty();
					temp.m_stock.m_ciStockCode = dlg.m_threeList.GetAt(i).myCodeInfo;
					m_threeList.Add(temp);

					str = temp.m_stock.m_ciStockCode.GetCode();
					ayList.Add(str);

					str.Format("%d", temp.m_stock.m_ciStockCode.m_cCodeType);
					ayList.Add(str);
				}
				m_pScrollCfg->SetThirdListStockInfo(ayList);
			}
			///////////////////////////////第四列/////////////////////////////////////////
			bRet = FALSE;
			if (m_fourList.GetCount() == dlg.m_fourList.GetCount())
			{
				for (int i = 0; i < m_fourList.GetCount(); i++)
				{
					if ( !m_fourList.GetAt(i).m_stock.m_ciStockCode.CompareCode(&dlg.m_fourList.GetAt(i).myCodeInfo))
					{
						bRet = TRUE;
						break;
					}
				}
			}
			else
				bRet = TRUE;
			if (bRet)
			{
				m_fourList.RemoveAll();
				ayList.RemoveAll();
				for(int i=0; i<dlg.m_fourList.GetCount(); i++)
				{
					temp.SetEmpty();
					temp.m_stock.m_ciStockCode = dlg.m_fourList.GetAt(i).myCodeInfo;
					m_fourList.Add(temp);

					str = temp.m_stock.m_ciStockCode.GetCode();
					ayList.Add(str);

					str.Format("%d", temp.m_stock.m_ciStockCode.m_cCodeType);
					ayList.Add(str);
				}
				m_pScrollCfg->SetFourthListStockInfo(ayList);
			}

			if (m_threeShow != dlg.m_threeShow)
			{
				m_threeShow = dlg.m_threeShow;
				m_pScrollCfg->SetDispThirdIndex(m_threeShow);
			}

			if (m_fourShow != dlg.m_fourShow)
			{
				m_fourShow = dlg.m_fourShow;
				m_pScrollCfg->SetDispFourthIndex(m_fourShow);
			}

			if (m_scrollSpeed != dlg.m_scrollSpeed)
			{
				m_scrollSpeed = dlg.m_scrollSpeed;
				m_pScrollCfg->SetScrollSpeed(m_scrollSpeed);
			}

//			WriteCfg();

			m_nShowIndex = 0;
			RequestRealTimeData(TRUE);

		}
		if(m_scrollSpeed != 0 && m_nScrollIndexID == 0)
			m_nScrollIndexID = this->SetTimer((long)this+1,m_scrollSpeed*1000,NULL);
	}

}

void CStatuBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_rectProgress.PtInRect(point))
	{
		CString strShow;
		if(m_ayConnectNotify.GetCount() == 0)
			GetAccountMng();
		for(int i=0; i<m_ayConnectNotify.GetCount(); )
		{
			strShow += m_ayConnectNotify.GetAt(i++); //服务器类别
			strShow += "-";
			strShow += m_ayConnectNotify.GetAt(i++); //服务器名称
			strShow += " : ";
			strShow += m_ayConnectNotify.GetAt(i++); //连接状态
			//在一条信息之后加入换行符
			strShow += "\r\n";
		}
		//m_tip.Show("连接\\断开", strShow);
		m_tip.Show(strShow,&point);
	}

}

///////////////////////////////以下是请求数据、接受数据///////////////////////////////////
//////////////////////////////////////////////////////////////////////////收到数据
BOOL CStatuBar::HSDataSourceSink_OnCommNotify(void* pData)
{
	if (pData == NULL)
	{
		return FALSE;
	}

	CommNotify* pNotify = (CommNotify*)pData;
	if (pNotify->m_uType == eDataSource_ReConnect || pNotify->m_uType == eDataSource_Init)
	{// 断线重连需要重新请求一遍
		RequestRealTimeData(TRUE);
	}
	return TRUE;
}
BOOL CStatuBar::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{
	if (!pData || IsBadReadPtr(pData,1))
	{
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;

    OnRecvData(pData, nLen);

	return TRUE;
}


void CStatuBar::RequestRealTimeData(BOOL bNeedAutoPush /* = TRUE */)
{
	GetStockInfo(); //获取stockUserInfo
	
	AskData*  paskData;
	int nCount = m_oneList.GetCount() + m_twoList.GetCount() + m_threeList.GetCount() + m_fourList.GetCount();
	nCount += 2; //在末尾还要加上上证数据和深证数据

	int lRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nCount-1);
	paskData = (AskData*) new char[lRetLen];
	memset(paskData, 0, lRetLen);

	paskData->m_nType  = RT_REALTIME;
	paskData->m_nSize  = nCount;
	CodeInfo* pCodeInfo = paskData->m_pCode;

	int j=0;
	CodeInfo pShowInfo;
	for(int i=0; i<m_oneList.GetCount(); i++)
	{
		pShowInfo = m_oneList.GetAt(i).m_stock.m_ciStockCode;
		pCodeInfo[j++] = pShowInfo;
	}
	for(int i=0; i<m_twoList.GetCount(); i++)
	{
		pShowInfo = m_twoList.GetAt(i).m_stock.m_ciStockCode;
		pCodeInfo[j++] = pShowInfo;	
	}
	for(int i=0; i<m_threeList.GetCount(); i++)
	{
		pShowInfo = m_threeList.GetAt(i).m_stock.m_ciStockCode;
		pCodeInfo[j++] = pShowInfo;	
	}
	for(int i=0; i<m_fourList.GetCount(); i++)
	{
		pShowInfo = m_fourList.GetAt(i).m_stock.m_ciStockCode;
		pCodeInfo[j++] = pShowInfo;
	}
	//上证
	strncpy(pShowInfo.m_cCode,"1A0001",6);
	pShowInfo.m_cCodeType = 0x1100;
	pCodeInfo[j++] = pShowInfo;
	//深证
	strncpy(pShowInfo.m_cCode,"2A01",4);
	pShowInfo.m_cCode[4]=pShowInfo.m_cCode[5]='\0';
	pShowInfo.m_cCodeType = 0x1200;
	pCodeInfo[j++] = pShowInfo;

	m_pDataSource->HSDataSource_RequestAsyncData(m_nDataID, (char*)paskData, lRetLen, 0);

	if (bNeedAutoPush)
	{
		paskData->m_nType = RT_AUTOPUSH;
		m_pDataSource->HSDataSource_RequestAsyncData(m_nDataID, (char*)paskData, lRetLen, 0, e_DataSouceSend_Clear);
	}

	delete[] paskData;
}

void CStatuBar::GetServerTime()
{
	int nCodeCount = 1;
	int lRetLen    = sizeof(AskData) + sizeof(CodeInfo)*(nCodeCount - 1);

	AskData* pAskData = (AskData*)(new char[lRetLen]);
	memset(pAskData, 0, lRetLen);

	pAskData->m_nType = RT_SERVERTIME;
	pAskData->m_nSize = nCodeCount;
	m_pDataSource->HSDataSource_RequestAsyncData(m_nDataID, (char*)pAskData, lRetLen, 0);
	
	delete[] (char*)pAskData;
}

BOOL CStatuBar::OnRecvData( void* pData, int nLen )
{	
	if (!pData || IsBadReadPtr(pData,1))
	{
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;


	if (pHead->m_nType == RT_REALTIME)
	{// 实时数据应答

		AnsRealTime*  pNowData  = (AnsRealTime*)pData;
		if(pNowData->m_nSize <= 0)
		{
			return FALSE;
		}
		HandleNowData( pHead,(CommRealTimeData *)pNowData->m_pnowData, pNowData->m_nSize );
	}
	else if (pHead->m_nType == RT_AUTOPUSH)
	{// 主推数据

		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if(pEveryData->m_nSize <= 0)
		{
			return FALSE;
		}
		CommRealTimeData* pData = (CommRealTimeData *)pEveryData->m_pstData;
		HandleAutoPushData(pHead,pData,pEveryData->m_nSize);
	}
	else if (pHead->m_nType == RT_SERVERTIME)
	{
		AnsServerTime* pTime = (AnsServerTime*)pData;
		m_lServerTime = pTime->m_nDateTime;
		m_dwTick = ::GetTickCount();	
	}

	return TRUE;
}

void CStatuBar::HandleNowData(DataHead* pHead, CommRealTimeData* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData* pData = (CommRealTimeData *)pnowData;
	CommRealTimeData* pNextData = NULL;

	long nUnit = 0;
	short nDecimal = 0;
	long  lClose = 0;
	StockUserInfo pStock;
	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{

		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		
		if ( 0 == memcmp(pData->m_ciStockCode.m_cCode,"1A0001",6) )
		{//上证数据

			m_TSHIndex.m_nPreRiseTrend = m_TSHIndex.m_nRiseTrend;
			m_TSHIndex.m_nPreFallTrend = m_TSHIndex.m_nFallTrend;

			ShareRealTimeData* pShareData = pData->GetShareData();
			m_TSHIndex.m_nRiseCount  = pShareData->m_indData.m_nRiseCount;
			m_TSHIndex.m_nFallCount  = pShareData->m_indData.m_nFallCount;
			m_TSHIndex.m_nRiseTrend  = pShareData->m_indData.m_nRiseTrend;
			m_TSHIndex.m_nFallTrend  = pShareData->m_indData.m_nFallTrend;
			m_TSHIndex.m_nTotalStock = pShareData->m_indData.m_nTotalStock2; 
			m_TSHIndex.m_nLead       = pShareData->m_indData.m_nLead;

			for( int i = 0; i < 7; i++ )
			{
				m_TSHIndex.m_nFlags[i] = m_TSHIndex.m_nFlags[i + 1];
			}

			m_TSHIndex.m_nFlags[7] = SetFlags( m_TSHIndex.m_nLead, m_TSHIndex.m_nRiseTrend,
				m_TSHIndex.m_nFallTrend, m_TSHIndex.m_nPreRiseTrend,
				m_TSHIndex.m_nPreFallTrend );

			this->InvalidateRect(m_rectRedGreen,FALSE);//绘制红绿灯
		}
		else if	( 0 == memcmp(pData->m_ciStockCode.m_cCode,"2A01",4))
		{
			m_TSZIndex.m_nPreRiseTrend = m_TSZIndex.m_nRiseTrend;
			m_TSZIndex.m_nPreFallTrend = m_TSZIndex.m_nFallTrend;

			ShareRealTimeData* pShareData = pData->GetShareData();
			m_TSZIndex.m_nRiseCount  = pShareData->m_indData.m_nRiseCount;
			m_TSZIndex.m_nFallCount  = pShareData->m_indData.m_nFallCount;
			m_TSZIndex.m_nRiseTrend  = pShareData->m_indData.m_nRiseTrend;
			m_TSZIndex.m_nFallTrend  = pShareData->m_indData.m_nFallTrend;
			m_TSZIndex.m_nTotalStock = pShareData->m_indData.m_nTotalStock2; 
			m_TSZIndex.m_nLead       = pShareData->m_indData.m_nLead;

			for( int i = 0; i < 7; i++ )
			{
				m_TSZIndex.m_nFlags[i] = m_TSZIndex.m_nFlags[i + 1];
			}

			m_TSZIndex.m_nFlags[7] = SetFlags( m_TSZIndex.m_nLead, m_TSZIndex.m_nRiseTrend,
				m_TSZIndex.m_nFallTrend, m_TSZIndex.m_nPreRiseTrend,
				m_TSZIndex.m_nPreFallTrend );

			this->InvalidateRect(m_rectRedGreen,FALSE);//绘制红绿灯
		}
		
		if(UpdateRealTimeData(&pData->m_ciStockCode,pData->GetShareData()) == FALSE)
		{
			pData = pNextData;
			continue;
		}
		
		pData = pNextData;		
	}
	
	this->Invalidate(FALSE);
	//this->InvalidateRect(m_rectIndex,FALSE);
}

//////////////////////////////////////////////////////////////////////////

void CStatuBar::HandleAutoPushData(DataHead* pHead, CommRealTimeData* pnowData, int nSize)
{
	if( pHead == NULL || nSize <= 0 )
		return;

	long nUnit = 0;
	short nDecimal = 0;
	long  lClose = 0;

	StockUserInfo pStock;
	CommRealTimeData* pRealTimeData = (CommRealTimeData *)pnowData;
	CommRealTimeData* pNext = NULL;
	int nNowSize;

	for( int i = 0; i < nSize; i++ )
	{
		pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);

		if ( 0 == memcmp(pRealTimeData->m_ciStockCode.m_cCode,"1A0001",6) )
		{//上证数据

			m_TSHIndex.m_nPreRiseTrend = m_TSHIndex.m_nRiseTrend;
			m_TSHIndex.m_nPreFallTrend = m_TSHIndex.m_nFallTrend;

			ShareRealTimeData* pShareData = pRealTimeData->GetShareData();
			m_TSHIndex.m_nRiseCount  = pShareData->m_indData.m_nRiseCount;
			m_TSHIndex.m_nFallCount  = pShareData->m_indData.m_nFallCount;
			m_TSHIndex.m_nRiseTrend  = pShareData->m_indData.m_nRiseTrend;
			m_TSHIndex.m_nFallTrend  = pShareData->m_indData.m_nFallTrend;
			m_TSHIndex.m_nTotalStock = pShareData->m_indData.m_nTotalStock2; 
			m_TSHIndex.m_nLead       = pShareData->m_indData.m_nLead;

			for( int i = 0; i < 7; i++ )
			{
				m_TSHIndex.m_nFlags[i] = m_TSHIndex.m_nFlags[i + 1];
			}

			m_TSHIndex.m_nFlags[7] = SetFlags( m_TSHIndex.m_nLead, m_TSHIndex.m_nRiseTrend,
				m_TSHIndex.m_nFallTrend, m_TSHIndex.m_nPreRiseTrend,
				m_TSHIndex.m_nPreFallTrend );

			this->InvalidateRect(m_rectRedGreen,FALSE);//绘制红绿灯
		}
		else if	( 0 == memcmp(pRealTimeData->m_ciStockCode.m_cCode,"2A01",4))
		{
			m_TSZIndex.m_nPreRiseTrend = m_TSZIndex.m_nRiseTrend;
			m_TSZIndex.m_nPreFallTrend = m_TSZIndex.m_nFallTrend;

			ShareRealTimeData* pShareData = pRealTimeData->GetShareData();
			m_TSZIndex.m_nRiseCount  = pShareData->m_indData.m_nRiseCount;
			m_TSZIndex.m_nFallCount  = pShareData->m_indData.m_nFallCount;
			m_TSZIndex.m_nRiseTrend  = pShareData->m_indData.m_nRiseTrend;
			m_TSZIndex.m_nFallTrend  = pShareData->m_indData.m_nFallTrend;
			m_TSZIndex.m_nTotalStock = pShareData->m_indData.m_nTotalStock2; 
			m_TSZIndex.m_nLead       = pShareData->m_indData.m_nLead;

			for( int i = 0; i < 7; i++ )
			{
				m_TSZIndex.m_nFlags[i] = m_TSZIndex.m_nFlags[i + 1];
			}

			m_TSZIndex.m_nFlags[7] = SetFlags( m_TSZIndex.m_nLead, m_TSZIndex.m_nRiseTrend,
				m_TSZIndex.m_nFallTrend, m_TSZIndex.m_nPreRiseTrend,
				m_TSZIndex.m_nPreFallTrend );

			this->InvalidateRect(m_rectRedGreen,FALSE);//绘制红绿灯
		}
	
		if(UpdateRealTimeData(&pRealTimeData->m_ciStockCode,pRealTimeData->GetShareData()) == FALSE)
		{
			pRealTimeData = pNext;
			continue;
		}

		pRealTimeData = pNext;
	}
	this->Invalidate(FALSE);
	//InvalidateRect(m_rectIndex,FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////
///上证 深证数据处理函数
short CStatuBar::SetFlags( short lead, short risetrend,short falltrend, short prerise, short prefall)
{
	if( lead >= risetrend )		//draw red flags
	{
		if( risetrend > prerise )
		{
			return 1;
		}
		if( risetrend < prerise )
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else						//draw green flags
	{
		if( falltrend > prefall )
		{
			return 5;
		}
		if( falltrend < prefall )
		{
			return 4;
		}
		else
		{
			return 6;
		}
	}
}


//////////////////////////////////////////////////////////////////////////更新市场数据
//数据更新入口
void CStatuBar::UpdateDataEntry(StatuBarData* pStatus, int index, int nType)
{
	StockUserInfo StockData;
	pStatus->GetStockData(&StockData);
	if( MakeMarket(StockData.m_ciStockCode.m_cCodeType) == QH_Bourse ||
		WP_MARKET == MakeMarket(StockData.m_ciStockCode.m_cCodeType) )		// 期货
	{

		HSQHRealTime qhData;
		pStatus->GetRealTimeData(&qhData);
		UpdateFuturesData(&StockData, StockData.m_nPriceUnit, StockData.m_nDecimal, &qhData, index , nType);
	}
	else if( MakeIndexMarket(StockData.m_ciStockCode.m_cCodeType) )			// 指数
	{
	 	HSStockRealTime srData;
		pStatus->GetRealTimeData(&srData);
		UpdateIndexData(&StockData, StockData.m_nPriceUnit, StockData.m_nDecimal, StockData.m_lPrevClose, &srData, index ,nType);
	}
	else if( (HSMarketDataType)FOREIGN_MARKET == MakeMarket(StockData.m_ciStockCode.m_cCodeType) ) // 外汇
	{
		// 		UpdateForeignData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSStockRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else if( HK_MARKET == MakeMarket(StockData.m_ciStockCode.m_cCodeType) )	// 港股
	{
		// 		UpdateHKData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSStockRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else																	// 股票
	{
		HSStockRealTime srData;
		pStatus->GetRealTimeData(&srData);
		UpdateStockData(&StockData, StockData.m_nPriceUnit, StockData.m_nDecimal, StockData.m_lPrevClose, &srData, index ,nType);
	}

}

//////////////////////////////////////////////////////////////////////////更新指数数据
void CStatuBar::UpdateIndexData(StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, 
								HSStockRealTime* pData, int index, int nType)
{
	int tempType;
	char strTemp[strStructLenth];
	//股票名称
	strncpy(m_indexString[index].name, pStock->m_cStockName, sizeof(pStock->m_cStockName)); 
	ReplaceName(m_indexString[index].name, pStock->m_ciStockCode.m_cCode);

	//现价 
	CGeneralHelper::LongToString(pData->m_lNewPrice,lClose,nUnit,nDecimal,m_indexString[index].nowPrice,(short)0);
	

	//涨跌
	CGeneralHelper::LongToString(pData->m_lNewPrice == 0?0:(pData->m_lNewPrice - lClose),
		lClose, nUnit, nDecimal, strTemp, (nType>>16));

	double fTemp = atof(strTemp);
	if(fTemp < 0)
	{
		strncpy(strTemp,strTemp+2,sizeof(strTemp)-2); //去掉负号
		m_indexString[index].trend = -1;
	}
	else if(fTemp == 0)
		m_indexString[index].trend = 0;
	else
	{
		strncpy(strTemp,strTemp+1,sizeof(strTemp)-1); //去掉前面一个空格	
		m_indexString[index].trend = 1;
	}
	strncpy(m_indexString[index].ricePrice, strTemp, sizeof(strTemp));

	//涨幅
	tempType = nType|MAKELONG(0,LTOA_MASK_SHOW_PERSENT);
	if(lClose == 0)
		CGeneralHelper::LongToString((long)0,(long)0,(long)0,nDecimal,m_indexString[index].riceRatio, (tempType>>16) );
	else
		CGeneralHelper::LongToString(pData->m_lNewPrice == 0?0:(pData->m_lNewPrice - lClose)*100,(long)0,lClose,nDecimal,m_indexString[index].riceRatio,
		(tempType>>16) );


	//总额
	CString strTotalMoney;
	int multiple  = 100; //指数扩大了100倍
	tempType = nType&~ (MAKELONG(0,LTOA_MASK_SHOW_PLUS)|MAKELONG(0,LTOA_MASK_SHOW_SUB));
	tempType |= MAKELONG(0,LTOA_MASK_SHOWEX_UNITAUTO);
	CGeneralHelper::LongToString(pData->m_fAvgPrice*multiple,lClose,1,nDecimal,strTemp,(tempType>>16));
	strncpy(m_indexString[index].totalMoney, strTemp, sizeof(strTemp));
// 	if(pData->m_fAvgPrice > 100000000/multiple)
// 	{
// 		CGeneralHelper::LongToString(pData->m_fAvgPrice,lClose,100000000/multiple,nDecimal,strTemp,	(tempType>>16));
// 		strTotalMoney = strTemp;
// 		strTotalMoney += "亿";
// 
// 	}
// 	else if(pData->m_fAvgPrice > 10000/multiple)
// 	{
// 		CGeneralHelper::LongToString(pData->m_fAvgPrice*multiple,lClose,10000,nDecimal,strTemp,	(tempType>>16));
// 		strTotalMoney = strTemp;
// 		strTotalMoney += "万";
// 	}
// 	else
// 	{
// 		CGeneralHelper::LongToString(pData->m_fAvgPrice*multiple,lClose,1,nDecimal,strTemp,	(tempType>>16));
// 		strTotalMoney = strTemp;
// 	}
// 	strncpy(m_indexString[index].totalMoney, strTotalMoney, strTotalMoney.GetLength()+1);


}

//////////////////////////////////////////////////////////////////////////更新股票数据
void CStatuBar::UpdateStockData(StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, 
								HSStockRealTime* pData, int index, int nType)
{

	BOOL isStop = FALSE;//是否停牌 
	if (pData->m_lNewPrice == 0)//如果当前成交价为0 则为停牌
	{
		isStop = TRUE;
	}
	
	int tempType;
	char strTemp[strStructLenth];
	//股票名称
	strncpy(m_indexString[index].name, pStock->m_cStockName, sizeof(pStock->m_cStockName)); 

	//现价 
	tempType = !isStop?nType:(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	CGeneralHelper::LongToString(pData->m_lNewPrice,lClose,nUnit,nDecimal,m_indexString[index].nowPrice,(tempType>>16));

	
	//涨跌
	tempType = !isStop?nType:(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	CGeneralHelper::LongToString(pData->m_lNewPrice == 0?0:(pData->m_lNewPrice - lClose),
		lClose, nUnit, nDecimal, strTemp, (tempType>>16) );

	double fTemp = atof(strTemp);
	if(fTemp < 0)
	{
		strncpy(strTemp,strTemp+2,sizeof(strTemp)-2); //去掉负号
		m_indexString[index].trend = -1;
	}
	else if(fTemp == 0)
		m_indexString[index].trend = 0;
	else
	{
		strncpy(strTemp,strTemp+1,sizeof(strTemp)-1); //去掉前面一个空格	
		m_indexString[index].trend = 1;
	}
	strncpy(m_indexString[index].ricePrice, strTemp, sizeof(strTemp));


	//涨幅
	tempType = !isStop?(nType|MAKELONG(0,LTOA_MASK_SHOW_PERSENT)):(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	if(lClose == 0)
		CGeneralHelper::LongToString((long)0,(long)0,(long)0,nDecimal,m_indexString[index].riceRatio, (tempType>>16) );
	else
		CGeneralHelper::LongToString(pData->m_lNewPrice == 0?0:(pData->m_lNewPrice - lClose)*100,(long)0,lClose,nDecimal,m_indexString[index].riceRatio,
		(tempType>>16) );


	//总额
	CString strTotalMoney;
	tempType = !isStop?(nType|MAKELONG(0,LTOA_MASK_SHOWEX_UNITWAN)):(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	CGeneralHelper::LongToString(pData->m_fAvgPrice,lClose,1,nDecimal,strTemp, (tempType>>16));
	strncpy(m_indexString[index].totalMoney, strTemp, sizeof(strTemp));
// 	if(pData->m_fAvgPrice > 100000000)
// 	{
// 		CGeneralHelper::LongToString(pData->m_fAvgPrice,lClose,100000000,nDecimal,strTemp, (tempType>>16));
// 		strTotalMoney = strTemp;
// 		strTotalMoney += "亿";
// 
// 	}
// 	else if(pData->m_fAvgPrice > 10000)
// 	{
// 		CGeneralHelper::LongToString(pData->m_fAvgPrice,lClose,10000,nDecimal,strTemp, (tempType>>16));
// 		strTotalMoney = strTemp;
// 		strTotalMoney += "万";
// 	}
// 	else
// 	{
// 		CGeneralHelper::LongToString(pData->m_fAvgPrice,lClose,1,nDecimal,strTemp, (tempType>>16));
// 		strTotalMoney = strTemp;
// 	}
//	strncpy(m_indexString[index].totalMoney, strTotalMoney, strTotalMoney.GetLength()+1);

}

/////////////////////////////////////////////////////////////////////////////////更新期货
void CStatuBar::UpdateFuturesData( StockUserInfo* pStock, long nUnit, short nDecimal, HSQHRealTime* pData, int index, int nType )
{
	BOOL isStop = FALSE;//是否停牌 
	if (pData->m_lNewPrice == 0)//如果当前成交价为0 则为停牌
	{
		isStop = TRUE;
	}

	int tempType;
	char strTemp[strStructLenth];
	//股票名称
	strncpy(m_indexString[index].name, pStock->m_cStockName, sizeof(pStock->m_cStockName)); 

	//现价 
	tempType = !isStop?nType:(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	CGeneralHelper::LongToString(pData->m_lNewPrice,pData->m_lPreJieSuanPrice,nUnit,nDecimal,m_indexString[index].nowPrice,(tempType>>16));


	//涨跌
	tempType = !isStop?nType:(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	CGeneralHelper::LongToString(pData->m_lNewPrice == 0?0:(pData->m_lNewPrice - pData->m_lPreJieSuanPrice),
		pData->m_lPreJieSuanPrice, nUnit, nDecimal, strTemp,(tempType>>16) );

	double fTemp = atof(strTemp);
	if(fTemp < 0)
	{
		strncpy(strTemp,strTemp+2,sizeof(strTemp)-2); //去掉负号
		m_indexString[index].trend = -1;
	}
	else if(fTemp == 0)
		m_indexString[index].trend = 0;
	else
	{
		strncpy(strTemp,strTemp+1,sizeof(strTemp)-1); //去掉前面一个空格	
		m_indexString[index].trend = 1;
	}
	strncpy(m_indexString[index].ricePrice, strTemp, sizeof(strTemp));


	//涨幅
	tempType = !isStop?(nType|MAKELONG(0,LTOA_MASK_SHOW_PERSENT)):(nType|MAKELONG(0,LTOA_MASK_HORLINE_IFZERO));
	if(pData->m_lPreJieSuanPrice == 0)
		CGeneralHelper::LongToString((long)0,(long)0,(long)0,nDecimal,m_indexString[index].riceRatio,(tempType>>16) );
	else
		CGeneralHelper::LongToString(pData->m_lNewPrice == 0?0:(pData->m_lNewPrice - pData->m_lPreJieSuanPrice)*100,(long)0,pData->m_lPreJieSuanPrice,nDecimal,m_indexString[index].riceRatio,
		(tempType>>16) );


	//总额
	m_indexString[index].totalMoney[0] = '\0';

}
//////////////////////////////////////////////////////////////////////////////////////////////////
void CStatuBar::GetStockInfo()
{
	StockUserInfo stkInfo;
	for(int i=0; i<m_oneList.GetCount(); i++)
	{
		stkInfo.SetEmpty();
		m_pDataSource->HSDataSourceEx_GetStockUserInfo(&m_oneList.GetAt(i).m_stock.m_ciStockCode, stkInfo);
		m_oneList.GetAt(i).m_stock.Copy(&stkInfo);
	}
	
	for(int i=0; i<m_twoList.GetCount(); i++)
	{
		stkInfo.SetEmpty();
		m_pDataSource->HSDataSourceEx_GetStockUserInfo(&m_twoList.GetAt(i).m_stock.m_ciStockCode, stkInfo);
		m_twoList.GetAt(i).m_stock.Copy(&stkInfo);
	}

	for(int i=0; i<m_threeList.GetCount(); i++)
	{
		stkInfo.SetEmpty();
		m_pDataSource->HSDataSourceEx_GetStockUserInfo(&m_threeList.GetAt(i).m_stock.m_ciStockCode, stkInfo);
		m_threeList.GetAt(i).m_stock.Copy(&stkInfo);
	}

	for(int i=0; i<m_fourList.GetCount(); i++)
	{
		stkInfo.SetEmpty();
		m_pDataSource->HSDataSourceEx_GetStockUserInfo(&m_fourList.GetAt(i).m_stock.m_ciStockCode, stkInfo);
		m_fourList.GetAt(i).m_stock.Copy(&stkInfo);
	}

}

BOOL CStatuBar::UpdateRealTimeData(CodeInfo* pCode,ShareRealTimeData* pStock)
{//将请求回来的数据 保存起来
	for(int i=0; i<m_oneList.GetCount(); i++)
	{
		if(m_oneList.GetAt(i).m_stock.m_ciStockCode.CompareCode(pCode))
		{
			if(QH_Bourse == MakeMarket(pCode->m_cCodeType) || WP_MARKET == MakeMarket(pCode->m_cCodeType))
				m_oneList.GetAt(i).CopyFromRealTimeData(&pStock->m_qhData);
			else
				m_oneList.GetAt(i).CopyFromRealTimeData(&pStock->m_nowData);
			return TRUE;
		}
	}
	for(int i=0; i<m_twoList.GetCount(); i++)
	{
		if(m_twoList.GetAt(i).m_stock.m_ciStockCode.CompareCode(pCode))
		{
			if(QH_Bourse == MakeMarket(pCode->m_cCodeType) || WP_MARKET == MakeMarket(pCode->m_cCodeType))
				m_twoList.GetAt(i).CopyFromRealTimeData(&pStock->m_qhData);
			else
				m_twoList.GetAt(i).CopyFromRealTimeData(&pStock->m_nowData);
			return TRUE;
		}
	}
	for(int i=0; i<m_threeList.GetCount(); i++)
	{
		if(m_threeList.GetAt(i).m_stock.m_ciStockCode.CompareCode(pCode))
		{
			if(QH_Bourse == MakeMarket(pCode->m_cCodeType) || WP_MARKET == MakeMarket(pCode->m_cCodeType))
				m_threeList.GetAt(i).CopyFromRealTimeData(&pStock->m_qhData);
			else
				m_threeList.GetAt(i).CopyFromRealTimeData(&pStock->m_nowData);
			return TRUE;
		}
	}
	for(int i=0; i<m_fourList.GetCount(); i++)
	{
		if(m_fourList.GetAt(i).m_stock.m_ciStockCode.CompareCode(pCode))
		{
			if(QH_Bourse == MakeMarket(pCode->m_cCodeType) || WP_MARKET == MakeMarket(pCode->m_cCodeType))
				m_fourList.GetAt(i).CopyFromRealTimeData(&pStock->m_qhData);
			else
				m_fourList.GetAt(i).CopyFromRealTimeData(&pStock->m_nowData);
			return TRUE;
		}
	}
	return FALSE;

}

void CStatuBar::OnDestroy()
{
	WriteCfg();
	KillTimer(m_nTimeIndexID);
	m_scrollNewsBarWnd.DestroyWindow();
	m_scrollFrame.DestroyWindow();

	if (m_pTradeCore && !::IsBadReadPtr(m_pTradeCore, 1))
	{	
		IAccountMng *pMng = m_pTradeCore->GetAccountMng();
		if(pMng)
			pMng->UnRegAccountChange(this);
	}


	m_oneList.RemoveAll();
	m_twoList.RemoveAll();
	m_threeList.RemoveAll();
	m_fourList.RemoveAll();

//	DeleteCriticalSection(&m_lock); 

	if (m_pDataSource && !::IsBadReadPtr(m_pDataSource, 1))
	{
		m_pDataSource->HSDataSource_DataSourceUnInit(m_nDataID);
	}

}

BOOL CStatuBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return __super::OnEraseBkgnd(pDC);
}
