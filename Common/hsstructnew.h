/*******************************************************************************
* Copyright (c)2010, 恒生信息技术有限公司
* All rights reserved.
*
* 文件名称：HsStructnew.h
* 文件标识：新版数据通用结构
* 摘    要：新版数据通用结构
*
* 当前版本：投资赢家2.0
* 作    者：
* 完成日期：2010-09
*
* 备    注：
*
*
*******************************************************************************/

#pragma once

#pragma	pack(1)
#pragma warning(disable : 4996)


/*
市场类别定义：
各位含义表示如下：
15		   12		8					0
|			|	  	  |					|
| 金融分类	|市场分类 |	交易品种分类	|
*/
typedef short HSMarketDataType;			  // 市场分类数据类型
/*金融大类*/
#define STOCK_MARKET			 0X1000   // 股票
#	define SH_BOURSE			 0x0100   // 上海
#	define SZ_BOURSE			 0x0200   // 深圳
#	define SYSBK_BOURSE			 0x0400   // 系统板块
#	define USERDEF_BOURSE		 0x0800   // 自定义（自选股或者自定义板块）
#			define KIND_INDEX		 0x0000   // 指数 
#			define KIND_STOCKA		 0x0001   // A股 
#			define KIND_STOCKB		 0x0002   // B股 
#			define KIND_BOND		 0x0003   // 债券
#			define KIND_FUND		 0x0004   // 基金
#			define KIND_THREEBOAD	 0x0005   // 三板
#			define KIND_SMALLSTOCK	 0x0006   // 中小盘股
#			define KIND_PLACE		 0x0007	  // 配售
#			define KIND_LOF			 0x0008	  // LOF
#			define KIND_ETF			 0x0009   // ETF
#			define KIND_QuanZhen	 0x000A   // 权证
#			define KIND_STOCKGE		 0x000D	  // 创业板(Growth Enterprise)

#			define KIND_OtherIndex	 0x000E   // 第三方行情分类，如:中信指数

#			define SC_Others		 0x000F   // 其他 0x09
#			define KIND_USERDEFINE	 0x0010   // 自定义指数
#			define KIND_STOCKA_SELECTED	0x0011	//光大行情报价保留
#			define KIND_STOCKA_NOTSEL	0x0012	//光大行情报价剔除


// 港股市场
#define HK_MARKET				 0x2000 // 港股分类
#	define HK_BOURSE			 0x0100 // 主板市场
#	define	GE_BOURSE			 0x0200 // 创业板市场(Growth Enterprise Market)
#	define	INDEX_BOURSE		 0x0300	// 指数市场	
//#		define HK_KIND_INDEX			 0x0000   // 港指
//#		define HK_KIND_FUTURES_INDEX	 0x0001   // 期指
//#		define	KIND_Option				 0x0002	  // 港股期权
# define HK_KIND_BOND  0x0000   // BOND-债券
# define HK_KIND_BWRT  0x0001   // BWRT-一篮子衍生权证
# define HK_KIND_EQTY  0x0002   // EQTY-公司股票
# define HK_KIND_TRST  0x0003   // TRST-信托
# define HK_KIND_WRNT  0x0004   // WRNT-权证

#	define SYSBK_BOURSE			 0x0400 // 港股板块(H股指数成份股，恒生指数成份股）。
#	define USERDEF_BOURSE		 0x0800 // 自定义（自选股或者自定义板块）
// #			define HK_KIND_BOND		 0x0000   // 债券
// #			define HK_KIND_MulFund	 0x0001   // 一揽子认股证
//#			define HK_KIND_FUND		 0x0002   // 基金
//#			define KIND_WARRANTS	 0x0003   // 认股证
//#			define KIND_JR			 0x0004   // 金融
#			define KIND_ZH			 0x0005   // 综合
#			define KIND_DC			 0x0006   // 地产
#			define KIND_LY			 0x0007   // 旅游
#			define KIND_GY			 0x0008   // 工业
#			define KIND_GG			 0x0009   // 公用
#			define KIND_QT			 0x000A   // 其它

/*期货大类*/
#define FUTURES_MARKET			 0x4000 // 期货
#		define DALIAN_BOURSE		 0x0100	// 大连
#				define KIND_BEAN		 0x0001	// 连豆
#				define KIND_YUMI		 0x0002	// 大连玉米
#				define KIND_SHIT		 0x0003	// 大宗食糖
#				define KIND_DZGY		 0x0004	// 大宗工业1
#				define KIND_DZGY2		 0x0005	// 大宗工业2
#				define KIND_DOUYOU		 0x0006	// 连豆油
#				define KIND_JYX			 0x0007	// 聚乙烯
#				define KIND_ZTY			 0x0008	// 棕榈油

#		define SHANGHAI_BOURSE	  0x0200	// 上海
#				define KIND_METAL		 0x0001	// 上海金属
#				define KIND_RUBBER		 0x0002	// 上海橡胶
#				define KIND_FUEL		 0x0003	// 上海燃油
//#				define KIND_GUZHI		 0x0004	// 股指期货
#				define KIND_QHGOLD		 0x0005	// 上海黄金

#		define ZHENGZHOU_BOURSE	  0x0300	// 郑州
#				define KIND_XIAOM		 0x0001	// 郑州小麦
#				define KIND_MIANH		 0x0002	// 郑州棉花
#				define KIND_BAITANG		 0x0003	// 郑州白糖
#				define KIND_PTA			 0x0004	// 郑州PTA
#				define KIND_CZY			 0x0005	// 菜籽油

#		define HUANGJIN_BOURSE	  0x0400	// 黄金交易所
#				define KIND_GOLD		 0x0001	// 上海黄金

#		define GUZHI_BOURSE		  0x0500	// 股指期货
#				define KIND_GUZHI		 0x0001	// 股指期货

/*外盘大类*/
#define WP_MARKET				 ((HSMarketDataType)0x5000) // 外盘
#		define WP_INDEX				0x0100	// 国际指数 // 不用了
#		define WP_LME				0x0200	// LME		// 不用了
#			define WP_LME_CLT			0x0210 //"场内铜";
#			define WP_LME_CLL			0x0220 //"场内铝";
#			define WP_LME_CLM			0x0230 //"场内镍";
#			define WP_LME_CLQ			0x0240 //"场内铅";
#			define WP_LME_CLX			0x0250 //"场内锌";
#			define WP_LME_CWT			0x0260 //"场外铝";
#			define WP_LME_CW			0x0270 //"场外";
#			define WP_LME_SUB			0x0000

