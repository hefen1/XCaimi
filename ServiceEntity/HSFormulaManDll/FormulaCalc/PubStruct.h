/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	PubStruct.h
*	文件标识：	
*	摘	  要：	旧版本预警(也可说高级预警),板块计算,
				交易系统优选及选股等计算行为中用到的
				一些数据结构及ID的定义.
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
***************************************************************/

#if !defined(_PUBSTRUCT_H_)
#define _PUBSTRUCT_H_

#include "ExchData.h"
#include "GeneralHelper.h"


/*************************************************************
*	类 名 称：	BlockRetRowData
*	类 描 述：	板块分析中一行的数据
***************************************************************/
struct BlockRetRowData
{
	int nIndex;  // 索引号,相当于行号
	CArray<double, double> ayValus; // 行里的所有数字数据
};

/*************************************************************
*	类 名 称：	BlockTabInitData
*	类 描 述：	板块分析里使用,板块类的管理
***************************************************************/
struct BlockTabInitData
{
	CArray<TYPEANDBLOCK*,TYPEANDBLOCK*> m_ayBlockCalc;	// 所有板块分类
	~BlockTabInitData()
	{
		Free();
	}
	// 清空占用空间
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
// 板块计算 - end

// 选股条件
#define ExchSys_Calc                 0x0001 // 交易系统计算

#define AlarmSystem_Calc			 0x0010 // 预警计算

#define SelectStock_ChuQuan			 0x0020 // 选股时，使用精确除权
#define SelectStock_InK_Disp_Mark	 0x0040 // 选股选中时，在k线上显示标记
#define SelectStock_EndDate_UNEXACT  0x0080 // 非精确的结束日期定位(否则就是精确)

#define SelectStock_Single			 0x0100 // 单个选股条件
#define SelectStock_Union			 0x0200 // 组合选股条件
#define SelectStock_Sort			 0x0400 // 排序

#define SelectStock_AndSucc			 0x0800 // 组合选股，同时成立
#define SelectStock_HisDate			 0x1000 // 选股时，使用历史时段

#define SelectStock_PhaseSort		 0x2000 // 阶段排序
#define SelectStock_BlockCalc		 0x4000 // 板块计算
#define SelectStock_SelStockAllCalc  0x8000 // 批量选股


//预警系统 - begin

/*************************************************************
*	
*	由于目前不使用这个,所以不说明旧预警相关	
*	
***************************************************************/
struct AlarmCalcData
{
	//条件
	SelStockCondition*	m_pCondition;
	//监控股票列表
	CArray<StockUserInfo*, StockUserInfo*> m_ayStkList;
	//是否参与计算
	BOOL					m_bFlag;
	//计算次数
	int						m_nCalculateCount;
	//开始与结束行号
	int						m_nUnionCalBegin;
	int						m_nUnionCalEnd;
	//预警周期
	int										m_nCycle;
	//预警声音文件
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
*	类 名 称：	TExchSys
*	类 描 述：	交易评测系统计算数据基类
***************************************************************/
struct TExchSys
{
	// 公式
	TExchExp		m_sExchExp;
	// 市场模式(股票)
	TExchModel		m_sTExchModel;
	// 买入条件及方式
	TExchBuy		m_sTExchBuy;
	// 卖出条件及方式
	TExchSell		m_sTExchSell;
	// 参数优化方法
	TExchOptimize	m_sTExchOptimize;
	// 计算结果
	CArray<TExchCaclResult*,TExchCaclResult*>	m_sCaclResult;

	// 原来公式的参数，优化计算完成后或取消优化设置后将参数设计成原来参数
	TExchParam		m_sDefParam;
};

#endif //_PUBSTRUCT_H_