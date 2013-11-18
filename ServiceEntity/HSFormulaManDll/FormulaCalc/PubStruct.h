/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	PubStruct.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	�ɰ汾Ԥ��(Ҳ��˵�߼�Ԥ��),������,
				����ϵͳ��ѡ��ѡ�ɵȼ�����Ϊ���õ���
				һЩ���ݽṹ��ID�Ķ���.
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
***************************************************************/

#if !defined(_PUBSTRUCT_H_)
#define _PUBSTRUCT_H_

#include "ExchData.h"
#include "GeneralHelper.h"


/*************************************************************
*	�� �� �ƣ�	BlockRetRowData
*	�� �� ����	��������һ�е�����
***************************************************************/
struct BlockRetRowData
{
	int nIndex;  // ������,�൱���к�
	CArray<double, double> ayValus; // �����������������
};

/*************************************************************
*	�� �� �ƣ�	BlockTabInitData
*	�� �� ����	��������ʹ��,�����Ĺ���
***************************************************************/
struct BlockTabInitData
{
	CArray<TYPEANDBLOCK*,TYPEANDBLOCK*> m_ayBlockCalc;	// ���а�����
	~BlockTabInitData()
	{
		Free();
	}
	// ���ռ�ÿռ�
	void Free()
	{
		if(m_ayBlockCalc.GetSize() > 0)
		{
			for(int i = 0; i < m_ayBlockCalc.GetSize(); i++)
			{
				delete m_ayBlockCalc.GetAt(i);
			}
			m_ayBlockCalc.RemoveAll();
		}
	}
};
// ������ - end

// ѡ������
#define ExchSys_Calc                 0x0001 // ����ϵͳ����

#define AlarmSystem_Calc			 0x0010 // Ԥ������

#define SelectStock_ChuQuan			 0x0020 // ѡ��ʱ��ʹ�þ�ȷ��Ȩ
#define SelectStock_InK_Disp_Mark	 0x0040 // ѡ��ѡ��ʱ����k������ʾ���
#define SelectStock_EndDate_UNEXACT  0x0080 // �Ǿ�ȷ�Ľ������ڶ�λ(������Ǿ�ȷ)

#define SelectStock_Single			 0x0100 // ����ѡ������
#define SelectStock_Union			 0x0200 // ���ѡ������
#define SelectStock_Sort			 0x0400 // ����

#define SelectStock_AndSucc			 0x0800 // ���ѡ�ɣ�ͬʱ����
#define SelectStock_HisDate			 0x1000 // ѡ��ʱ��ʹ����ʷʱ��

#define SelectStock_PhaseSort		 0x2000 // �׶�����
#define SelectStock_BlockCalc		 0x4000 // ������
#define SelectStock_SelStockAllCalc  0x8000 // ����ѡ��


//Ԥ��ϵͳ - begin

/*************************************************************
*	
*	����Ŀǰ��ʹ�����,���Բ�˵����Ԥ�����	
*	
***************************************************************/
struct AlarmCalcData
{
	//����
	SelStockCondition*	m_pCondition;
	//��ع�Ʊ�б�
	CArray<StockUserInfo*, StockUserInfo*> m_ayStkList;
	//�Ƿ�������
	BOOL					m_bFlag;
	//�������
	int						m_nCalculateCount;
	//��ʼ������к�
	int						m_nUnionCalBegin;
	int						m_nUnionCalEnd;
	//Ԥ������
	int										m_nCycle;
	//Ԥ�������ļ�
	CString									m_strSoundFileName;
	AlarmCalcData(SelStockCondition* pCon = NULL, BOOL bFlag = FALSE, int nCalBegin = 0, int nCalEnd = 1, int nCount = 0)
	{
		m_bFlag = bFlag;
		m_nCalculateCount = nCount;
		m_pCondition = pCon;
		m_nUnionCalBegin = nCalBegin;
		m_nUnionCalEnd = nCalEnd;
		m_nCycle = 1;
		m_strSoundFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) + "AlarmSound.wav";
	}

	~AlarmCalcData()
	{
		if( m_pCondition != NULL )
			delete m_pCondition;
		m_pCondition = NULL;
		m_ayStkList.RemoveAll();
	}
};

#define Alarm_Sytle_New 0x0001

#define PMD_State_Mode	0x0001
#define PMD_Single_Win	0x0002
#define PMD_Multi_Win	0x0004
#define PMD_Info		0x0100
#define PMD_Aalrm		0x0200
#define PMD_Stk			0x0400
#define PMD_Show		0x1000
#define PMD_Alarm_Show	0x2000

struct AlarmDisplayData
{
	int m_nTime;
	CodeInfo m_Stock;
	CString m_strDesc;
	CString m_strName;
	CArray<double, double>	m_ayOtherData;
	int	m_nIndex;
	WORD m_wSytle;
	void* m_pData;