#			define WP_CBOT				0x0300	// CBOT			
#			define WP_NYMEX	 			0x0400	// NYMEX	 	
#			define WP_NYMEX_YY			0x0000	//"原油";
#			define WP_NYMEX_RY			0x0001	//"燃油";
#			define WP_NYMEX_QY			0x0002	//"汽油";

#			define WP_COMEX	 			0x0500	// COMEX	 	
#			define WP_TOCOM	 			0x0600	// TOCOM	 	
#			define WP_IPE				0x0700	// IPE			
#			define WP_NYBOT				0x0800	// NYBOT		
#			define WP_NOBLE_METAL		0x0900	// 贵金属	
#				define WP_NOBLE_METAL_XH	0x0000  //"现货";
#				define WP_NOBLE_METAL_HJ	0x0001  //"黄金";
#				define WP_NOBLE_METAL_BY	0x0002  //"白银";

#			define WP_FUTURES_INDEX		0x0a00	// 期指
#			define WP_SICOM				0x0b00	// SICOM		
#			define WP_LIBOR				0x0c00	// LIBOR		
#			define WP_NYSE				0x0d00	// NYSE
#			define WP_CEC				0x0e00	// CEC

#			define WP_INDEX_AZ	 		0x0110 //"澳洲";
#			define WP_INDEX_OZ	 		0x0120 //"欧洲";
#			define WP_INDEX_MZ	 		0x0130 //"美洲";
#			define WP_INDEX_TG	 		0x0140 //"泰国";
#			define WP_INDEX_YL	 		0x0150 //"印尼";
#			define WP_INDEX_RH	 		0x0160 //"日韩";
#			define WP_INDEX_XHP 		0x0170 //"新加坡";
#			define WP_INDEX_FLB 		0x0180 //"菲律宾";
#			define WP_INDEX_CCN 		0x0190 //"中国大陆";
#			define WP_INDEX_TW  		0x01a0 //"中国台湾";
#			define WP_INDEX_MLX 		0x01b0 //"马来西亚";
#			define WP_INDEX_SUB 		0x0000 


/*外汇大类*/
#define FOREIGN_MARKET			 ((HSMarketDataType)0x8000) // 外汇
#	define WH_BASE_RATE			0x0100	// 基本汇率
#	define WH_ACROSS_RATE		0x0200	// 交叉汇率
#		define FX_TYPE_AU 			0x0000 // AU	澳元
#		define FX_TYPE_CA 			0x0001 // CA	加元
#		define FX_TYPE_CN 			0x0002 // CN	人民币
#		define FX_TYPE_DM 			0x0003 // DM	马克
#		define FX_TYPE_ER 			0x0004 // ER	欧元	 
#		define FX_TYPE_HK 			0x0005 // HK	港币
#		define FX_TYPE_SF 			0x0006 // SF	瑞士 
#		define FX_TYPE_UK 			0x0007 // UK	英镑
#		define FX_TYPE_YN 			0x0008 // YN	日元

#	define WH_FUTURES_RATE			0x0300  // 期汇

// 内部分类，给股票曾用名用
#define STOCK_WHILOM_NAME_MARKET ((HSMarketDataType)0xF000)


//#define	MakeMarket(x)			((HSMarketDataType)((x) & 0xF000))
//#define MakeMainMarket(x)		((HSMarketDataType)((x) & 0xFFF0))
//#define	MakeMidMarket(x)		((HSMarketDataType)((x) & 0x0F00)) // 分类第二位

static int	MakeMarket(HSMarketDataType x)
{
	return ((HSMarketDataType)((x) & 0xF000));
}
static int  MakeMainMarket(HSMarketDataType x)
{
	return ((HSMarketDataType)((x) & 0xFFF0));
}
static int	MakeMidMarket(HSMarketDataType x)
{
	return ((HSMarketDataType)((x) & 0x0F00)); // 分类第二位
}

static bool IsSameMarket(HSMarketDataType x, HSMarketDataType y)
{
	int a = MakeMarket(x);
	int b = MakeMarket(y);

	if(a && b && a != b)
		return false;

	a = MakeMidMarket(x);
	b = MakeMidMarket(y);

	if(a && b && a != b)
		return false;

	a = x & 0x00F0;
	b = y & 0x00F0;

	if(a && b && a != b)
		return false;

	a = x & 0x000F;
	b = y & 0x000F;

	if(!a || !b)
		return true;

	return a == b ? true : false;
}

//#define MakeSubMarket(x)		((HSMarketDataType)((x) & 0x000F))
static int MakeSubMarket(HSMarketDataType x)
{
	return ((HSMarketDataType)((x) & 0x000F));
}

//#define MakeHexSubMarket(x)		( (HSMarketDataType)((x) & 0x000F) )
//#define MakeSubMarketPos(x)		( ((MakeHexSubMarket(x) / 16) * 10) + (MakeHexSubMarket(x) % 16) )

static int MakeHexSubMarket(HSMarketDataType x)
{
	return ( (HSMarketDataType)((x) & 0x000F) );
}

static int MakeSubMarketPos(HSMarketDataType x)
{
	return ( ((MakeHexSubMarket(x) / 16) * 10) + (MakeHexSubMarket(x) % 16) );
}

// 是否为国内股票指数
//#define MakeIndexMarket(x)		( (MakeMarket(x) == STOCK_MARKET) && (MakeSubMarket(x) == KIND_INDEX))
static int MakeIndexMarket(HSMarketDataType x)
{
	return ( (MakeMarket(x) == STOCK_MARKET) &&
		(MakeMidMarket(x) != 0) &&
		((MakeSubMarket(x) == KIND_INDEX) ||
		(MakeSubMarket(x) == KIND_OtherIndex))
		// #ifdef SUPPORT_GG
		// 			 || (MakeMarket(x) == HK_MARKET && MakeMidMarket(x) == INDEX_BOURSE 
		// 			   && MakeSubMarket(x) == 0)
		// #endif
		);
}

static int MakeIndexSelf(HSMarketDataType x)
{
	return ( (MakeMarket(x) == STOCK_MARKET) &&
		(MakeMidMarket(x) != 0) &&
		((MakeSubMarket(x) == KIND_OtherIndex)));
}

// 是否为基金
static int MakeFundMarket(HSMarketDataType x)
{
	return ( (MakeMarket(x) == STOCK_MARKET) &&
		(MakeMidMarket(x) != 0) &&
		(MakeSubMarket(x) == KIND_FUND) );
}

