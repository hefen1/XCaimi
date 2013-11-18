/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TechKlineMgr.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��������ͼԪ�ӿ�(�����)
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-4-25
*
*	��	  ע�� �����ʵ��ͳһ��������������任�ȵ�
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once
#include "..\..\..\Common\hsds_comudata.h"
#include "..\..\..\Common\QuoteComm\DrawBase.h"
#include "..\..\..\Common\FormulaManPublic.h"
#include "..\..\..\Common\FormulaManInterFace.h"
#include "..\..\..\Common\QuoteComm\QuoteDiagram.h"
#include "..\..\..\Common\DataSourceDefineEx.h"
#include "..\..\..\Common\HSMessage.h"

#include "HSLinear.h"
#include "HSDrawBaseStruct.h"

#define MAX_FENSHI_DAYS	10
#define PKLINE_TYPE_FENSHI   		0x0001  // ��ʱͼ
#define PKLINE_TYPE_K       		0x0002  // k ��ͼ
#define PKLINE_NAME_FENSHI   		"PKLINE_NAME_FENSHI"  // ��ʱͼ
#define PKLINE_NAME_K       		"PKLINE_NAME_K"  // k ��ͼ
//////////////////////////////////////////////////////////////////////////
//
#define HsTechLine_Left_ChuQuan	    0x00000001 // ��߳�Ȩ
#define HsTechLine_Back_ChuQuan		0x00000002 // ����Ȩ

struct CTechKLineData
{
	StockDay*                       m_pStockDayData;				//ԭʼ����(��Գ�Ȩ����)
	int								m_nCurrentDataLen;				//��ǰ���ݳ���
	float							m_afKLineHorizCellWidth[20];	//����Ԫ��������
	int								m_nWidthIndex;					//��ǰʹ������������
	BOOL							m_bDataFinished;				//�Ƿ�������м���������?
	int								m_nCurrentRequestLen;			//��ǰ����ĳ���
	BOOL							m_bNeedRefresh;					//���¹�Ʊ����ʱ���Ƿ���Ҫ�����������ݵ��ж�����
	int								m_nCurrentDataPos;				//��ǰ��λ��
	long							m_lChuquanMask;					//��Ȩ��Ȩ��־
	StockDay*						m_pStockDayDataChuquan;			//��Ȩ��������

	CTechKLineData();
	BOOL				Init(int nSize);					//��ʼ��
	BOOL				Release();							//�ͷ��ڴ�
	BOOL				Refresh(int nNewSize);				//ˢ��
	BOOL				CopyData(StockCompDayDataEx* pData, int nSize);
	float				GetKLineHorizCellWidth();
	float				GetNextWidth(BOOL bNext);
	BOOL				IsKLineDataFinished(int nReqireLen, int nCurIndex);
	long				GetIndexDate(int nIndex);
	BOOL				SetRefreshFlag(BOOL bNeedRefresh);

	/*��ʱ�з����ߵ����ݴ�������*/
	int					ResetPos(int nNewLen);
//	StockCompDayDataEx*	GetStockCompDayDataEx();
	StockDay*			GetStockDayData();
	int					GetStockCompDayDataLen();

//��Ȩ��Ȩ����
	long				GetChuquanMask();
	BOOL				CalcChuQuan(int nDayType, CArray<ChuQuanData*,ChuQuanData*>& ayChuQuanData, 
							long lMask, long nPriceUnit);
	long				GetSpanDays(long lBgnDate,long lEndDate);
	BOOL				InitChuquanDayData(BOOL bRelease = FALSE);
	StockDay*			GetCurrentDrawData();
};

//////////////////////////////////////////////////////////////////////////
class CTechKLineMgr;
struct CFenshiData
{
	StockUserInfo					m_info;
	DrawFenShiData*					m_pPriceValues; // ������������ָ��
	long*							m_pChengJiaoValues; // �ɽ�������ָ��
	long*							m_pAvePriceValues; // ��������ָ��
	double long						m_lTotalVol; // ��������
	double long						m_lTotalPrice; // �ܽ���
	int								m_nTimeID;		//��ʱ��-�������������Ӷ�û���κ���������
	CTechKLineMgr*					m_pMgr;


