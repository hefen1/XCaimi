/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TechKlineMgr.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��������ͼԪ�ӿ�ʵ��(�����)
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-12-1
*
*	��	  ע�� �����ʵ��ͳһ��������������任�ȵ�
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "ManagerBase.h"
#include "DialogHisTrend.h"
#include "DrawBox.h"
#include "DrawBottom.h"
#include "TechKLineTab.h"
#include "DrawInfo.h"
#include "DrawUnitPositonCtrl.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "..\..\..\Common\QuoteLangMngInterFace.h"

enum eTechLineOperator
{
	eTechLineOperator_mainDel		= 0x0001,		//��ͼɾ��
	eTechLineOperator_subDel		= 0x0002,		//��ͼɾ��
	eTechLineOperator_mainAddnew	= 0x0010,		//��ͼ����������
	eTechLineOperator_subAddnew		= 0x0020,		//��ͼ����
	eTechLineOperator_mainModify	= 0x0100,		//��ͼԭ���޸�
	eTechLineOperator_subModify		= 0x0200,		//��ͼԭ���޸�
};

class CTechKLineMgr : public CDrawBase, public CManagerBase
{
//����
public:
	CTechKLineMgr();
	~CTechKLineMgr();
protected:
//��Ա

//	std::list<StockCompDayDataEx*>	m_listData;

	int								m_nRequestPeriod;		//�������ݴ�С
	std::list<CTechKLineImp*>		m_listKLine;			//K��ͼԪ����
	std::list<CTechKLineImp*>		m_listFenshi;			//��ʱͼԪ����
	CDrawUnitPosCtrlMgr*			m_pPostionCtrlMgr;		//λ�ÿ�����

	short				m_nKLineType;				//��ͼ����
	IDataSourceEx*		m_pDataSourceEx;
	IBlockManager*		m_pBlockManager;			// ������
	IQuoteLang*			m_pQuoteLang;				//	��Դ
//	CImageList			m_imageList;

	//���λ��
	CPoint				m_ptMouseFocus;					//���
	CPoint				m_ptCursorLine;					//ʮ�ֹ��
	//��Ϣ��ʾ
	CDrawBox*			m_pDrawBox;
	//���������еĵײ�
	CDrawBottom*		m_pDrawBottom;
	//��Ϣ����
	CDrawInfo*			m_pDrawInfo;

	MouseTipTextInfo	m_sMouseTipTextInfo;			//��ʾ��Ϣ
	int					m_nSelDataIndex;
	CurrentFinanceData	m_curFinance;	
	long				m_lFinaceHand;
	WORD				m_wPeriodType;

	CDialogHisTrend*	m_pDialogHisTrend;
	CTechKLineTab		m_bottomTab;					//TABҳǩ
	CTechKLineTabNotify	m_tabNotify;					//TAB��Ϣ����

	CString				m_strAppName;
	CString				m_strYPerMsg;
	CString				m_strPosChangeMsg;				//
	BOOL				m_bPosChanged;					//ͼԪλ�ñ����־
	CArray<ChuQuanData*,ChuQuanData*>	m_ayChuQuanData;		//��Ȩ����

	int					m_nDelayDeleteTimerID;
	CTechKLineImp*		m_pTechLineDelayDelete;
//interface:
public:
	virtual BOOL		Create(IDrawBaseParam *pParam);
	virtual void		SetRegionNode(IRegionNode* pNode);
	virtual BOOL		SetRect(CRect rc);
	virtual CRect		GetRect();
	virtual void		SetVisible(BOOL bVisible);
	virtual BOOL		GetVisible();
	virtual void		UpdateCodeInfo(StockUserInfo* pCodeInfoNew);
	virtual StockUserInfo*	GetCodeInfo();
	virtual short		GetDrawType();
	virtual BOOL		MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL		RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL		RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
	virtual BOOL		OnCursor(UINT nFlags, CPoint point);
	virtual int			OnKey(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL		OnKeyBoardNotify(WPARAM wParam, LPARAM lParam);

	virtual long		OnMenuMsg(WPARAM wParam, LPARAM lParam);
	virtual int			Draw(CDC* pDC);
	virtual BOOL		GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce);
	virtual int			OnUpdateData(DataHead* pData);
	virtual void		OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam);
	virtual int			Delete();