// 是否为港股指数
//#define MakeHKIndex(x)          ( MakeMainMarket(x) == (HK_MARKET | INDEX_BOURSE) )
static int MakeHKIndex(HSMarketDataType x)
{
	return ( MakeMainMarket(x) == (HK_MARKET | INDEX_BOURSE) );
}

// 是否为股指期货
static int MakeGZIndex(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) == (FUTURES_MARKET | GUZHI_BOURSE)) &&
		(MakeSubMarket(x) == KIND_GUZHI) );
}

// 是否为黄金期货
static int MakeGoldIndex(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) == (FUTURES_MARKET | HUANGJIN_BOURSE)) &&
		(MakeSubMarket(x) == KIND_GOLD) );
}
//yangdl 2008.03.06 特殊处理价格
// 是否为需要特殊处理价格的分类
static int MakeNegativeIndex(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) == (FOREIGN_MARKET | WH_FUTURES_RATE)) &&
		( (MakeSubMarket(x) == FX_TYPE_AU)  || (MakeSubMarket(x) == FX_TYPE_YN)));
}

//#define MakeWPIndex(x)          ( (MakeMainMarket(x) >= (WP_MARKET | WP_INDEX_AZ)) && (MakeMainMarket(x) <= (WP_MARKET | WP_INDEX_MLX)) )
//#define MakeWP_LME(x)           ( (MakeMainMarket(x) >= (WP_MARKET | WP_LME_CLT))  && (MakeMainMarket(x) <= (WP_MARKET | WP_LME_CW)) )
static int MakeWPIndex(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) >= (WP_MARKET | WP_INDEX_AZ)) && (MakeMainMarket(x) <= (WP_MARKET | WP_INDEX_MLX)) );
}

static int MakeWP_LME(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) >= (WP_MARKET | WP_LME_CLT))  && (MakeMainMarket(x) <= (WP_MARKET | WP_LME_CW)) );
}

//#define	WhoMarket(x,y)			( MakeMarket(x) == MakeMarket(y) )
static int	WhoMarket(HSMarketDataType x,HSMarketDataType y)
{
	return ( MakeMarket(x) == MakeMarket(y) );
}

// 是否为国内股票ETF
//#define MakeETF(x)				( (MakeMarket(x) == STOCK_MARKET) && (MakeSubMarket(x) == KIND_ETF))
static int MakeETF(HSMarketDataType x)
{
	return ( (MakeMarket(x) == STOCK_MARKET) && (MakeSubMarket(x) == KIND_ETF));
}

#define IsFund(Type) (MakeMarket(Type) == STOCK_MARKET && (MakeSubMarket(Type) == KIND_FUND || MakeSubMarket(Type) == KIND_LOF))


#define SH_Bourse	((HSMarketDataType)(STOCK_MARKET|SH_BOURSE))
#define SZ_Bourse	((HSMarketDataType)(STOCK_MARKET|SZ_BOURSE))

#define QH_Bourse	FUTURES_MARKET
#define BK_Bourse	(STOCK_MARKET|SYSBK_BOURSE)
#define OT_Bourse	STOCK_WHILOM_NAME_MARKET // (其它市场，目前主要用于内部分类，给股票曾用名用)

#define SC_Union			0x40 // 组合

#define ToDataType(x) ((HSMarketDataType)(x))

#define Negative(x,y)  ( y ? x&0x0FFFFFFF : x )
//#define Negative(x,y)   x


#define PYJC_MAX_LENGTH		16			// 拼音长度
#define STOCK_NAME_SIZE		16			// 股票名称长度

extern UINT HashKey(LPCTSTR key,int nKeyCount,const int nHashTableSize);
// 股票代码结构
struct CodeInfo
{
	HSMarketDataType	m_cCodeType;	// 证券类型
	char				m_cCode[6];		// 证券代码

	void To(char cIntelToComputer )
	{
	}

	CodeInfo()
	{
		SetEmpty();
	}

	void SetEmpty()
	{
		m_cCodeType = 0;
		memset(m_cCode, 0, 6);
	}

	CodeInfo(HSMarketDataType cCodeType,CString cCode)
	{
		m_cCodeType = cCodeType;
		strncpy(m_cCode,cCode,sizeof(m_cCode));
	}

	// 建立哈希表, 返回哈希长度
	long GetLongKey(int nHashTableSize = 999999)
	{
		return HashKey(m_cCode,6,nHashTableSize);
	}

	// 比较类型	返回TRUE or FALSE
	BOOL IsType(HSMarketDataType cType)
	{
		return (MakeSubMarket(m_cCodeType) == cType);
	}

	// 返回股票代码
	LPCTSTR GetKey(char* strKey) // strKey > 9
	{
		strncpy(strKey,m_cCode,6);
		strKey[6] = '\0';

		return strKey;
	}
	// 返回股票代码
	LPCTSTR GetKey()
	{
		static char strKey[7];
		strncpy_s(strKey,m_cCode,6);
		strKey[6] = '\0';

		return strKey;
	}

	// 操作符 复制数据
	void operator=(CodeInfo* pInfo)
	{
		if(pInfo != NULL)
		{
			memcpy(this,pInfo,sizeof(CodeInfo));
		}
	}
	// 操作符 复制数据
	void operator=(CodeInfo pInfo)
	{
		memcpy(this,&pInfo,sizeof(CodeInfo));		
	}


	// 返回代码
	CString GetCode()
	{
		//20090620 YJT 调整此处写法 begin
		char cCode[7];
		memset(cCode, 0, sizeof(cCode));
		strncpy_s(cCode, m_cCode, 6);
		return CString(cCode);//.Left(6);
		//20090620 YJT 调整此处写法 end
	}

	// 取得代码, 如果是4位, 也将返回四位代码
	CString GetFindCode()
	{
		switch(MakeMainMarket(m_cCodeType))
		{
		case SH_Bourse:
			return CString(m_cCode).Left(6);
		case SZ_Bourse:
			return CString(m_cCode).Mid(2,4);
		default:
			break;
		}
		return CString(m_cCode).Left(6);
	}		

	BOOL CompareCode(const CodeInfo* pDes)
	{
		if( pDes == NULL )
			return FALSE;

		CString str1 = GetCode();
		CString str2 = ((CodeInfo*)pDes)->GetCode();
		return ( m_cCodeType == pDes->m_cCodeType && !str1.CompareNoCase(str2) );
	}
};

