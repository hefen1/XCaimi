/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：FxjFormulaStruct.h
 * 文件标识：
 * 摘    要：
 *
 * 当前版本：2.0
 * 作    者：
 * 完成日期：
 *
 * 取代版本：
 * 原 作 者：
 * 完成日期：
 * 备	 注：
 *******************************************************************************/

#if !defined(_FXJFORMULASTRUCT_H_)
#define _FXJFORMULASTRUCT_H_

#pragma	pack(1)

#include "ExportDll.h"

//#include "afxtempl.h"

//分析家公式文件格式

/*
//文件头结构
typedef struct tagSUPERSTK_HEAD
{
	BYTE		chSign[16];		//公式文件标志,	3.1版本为 10 00 00 00 7D CC 34 C0
	long		lCount;			//公式文件中公式的个数
	long		lUnknown;		//未知字段
//!!以下有 lCount 个 long 型数据!!
} SSTK_HEAD;

//!!禁用周期信息，这部分属于公式头结构，不属于文件头结构!!
typedef struct	STRMSG
{
	short	nMsgUnknown;
	short	nMsgWeek;			//禁用周期信息，enumMsgWeek
} strMsg;

typedef union unionItemMsg
{
	long		lMsg;
	strMsg      sMsg;
} SSTK_ITEMMSG;

//公式头结构
typedef struct tagSUPERSTK_ITEMHEAD
{
	long			lUnknown1;		//未知用法
	SSTK_ITEMMSG	unionMsg;		//禁用周期信息
	long			lIcon;			//公式图标(绘制方式)，
									//为3时候，主图叠加，为4时副图
	char 			pszItemName[10];//公式名称, 此处不是序列化，
	long			lUnknown2;		//用途未知，估计和异或有关。
	BYTE			chKey[4];		//公式密码的解密密钥(完全解密的密钥不在此处)
									//按字节顺序循环异或加密
	long			lParameterCount;//参数个数
									//以下紧接着SSTK_ITEMPARA para[lParameterCounter];
} SSTK_ITEMHEAD;

//参数结构
typedef struct tagSUPERSTK_ITEMPARA
{
	float		fMin;				//参数的最小值
	float		fMax;				//参数的最大值
	float		fDefault;			//参数的默认值
	float		fStep;				//参数的步长
	BYTE		chParameterName[15];//参数名称
} SSTK_ITEMPARA;

//此结构可以忽略，不过完全加密的时候就要用这个结构来解开公式表达式
//长度未定，以两个长整型为单位，当两个长整型的值为 0x44 0x? 
//("?"参考后面说明)的时候结构体结束
//然后是一个长整型的0表示结束。( "?" 为公式的参数个数和表达式中定义的参量之和
//作为快速判断，只要 "?" 小于0xffff就可以认为结构体已经结束，一般来说
//"?"不可能大于0xff)
//这里的表达式采用操作符后置表示，操作符专门有一个表来存储
//第一个长整型用来说明操作符的类型，接下来是参数。
struct tagUnknown
{
	//!!需要添加相应代码来忽略或解析公式
};

//公式体结构
typedef struct tagSUPERSTK_ITEMBODY
{
	struct tagPoints				
	{									//绘制方式为副图时的7个坐标
		long		nCount;				//坐标个数
		float		nPoint[7];			//最多7个坐标
	} strPoints;
	CString				pszDepiction;	//公式描述
	CString				pszContent;		//公式内容，完全加密时为空
	CString				pszExegesis;	//公式注释
	CString				pszHelp;		//参数精灵
	CString				pszClass;		//参数所属类别，文本
	long				lEnd;			//结束标志，通常在20, 80之间变化
}SUPERSTK_ITEMBODY;

//禁用周期
char*	chMsgWeek[] = {
	"分笔成交", //0x20
	"1分钟线",	//0x40
	"5分钟线",	//0x80
	"10分钟线",	//0x100
	"15分钟线",	//0x200
	"30分钟线",	//0x400
	"60分钟线",	//0x800
	"日线",		//0x1000
	"周线",		//0x2000
	"月线",		//0x4000
	"多日线"	//0x8000
};

enum enumMsgWeek
{
	IM_A	=	0x20,
	IM_1M	=	0x40,
	IM_5M	=	0x80,
	IM_10M	=	0x100,
	IM_15M	=	0x200,
	IM_30M	=	0x400,
	IM_60M	=	0x800,
	IM_D	=	0x1000,
	IM_W	=	0x2000,
	IM_M	=	0x4000,
	IM_SD	=	0x8000
};
*/

////////
struct FxjParam  // 参数
{
	float m_fMin;
	float m_fMax; 
	float m_fDefault;
	float m_fStep;
	char  m_strName[16]; // 参数名
	char  m_pNextData[1];

	CString GetText()
	{
		return m_strName;
	}

	int  GetLen()
	{
		return sizeof(FxjParam) - sizeof(char);
	}
};

struct FxjPerhaps  // 以后解密
{
	int    m_nExpressLen;     // m_nCount*int
	char   m_pNextData[1];
};

struct FxjPerhaps1  // 以后解密
{
	char m_cData[52];
	char m_pNextData[1];
};

struct FxjHead
{
    char   m_cFlag[8];      // 标志 8
    int    m_nCount;		// 公式总数 8+4
    int    m_nUnknown1[2];  // 
	char   m_pNextData[1];

	int  GetLen()
	{
		return sizeof(FxjHead) - sizeof(char);
	}
};

