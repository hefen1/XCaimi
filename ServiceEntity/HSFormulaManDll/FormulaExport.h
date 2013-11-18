
/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	FormulaExport.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��ʽ����ģ��Ľӿ��ඨ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/

#if !defined(AFX_COMPEXPORT_H__FC0269FD_A095_4EE3_823A_54DF4E9D6518__INCLUDED_)
#define AFX_COMPEXPORT_H__FC0269FD_A095_4EE3_823A_54DF4E9D6518__INCLUDED_

#include "FormulaManInterFace.h"
#include "CompileInterFace.h"
#include "FormulaStruct.h"
#include "ExpPublic.h"
#include "ConfigInterface.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*************************************************************
*	�� �� �ƣ�	CFormulaExport
*	�� �� ����	��ʽ����ģ��Ľӿ���
***************************************************************/
class CFormulaExport: public IFormulaMan
{
public:
	/******************************************************************
	*	�������ܣ�	����
	*	����������	long lExe = 0 : ��ģ���Ƿ���Exe����ʽ����
	*	�� �� ֵ��	��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	CFormulaExport(long lExe = 0);
	virtual ~CFormulaExport();

public:
	/******************************************************************
	*	�������ܣ�	�򿪹�ʽ������(��ͬʱ���ع�ʽ���������ھ��)
	*	����������	HWND hwndNotify : [in] ��ʽ�������ĸ�����
	*				long** pThis	: [in] ������
	*	�� �� ֵ��	long : ��ʽ���������ھ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	long OpenCompile(HWND hwndNotify,long** pThis);

	/******************************************************************
	*	�������ܣ�	�ر�(�˳���ʽ�����ڵ���Ϣѭ�������ٱ�ģ��ռ�õ���Դ)
	*	����������	HWND hwndNotify : [in] ������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	void CloseCompile(HWND hwndNotify);

	/******************************************************************
	*	�������ܣ�	��ʽ����ģ������ӿ�
	*	����������	WPARAM wParam : [in] ��lParam���,��lParam˵��
	*				LPARAM lParam : [in] HSOpenExpressUseData*��������,������ʽ����HSOpenExpressUseData::m_wType&0xF00����
										hxOpenExpress_Edit	  : ���޸Ĺ�ʽ����, wp������, ����0
										hxOpenExpress_Notes   : ��ע�ʹ���, wp������
										hxOpenExpress_Eidolon : �򿪲����޸Ĵ���, wp������
										hxShowAlignWindow	  : �õ�ָ����(����Ĵ���(CWnd*)HSOpenExpressUseData::m_pData�ϴ���)����, wp������, ����ָ����ָ��
										hxShowKeyboardWindow  : ��ʾ���̾��鴰��, wp ��ʾ�����ش���(0���0), ����0
										hxInitKeyboardWindow  : ��ʼ�����̾��鴰��, wp������, ����0
										hxShowByExpressType   : �ɹ�ʽ��������ʾ��ʽ, wp������, ����0
										hxShowReturnByExpressType : �ɹ�ʽ��������ʾ��ʽ,������ѡ�й�ʽ, wp������
										hxOpenExpressByConditionStockType : ѡ��, wp������, ����0
										hxOpenExpressBySortStockType : ָ������, wp������, ����0
										hxOpenBlockAnalysis   : ������, wp������, ����0
										hxOpenUniteAnalysis   : ��������, wp������, ����0
										hxOpenAlarmManagerDlg : ��Ԥ������, wp������, ����Ԥ������ָ��
										hxOpenFindDataDlg     : �򿪹�ʽ���Ҵ���, wp������, ����0
										hxOpenAddStkToAlarm   : �򿪼ӹ�Ʊ��Ԥ������, wp������, ����0
											
	*	�� �� ֵ��	long : ��lParam���,��lParam˵��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	long OpenExpress( WPARAM wParam,LPARAM lParam );

	/******************************************************************
	*	�������ܣ�	����Ϣ���͵���ʽ�����ڽ��д���
	*	����������	UINT message : [in] ��ϢID
	*				WPARAM wParam: [in] ��Ϣ����
	*				LPARAM lParam: [in] ��Ϣ����
	*	�� �� ֵ��	long : ������Ϣ�ķ���ֵ������ͬ,��������Ϣ��������˵��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
	long OnMessage( UINT message ,WPARAM wParam,LPARAM lParam);

/******************************************************************
	*	�������ܣ�	��ʽ����
	*	����������	CString strname             : [in] ָ������
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] ���ؼ�����
	*				DWORD style					: [in] ָ������
	*				void* data                  : [out] ���ؽ��
	*				HSCurDataInfoStatus* pCurDiataStatus = NULL : [out] ֻ���ڱ��ʽ����ʱ����,��ż�����
	*				CTValues<void>* pRefCalcValues = NULL : [out] ֻ���ڱ��ʽ����ʱ����,��ż�����λ��(��㼰�յ�)
	*	�� �� ֵ��	BOOL  : ��������ȷ(FALSE), ������Ϊ(TRUE)
	*	�� �� ֵ��	long : ������Ϣ�ķ���ֵ������ͬ,��������Ϣ��������˵��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
 	virtual BOOL CalcValue(  CString strName,DWORD style ,CArray<CLineProperty*,CLineProperty*>* pAyData
		
		/*,HSCurDataInfoStatus* pCurDataStatus = NULL,*/
		/*CTValues<void>* pRefCalcValues = NULL*/) ;
	/******************************************************************
	*	�������ܣ�	�����������ݵ�chsdatasorce�����б�m_ayDayData��
	*	����������	StockDay* pDay		: [in] ��������
	*				long lCurSize		: [in] �������ݳ���
	*				int nVolumeUnit		: [in] �ɽ�����λ
	*				int nMoneyUnit		: [in] �ɽ��λ
	*				short nMoneyDecimal	: [in] �ɽ���С��λ
	*				int nPriceUnit		: [in] �۸�λ
	*				short nPriceDecimal	: [in] �۸�С��λ
	*				WORD nKey = 0		: [in] Ϊ0ʱ,��ʾ�������е�K��,����,ֻ����һ����ӦnKey��ֵ������
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	virtual BOOL	SetDayData(StockDay* pDay,long lCurSize,
							   int nVolumeUnit,
							   int nMoneyUnit,short nMoneyDecimal,
							   int nPriceUnit,short nPriceDecimal,CodeInfo* pCodeinfo = NULL,
							   WORD nKey = 0);
	/******************************************************************
	*	�������ܣ�	��ȡָ���߸���
	*	����������	CString strname             : [in] ָ������
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] ���ؼ�����
	*				DWORD style					: [in] ָ������
	*				
	*	�� �� ֵ��	int : �ߵĸ���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual int GetLineNum(CString strName,DWORD style) ;
	/******************************************************************
	*	�������ܣ�	��ȡָ�����
	*	����������	CString strname             : [in] ָ������
	*			
	*				DWORD style					: [in] ָ������
	*				
	*	�� �� ֵ��	char*   [out] ���ؼ�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual CString GetExpressVar(CString strName,DWORD style	) ;
	/******************************************************************
	*	�������ܣ�	��ȡָ�����������
	*	����������	CString strname             : [in] ָ������
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] ���ؼ�����
	*				DWORD style					: [in] ָ������
	*				
	*	�� �� ֵ��	bool : 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	 BOOL GetLineProperty(CString strName, DWORD style,CArray<CLineProperty*,CLineProperty*>*& pAyData);

		/******************************************************************
	*	�������ܣ�	�������ݴ���������߳�Ȩ,����Ȩ��
	*	����������	Dword style   : [in] ���ݴ�����
	*			
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
   void	AddStyle(long dwStyle);
	 	/******************************************************************
	*	�������ܣ�	�������Ƶ�ĳ���ֲ���ƥ�������ָ��,���ڼ��̾�����ҹ�ʽ
	*	����������	CArray<ExpPropery*,ExpPropery*>& ayNameProp
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	void  GetExpProp(CArray<ExpPropery*,ExpPropery*>& ayNameProp);
	/******************************************************************
	*	�������ܣ�	��ȡʵʱ����
	*	����������	CodeInfo* pCode [in] ��Ʊ����
					CArray<int> ayID [in] ʵʱ���ݵ�id
	*				CArray<DOUBLE>& ayData [out] ʵʱ����ֵ
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	void	GetNowData(CodeInfo* pCode,CArray<int> ayID, CArray<DOUBLE>& ayData);

	//��ʼ����ʽ
	void initExpress(HWND hwndNotify,long** pThis);

	//�򿪹�ʽ����
	void  OpenExpWindow(CString strName, DWORD dwStyle, DWORD dwType);

	long ShowExpression( WPARAM wParam );
	long FindExpression( WPARAM wParam );
	long PreTranslateMsgDlg(MSG* pMsg); 

private:
	IHsColor*  m_pHSColor;     //��ɫ����ָ��
	CArray<CLineProperty*,CLineProperty*> m_AyData;   //������
	CMapStringToPtr m_mapDataSource;                  //����Դ
};


#endif // !defined(AFX_COMPEXPORT_H__FC0269FD_A095_4EE3_823A_54DF4E9D6518__INCLUDED_)