struct StockInfoEx
{
	CodeInfo  Code;
	CString * pFullName;
	CString * pPyjcArray;

	StockInfoEx(const CodeInfo *pCodeInfo)
	{
		memcpy(&Code, pCodeInfo, sizeof(CodeInfo));
		pFullName = NULL;
		pPyjcArray = NULL;
	}

	~StockInfoEx()
	{
		if(pFullName)
			delete pFullName;
		if(pPyjcArray)
			delete pPyjcArray;
	}

	void Read(CFile &fp)
	{
		if((HANDLE)fp == INVALID_HANDLE_VALUE)
			return;

		fp.Read(&Code, sizeof(CodeInfo));

		long len;
		fp.Read(&len, sizeof(long));
		if(len >= 0)  //qinhn 20100401 将判断由if(len)修改为if(len >= 0),防止len为-1时导致异常的情况出现
		{
			pFullName = new CString;
			LPSTR pstr = pFullName->GetBuffer(len + 1);
			fp.Read(pstr, len);
			pstr[len] = 0;
			pFullName->ReleaseBuffer();
		}

		fp.Read(&len, sizeof(long));
		if(len >= 0)
		{
			pPyjcArray = new CString;
			LPSTR pstr = pPyjcArray->GetBuffer(len + 1);
			fp.Read(pstr, len);
			pstr[len] = 0;
			pPyjcArray->ReleaseBuffer();
		}
	}

	void Write(CFile &fp)
	{
		fp.Write(&Code, sizeof(CodeInfo));
		long len;

		len = pFullName ? pFullName->GetLength() : 0;
		fp.Write(&len, sizeof(long));
		if(len)
		{
			fp.Write(pFullName->GetBuffer(), len);
			pFullName->ReleaseBuffer();
		}

		len = pPyjcArray ? pPyjcArray->GetLength() : 0;
		fp.Write(&len, sizeof(long));
		if(len)
		{
			fp.Write(pPyjcArray->GetBuffer(), len);
			pPyjcArray->ReleaseBuffer();
		}
	}
};

//股票详细信息
struct StockUserInfo
{
	CodeInfo			m_ciStockCode;		//股票代码结构
	char				m_cStockName[STOCK_NAME_SIZE];	//股票名称
	char				m_cShortPyjc[PYJC_MAX_LENGTH];	//拼音简称
	long				m_lPrevClose;		//昨收
	unsigned long		m_l5DayVol;			//五日量
	short				m_nPriceUnit;		//价格单位
	short				m_nDecimal;			//价格精度
	short				m_nHandNum;         //每手股数
	float				m_fUpPrice;     // 涨停板价
	float				m_fDownPrice;   // 跌停板价

	CString	GetKey()	//股票代码信息
	{
		return m_ciStockCode.GetKey();
	}