//CManagerBase interface
//	virtual int									GetStockCompDayDataEx(StockCompDayDataEx*& pStockCompDayData);
	virtual	int									GetStockDayData(StockDay*& pStockDaya);
	virtual StockUserInfo*						GetStockUserInfo();
	virtual void								GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual int									DrawTitle(void* pRefDraw);
	virtual short								GetKLineType();
	virtual CWnd*								GetParentWnd();
	/*��ȡ��ǰ��ͼԪ����*/
	std::list<CTechKLineImp*>*					GetCurrentTechLines();
	/*��ȡ��ͼ�ϵ������Сֵ*/
	void										GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice);
	virtual HWND								GetParentHWnd();
	virtual int									DelayDeleteTechLine(CTechKLineImp* pTechLine);

//����
	void								SetChildRect();
	void								ReCreate(IDrawBaseParam *pParam);
	
	void								DrawCursorLine(CDC* pDC);
	int									GetKLineNextTime(int nIndex, char* pszTime, WORD wPeriodType);
	int									GetDataXPos(int nDataIndex);
	CString								FormatTip(int nDataIndex, WORD wPeriodType);
	int									HitTest(CPoint point);
	int									SwitchPage(WORD wPageType);
	int									ChangeType();

	/*��ȡ��һ����������*/
	WORD								GetNextPageType(BOOL bNext);
	/*ʵʱ��������*/
	int									SendRealTimeRequest();							
	/*��ȡ������ͼԪ*/
	CTechKLineImp*						GetTopLine();
	/*��ȡ�ײ���ͼԪ*/
	CTechKLineImp*						GetBottomLine();
	/*�л���ʼ����*/
	int									MovePosition(CPoint ptMove);
	/*�Ե�ǰΪ��׼���ƶ�ʮ�ֹ��λ��*/
	int									MoveCursorPos(BOOL bNext);				
	/*��ʾ��ʷ����*/
	int									ShowHisTrend();
	/*ˢ��*/
	void								DiagramInValidate(CRect rcInvalidate = CRect(0, 0, 0, 0));
	/*��ȡԭ�ȵĵ�����*/
	int									GetAddLines(std::list<CTechKLineImp*>&listKLine, short nType);
	/*ָ���߲���*/
	int									ModifyTechLine(ExpPropery* pExp, BOOL bDelOld = TRUE);
	/*���ӣ���Ʊ���������*/
	int									ModifyTechLine(StockUserInfo* pStock, BOOL bDelOld = TRUE);
	/*��ͼ���������ݽ���*/
	int									UpdateAddLinesData(DataHead* pData);
	/*tab��ı�*/
	int									OnTabItemChanged(TechIndex* pTechIndex);
	int									OnTabItemChanged(TechTemplete* pTechTemplete);
	/*����������ͼ����л�*/
	int									SwitchMultiDiagram(CStringArray& ayStrtemplete, float fRadio);
	/*��Ϣ��������Ҫ����������ת��ֵ*/
	int									UpdateDrawInfoData(CArray<CDrawInfoUnitData>& ayData, long lDateLast);
	/*ɾ��ָ����*/
	int									DeleteLine(CTechKLineImp* pImpDel = NULL);
	/*��Ȩ��Ȩ����*/
	int									CalcChuquanMark(int nMenuID);
	int									ClearOldChuquanData();
	CArray<ChuQuanData*,ChuQuanData*>*	GetChuquanData();

	static VOID CALLBACK				DelayDelete(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime); 
};