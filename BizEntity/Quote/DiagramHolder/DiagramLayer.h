#pragma once
#include "afx.h"
#include "tinyxml.h"
#include "QuoteDiagram.h"
#include "DataSourceDefineEx.h"
#include "DrawBase.h"
#include "..\DiagramFoundation\DiagramTab.h"
#include "HSBizDialog.h"

//������λ��RegionNodePostion
#define RNP_UNKNOW  'N'
#define RNP_TOP     'T'
#define RNP_BOTTOM  'B'
#define RNP_LEFT    'L'
#define RNP_RIGHT   'R'

//���������в��Խ��
#define RNHT_NOWHERE            0x0000
#define RNHT_IN_HOR_SPLITER     0x0001   //in horizontal spliter
#define RNHT_IN_VER_SPLITER     0x0002   //in vertical  spliter
#define RNHT_INRECTUNIT         0x0004   //in rect
#define RNHT_INTABBAR           0x0005   //in tab bar

//���ַָ���
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

//��������
typedef CTypedPtrArray<CPtrArray,CDiagramInfo*>  CDiagramInfoArray;
class CRegionNode: public IRegionNode,public IDiagramTabNotify
{
public:
	friend class CDiagramLayer;
	CRegionNode();
	~CRegionNode();
	//IRegionNode�ӿ�ʵ��
	virtual HWND GetHostHandle();
	virtual BOOL ActiveDiagram(CString strDiagramName);
	virtual BOOL Invalidate(CRect* pRC = NULL,BOOL bErase = TRUE);
	virtual BOOL SetCursor(LPCSTR lpCursorName);
	virtual IRequestCollector* GetRequestCollector();
	virtual BOOL SendRequest(IRequestCollector* pReqCollector, BOOL bAutoClear = TRUE);
	virtual void SendNotifyMsg(UINT nMsgId,IDrawBaseParam* pParam);
	virtual void SendLinkageMsg(UINT nMsg,IDrawBaseParam* pParam,
		CString strPanelName = _T(""),CString strLayout = _T(""),CString strOwner = PUBLIC_OWNER );
	//IDiagramTabNotify�ӿ�ʵ��
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
	char               m_cLocation; //λ��:T-��;B-��;L-��;R-��
	int                m_nMeasure;  //�ߴ�:��m_cLocationһ�����m_rect

	CDiagramLayer*     m_pLayer;
	CWnd*              m_pHostWnd;
	CRect              m_regionRect;    //�����С
	CRect              m_spliterRect;   //�ָ�����С
	BOOL               m_bFix;          //���ӽ���Ƿ�̶���С
	CRegionNode*       m_pLeft;         //��ڵ�:�����     
	CRegionNode*       m_pRight;        //�ҽڵ�:�һ���
	CDiagramTab        m_tabBar;        //��ǩҳ
	CDiagramInfoArray  m_arrDiagramInfo;
	int                m_nActiveDiagramIndex;
	int                m_nInitActiveDiagramIndex;
    
	void LoadNodeInfo(TiXmlElement* pNodeXml);
};

//ͼ��ʵ����
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
	
	//��ȡ��������
	CString GetLayoutName() { return m_strLayoutName; }
	//�Ƿ���ָ������
	BOOL IsLayout(CString strLayoutName) { return m_strLayoutName == strLayoutName; }
	//���ز�����Ϣ
	BOOL LoadLayout(CWnd* pHostWnd,CString strStandardFile);
	//����������С
	void RecalcLayout(int X,int Y,int nWidth, int nHeight);
	//������������ָ��
	void SetDataSource(IDataSourceEx* pDataSource,long nSourceID);
	//����ͼ���Ƿ�ɼ�
	void SetVisable(BOOL bVisable);
	BOOL IsVisable() { return m_bVisable; }
	//��ȡ��������ӿ�ָ��
	IDataSourceEx* GetDataSource() { return m_pDataSource;}
	//��ȡ��������ID
	long GetDataSourceID() {return m_nDataSourceID; }
	//
	void SetBizDialog(CHSBizDialog* pHSBizDialog) { m_pHSBizDialog = pHSBizDialog; }
	CHSBizDialog* GetBizDialog() { return m_pHSBizDialog; }

	CRegionNode* GetActiveNode() { return m_pActiveNode;}
	//��������
	virtual void GetRequestData(IRequestCollector* pReqtCollecter, StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	//��������
	virtual int	OnUpdateData(DataHead* pData);
	//������ִ�С
	virtual void OnSize(int cx, int cy,CWnd* pWnd);
	//����
	virtual void OnDraw(CDC* pDC);
	//������
	void OnMouseAction(ActionType action,UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	//����RegionĬ��ҳǩ
	void ResetRegionNodeActiveDiagram();
	//�����¼�
	int	 OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	//�����ֹ���
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//���̾���֪ͨ
	BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);
	//��ͼ���ͼԪ����֪ͨ��Ϣ
	void SendNotifyToDrawBase(UINT nMsgId,IDrawBaseParam* pParam);
protected:
	BOOL BuildLayoutTree(CRegionNode* pParentNode,TiXmlElement* pParentNodeXml);
	void RecalcLayout(CRegionNode* pNode,int X,int Y,int nWidth, int nHeight);
	void SetActiveNode(CRegionNode* pActiveNode) { m_pActiveNode = pActiveNode; }
private:
	CHSBizDialog*    m_pHSBizDialog;     
	CString          m_strLayoutName;
	IDataSourceEx*	 m_pDataSource;				// ��������ӿ�ָ��
	long		     m_nDataSourceID;			// ��������ID
	CWnd*            m_pHostWnd;
	CRegionNode      m_rootNode;                //��������
	CRegionNode*     m_pActiveNode;             //��ǰ��������
	BOOL             m_bVisable;
	void ClearLayoutTree();
	void RemoveChildrenNode(CRegionNode* pNode);
};