	virtual CString	GetName(BOOL bFullName = FALSE)
	{
		if(bFullName)
		{
			StockInfoEx *pStockEx = *((StockInfoEx **)&m_cShortPyjc[12]);
			if(pStockEx && pStockEx->pFullName)
				return *pStockEx->pFullName;
		}
		CString str;
		LPSTR pstr = str.GetBuffer(STOCK_NAME_SIZE + 1);
		memcpy(pstr, m_cStockName, STOCK_NAME_SIZE);
		pstr[STOCK_NAME_SIZE] = 0;
		str.ReleaseBuffer();

		return str;
	}
	//取得拼音简称
	LPCTSTR GetShortPyjc()
	{
		StockInfoEx *pStockEx = *((StockInfoEx **)&m_cShortPyjc[12]);

		if(pStockEx == NULL || pStockEx->pPyjcArray == NULL)
			return NULL;

		return *pStockEx->pPyjcArray;
	}
	CString		GetCode() { return m_ciStockCode.GetCode(); }
	// 查找股票代码或者名称或者拼音简称
	int  IsFind(const char* strKey,int& nUnique,int& nStart,int& nEnd, CString &szMarch)
	{
		CString strCurKey = strKey;
		return IsFind(strCurKey,nUnique,nStart,nEnd, szMarch);
	}
	int  IsFind(CString& strKey,int& nUnique,int& nStart,int& nEnd, CString &szMarch)
	{
		szMarch = GetDefaultPyjc();

		CString strSource = m_ciStockCode.GetCode();
		strSource.MakeUpper();

		if(strKey.GetLength() == 4 && m_ciStockCode.m_cCodeType & SZ_BOURSE)
		{
			CString s = _T("00");
			s += strKey;
			if((nStart = strSource.Find(s)) != -1)
				return nStart;
		}

		nStart = strSource.Find(strKey);
		if( nStart == 0 )
		{
			return 0;
		}
		else if( nStart != -1 )
		{
			return nStart;
		}
		// 深圳代码特别处理,去除前面00过滤处理。
		if( strSource[0] == '0' && strKey.GetLength() > 0 && strKey[0] == '0' )
		{
			strSource = strSource.Mid(1);
			nStart = strSource.Find(strKey);
			if( nStart == 0 )
			{
				return 0;
			}
			else if( nStart != -1 )
			{
				return nStart;
			}

			if( strKey.GetLength() > 1 )//&& strKey[0] == '0' && strKey[1] == '0' )
			{
				strSource = strSource.Mid(1);
				nStart = strSource.Find(strKey);
				if( nStart == 0 )
				{
					return 0;
				}
				else if( nStart != -1 )
				{
					return nStart;
				}
			}
		}
		strSource = this->GetName();
		strSource.TrimLeft();
		strSource.MakeUpper();
		nStart = strSource.Find(strKey);
		if( nStart == 0 )
		{
			return 0;
		}
		else if( nStart != -1 )
		{
			return nStart;
		}

		nStart = this->CompShortName(strKey, szMarch);
		if( nStart == 0 )
		{
			return 0;
		}
		else if( nStart != -1 )
		{
			return nStart;
		}
		return -1;
	}
	int  FindFromTail(const CString& strKey,int& nUnique,int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex)
	{
		CString strSource = m_ciStockCode.GetCode();	
		strSource.MakeUpper();
		int nIndex = 0;
		int nCodeLen = strSource.GetLength();
		int nKeyLen = strKey.GetLength();
		szMarch = GetDefaultPyjc();

		if(nKeyLen == 4 && m_ciStockCode.m_cCodeType & SZ_BOURSE)
		{
			CString s = _T("00");
			s += strKey;
			if((nStart = strSource.Find(s)) != -1)
				return nStart;
		}

		nIndex = nCodeLen - nKeyLen > 0 ? nCodeLen - nKeyLen:0;
		nStart = strSource.Find(strKey,nIndex);
		if (nStart == -1)
		{
			nStart = strSource.Find(strKey);
			if (nStart != -1)
				return nStart;
		}
		else
		{
			if (strKey.Find("6") >= 0)
			{
				if (strSource.CompareNoCase("600006")== 0 ||strSource.CompareNoCase("600600") ==0 ||strSource.CompareNoCase("600060") == 0)
					nBetter = nCurIndex;
			}
			if ( nKeyLen > 3)
			{
				nBetter = 0;
			}
			return 0;
		}
		// 深圳代码特别处理,去除前面00过滤处理。
		if( strSource[0] == '0' && strKey.GetLength() > 0 && strKey[0] == '0' )
		{
			strSource = strSource.Mid(1);
			nCodeLen = strSource.GetLength();
			nIndex = nCodeLen - nKeyLen > 0 ? nCodeLen - nKeyLen:0; 
			nStart = strSource.Find(strKey,nIndex);
			if( nStart != -1 )
			{
				return 0;
			}
			else 
			{
				nStart = strSource.Find(strKey);
				if (nStart != -1)
					return nStart;
			}
			if( strKey.GetLength() > 1 )
			{
				strSource = strSource.Mid(1);
				nCodeLen = strSource.GetLength();
				nIndex = nCodeLen - nKeyLen > 0 ? nCodeLen - nKeyLen:0;
				nStart = strSource.Find(strKey,nIndex);
				if( nStart != -1 )
				{
					return 0;
				}
				else 
				{
					nStart = strSource.Find(strKey);
					if (nStart != -1)
						return nStart;
				}
			}
		}

		strSource = this->GetName();
		strSource.TrimLeft();
		strSource.MakeUpper();
		nStart = strSource.Find(strKey);
		if( nStart == 0 )
		{
			return 0;
		}
		else if( nStart != -1 )
		{
			return nStart;
		}

		nStart = this->CompShortName(strKey, szMarch);
		if( nStart == 0 )
		{
			return 0;
		}
		else if( nStart != -1 )
		{
			return nStart;
		}

		return -1;
	}
	BOOL IsSz4(CString &strKey)
	{
		CString strSource = m_ciStockCode.GetCode();
		strSource.MakeUpper();

		if(strKey.GetLength() == 4 && m_ciStockCode.m_cCodeType & SZ_BOURSE)
		{
			CString s = _T("00");
			s += strKey;
			if(s.CompareNoCase(strSource) == 0)
				return TRUE;
		}
		return FALSE;
	}
	int       CompShortName(const CString& pWord, CString &szMarch)
	{
		int index;
		CStringArray pyjc;
		GetPyjcTable(pyjc);
		for(int i = 0; i < pyjc.GetSize(); i++)
		{
			index = pyjc[i].Find(pWord);
			if(index != -1)
			{
				szMarch = pyjc[i];
				return index;
			}
		}
		return -1;
	}
	void      GetPyjcTable(CStringArray &PyjcTable)
	{
		int i = 0, j, k, n;
		CString str = "";
		LPCTSTR pstr;
		LPCTSTR pShortName = GetShortPyjc();
		PyjcTable.RemoveAll();

		if(pShortName == NULL || pShortName[0] == 0)
		{
			LPSTR pBuf;
			while(m_cShortPyjc[i] && i < 12)
			{
				pBuf = str.GetBuffer(5);
				memcpy(pBuf, m_cShortPyjc, 4);
				pBuf[4] = 0;
				str.ReleaseBuffer(4);
				PyjcTable.Add(str);
				i += 4;
			}
			return;
		}

		PyjcTable.Add(str);

		while(pShortName[i])
		{
			if(isalpha(pShortName[i]) || isdigit(pShortName[i]))
			{
				n = 1;
				for(j = 0; j < PyjcTable.GetCount(); j++)
					PyjcTable[j] += pShortName[i];
			}
			else
			{
				n = (int)pShortName[i++];
				pstr = pShortName + i;
				CStringArray strArray;
				strArray.Copy(PyjcTable);
				for(j = 0; j < PyjcTable.GetCount(); j++)
					PyjcTable[j] += pstr[0];
				//      pstr++;
				for(k = 1; k < n; k++)
				{
					for(j = 0; j < strArray.GetCount(); j++)
						PyjcTable.Add(strArray[j] + pstr[k]);
				}
			}
			i += n;
		}
	}
	CString   GetDefaultPyjc()
	{
		CStringArray pyjc;
		GetPyjcTable(pyjc);
		if(pyjc.GetSize() == 0)
			return _T("");
		return pyjc.GetAt(0);
	}
	int  Compare(CString& strKey,int& nStart,int& nEnd, CString &szMarch)
	{
		if(!GetName().CompareNoCase(strKey) || !m_ciStockCode.GetCode().CompareNoCase(strKey))
			return TRUE;
		return CompShortName(strKey, szMarch) > 0 ? TRUE : FALSE;
	}
	virtual void Copy(StockUserInfo* pStock)
	{
		if( pStock )
		{
			m_ciStockCode = pStock->m_ciStockCode;      //股票代码结构
			strncpy(m_cStockName, pStock->m_cStockName, min(strlen(pStock->m_cStockName) + 1, STOCK_NAME_SIZE - 1));	    //股票名称
		//	strncpy_s(m_cShortPyjc, pStock->m_cShortPyjc, sizeof(pStock->m_cShortPyjc) + 1);	      
			memcpy(m_cShortPyjc, pStock->m_cShortPyjc, PYJC_MAX_LENGTH);       //拼音简称
			m_lPrevClose	= pStock->m_lPrevClose ;		//昨收
			m_l5DayVol		= pStock->m_l5DayVol   ;		//五日量
			m_nPriceUnit	= pStock->m_nPriceUnit;			//价格单位		
			m_nDecimal		= pStock->m_nDecimal;
			m_nHandNum      = pStock->m_nHandNum;
			m_fDownPrice	= pStock->m_fDownPrice;
			m_fUpPrice		= pStock->m_fUpPrice;
		}
	}
	void SetEmpty()
	{
		m_ciStockCode.SetEmpty();
		memset(m_cStockName, 0, STOCK_NAME_SIZE);
		memset(m_cShortPyjc, 0, PYJC_MAX_LENGTH);
		m_lPrevClose = 0;
		m_l5DayVol = 0;
		m_nPriceUnit = 0;
		m_nDecimal = 0;
		m_nHandNum = 0;
		m_fDownPrice = 0;
		m_fUpPrice = 0;
	}
	virtual void Serialize(CArchive& ar)
	{
		if( ar.IsStoring() )
		{
			ar << m_ciStockCode.m_cCodeType;			// 证券类型
			ar << m_ciStockCode.GetCode();
			ar << GetName();
			ar << CString(m_cShortPyjc).Left(12);//GetPyjc();
			ar << m_lPrevClose;
			ar << m_l5DayVol;
			ar << m_nPriceUnit;
			ar << m_nDecimal;
			ar << m_nHandNum;
			ar << m_fUpPrice;
			ar << m_fDownPrice;
		}
		else
		{
			ar >> m_ciStockCode.m_cCodeType;			// 证券类型
			CString str;
			ar >> str;
			strncpy(m_ciStockCode.m_cCode,str,sizeof(m_ciStockCode.m_cCode));
			ar >> str;
			strncpy(m_cStockName,str,sizeof(m_cStockName));

			//ar >> m_cStockPYJC;
			ar >> str;
			strncpy(m_cShortPyjc,str,sizeof(char) * 12);
			ar >> m_lPrevClose;
			ar >> m_l5DayVol;
			ar >> m_nPriceUnit;
			ar >> m_nDecimal;
			ar >> m_nHandNum;
			ar >> m_fUpPrice;
			ar >> m_fDownPrice;
		}
	}
};

