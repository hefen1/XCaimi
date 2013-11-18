/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�IDrawBase.h
 * �ļ���ʶ����ͼͼԪ����
 * ժ    Ҫ����ͼͼԪ����
 *
 * ��ǰ�汾��1.0
 * ��    �ߣ�
 * ������ڣ�2010-11-25
 *

 *******************************************************************************/
#pragma once

#include "..\hsstructnew.h"
#include "..\hsds_comudata.h"
#include "..\HSWinner.h"

#define OBJ_DIAGRAM_TECHKLINE	_T("DiagramTechKLine.DiagramTechKLine")

//�����Ϣ���ͽṹ
struct hsRefMouseMsg
{
	char m_cType;		//����

	enum Type 
	{
		none,
		RMove,RMoveDone,	     // �ƶ�
		RSel,RSelDone,		     // ѡ��
		StartPlanMove,		     // ����ƽ��
		CancelPlanMove,		     // ȡ��ƽ��
		DBClickPlanMove,	     // ˫��ƽ��
		VerPlanMove,		     // ����ƽ��
		RightHoriPlanMove,	     // �Ҽ�����ƽ��
		RightHoriPlanMoveDone,	 // �Ҽ�����ƽ�ƽ���
		RightVerPlanMove,	     // �Ҽ�����ƽ��
		RightZoomIn,		     // �Ҽ��Ŵ�
		RightZoomRect,		     // �õ��Ҽ��Ŵ�����
		ClickInExplainRect,      // �����ͼ��������ʾ����
		ClickInExplainDownRect,
		NextRefresh,		     // �´���Ҫˢ��
		LButtonMove,		     // ��������ƶ�(�ڷ�ͼ������)
		RButtonMove,		     // �Ҽ������ƶ�(�ڷ�ͼ������)
		LButtonRightOperator     // ͼ��������
	};

	hsRefMouseMsg()
	{
		m_cType = -1;
	}
	hsRefMouseMsg(Type cType)
	{
		m_cType = (char)cType;
	}
	//�ж�����
	BOOL IsType(Type cType) { return (m_cType == cType); }

};


class IRequestCollector
{
public:
	virtual void AddRequest(char* pAskData,short lAskLen) = 0;
	virtual void Release() = 0;
};

//ҵ�񴰿ڲ����ӿ�
struct IDrawBaseParam
{
	virtual CString WINAPI GetStringParam(CString strParamName,CString strDefault = _T("")) = 0;
	virtual int WINAPI GetIntegerParam(CString strParamName,int nDefault = 0  ) = 0;
	virtual double WINAPI GetDoubleParam(CString strParamName,double nDefault = 0 ) = 0;
	virtual POSITION GetStartPosition() = 0;
	virtual void GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue) = 0;
};

struct IRegionNode;
struct IDrawBase
{
	/********************************************************************************
	* �������� : ����ͼԪ
	* �������� : pParam�� �����ӿ�ָ��
	*			 
	*  ����ֵ  : ��
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL Create(IDrawBaseParam *pParam) = 0;
	virtual CString GetName() = 0;
	virtual void SetName(CString strName) = 0;
	/********************************************************************************
	* �������� : ��������λ��
	* �������� : ����ָ��
	*			 
	*  ����ֵ  : ��
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual void SetRegionNode(IRegionNode* pNode) = 0;
	/********************************************************************************
	* �������� : ����ͼԪλ��
	* �������� : λ������
	*			 
	*  ����ֵ  : 0 �ɹ� ����Ϊ�����
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL	SetRect(CRect rc) = 0;

	/********************************************************************************
	* �������� : ��ȡͼԪλ��
	* �������� : ��
	*			 
	*  ����ֵ  : ͼԪλ��
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual CRect	GetRect() = 0;

	/********************************************************************************
	* �������� : ����ͼԪ�ɼ�����
	* �������� : �Ƿ�ɼ�
	*			 
	*  ����ֵ  : ��
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual void SetVisible(BOOL bVisible) = 0;

	/********************************************************************************
	* �������� : ��ȡͼԪ�ɼ�����
	* �������� : ��
	*			 
	*  ����ֵ  : �Ƿ�ɼ�
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual BOOL GetVisible() = 0;

	/********************************************************************************
	* �������� : ����ͼԪ������Ϣ
	* �������� : �´�����Ϣ
	*			 
	*  ����ֵ  : ԭ������Ϣ
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual void UpdateCodeInfo(StockUserInfo* pCodeInfoNew) = 0;

	/********************************************************************************
	* �������� : ��ȡͼԪ������Ϣ
	* �������� : ��
	*			 
	*  ����ֵ  : ������Ϣ
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual StockUserInfo* GetCodeInfo() = 0;

	/********************************************************************************
	* �������� : ��ȡͼԪ��������
	* �������� : ��
	*			 
	*  ����ֵ  : ��������
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/
	virtual short	GetDrawType() = 0;

	/********************************************************************************
	* �������� : ��Ϣ��Ӧ ���������Ϣ��������Ϣ��
	* �������� : ��Ϣ������hsRefMouseMsg* pData = NULL, �����Ĳ�������
	*			 
	*  ����ֵ  : �ɹ�TRUE
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	* ��ע	   : �Ҽ���Ϣ��Ҫ���봰�ڲ���
	*******************************************************************************/
	virtual BOOL MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL RButtonUp(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL) = 0;
	virtual BOOL OnCursor(UINT nFlags, CPoint point) = 0;
	virtual int	 OnKey(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual int  OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) = 0;
	/********************************************************************************
	* �������� : ���̾���֪ͨ
	* �������� : 
	*			 
	*  ����ֵ  : TRUE:��֪ͨ�������� FALSE:��֪ͨδ������
	*******************************************************************************/
	virtual BOOL OnKeyBoardNotify(WPARAM wParam, LPARAM lParam) = 0;
	// �˵���Ϣ
	virtual long OnMenuMsg(WPARAM wParam, LPARAM lParam) = 0;