	int								m_nCurrentDataLen;		//��ǰ���ݳ���
	int								m_nLastReqDay;			//��һ�����������
	int								m_nLastRecvDay;			//��һ���յ��Ķ��շ�ʱ����
	int								m_nOneDayDataNum;		//ÿ�����ݸ���
	int								m_nRecvDataLen;			//��ǰ�Ѿ�������ݸ���
	int								m_nCurFenshiPos;		//��ǰ�Ķ�λ��--����Ҫ������������

	DrawFenShiData*					m_pPriceValuesCur;		//��ǰ��λ��- ������������
	long*							m_pChengJiaoValuesCur;	//��ǰ��λ��- �ɽ�������ָ��
	long*							m_pAvePriceValuesCur;	//��ǰ��λ��- ��������ָ��
	long							m_ayDayData[MAX_FENSHI_DAYS + 1];
	long							m_ayPrevClose[MAX_FENSHI_DAYS + 1];		//����	
	CTechKLineData					m_minData;				//�������������

	CFenshiData();
	BOOL				Init(StockUserInfo* pInfo, int nSize, BOOL bForce);	//��ʼ��
	BOOL				Release();							//�ͷ��ڴ�
	BOOL				Refresh();							//ˢ��
	int					UpdataHisTrend(AnsHisTrend* pTrendData, int& nCurDataLen);
	int					UpdataTreadEx(AnsTrendData_Ext* pExtData);
	int					UpdateRealTimeData(CommRealTimeData_Ext* pCommRealTimeData);
	int					UpdateFenshiDataPos(int nDayPosNew, int& nLeftLen);
	int					GetPosTime(int nPos, CString& strText, BOOL bIntegral = TRUE);
	long				GetPrevClose();
	BOOL				GetMinDataRequest(IRequestCollector* pReqCollector, int nReqLen);
	int					UpdateMinTechData(DataHead* pData);
	BOOL				GetNeedRefresh(StockUserInfo* pInfo);
	CString				GetLastNewPrice(BOOL bBuy);				//��ȡ���¼۸���һ����һ��
	void				UpdateOneMinuteEmptyData();				//һ���ӿ�����
	void				SetTechMgr(CTechKLineMgr* pMgr);
	void				StartTimer();
	void				StopTimer();

	static VOID CALLBACK    MinuteProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);  // ��ʱ������������䶨ʱ��
};

//////////////////////////////////////////////////////////////////////////
class CManagerBase
{
public:
	CManagerBase();
	~CManagerBase();
protected:
	//��ʱ����
	CFenshiData						m_dataFenshi;		
	//K������
	CTechKLineData					m_dataKLine;
	//���ݳ���
	long							m_lDataVauleLen;
	IFormulaMan*					m_pFormulaMan;
	int								m_nStartPos;
	int								m_nEndPos;
	//����
	long							m_lDate;
	StockType						m_stockType;
	CGraphics::CAxesType			m_nAxesType;

public:
//	virtual		int									GetStockCompDayDataEx(StockCompDayDataEx*& pStockCompDayData);
	virtual		int									GetStockDayData(StockDay*& pStockDaya);
	virtual		int									GetStockDay(StockDay*& pStockDay);
	virtual		int									GetStockFenShiData(DrawFenShiData*& pDrawFenshiData);
	virtual		int									GetStockChengJiaoValues(long*& pChengJiaoValues);
	virtual		int									GetStockAveFenshiPrice(long*& pAvePriceValues);
	virtual		BOOL								GetStartEndPos(int& nStartPos, int& nEndPos);
	virtual		IFormulaMan*						GetFormulaMan();
	virtual		CString								GetTimeByPos(int nCursorPos,CString& strText,int nType = 0);
	virtual		long								GetFenshiPrevClose();
	virtual		CGraphics::CAxesType				GetAxesType();
	virtual		int									DelayDeleteTechLine(CTechKLineImp* pTechLine);
	virtual 	StockUserInfo*						GetStockUserInfo() = 0;
	virtual		void								GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice) = 0;
	virtual		int									DrawTitle(void* pRefDraw) = 0;
	virtual		short								GetKLineType() = 0;
	virtual		CWnd*								GetParentWnd() = 0;
	virtual		void								GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice) = 0;
};