	AlarmDisplayData(AlarmDisplayData *pData = NULL)
	{
		if( pData == NULL )
		{
			CTime timeDest = CTime::GetCurrentTime();
			m_nTime = 1000000 * timeDest.GetMonth() + 10000 * timeDest.GetDay() + 100 * timeDest.GetHour() + timeDest.GetMinute();
			m_pData = NULL;
			m_nIndex = -1;
			m_wSytle = Alarm_Sytle_New;
		}
		else
		{
			m_nTime = pData->m_nTime;
			m_Stock = pData->m_Stock;
			m_strDesc = pData->m_strDesc;
			m_pData = pData->m_pData;
			m_nIndex = pData->m_nIndex;
			m_strName = pData->m_strName;
			m_ayOtherData.Copy( pData->m_ayOtherData);
			m_wSytle = pData->m_wSytle;
		}
	}

	~AlarmDisplayData()
	{
		if( m_pData != NULL )
			delete m_pData;
		m_ayOtherData.RemoveAll();
	}

	void Serialize(CArchive& ar)
	{
		if( ar.IsStoring() )
		{
			ar << m_nTime;
			ar << m_Stock.m_cCodeType;
			ar << m_Stock.GetCode();
			ar << m_strDesc;
			ar << m_nIndex;
			ar << m_strName;
			ar << m_ayOtherData.GetSize();
			for( int i = 0; i < m_ayOtherData.GetSize(); i++ )
			{
				ar << m_ayOtherData.GetAt(i);
			}
			SetStyle(~Alarm_Sytle_New, FALSE);
			ar << m_wSytle;
		}
		else
		{
			CString strCode;
			ar >> m_nTime;
			ar >> m_Stock.m_cCodeType;
			ar >> strCode;
			strncpy(m_Stock.m_cCode, strCode, sizeof(char) * 6);
			ar >> m_strDesc;
			ar >> m_nIndex;
			ar >> m_strName;
			int nCount;
			double dTemp;
			ar >> nCount;
			m_ayOtherData.RemoveAll();
			for(int i = 0; i < nCount; i++ )
			{
				ar >> dTemp;
				m_ayOtherData.Add(dTemp);
			}
			ar >> m_wSytle;
		}
	}

	void SetTime(int Mon, int Day, int Hour, int Min)
	{
		m_nTime = 1000000 * Mon + 10000 * Day + 100 * Hour + Min;
    }

	void GetTime(int* Mon, int* Day, int* Hour, int* Min)
	{
		*Mon = m_nTime / 1000000;
		*Day = (m_nTime / 10000) % 100;
		*Hour = (m_nTime / 100) % 100;
		*Min = m_nTime % 100;
	}
	
	void GetStrTime(CString& str)
	{
		int Mon, Day, Hour, Min;
		GetTime(&Mon, &Day, &Hour, &Min);

		if( Mon < 10 )
		{
			str.Format("0%d", Mon);
		}
		else
		{
			str.Format("%d", Mon);
		}

		CString strTemp;
		if( Day < 10 )
		{
			strTemp.Format("-0%d", Day);
		}
		else
		{
			strTemp.Format("-%d", Day);
		}
		str += strTemp;

		if( Hour < 10 )
		{
			strTemp.Format(" 0%d", Hour);
		}
		else
		{
			strTemp.Format(" %d", Hour);
		}
		str += strTemp;

		if( Min < 10 )
		{
			strTemp.Format(":0%d", Min);
		}
		else
		{
			strTemp.Format(":%d", Min);
		}
		str += strTemp;
	}

	void SetStyle(WORD wStyle, BOOL bOr = TRUE )
	{
		if( bOr )
		{
			m_wSytle |= wStyle;
		}
		else
		{
			m_wSytle &= wStyle;
		}
	}
};

struct AlarmMsgData
{
	MSG* pMsg;
	CWnd* pWnd;
};

//HS end

/*************************************************************
*	�� �� �ƣ�	TExchSys
*	�� �� ����	��������ϵͳ�������ݻ���
***************************************************************/
struct TExchSys
{
	// ��ʽ
	TExchExp		m_sExchExp;
	// �г�ģʽ(��Ʊ)
	TExchModel		m_sTExchModel;
	// ������������ʽ
	TExchBuy		m_sTExchBuy;
	// ������������ʽ
	TExchSell		m_sTExchSell;
	// �����Ż�����
	TExchOptimize	m_sTExchOptimize;
	// ������
	CArray<TExchCaclResult*,TExchCaclResult*>	m_sCaclResult;

	// ԭ����ʽ�Ĳ������Ż�������ɺ��ȡ���Ż����ú󽫲�����Ƴ�ԭ������
	TExchParam		m_sDefParam;
};

#endif //_PUBSTRUCT_H_