//证券信息
struct HSTypeTime
{
	short	m_nOpenTime;	// 前开市时间
	short	m_nCloseTime;	// 前闭市时间

	short   GetDist() { return (m_nCloseTime - m_nOpenTime); }
};

struct  HSTypeTime_Unoin
{
	short	m_nAheadOpenTime;	// 前开市时间
	short	m_nAheadCloseTime;	// 前闭市时间
	short	m_nAfterOpenTime;	// 后开市时间
	short	m_nAfterCloseTime;	// 后闭市时间

	HSTypeTime	m_nTimes[9];	// 新加入区段边界,两边界为-1时，为无效区段

	HSTypeTime	m_nPriceDecimal;   // 小数位, < 0

	// 第一个无效区段后所有区段全为无效。
};
// 股票分类名称
struct StockTypeName 
{
	char m_szName[20];	// 股票分类名称
};

struct StockType
{
	StockTypeName m_stTypeName;	// 对应分类的名称

	short   m_nStockType;		// 证券类型
	short   m_nTotal;			// 证券总数
	short   m_nOffset;			// 偏移量
	short   m_nPriceUnit;		// 价格单位
	short   m_nTotalTime;		// 总开市时间（分钟）
	short   m_nCurTime;			// 现在时间（分钟）

	union
	{
		HSTypeTime		 m_nNewTimes[11];
		HSTypeTime_Unoin m_union;
	};
	StockType()
	{
		memset(this,0,sizeof(StockType));
	}
	BOOL IsInitTime(int nTimer,int nDist = 10)
	{
		return ( nTimer >= (m_nNewTimes->m_nOpenTime - nDist) && nTimer < m_nNewTimes->m_nOpenTime );
	}
};

#define STOCKTYPE_COUNT 60
// 市场信息结构(客户端本地使用)
struct BourseInfo
{
	StockTypeName	m_stGroupName;	// 市场名称(对应市场类别)
	short			m_nMarketType;	// 市场类别(最高俩位)

	unsigned int	m_dwCRC;		// CRC校验码（市场）
	long			m_lDate;		// 今日日期（19971230）
	short			m_cType;		// 有效的证券类型个数
	int				m_nTotalCount;	//市场中股票个数
	StockType		 m_stNewType[STOCKTYPE_COUNT];	// 证券信息
	BourseInfo()
	{
		memset(this, 0, sizeof(BourseInfo));
	}
};

// 市场信息结构;
struct CommBourseInfo
{
	StockTypeName	m_stTypeName;	// 市场名称(对应市场类别)
	short			m_nMarketType;	// 市场类别(最高俩位)

	short			m_cCount;		// 有效的证券类型个数

	long			m_lDate;		// 今日日期（19971230）
	unsigned int	m_dwCRC;		// CRC校验码（分类）

	StockType		m_stNewType[1];	// 证券信息	

	CommBourseInfo()
	{
		memset(this, 0, sizeof(CommBourseInfo));
	}
};

// Level 2 行情数据
struct LevelRealTime 
{
	long   m_lOpen;			//开
	long   m_lMaxPrice;		//高
	long	m_lMinPrice;		//低
	long	m_lNewPrice;		//新
	unsigned long	m_lTotal;			//成交量
	//long	m_fAvgPrice;		//6  成交额(单位:百元)
	float	m_fAvgPrice;		//modify by lxqi 20090303	类型改为float防止越界

	long				m_lBuyPrice1;			// 买一价
	unsigned long		m_lBuyCount1;			// 买一量
	long				m_lBuyPrice2;			// 买二价
	unsigned long		m_lBuyCount2;			// 买二量
	long				m_lBuyPrice3;			// 买三价
	unsigned long		m_lBuyCount3;			// 买三量
	long				m_lBuyPrice4;			// 买四价
	unsigned long		m_lBuyCount4;			// 买四量
	long				m_lBuyPrice5;			// 买五价
	unsigned long		m_lBuyCount5;			// 买五量

	long				m_lSellPrice1;			// 卖一价
	unsigned long		m_lSellCount1;			// 卖一量
	long				m_lSellPrice2;			// 卖二价
	unsigned long		m_lSellCount2;			// 卖二量
	long				m_lSellPrice3;			// 卖三价
	unsigned long		m_lSellCount3;			// 卖三量
	long				m_lSellPrice4;			// 卖四价
	unsigned long		m_lSellCount4;			// 卖四量
	long				m_lSellPrice5;			// 卖五价
	unsigned long		m_lSellCount5;			// 卖五量

	long				m_lBuyPrice6;			// 买六价
	unsigned long		m_lBuyCount6;			// 买六量
	long				m_lBuyPrice7;			// 买七价
	unsigned long		m_lBuyCount7;			// 买七量
	long				m_lBuyPrice8;			// 买八价
	unsigned long		m_lBuyCount8;			// 买八量
	long				m_lBuyPrice9;			// 买九价
	unsigned long		m_lBuyCount9;			// 买九量
	long				m_lBuyPrice10;			// 买十价
	unsigned long		m_lBuyCount10;			// 买十量

