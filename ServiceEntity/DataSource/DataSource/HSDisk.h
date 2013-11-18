/*******************************************************************************
* Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
* All rights reserved.
*
* �ļ����ƣ�HSDisk.h
* �ļ���ʶ��DISK�ļ���������
* ժ    Ҫ��DISK�ļ���������
*
* ��ǰ�汾��
* ��    �ߣ�
* ������ڣ�
*
* ȡ���汾��
* ԭ �� �ߣ�
* ������ڣ�
* �ο�����: DISK�������������.DOC
*******************************************************************************/
#if !defined(AFX_HSDISK_H__27C28C1C_61E2_4180_BC44_16F55261C145__INCLUDED_)
#define AFX_HSDISK_H__27C28C1C_61E2_4180_BC44_16F55261C145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrendFile.h"
#include "DataSourceDefineEx.h"
#include "FormulaManPublic.h"
#include "KLineDataFile.h"

#define  AnalisysFor5             0x0002  //�����
#define  AnalisysFor15            0x0003  //ʮ�����
#define  AnalisysFor30            0x0004  //��ʮ����
#define  AnalisysFor60            0x0005  //��ʮ����
#define  AnalisysForDay           0x0006  //����
#define  AnalisysForWeek          0x0007  //����
#define  AnalisysForMonth         0x0008  //����
#define  AnalisysForMoreDay       0x0009  //������

class  CHSDisk 
{
public:
	

public:
	CHSDisk();
	CHSDisk(HWND hWnd,UINT	nNotifyMessage);
	virtual ~CHSDisk();
protected:



public:
	static int CalcChuQuan(unsigned long lPreDate,int nDayType,StockDay& pDay,
		CArray<ChuQuanData*,ChuQuanData*>& ayChuQuanData,
		long lMask,long nPriceUnit);

	static int  CaclExRight(StockDay* pDay, int nLen, CodeInfo* pCodeInfo, int nExRight/* = 0*/);




public:
	