struct FxjName
{
    int   m_nPeriod;       // 周期 24+4
    int   m_nChart;        // 主图/副图 // 28+4
    char  m_strName[10];   // 公式名称 // 32+10
	int   m_nUnknown;      // 42+4
	char  m_cKey[4];       // 密钥46+4
	int   m_nParamCount;   // 参数总数42+4
	char  m_pNextData[1];

	CString GetText()
	{
		return m_strName;
	}
	int  GetLen()
	{
		return sizeof(FxjName) - sizeof(char);
	}
};

struct FxjSerialString
{
	int   m_lLength;
	char  m_cData[1];

	CString GetText()
	{
		return CString(m_cData,m_lLength);
	}

	char* GetNextData(int lCountLen)
	{
		if(m_lLength < lCountLen)
		{
			return (char*)(&m_cData[m_lLength]);
		}
		return NULL;
	}
	int  GetLen()
	{
		return m_lLength;
	}
};

struct FxjExpressEnd  // 一个公式结尾
{
	char  m_cEnd[8];      // 00 00 00 00 08 00 00 00
	char  m_cEnd1[4];     // 10 13 00 00
	char  m_pNextData[1];
	BOOL  IsEnd()
	{
		long* p = (long*)m_cEnd;
		if( p && *p == 0)
		{
			p++;
			if(*p == 8)
			{
				p = (long*)m_cEnd1;
				if( p && *p == 784)
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	static int  GetLen()
	{
		return sizeof(FxjExpressEnd) - sizeof(char);
	}
};

class BCGCONTROLBARDLLEXPORT FxjUserDefCoordinate
{	
public:
									//绘制方式为副图时的7个坐标
	long	m_nCount;				//坐标个数
	float	m_fPoint[7];			//最多7个坐标

	int		GetOffset();

	int  GetLen()
	{
		return sizeof(FxjUserDefCoordinate);
	}
};

class BCGCONTROLBARDLLEXPORT FxjEidolon  // 参数精灵
{	
public:			
	char			m_strUnknown[9];
	FxjSerialString m_sEidolon;

	BOOL  IsValid();
	char* GetNextData(int lCountLen)
	{
		return m_sEidolon.GetNextData(lCountLen);
	}

	int  GetLen()
	{
		return sizeof(FxjEidolon);
	}
};

//公式体结构
struct FxjBody
{
	FxjUserDefCoordinate  m_Coodi;
	char				  m_pNextData[1];

	int  GetLen()
	{
		return m_Coodi.GetLen();
	}
};

struct  FxjExpress
{
	FxjName*					 m_pName;      // 名称
	CArray<FxjParam*,FxjParam*>* m_ayParam;    // 参数
	
	FxjSerialString*			 m_pDepiction; // 描述
	FxjSerialString*			 m_pBody;      // 公式体
	FxjSerialString*			 m_pNotes;     // 注释
	FxjEidolon*					 m_pEidolon;   // 参数精灵
	FxjSerialString*			 m_pType;	  // 所属类型

	FxjExpress()
	{
		m_pName = NULL;
		m_ayParam = new CArray<FxjParam*,FxjParam*>;
		
		m_pDepiction = NULL;
		m_pBody = NULL;     
		m_pNotes = NULL;    
		m_pEidolon = NULL;  
		m_pType = NULL;	 
	}
	~FxjExpress()
	{
		if(m_ayParam != NULL)
		{
			delete m_ayParam;
			m_ayParam = NULL;
		}
	}
};


//分析家盘后数据文件格式///////////////////////////////////////////////
//分析家基本数据结构,28字节
struct FXJStockData 
{ 
	time_t		m_time;		//时间,UCT 
	float		m_fOpen;	//开盘 
	float		m_fHigh;	//最高 
	float		m_fLow;		//最低 
	float		m_fClose;	//收盘 
	float		m_fVolume;	//成交量 
	float		m_fAmount;	//成交额

};
//追加了结束标志的分析家基本数据结构 32字节
class FXJAStockData
{
public:
	FXJStockData	m_FXJStockData;
	float			m_fADataEnd;	// 结束标志 0
public:	
	FXJAStockData()
	{
		m_fADataEnd = 0L;
	};
};
//某支股票的数据  32 + 32 * N 字节
//#define A_PERIOD_TYPE_DAY_A_STOCK_LEN 64
//#define A_PERIOD_TYPE_DAY_A_STOCK_NODATA_LEN 32

class FXJStockDataHead
{
public:	
	long	m_lStart;	//Must be -1
	char	m_sStockCode[8];	//股票代号，SH(SZ)123456
	long	m_lSplit0;	//保留0
	long	m_lSplit1;	//保留1
	char	m_lpsStockName[8];	//股票名称
	long	m_lDataStart; //Must Be 0
	
	FXJStockDataHead()
	{
		 m_lStart = -1;
		 m_lSplit0 = 0;
		 m_lSplit1 = 0;
		 m_lDataStart = 0;
	}

};
//供参考,未使用
class FXJStockDays
{
	FXJStockDataHead m_FXJStockDataHead;
	
	CArray < FXJAStockData* , FXJAStockData*& > m_StockDayArray;
public:	
	FXJStockDays()
	 {
		
	 };
	~FXJStockDays()
	{
		if(m_StockDayArray.GetSize() <= 0)
			return;
		for(int i = 0; i < m_StockDayArray.GetSize(); i++)
		{
			delete m_StockDayArray.GetAt(i);
		}
		m_StockDayArray.RemoveAll();
	}
};

struct FxjNewStockPosition
{
	long	 m_lCurPos;
	short	 m_nSize;
	FxjNewStockPosition()
	{
		memset(this,0,sizeof(FxjNewStockPosition));
	};
};

#pragma	pack()
#endif // _FXJFORMULASTRUCT_H_