	long				m_lSellPrice6;			// 卖六价
	unsigned long		m_lSellCount6;			// 卖六量
	long				m_lSellPrice7;			// 卖七价
	unsigned long		m_lSellCount7;			// 卖七量
	long				m_lSellPrice8;			// 卖八价
	unsigned long		m_lSellCount8;			// 卖八量
	long				m_lSellPrice9;			// 卖九价
	unsigned long		m_lSellCount9;			// 卖九量
	long				m_lSellPrice10;			// 卖十价
	unsigned long		m_lSellCount10;			// 卖十量

	unsigned long       m_lTickCount;			// 成交笔数	

	float				m_fBuyTotal;			// 委托买入总量
	float				WeightedAvgBidPx;		// 加权平均委买价格
	float				AltWeightedAvgBidPx;

	float				m_fSellTotal;			// 委托卖出总量
	float				WeightedAvgOfferPx;		// 加权平均委卖价格
	float				AltWeightedAvgOfferPx;

	float				m_IPOV;					// ETF IPOV	

	unsigned long		m_Time;					// 时间戳
};

// 最优盘口
struct HSLevelOrderQueue
{
	char		m_Side;				// 买卖方向
	long		m_Price;			// 价格水平
	unsigned short 		m_nActualOrderNum;	// 委托笔数	表示实际委托笔数，可能比50笔多  //2010.7.15 litao修改委托笔数为无符号
	short		m_noOrders;			// 上证通推过来的笔数  最多50笔	FAST协议最多100笔
	long		m_lData[1];			// 数据指针，指向的数据为订单数量
};

struct HSHkOrder
{
	short m_nPriceSpread;   //档位
	unsigned short m_nBroker;   //席位号
};

struct HSLevelTransaction
{
	long			m_TradeRef;		// 成交编号
	unsigned long 	m_TradeTime;	// 成交时间
	long 			m_TradePrice;	// 成交价格
	long 			m_TradeQty;		// 成交数量
	float 			m_TradeMoney;	// 成交金额
};

struct HSRealTimeHeader
{
	long				m_lOpen;         		// 今开盘
	long				m_lMaxPrice;     		// 最高价
	long				m_lMinPrice;     		// 最低价
	long				m_lNewPrice;     		// 最新价
	unsigned long		m_lTotal;				// 成交量(单位:股)
	float				m_fAvgPrice;			// 成交金额
};

// 实时数据
struct HSStockRealTime 
{
	long				m_lOpen;         		// 今开盘
	long				m_lMaxPrice;     		// 最高价
	long				m_lMinPrice;     		// 最低价
	long				m_lNewPrice;     		// 最新价
	unsigned long		m_lTotal;				// 成交量(单位:股)
	float				m_fAvgPrice;			// 成交金额

	long				m_lBuyPrice1;			// 买一价
	unsigned long		m_lBuyCount1;			// 买一量
	long				m_lBuyPrice2;			// 买二价
	unsigned long		m_lBuyCount2;			// 买二量
	long				m_lBuyPrice3;			// 买三价
	unsigned long		m_lBuyCount3;			// 买三量
	long				m_lBuyPrice4;			// 买四价
	unsigned long		m_lBuyCount4;			// 买四量
	long				m_lBuyPrice5;			// 买五价
	unsigned long		m_lBuyCount5;			// 买五量

	long				m_lSellPrice1;			// 卖一价
	unsigned long		m_lSellCount1;			// 卖一量
	long				m_lSellPrice2;			// 卖二价
	unsigned long		m_lSellCount2;			// 卖二量
	long				m_lSellPrice3;			// 卖三价
	unsigned long		m_lSellCount3;			// 卖三量
	long				m_lSellPrice4;			// 卖四价
	unsigned long		m_lSellCount4;			// 卖四量
	long				m_lSellPrice5;			// 卖五价
	unsigned long		m_lSellCount5;			// 卖五量

	long				m_nHand;				// 每手股数	(是否可放入代码表中？？？？）
	long				m_lNationalDebtRatio;	// 国债利率,基金净值

};

// 指标类实时数据
struct HSIndexRealTime  
{
	long		m_lOpen;				// 今开盘
	long		m_lMaxPrice;			// 最高价
	long		m_lMinPrice;			// 最低价
	long		m_lNewPrice;			// 最新价
	unsigned long		m_lTotal;				// 成交量
	float		m_fAvgPrice;			// 成交金额

	short		m_nRiseCount;			// 上涨家数
	short		m_nFallCount;			// 下跌家数
	long		m_nTotalStock1;			/* 对于综合指数：所有股票 - 指数
										对于分类指数：本类股票总数 */
	unsigned long		m_lBuyCount;			// 委买数
	unsigned long		m_lSellCount;			// 委卖数
	short		m_nType;				// 指数种类：0-综合指数 1-A股 2-B股
	short		m_nLead;            	// 领先指标
	short		m_nRiseTrend;       	// 上涨趋势
	short		m_nFallTrend;       	// 下跌趋势
	short		m_nNo2[5];				// 保留
	short		m_nTotalStock2;			/* 对于综合指数：A股 + B股 
										对于分类指数：0 */
	long		m_lADL;					// ADL 指标
	long		m_lNo3[3];				// 保留
	long		m_nHand;				// 每手股数	
};

// 期货、外盘实时数据
struct HSQHRealTime 
{
	long		m_lOpen;         	// 今开盘
	long		m_lMaxPrice;     	// 最高价
	long		m_lMinPrice;     	// 最低价
	long		m_lNewPrice;     	// 最新价

	unsigned long		m_lTotal;		   	// 成交量(单位:合约单位)
	long				m_lChiCangLiang;    // 持仓量(单位:合约单位)

	long		m_lBuyPrice1;		// 买一价
	long		m_lBuyCount1;		// 买一量
	long		m_lSellPrice1;		// 卖一价
	long		m_lSellCount1;		// 卖一量

	long		m_lPreJieSuanPrice; // 昨结算价

	//long		m_nHand;				// 每手股数
	//long 		m_lPreCloseChiCang;		// 昨持仓量(单位:合约单位)


	long		m_lJieSuanPrice;    // 现结算价
	long		m_lCurrentCLOSE;	// 今收盘
	long		m_lHIS_HIGH;		// 史最高
	long		m_lHIS_LOW;	 		// 史最低
	long		m_lUPPER_LIM;		// 涨停板
	long		m_lLOWER_LIM;		// 跌停板