	/********************************************************************************
	* �������� :�����������Ϣ,���ؾ������ڳ�ȡ������(����ϳ����ߵ�)	
	* �������� :void* pAsk  - AskData�ṹ��ַ
				long lRetSize - �����StockCompDayDataEx���ݸ��� 
								����0���߾���ֵ�����ļ����ݸ���,�����ļ����ݸ���
								С��0�Ҿ���ֵС���ļ����ݸ���, ���ļ�ĩβ��ǰƫ��|lRetSize | ������
								����0�Ҿ���ֵ�����ļ����ݸ��� 
				int nMask - 1 : ��ʾ��ȡָ���ļ�, ���򿪼��ر�, �ļ�ָ�����ļ�ͷ
							0 : ��ʾ��ȡָ���ļ�, �ļ�ָ�뱻����, ���ر��ļ�
				int nExRight  : ��Ȩ״̬ 0 ����Ȩ  HS_HxDataSource_Left_ChuQuan ǰ��Ȩ HS_HxDataSource_Back_ChuQuan ���Ȩ
	*  ����ֵ  :RetData * ָ�����ݵĵ�ַ(���ݱ�StockCompDayDataEx���), ʧ�ܷ���NULL	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/ 
	void*   GetDayData(void* pAsk,long lRetSize,int nMask = 0, int nExRight = 0);

	/********************************************************************************
	* �������� :�����������Ϣ,���ؾ������ڳ�ȡ������(����ϳ����ߵ�)	
	* �������� :void* pAsk  - AskData�ṹ��ַ
				int nMask - δʹ��	
				int nExRight - ��Ȩ״̬ 0 ����Ȩ  HS_HxDataSource_Left_ChuQuan ǰ��Ȩ HS_HxDataSource_Back_ChuQuan ���Ȩ
	*  ����ֵ  :�ϳɺ�����ָ��RetData *,ʧ�ܷ���NULL	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/ 
	void*   GetDayData(void* pAsk,int nMask = 0, int nExRight = 0);

	//��ȡ��ʱ����	
	void*   GetTraceData(void* pAsk,long& nSize,long lDate);  
	//��ȡ��ʷ��ʱ����
	void*   GetHisTraceData(void* pAsk,long& nSize);  
	//�ֱ�
	void*   GetTickData(void* pAsk,long& nSize,long lDate);
	/********************************************************************************
	* �������� :����InitPath();InitData();	
	* �������� :	
	*  ����ֵ  :	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	* ����֮�� : ����ȥ��
	*******************************************************************************/ 
	static void			Init();
	/********************************************************************************
	* �������� :��ʼ�����г�����Ҫʹ�õ�������·�� ��������,�ڻ�,��Ѷ����·��
				��·�����������ļ���ȡ�û���������,��Ѷ��·��,����Ĭ��·��
	* �������� :void	
	*  ����ֵ  :void	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/ 
	static void			InitPath();

	/********************************************************************************
	* �������� :ȡ��ָ��ָ���ļ����ļ�ָ��	
	* �������� :CFile*& pFile - ָ��,�·���ָ�뽫���浽��
				CString& strFileName- �ļ��� ����Ҫ����
	*  ����ֵ  :�ɹ�TRUE ʧ��FALSE	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	* ����֮�� :�˺���δʹ��   ����ȥ��
	*******************************************************************************/ 
	static BOOL			InitFile(CFile*& pFile, CString& strFileName);


public:
	/********************************************************************************
	* �������� :д��ʷ����			
	* �������� :DataHead* pHead - ���ݵ�ַ	
	*			����:
	*			RT_STOCKTICK - �޴���
	*			RT_MAJORINDEXTICK - BigTraceData*  дStockHistoryData����
	*			RT_BUYSELLORDER   - �޴���
	*			RT_TREND		 - ����ͷҪ��AnsTrendData, дStockHistoryData
	*			RT_BUYSELLPOWER  - ������������  ����ͷ��ַDOWNCAST(BuySellData)	дStockHistoryData
	*			HISBUYSELLDATA - DOWNCAST(NewBuySellDataOld),дStockHistoryData
	*			RT_MAJORINDEXTREND	- DOWNCAST(AnsMajorIndexTrend), дIndexRealTime
	*			RT_MAJORINDEXDBBI - ���ָ�� DOWNCAST(BigDataDuoKongOld) дStockHistoryData
	*			RT_MAJORINDEXADL - ����ADL DOWNCAST(BigDataADLOld) дStockHistoryData
	*			RT_MAJORINDEXBUYSELL - ������������ DOWNCAST(BigDataBuySellOld) дStockHistoryData
	*			RT_LEAD   - ����ָ��DOWNCAST(AnsLeadData) дHSIndexRealTime
	*			RT_ADDTREND - ���ͬ�� DOWNCAST(MultiStockHisDataOld) NowData & OtherData
	*			HISTEND - δ����			
	*  ����ֵ  :void	
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/ 
	void WriteHistoryData(DataHead* pHead);
	/********************************************************************************
	* �������� :д��������	
	* �������� :AskData* pCurAsk - ���ΪNULL,��ָ��ǰ����,
	*			StockCompDayDataEx* pNewData - ����ָ��
	*			int nSize	- ���ݸ���
	*  ����ֵ  :BOOL �ɹ�����TRUE ʧ�ܷ���FALSE		
	* �������� :	
	* ��    �� :
	* ������� :
	* �޸ļ�¼ :
	*******************************************************************************/ 
	BOOL WriteDayDataNew(CodeInfo* info,int period,StockCompDayDataEx* pNewData,long nSize);

	//д��ʷ��ʱ 
	BOOL WriteHisTraceData(CodeInfo* pCode,long lDate, void* pData, long lSize);
	//д��ʱ����
	BOOL WriteNewTraceData(CodeInfo* pCode, void* pData, long lSize,long lDate);
	//д�ֱ�
	BOOL WriteTickData(CodeInfo* pCode, void* pData, long lSize,long lDate);
	//
protected:
	//��װ������,����ָʾ��ǰ������·��
	static CString	m_strCurPath;

public:
	static short g_nCurServerID;
	
private:
	CMap<CString,LPCTSTR,SeverCalculateData,SeverCalculateData>  m_calculateMap;
	CMapWordToPtr  m_mapTrendfile;
	CMapStringToPtr  m_mapHisTrendfile;
	CMapStringToPtr  m_mapTickFile;
public:
	static void InitRiseFallData(char* pData);
	void MakeServerCalculate(AskData* pAsk);
	RetDiskData* MakePeriodData(StockCompDayDataEx* pDataStartPos, UINT nCount, 
		ReqDayData* pAskData,
		int nMask /*= 0*/, int nExRight/* = 0*/);
};

#endif // !defined(AFX_HSDISK_H__27C28C1C_61E2_4180_BC44_16F55261C145__INCLUDED_)