	/********************************************************************************
	* �������� : ��ͼ
	* �������� : �豸DC
	*			 
	*  ����ֵ  : 0 �ɹ� ����Ϊ�����
	*******************************************************************************/
	virtual int Draw(CDC* pDC) = 0;


	/********************************************************************************
	* �������� : ��ȡͼԪ�����
	* �������� : nMask ���룻 lRetLen ������
	*			 
	*  ����ֵ  : ������ַ���
	* ��ע	   :
	*******************************************************************************/
	virtual BOOL GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE) = 0;

	/********************************************************************************
	* �������� : �յ����ݸ���
	* �������� : 
	*			 
	*  ����ֵ  : 0 �ɹ� ����Ϊ�����(�粻��Ҫ�����������ȵ�)
	*******************************************************************************/
	virtual int	OnUpdateData(DataHead* pData) = 0;

	virtual void OnActiveChange(BOOL bShow) = 0;
	virtual void OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam) = 0;

	/********************************************************************************
	* �������� : ɾ������
	* �������� : ��
	*			 
	*  ����ֵ  : 0 �ɹ� ����Ϊ�����
	*******************************************************************************/
	virtual int Delete() = 0; 
};

struct IRegionNode 
{
	/********************************************************************************
	* �������� : ��ȡ�������ھ��
	* �������� : 
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual HWND GetHostHandle() = 0;
	/********************************************************************************
	* �������� : ����ͼԪ
	* �������� : strDiagramName ͼԪ����
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual BOOL ActiveDiagram(CString strDiagramName) = 0;

	/********************************************************************************
	* �������� : ˢ������
	* �������� : 
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual BOOL Invalidate(CRect* pRC = NULL,BOOL bErase = TRUE) = 0;

	/********************************************************************************
	* �������� : ���ù��
	* �������� : 
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual BOOL SetCursor(LPCSTR lpCursorName) = 0;

	/********************************************************************************
	* �������� : ��������ռ�������ȡ�����IRequestCollector::Release�ͷ�
	* �������� : 
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual IRequestCollector* GetRequestCollector() = 0;

	/********************************************************************************
	* �������� : ��������
	* �������� : 
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual BOOL SendRequest(IRequestCollector* pReqCollector, BOOL bAutoClear = TRUE) = 0;

	/********************************************************************************
	* �������� : ͼ���ڲ�֪ͨ
	* �������� : nMsgId:֪ͨ��Ϣid,pParam:֪ͨ����
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual void SendNotifyMsg(UINT nMsgId,IDrawBaseParam* pParam) = 0;
	/********************************************************************************
	* �������� : ��ҵ��
	* �������� : nMsgId:֪ͨ��Ϣid,pParam:֪ͨ����
	*			 
	*  ����ֵ  : 
	*******************************************************************************/
	virtual void SendLinkageMsg(UINT nMsg,IDrawBaseParam* pParam,
		CString strPanelName = _T(""),CString strLayout = _T(""),CString strOwner = PUBLIC_OWNER ) = 0;
};

struct IDiagramLayer
{
	
};