	long		m_nHand;				// 每手股数
	long 		m_lPreCloseChiCang;		// 昨持仓量(单位:合约单位)

	long 		m_lLongPositionOpen;	// 多头开(单位:合约单位)
	long 		m_lLongPositionFlat;	// 多头平(单位:合约单位)
	long 		m_lNominalOpen;			// 空头开(单位:合约单位)	
	long 		m_lNominalFlat;			// 空头平(单位:合约单位)

	long		m_lPreClose;			// 前天收盘????
};

// 期货、外盘主推实时数据(精简)
struct HSQHRealTime_Min
{
	long		m_lOpen;         	// 今开盘
	long		m_lMaxPrice;     	// 最高价
	long		m_lMinPrice;     	// 最低价
	long		m_lNewPrice;     	// 最新价

	unsigned long		m_lTotal;		   	// 成交量(单位:合约单位)
	long				m_lChiCangLiang;    // 持仓量(单位:合约单位)

	long		m_lBuyPrice1;		// 买一价
	long		m_lBuyCount1;		// 买一量
	long		m_lSellPrice1;		// 卖一价
	long		m_lSellCount1;		// 卖一量

	long		m_lPreJieSuanPrice; // 昨结算价
};

struct StockOtherDataDetailTime
{
	unsigned short m_nTime;
	unsigned short m_nSecond;
};

// 各股票其他数据
struct StockOtherData
{
	union
	{
		unsigned long					 m_nTimeOld;	  // 现在时间	
		unsigned short					 m_nTime;		  // 现在时间	
		StockOtherDataDetailTime		 m_sDetailTime;
	};

	unsigned long  m_lCurrent;    // 现在总手

	unsigned long  m_lOutside;    // 外盘
	unsigned long  m_lInside;     // 内盘

	union
	{
		unsigned long  m_lKaiCang;    // 今开仓,深交所股票单笔成交数,港股交易宗数
		unsigned long  m_lPreClose;   // 对于外汇时，昨收盘数据		
	};

	union
	{
		unsigned long  m_rate_status; // 对于外汇时，报价状态
		// 对于股票，信息状态标志,
		// MAKELONG(MAKEWORD(nStatus1,nStatus2),MAKEWORD(nStatus3,nStatus4))
		unsigned long  m_lPingCang;   // 今平仓
	};

	void GetStatus(BYTE& bStatus1,BYTE& bStatus2,BYTE& bStatus3,BYTE& bStatus4)
	{
		WORD lo = LOWORD(m_rate_status);
		WORD hi = HIWORD(m_rate_status);

		bStatus1 = LOBYTE(lo);
		bStatus2 = HIBYTE(lo);
		bStatus3 = LOBYTE(hi);
		bStatus4 = HIBYTE(hi);
	}
};

// 时时数据分类
union ShareRealTimeData
{
	HSStockRealTime			m_nowData;		// 个股实时基本数据
	HSStockRealTime			m_stStockData;

	HSIndexRealTime			m_indData;		// 指数实时基本数据
	HSQHRealTime			m_qhData;		// 期货实时基本数据

	HSQHRealTime_Min		m_qhMin;	
};


// 服务器证券简短信息
struct ServerCompare	
{
	HSMarketDataType	m_cBourse;			// 证券分类类型
	short				m_nAlignment;    	// 为了4字节对齐而添加的字段
	unsigned int		m_dwCRC;			// CRC校验码
};


//K线数据
struct StockDay
{
	long	m_lDate;  	/*year-month-day ,example: 19960616
						分钟数据的表示方法如下：yymmddhhnn(年月日时分)
						yy指的是year - 1990，故年份表达范围：1990 - 2011
						如0905131045，指的是：1999年5月13号10点45分。*/
	long	m_lOpenPrice;		//开
	long	m_lMaxPrice;		//高
	long	m_lMinPrice;		//低
	long	m_lClosePrice;		//收
	float   m_lMoney;
	float   m_lTotal;

	long	m_lNationalDebtRatio; // 国债利率(单位为0.1分),基金净值


	union
	{
		long	m_lPrevClose;
		struct
		{
			short	m_nMessageMask;	//资料信息掩码
			short	m_nOpenVolumn;  //开盘成交量
		};
	};

	union
	{
		struct
		{
			short  m_nVolAmount;   //成交次数
			short  m_nZeroVol;	   //对倒成交量。
		};
		long* m_pDataEx;		   // 如果是除权，是 ChuQuanData 结构数据指针
	};

	void To(char cIntelToComputer )
	{

	}
};

struct StockTickDetailTime
{
	char m_nBuyOrSell;
	char m_nSecond;
};

// 分笔记录
struct StockTick
{
	short		   m_nTime;			   // 当前时间（距开盘分钟数）

	union
	{

		short			    m_nBuyOrSellOld;	 // 旧的，保留
		char			    m_nBuyOrSell;	     // 是按价成交还是按卖价成交(1 按买价 0 按卖价)
		StockTickDetailTime m_sDetailTime;       // 包含秒数据
	};

	long		   m_lNewPrice;        // 成交价
	unsigned long  m_lCurrent;		   // 成交量

	long		   m_lBuyPrice;        // 委买价
	long		   m_lSellPrice;       // 委卖价

	//
	unsigned long  m_nChiCangLiang;	   // 持仓量,深交所股票单笔成交数,港股成交盘分类(Y,M,X等，根据数据源再确定）

	//long  m_lOutside;         // 外盘
	//long  m_lInside;          // 内盘

	void To(char cIntelToComputer )
	{

	}
};

//通用文件头结构
struct HSCommonFileHead
{
	long 	m_lFlag;   // 文件类型标识
	time_t 	m_lDate;   // 文件更新日期(长度:32bit)
	long 	m_lVersion; // 文件结构版本标识
	long 	m_lCount;  // 数据总个数	 
};

struct CalcData_Share // 股票计算
{
	long 			m_lMa10;			// 10天，20天，50天收盘均价
	long 			m_lMa20;
	long 			m_lMa50;

	long 			m_lMonthMax;		// 月最高最低
	long 			m_lMonthMin;		

	long 			m_lYearMax;			// 年最高最低
	long 			m_lYearMin;

	long			m_lHisAmplitude;	// 历史波幅(使用时除1000为百分比数）

	void To(char cIntelToComputer )
	{
	}
};


#pragma	pack()
