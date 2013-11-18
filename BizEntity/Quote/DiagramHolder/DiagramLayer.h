#pragma once
#include "afx.h"
#include "tinyxml.h"
#include "QuoteDiagram.h"
#include "DataSourceDefineEx.h"
#include "DrawBase.h"
#include "..\DiagramFoundation\DiagramTab.h"
#include "HSBizDialog.h"

//区域结点位置RegionNodePostion
#define RNP_UNKNOW  'N'
#define RNP_TOP     'T'
#define RNP_BOTTOM  'B'
#define RNP_LEFT    'L'
#define RNP_RIGHT   'R'

//区域内命中测试结果
#define RNHT_NOWHERE            0x0000
#define RNHT_IN_HOR_SPLITER     0x0001   //in horizontal spliter
#define RNHT_IN_VER_SPLITER     0x0002   //in vertical  spliter
#define RNHT_INRECTUNIT         0x0004   //in rect
#define RNHT_INTABBAR           0x0005   //in tab bar

//布局分割风格
#define SS_NOSPLIT              0x0000
#define SS_VERTICAL             0x0001   //split sytle vertical
#define SS_HORIZONTAL           0x0002   //split sytle horizontal 

#define DIAGRAM_SPLITBAR_WIDTH   1
class CRegionNode;
class CDiagramLayer;

class CDiagramInfo
{
private:
	CString         m_strCaption;
	CString         m_strLocation;
	IDrawBase*      m_pDraw;
	CDrawBaseParam  m_param;
public:
	friend class CRegionNode;
};

//区域结点类
typedef CTypedPtrArray<CPtrArray,CDiagramInfo*>  CDiagramInfoArray;
class CRegionNode: public IRegionNode,public IDiagramTabNotify
{
public:
	friend class CDiagramLayer;
	CRegionNode();
	~CRegionNode();
	//IRegionNode接口实现
	virtual HWND GetHostHandle();
	virtual BOOL ActiveDiagram(CString strDiagramName);
	virtual BOOL Invalidate(CRect* pRC = NULL,BOOL bErase = TRUE);
	virtual BOOL SetCursor(LPCSTR lpCursorName);
	virtual IRequestCollector* GetRequestCollector();
	virtual BOOL SendRequest(IRequestCollector* pReqCollector, BOOL bAutoClear = TRUE);
	virtual void SendNotifyMsg(UINT nMsgId,IDrawBaseParam* pParam);
	virtual void SendLinkageMsg(UINT nMsg,IDrawBaseParam* pParam,
		CString strPanelName = _T(""),CString strLayout = _T(""),CString strOwner = PUBLIC_OWNER );
	//IDiagramTabNotify接口实现
	virtual void OnActiveTabChange(CDiagramTabItem* pActiveItem,CDiagramTabItem* pOldActiveItem); 

    BOOL HasChild();
	BOOL HasTabBar();
    int GetSplitStyle();
	void SetRegionRect(const CRect &rc);
	void SetOwnerLayer(CDiagramLayer *pLayer) { m_pLayer = pLayer; }
	void ResetActiveDiagram();
	void SetVisable(BOOL bVisable);
	BOOL IsActive();

	void GetRequestData(IRequestCollector* pReqtCollecter,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	int	OnUpdateData(DataHead* pData);
	BOOL HitTest(CPoint pt,UINT* pFlags = NULL);
	BOOL OnCursor(UINT nFlags, CPoint point);
	void OnDraw(CDC* pDC); 
	BOOL OnMouseDown(UINT nFlags, CPoint point,CWnd *pHostWnd);
	BOOL OnMouseMove(UINT nFlags, CPoint point);
	BOOL MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	BOOL RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	int	 OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	void SendNotifyToDrawBase(UINT nMsgId,IDrawBaseParam* pParam);
private:
	char               m_cLocation; //位置:T-上;B-下;L-左;R-右
	int                m_nMeasure;  //尺寸:与m_cLocation一起决定m_rect

	CDiagramLayer*     m_pLayer;
	CWnd*              m_pHostWnd;
	CRect              m_regionRect;    //区域大小
	CRect              m_spliterRect;   //分割条大小
	BOOL               m_bFix;          //孩子结点是否固定大小
	CRegionNode*       m_pLeft;         //左节点:左或上     
	CRegionNode*       m_pRight;        //右节点:右或下
	CDiagramTab        m_tabBar;        //标签页
	CDiagramInfoArray  m_arrDiagramInfo;
	int                m_nActiveDiagramIndex;
	int                m_nInitActiveDiagramIndex;
    
	void LoadNodeInfo(TiXmlElement* pNodeXml);
};

//图层实现类
class CDiagramLayer :public CObject
{
public:
	enum ActionType
	{
		MouseMove,
		MouseDown,
		MouseUp,
		MouseDBDown,
		RButtonDown,
		RButtonUp
	};
public:
	CDiagramLayer(CString strLayoutName);
	~CDiagramLayer(void);
	
	//获取布局名称
	CString GetLayoutName() { return m_strLayoutName; }
	//是否是指定布局
	BOOL IsLayout(CString strLayoutName) { return m_strLayoutName == strLayoutName; }
	//加载布局信息
	BOOL LoadLayout(CWnd* pHostWnd,CString strStandardFile);
	//计算各区域大小
	void RecalcLayout(int X,int Y,int nWidth, int nHeight);
	//设置数据引擎指针
	void SetDataSource(IDataSourceEx* pDataSource,long nSourceID);
	//设置图层是否可见
	void SetVisable(BOOL bVisable);
	BOOL IsVisable() { return m_bVisable; }
	//获取数据引擎接口指针
	IDataSourceEx* GetDataSource() { return m_pDataSource;}
	//获取数据引擎ID
	long GetDataSourceID() {return m_nDataSourceID; }
	//
	void SetBizDialog(CHSBizDialog* pHSBizDialog) { m_pHSBizDialog = pHSBizDialog; }
	CHSBizDialog* GetBizDialog() { return m_pHSBizDialog; }

	CRegionNode* GetActiveNode() { return m_pActiveNode;}
	//请求数据
	virtual void GetRequestData(IRequestCollector* pReqtCollecter, StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	//更新数据
	virtual int	OnUpdateData(DataHead* pData);
	//变更布局大小
	virtual void OnSize(int cx, int cy,CWnd* pWnd);
	//绘制
	virtual void OnDraw(CDC* pDC);
	//鼠标操作
	void OnMouseAction(ActionType action,UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	//重置Region默认页签
	void ResetRegionNodeActiveDiagram();
	//按键事件
	int	 OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	//鼠标滚轮滚动
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//键盘精灵通知
	BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	//向图层的图元发送通知消息
	void SendNotifyToDrawBase(UINT nMsgId,IDrawBaseParam* pParam);
protected:
	BOOL BuildLayoutTree(CRegionNode* pParentNode,TiXmlElement* pParentNodeXml);
	void RecalcLayout(CRegionNode* pNode,int X,int Y,int nWidth, int nHeight);
	void SetActiveNode(CRegionNode* pActiveNode) { m_pActiveNode = pActiveNode; }
private:
	CHSBizDialog*    m_pHSBizDialog;     
	CString          m_strLayoutName;
	IDataSourceEx*	 m_pDataSource;				// 数据引擎接口指针
	long		     m_nDataSourceID;			// 数据请求ID
	CWnd*            m_pHostWnd;
	CRegionNode      m_rootNode;                //根区域结点
	CRegionNode*     m_pActiveNode;             //当前激活区域
	BOOL             m_bVisable;
	void ClearLayoutTree();
	void RemoveChildrenNode(CRegionNode* pNode);
};
