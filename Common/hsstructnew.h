/*******************************************************************************
* Copyright (c)2010, ������Ϣ�������޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsStructnew.h
* �ļ���ʶ���°�����ͨ�ýṹ
* ժ    Ҫ���°�����ͨ�ýṹ
*
* ��ǰ�汾��Ͷ��Ӯ��2.0
* ��    �ߣ�
* ������ڣ�2010-09
*
* ��    ע��
*
*
*******************************************************************************/

#pragma once

#pragma	pack(1)
#pragma warning(disable : 4996)


/*
�г�����壺
��λ�����ʾ���£�
15		   12		8					0
|			|	  	  |					|
| ���ڷ���	|�г����� |	����Ʒ�ַ���	|
*/
typedef short HSMarketDataType;			  // �г�������������
/*���ڴ���*/
#define STOCK_MARKET			 0X1000   // ��Ʊ
#	define SH_BOURSE			 0x0100   // �Ϻ�
#	define SZ_BOURSE			 0x0200   // ����
#	define SYSBK_BOURSE			 0x0400   // ϵͳ���
#	define USERDEF_BOURSE		 0x0800   // �Զ��壨��ѡ�ɻ����Զ����飩
#			define KIND_INDEX		 0x0000   // ָ�� 
#			define KIND_STOCKA		 0x0001   // A�� 
#			define KIND_STOCKB		 0x0002   // B�� 
#			define KIND_BOND		 0x0003   // ծȯ
#			define KIND_FUND		 0x0004   // ����
#			define KIND_THREEBOAD	 0x0005   // ����
#			define KIND_SMALLSTOCK	 0x0006   // ��С�̹�
#			define KIND_PLACE		 0x0007	  // ����
#			define KIND_LOF			 0x0008	  // LOF
#			define KIND_ETF			 0x0009   // ETF
#			define KIND_QuanZhen	 0x000A   // Ȩ֤
#			define KIND_STOCKGE		 0x000D	  // ��ҵ��(Growth Enterprise)

#			define KIND_OtherIndex	 0x000E   // ������������࣬��:����ָ��

#			define SC_Others		 0x000F   // ���� 0x09
#			define KIND_USERDEFINE	 0x0010   // �Զ���ָ��
#			define KIND_STOCKA_SELECTED	0x0011	//������鱨�۱���
#			define KIND_STOCKA_NOTSEL	0x0012	//������鱨���޳�


// �۹��г�
#define HK_MARKET				 0x2000 // �۹ɷ���
#	define HK_BOURSE			 0x0100 // �����г�
#	define	GE_BOURSE			 0x0200 // ��ҵ���г�(Growth Enterprise Market)
#	define	INDEX_BOURSE		 0x0300	// ָ���г�	
//#		define HK_KIND_INDEX			 0x0000   // ��ָ
//#		define HK_KIND_FUTURES_INDEX	 0x0001   // ��ָ
//#		define	KIND_Option				 0x0002	  // �۹���Ȩ
# define HK_KIND_BOND  0x0000   // BOND-ծȯ
# define HK_KIND_BWRT  0x0001   // BWRT-һ��������Ȩ֤
# define HK_KIND_EQTY  0x0002   // EQTY-��˾��Ʊ
# define HK_KIND_TRST  0x0003   // TRST-����
# define HK_KIND_WRNT  0x0004   // WRNT-Ȩ֤

#	define SYSBK_BOURSE			 0x0400 // �۹ɰ��(H��ָ���ɷݹɣ�����ָ���ɷݹɣ���
#	define USERDEF_BOURSE		 0x0800 // �Զ��壨��ѡ�ɻ����Զ����飩
// #			define HK_KIND_BOND		 0x0000   // ծȯ
// #			define HK_KIND_MulFund	 0x0001   // һ�����Ϲ�֤
//#			define HK_KIND_FUND		 0x0002   // ����
//#			define KIND_WARRANTS	 0x0003   // �Ϲ�֤
//#			define KIND_JR			 0x0004   // ����
#			define KIND_ZH			 0x0005   // �ۺ�
#			define KIND_DC			 0x0006   // �ز�
#			define KIND_LY			 0x0007   // ����
#			define KIND_GY			 0x0008   // ��ҵ
#			define KIND_GG			 0x0009   // ����
#			define KIND_QT			 0x000A   // ����

/*�ڻ�����*/
#define FUTURES_MARKET			 0x4000 // �ڻ�
#		define DALIAN_BOURSE		 0x0100	// ����
#				define KIND_BEAN		 0x0001	// ����
#				define KIND_YUMI		 0x0002	// ��������
#				define KIND_SHIT		 0x0003	// ����ʳ��
#				define KIND_DZGY		 0x0004	// ���ڹ�ҵ1
#				define KIND_DZGY2		 0x0005	// ���ڹ�ҵ2
#				define KIND_DOUYOU		 0x0006	// ������
#				define KIND_JYX			 0x0007	// ����ϩ
#				define KIND_ZTY			 0x0008	// �����

#		define SHANGHAI_BOURSE	  0x0200	// �Ϻ�
#				define KIND_METAL		 0x0001	// �Ϻ�����
#				define KIND_RUBBER		 0x0002	// �Ϻ���
#				define KIND_FUEL		 0x0003	// �Ϻ�ȼ��
//#				define KIND_GUZHI		 0x0004	// ��ָ�ڻ�
#				define KIND_QHGOLD		 0x0005	// �Ϻ��ƽ�

#		define ZHENGZHOU_BOURSE	  0x0300	// ֣��
#				define KIND_XIAOM		 0x0001	// ֣��С��
#				define KIND_MIANH		 0x0002	// ֣���޻�
#				define KIND_BAITANG		 0x0003	// ֣�ݰ���
#				define KIND_PTA			 0x0004	// ֣��PTA
#				define KIND_CZY			 0x0005	// ������

#		define HUANGJIN_BOURSE	  0x0400	// �ƽ�����
#				define KIND_GOLD		 0x0001	// �Ϻ��ƽ�

#		define GUZHI_BOURSE		  0x0500	// ��ָ�ڻ�
#				define KIND_GUZHI		 0x0001	// ��ָ�ڻ�

/*���̴���*/
#define WP_MARKET				 ((HSMarketDataType)0x5000) // ����
#		define WP_INDEX				0x0100	// ����ָ�� // ������
#		define WP_LME				0x0200	// LME		// ������
#			define WP_LME_CLT			0x0210 //"����ͭ";
#			define WP_LME_CLL			0x0220 //"������";
#			define WP_LME_CLM			0x0230 //"������";
#			define WP_LME_CLQ			0x0240 //"����Ǧ";
#			define WP_LME_CLX			0x0250 //"����п";
#			define WP_LME_CWT			0x0260 //"������";
#			define WP_LME_CW			0x0270 //"����";
#			define WP_LME_SUB			0x0000

#			define WP_CBOT				0x0300	// CBOT			
#			define WP_NYMEX	 			0x0400	// NYMEX	 	
#			define WP_NYMEX_YY			0x0000	//"ԭ��";
#			define WP_NYMEX_RY			0x0001	//"ȼ��";
#			define WP_NYMEX_QY			0x0002	//"����";

#			define WP_COMEX	 			0x0500	// COMEX	 	
#			define WP_TOCOM	 			0x0600	// TOCOM	 	
#			define WP_IPE				0x0700	// IPE			
#			define WP_NYBOT				0x0800	// NYBOT		
#			define WP_NOBLE_METAL		0x0900	// �����	
#				define WP_NOBLE_METAL_XH	0x0000  //"�ֻ�";
#				define WP_NOBLE_METAL_HJ	0x0001  //"�ƽ�";
#				define WP_NOBLE_METAL_BY	0x0002  //"����";

#			define WP_FUTURES_INDEX		0x0a00	// ��ָ
#			define WP_SICOM				0x0b00	// SICOM		
#			define WP_LIBOR				0x0c00	// LIBOR		
#			define WP_NYSE				0x0d00	// NYSE
#			define WP_CEC				0x0e00	// CEC

#			define WP_INDEX_AZ	 		0x0110 //"����";
#			define WP_INDEX_OZ	 		0x0120 //"ŷ��";
#			define WP_INDEX_MZ	 		0x0130 //"����";
#			define WP_INDEX_TG	 		0x0140 //"̩��";
#			define WP_INDEX_YL	 		0x0150 //"ӡ��";
#			define WP_INDEX_RH	 		0x0160 //"�պ�";
#			define WP_INDEX_XHP 		0x0170 //"�¼���";
#			define WP_INDEX_FLB 		0x0180 //"���ɱ�";
#			define WP_INDEX_CCN 		0x0190 //"�й���½";
#			define WP_INDEX_TW  		0x01a0 //"�й�̨��";
#			define WP_INDEX_MLX 		0x01b0 //"��������";
#			define WP_INDEX_SUB 		0x0000 


/*������*/
#define FOREIGN_MARKET			 ((HSMarketDataType)0x8000) // ���
#	define WH_BASE_RATE			0x0100	// ��������
#	define WH_ACROSS_RATE		0x0200	// �������
#		define FX_TYPE_AU 			0x0000 // AU	��Ԫ
#		define FX_TYPE_CA 			0x0001 // CA	��Ԫ
#		define FX_TYPE_CN 			0x0002 // CN	�����
#		define FX_TYPE_DM 			0x0003 // DM	���
#		define FX_TYPE_ER 			0x0004 // ER	ŷԪ	 
#		define FX_TYPE_HK 			0x0005 // HK	�۱�
#		define FX_TYPE_SF 			0x0006 // SF	��ʿ 
#		define FX_TYPE_UK 			0x0007 // UK	Ӣ��
#		define FX_TYPE_YN 			0x0008 // YN	��Ԫ

#	define WH_FUTURES_RATE			0x0300  // �ڻ�

// �ڲ����࣬����Ʊ��������
#define STOCK_WHILOM_NAME_MARKET ((HSMarketDataType)0xF000)


//#define	MakeMarket(x)			((HSMarketDataType)((x) & 0xF000))
//#define MakeMainMarket(x)		((HSMarketDataType)((x) & 0xFFF0))
//#define	MakeMidMarket(x)		((HSMarketDataType)((x) & 0x0F00)) // ����ڶ�λ

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
	return ((HSMarketDataType)((x) & 0x0F00)); // ����ڶ�λ
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

// �Ƿ�Ϊ���ڹ�Ʊָ��
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

// �Ƿ�Ϊ����
static int MakeFundMarket(HSMarketDataType x)
{
	return ( (MakeMarket(x) == STOCK_MARKET) &&
		(MakeMidMarket(x) != 0) &&
		(MakeSubMarket(x) == KIND_FUND) );
}

// �Ƿ�Ϊ�۹�ָ��
//#define MakeHKIndex(x)          ( MakeMainMarket(x) == (HK_MARKET | INDEX_BOURSE) )
static int MakeHKIndex(HSMarketDataType x)
{
	return ( MakeMainMarket(x) == (HK_MARKET | INDEX_BOURSE) );
}

// �Ƿ�Ϊ��ָ�ڻ�
static int MakeGZIndex(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) == (FUTURES_MARKET | GUZHI_BOURSE)) &&
		(MakeSubMarket(x) == KIND_GUZHI) );
}

// �Ƿ�Ϊ�ƽ��ڻ�
static int MakeGoldIndex(HSMarketDataType x)
{
	return ( (MakeMainMarket(x) == (FUTURES_MARKET | HUANGJIN_BOURSE)) &&
		(MakeSubMarket(x) == KIND_GOLD) );
}
//yangdl 2008.03.06 ���⴦��۸�
// �Ƿ�Ϊ��Ҫ���⴦��۸�ķ���
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

// �Ƿ�Ϊ���ڹ�ƱETF
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
#define OT_Bourse	STOCK_WHILOM_NAME_MARKET // (�����г���Ŀǰ��Ҫ�����ڲ����࣬����Ʊ��������)

#define SC_Union			0x40 // ���

#define ToDataType(x) ((HSMarketDataType)(x))

#define Negative(x,y)  ( y ? x&0x0FFFFFFF : x )
//#define Negative(x,y)   x


#define PYJC_MAX_LENGTH		16			// ƴ������
#define STOCK_NAME_SIZE		16			// ��Ʊ���Ƴ���

extern UINT HashKey(LPCTSTR key,int nKeyCount,const int nHashTableSize);
// ��Ʊ����ṹ
struct CodeInfo
{
	HSMarketDataType	m_cCodeType;	// ֤ȯ����
	char				m_cCode[6];		// ֤ȯ����

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

	// ������ϣ��, ���ع�ϣ����
	long GetLongKey(int nHashTableSize = 999999)
	{
		return HashKey(m_cCode,6,nHashTableSize);
	}

	// �Ƚ�����	����TRUE or FALSE
	BOOL IsType(HSMarketDataType cType)
	{
		return (MakeSubMarket(m_cCodeType) == cType);
	}

	// ���ع�Ʊ����
	LPCTSTR GetKey(char* strKey) // strKey > 9
	{
		strncpy(strKey,m_cCode,6);
		strKey[6] = '\0';

		return strKey;
	}
	// ���ع�Ʊ����
	LPCTSTR GetKey()
	{
		static char strKey[7];
		strncpy_s(strKey,m_cCode,6);
		strKey[6] = '\0';

		return strKey;
	}

	// ������ ��������
	void operator=(CodeInfo* pInfo)
	{
		if(pInfo != NULL)
		{
			memcpy(this,pInfo,sizeof(CodeInfo));
		}
	}
	// ������ ��������
	void operator=(CodeInfo pInfo)
	{
		memcpy(this,&pInfo,sizeof(CodeInfo));		
	}


	// ���ش���
	CString GetCode()
	{
		//20090620 YJT �����˴�д�� begin
		char cCode[7];
		memset(cCode, 0, sizeof(cCode));
		strncpy_s(cCode, m_cCode, 6);
		return CString(cCode);//.Left(6);
		//20090620 YJT �����˴�д�� end
	}

	// ȡ�ô���, �����4λ, Ҳ��������λ����
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
		if(len >= 0)  //qinhn 20100401 ���ж���if(len)�޸�Ϊif(len >= 0),��ֹlenΪ-1ʱ�����쳣���������
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

//��Ʊ��ϸ��Ϣ
struct StockUserInfo
{
	CodeInfo			m_ciStockCode;		//��Ʊ����ṹ
	char				m_cStockName[STOCK_NAME_SIZE];	//��Ʊ����
	char				m_cShortPyjc[PYJC_MAX_LENGTH];	//ƴ�����
	long				m_lPrevClose;		//����
	unsigned long		m_l5DayVol;			//������
	short				m_nPriceUnit;		//�۸�λ
	short				m_nDecimal;			//�۸񾫶�
	short				m_nHandNum;         //ÿ�ֹ���
	float				m_fUpPrice;     // ��ͣ���
	float				m_fDownPrice;   // ��ͣ���

	CString	GetKey()	//��Ʊ������Ϣ
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
	//ȡ��ƴ�����
	LPCTSTR GetShortPyjc()
	{
		StockInfoEx *pStockEx = *((StockInfoEx **)&m_cShortPyjc[12]);

		if(pStockEx == NULL || pStockEx->pPyjcArray == NULL)
			return NULL;

		return *pStockEx->pPyjcArray;
	}
	CString		GetCode() { return m_ciStockCode.GetCode(); }
	// ���ҹ�Ʊ����������ƻ���ƴ�����
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
		// ���ڴ����ر���,ȥ��ǰ��00���˴���
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
		// ���ڴ����ر���,ȥ��ǰ��00���˴���
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
			m_ciStockCode = pStock->m_ciStockCode;      //��Ʊ����ṹ
			strncpy(m_cStockName, pStock->m_cStockName, min(strlen(pStock->m_cStockName) + 1, STOCK_NAME_SIZE - 1));	    //��Ʊ����
		//	strncpy_s(m_cShortPyjc, pStock->m_cShortPyjc, sizeof(pStock->m_cShortPyjc) + 1);	      
			memcpy(m_cShortPyjc, pStock->m_cShortPyjc, PYJC_MAX_LENGTH);       //ƴ�����
			m_lPrevClose	= pStock->m_lPrevClose ;		//����
			m_l5DayVol		= pStock->m_l5DayVol   ;		//������
			m_nPriceUnit	= pStock->m_nPriceUnit;			//�۸�λ		
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
			ar << m_ciStockCode.m_cCodeType;			// ֤ȯ����
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
			ar >> m_ciStockCode.m_cCodeType;			// ֤ȯ����
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

//֤ȯ��Ϣ
struct HSTypeTime
{
	short	m_nOpenTime;	// ǰ����ʱ��
	short	m_nCloseTime;	// ǰ����ʱ��

	short   GetDist() { return (m_nCloseTime - m_nOpenTime); }
};

struct  HSTypeTime_Unoin
{
	short	m_nAheadOpenTime;	// ǰ����ʱ��
	short	m_nAheadCloseTime;	// ǰ����ʱ��
	short	m_nAfterOpenTime;	// ����ʱ��
	short	m_nAfterCloseTime;	// �����ʱ��

	HSTypeTime	m_nTimes[9];	// �¼������α߽�,���߽�Ϊ-1ʱ��Ϊ��Ч����

	HSTypeTime	m_nPriceDecimal;   // С��λ, < 0

	// ��һ����Ч���κ���������ȫΪ��Ч��
};
// ��Ʊ��������
struct StockTypeName 
{
	char m_szName[20];	// ��Ʊ��������
};

struct StockType
{
	StockTypeName m_stTypeName;	// ��Ӧ���������

	short   m_nStockType;		// ֤ȯ����
	short   m_nTotal;			// ֤ȯ����
	short   m_nOffset;			// ƫ����
	short   m_nPriceUnit;		// �۸�λ
	short   m_nTotalTime;		// �ܿ���ʱ�䣨���ӣ�
	short   m_nCurTime;			// ����ʱ�䣨���ӣ�

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
// �г���Ϣ�ṹ(�ͻ��˱���ʹ��)
struct BourseInfo
{
	StockTypeName	m_stGroupName;	// �г�����(��Ӧ�г����)
	short			m_nMarketType;	// �г����(�����λ)

	unsigned int	m_dwCRC;		// CRCУ���루�г���
	long			m_lDate;		// �������ڣ�19971230��
	short			m_cType;		// ��Ч��֤ȯ���͸���
	int				m_nTotalCount;	//�г��й�Ʊ����
	StockType		 m_stNewType[STOCKTYPE_COUNT];	// ֤ȯ��Ϣ
	BourseInfo()
	{
		memset(this, 0, sizeof(BourseInfo));
	}
};

// �г���Ϣ�ṹ;
struct CommBourseInfo
{
	StockTypeName	m_stTypeName;	// �г�����(��Ӧ�г����)
	short			m_nMarketType;	// �г����(�����λ)

	short			m_cCount;		// ��Ч��֤ȯ���͸���

	long			m_lDate;		// �������ڣ�19971230��
	unsigned int	m_dwCRC;		// CRCУ���루���ࣩ

	StockType		m_stNewType[1];	// ֤ȯ��Ϣ	

	CommBourseInfo()
	{
		memset(this, 0, sizeof(CommBourseInfo));
	}
};

// Level 2 ��������
struct LevelRealTime 
{
	long   m_lOpen;			//��
	long   m_lMaxPrice;		//��
	long	m_lMinPrice;		//��
	long	m_lNewPrice;		//��
	unsigned long	m_lTotal;			//�ɽ���
	//long	m_fAvgPrice;		//6  �ɽ���(��λ:��Ԫ)
	float	m_fAvgPrice;		//modify by lxqi 20090303	���͸�Ϊfloat��ֹԽ��

	long				m_lBuyPrice1;			// ��һ��
	unsigned long		m_lBuyCount1;			// ��һ��
	long				m_lBuyPrice2;			// �����
	unsigned long		m_lBuyCount2;			// �����
	long				m_lBuyPrice3;			// ������
	unsigned long		m_lBuyCount3;			// ������
	long				m_lBuyPrice4;			// ���ļ�
	unsigned long		m_lBuyCount4;			// ������
	long				m_lBuyPrice5;			// �����
	unsigned long		m_lBuyCount5;			// ������

	long				m_lSellPrice1;			// ��һ��
	unsigned long		m_lSellCount1;			// ��һ��
	long				m_lSellPrice2;			// ������
	unsigned long		m_lSellCount2;			// ������
	long				m_lSellPrice3;			// ������
	unsigned long		m_lSellCount3;			// ������
	long				m_lSellPrice4;			// ���ļ�
	unsigned long		m_lSellCount4;			// ������
	long				m_lSellPrice5;			// �����
	unsigned long		m_lSellCount5;			// ������

	long				m_lBuyPrice6;			// ������
	unsigned long		m_lBuyCount6;			// ������
	long				m_lBuyPrice7;			// ���߼�
	unsigned long		m_lBuyCount7;			// ������
	long				m_lBuyPrice8;			// ��˼�
	unsigned long		m_lBuyCount8;			// �����
	long				m_lBuyPrice9;			// ��ż�
	unsigned long		m_lBuyCount9;			// �����
	long				m_lBuyPrice10;			// ��ʮ��
	unsigned long		m_lBuyCount10;			// ��ʮ��

	long				m_lSellPrice6;			// ������
	unsigned long		m_lSellCount6;			// ������
	long				m_lSellPrice7;			// ���߼�
	unsigned long		m_lSellCount7;			// ������
	long				m_lSellPrice8;			// ���˼�
	unsigned long		m_lSellCount8;			// ������
	long				m_lSellPrice9;			// ���ż�
	unsigned long		m_lSellCount9;			// ������
	long				m_lSellPrice10;			// ��ʮ��
	unsigned long		m_lSellCount10;			// ��ʮ��

	unsigned long       m_lTickCount;			// �ɽ�����	

	float				m_fBuyTotal;			// ί����������
	float				WeightedAvgBidPx;		// ��Ȩƽ��ί��۸�
	float				AltWeightedAvgBidPx;

	float				m_fSellTotal;			// ί����������
	float				WeightedAvgOfferPx;		// ��Ȩƽ��ί���۸�
	float				AltWeightedAvgOfferPx;

	float				m_IPOV;					// ETF IPOV	

	unsigned long		m_Time;					// ʱ���
};

// �����̿�
struct HSLevelOrderQueue
{
	char		m_Side;				// ��������
	long		m_Price;			// �۸�ˮƽ
	unsigned short 		m_nActualOrderNum;	// ί�б���	��ʾʵ��ί�б��������ܱ�50�ʶ�  //2010.7.15 litao�޸�ί�б���Ϊ�޷���
	short		m_noOrders;			// ��֤ͨ�ƹ����ı���  ���50��	FASTЭ�����100��
	long		m_lData[1];			// ����ָ�룬ָ�������Ϊ��������
};

struct HSHkOrder
{
	short m_nPriceSpread;   //��λ
	unsigned short m_nBroker;   //ϯλ��
};

struct HSLevelTransaction
{
	long			m_TradeRef;		// �ɽ����
	unsigned long 	m_TradeTime;	// �ɽ�ʱ��
	long 			m_TradePrice;	// �ɽ��۸�
	long 			m_TradeQty;		// �ɽ�����
	float 			m_TradeMoney;	// �ɽ����
};

struct HSRealTimeHeader
{
	long				m_lOpen;         		// ����
	long				m_lMaxPrice;     		// ��߼�
	long				m_lMinPrice;     		// ��ͼ�
	long				m_lNewPrice;     		// ���¼�
	unsigned long		m_lTotal;				// �ɽ���(��λ:��)
	float				m_fAvgPrice;			// �ɽ����
};

// ʵʱ����
struct HSStockRealTime 
{
	long				m_lOpen;         		// ����
	long				m_lMaxPrice;     		// ��߼�
	long				m_lMinPrice;     		// ��ͼ�
	long				m_lNewPrice;     		// ���¼�
	unsigned long		m_lTotal;				// �ɽ���(��λ:��)
	float				m_fAvgPrice;			// �ɽ����

	long				m_lBuyPrice1;			// ��һ��
	unsigned long		m_lBuyCount1;			// ��һ��
	long				m_lBuyPrice2;			// �����
	unsigned long		m_lBuyCount2;			// �����
	long				m_lBuyPrice3;			// ������
	unsigned long		m_lBuyCount3;			// ������
	long				m_lBuyPrice4;			// ���ļ�
	unsigned long		m_lBuyCount4;			// ������
	long				m_lBuyPrice5;			// �����
	unsigned long		m_lBuyCount5;			// ������

	long				m_lSellPrice1;			// ��һ��
	unsigned long		m_lSellCount1;			// ��һ��
	long				m_lSellPrice2;			// ������
	unsigned long		m_lSellCount2;			// ������
	long				m_lSellPrice3;			// ������
	unsigned long		m_lSellCount3;			// ������
	long				m_lSellPrice4;			// ���ļ�
	unsigned long		m_lSellCount4;			// ������
	long				m_lSellPrice5;			// �����
	unsigned long		m_lSellCount5;			// ������

	long				m_nHand;				// ÿ�ֹ���	(�Ƿ�ɷ��������У���������
	long				m_lNationalDebtRatio;	// ��ծ����,����ֵ

};

// ָ����ʵʱ����
struct HSIndexRealTime  
{
	long		m_lOpen;				// ����
	long		m_lMaxPrice;			// ��߼�
	long		m_lMinPrice;			// ��ͼ�
	long		m_lNewPrice;			// ���¼�
	unsigned long		m_lTotal;				// �ɽ���
	float		m_fAvgPrice;			// �ɽ����

	short		m_nRiseCount;			// ���Ǽ���
	short		m_nFallCount;			// �µ�����
	long		m_nTotalStock1;			/* �����ۺ�ָ�������й�Ʊ - ָ��
										���ڷ���ָ���������Ʊ���� */
	unsigned long		m_lBuyCount;			// ί����
	unsigned long		m_lSellCount;			// ί����
	short		m_nType;				// ָ�����ࣺ0-�ۺ�ָ�� 1-A�� 2-B��
	short		m_nLead;            	// ����ָ��
	short		m_nRiseTrend;       	// ��������
	short		m_nFallTrend;       	// �µ�����
	short		m_nNo2[5];				// ����
	short		m_nTotalStock2;			/* �����ۺ�ָ����A�� + B�� 
										���ڷ���ָ����0 */
	long		m_lADL;					// ADL ָ��
	long		m_lNo3[3];				// ����
	long		m_nHand;				// ÿ�ֹ���	
};

// �ڻ�������ʵʱ����
struct HSQHRealTime 
{
	long		m_lOpen;         	// ����
	long		m_lMaxPrice;     	// ��߼�
	long		m_lMinPrice;     	// ��ͼ�
	long		m_lNewPrice;     	// ���¼�

	unsigned long		m_lTotal;		   	// �ɽ���(��λ:��Լ��λ)
	long				m_lChiCangLiang;    // �ֲ���(��λ:��Լ��λ)

	long		m_lBuyPrice1;		// ��һ��
	long		m_lBuyCount1;		// ��һ��
	long		m_lSellPrice1;		// ��һ��
	long		m_lSellCount1;		// ��һ��

	long		m_lPreJieSuanPrice; // ������

	//long		m_nHand;				// ÿ�ֹ���
	//long 		m_lPreCloseChiCang;		// ��ֲ���(��λ:��Լ��λ)


	long		m_lJieSuanPrice;    // �ֽ����
	long		m_lCurrentCLOSE;	// ������
	long		m_lHIS_HIGH;		// ʷ���
	long		m_lHIS_LOW;	 		// ʷ���
	long		m_lUPPER_LIM;		// ��ͣ��
	long		m_lLOWER_LIM;		// ��ͣ��

	long		m_nHand;				// ÿ�ֹ���
	long 		m_lPreCloseChiCang;		// ��ֲ���(��λ:��Լ��λ)

	long 		m_lLongPositionOpen;	// ��ͷ��(��λ:��Լ��λ)
	long 		m_lLongPositionFlat;	// ��ͷƽ(��λ:��Լ��λ)
	long 		m_lNominalOpen;			// ��ͷ��(��λ:��Լ��λ)	
	long 		m_lNominalFlat;			// ��ͷƽ(��λ:��Լ��λ)

	long		m_lPreClose;			// ǰ������????
};

// �ڻ�����������ʵʱ����(����)
struct HSQHRealTime_Min
{
	long		m_lOpen;         	// ����
	long		m_lMaxPrice;     	// ��߼�
	long		m_lMinPrice;     	// ��ͼ�
	long		m_lNewPrice;     	// ���¼�

	unsigned long		m_lTotal;		   	// �ɽ���(��λ:��Լ��λ)
	long				m_lChiCangLiang;    // �ֲ���(��λ:��Լ��λ)

	long		m_lBuyPrice1;		// ��һ��
	long		m_lBuyCount1;		// ��һ��
	long		m_lSellPrice1;		// ��һ��
	long		m_lSellCount1;		// ��һ��

	long		m_lPreJieSuanPrice; // ������
};

struct StockOtherDataDetailTime
{
	unsigned short m_nTime;
	unsigned short m_nSecond;
};

// ����Ʊ��������
struct StockOtherData
{
	union
	{
		unsigned long					 m_nTimeOld;	  // ����ʱ��	
		unsigned short					 m_nTime;		  // ����ʱ��	
		StockOtherDataDetailTime		 m_sDetailTime;
	};

	unsigned long  m_lCurrent;    // ��������

	unsigned long  m_lOutside;    // ����
	unsigned long  m_lInside;     // ����

	union
	{
		unsigned long  m_lKaiCang;    // �񿪲�,�����Ʊ���ʳɽ���,�۹ɽ�������
		unsigned long  m_lPreClose;   // �������ʱ������������		
	};

	union
	{
		unsigned long  m_rate_status; // �������ʱ������״̬
		// ���ڹ�Ʊ����Ϣ״̬��־,
		// MAKELONG(MAKEWORD(nStatus1,nStatus2),MAKEWORD(nStatus3,nStatus4))
		unsigned long  m_lPingCang;   // ��ƽ��
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

// ʱʱ���ݷ���
union ShareRealTimeData
{
	HSStockRealTime			m_nowData;		// ����ʵʱ��������
	HSStockRealTime			m_stStockData;

	HSIndexRealTime			m_indData;		// ָ��ʵʱ��������
	HSQHRealTime			m_qhData;		// �ڻ�ʵʱ��������

	HSQHRealTime_Min		m_qhMin;	
};


// ������֤ȯ�����Ϣ
struct ServerCompare	
{
	HSMarketDataType	m_cBourse;			// ֤ȯ��������
	short				m_nAlignment;    	// Ϊ��4�ֽڶ������ӵ��ֶ�
	unsigned int		m_dwCRC;			// CRCУ����
};


//K������
struct StockDay
{
	long	m_lDate;  	/*year-month-day ,example: 19960616
						�������ݵı�ʾ�������£�yymmddhhnn(������ʱ��)
						yyָ����year - 1990������ݱ�ﷶΧ��1990 - 2011
						��0905131045��ָ���ǣ�1999��5��13��10��45�֡�*/
	long	m_lOpenPrice;		//��
	long	m_lMaxPrice;		//��
	long	m_lMinPrice;		//��
	long	m_lClosePrice;		//��
	float   m_lMoney;
	float   m_lTotal;

	long	m_lNationalDebtRatio; // ��ծ����(��λΪ0.1��),����ֵ


	union
	{
		long	m_lPrevClose;
		struct
		{
			short	m_nMessageMask;	//������Ϣ����
			short	m_nOpenVolumn;  //���̳ɽ���
		};
	};

	union
	{
		struct
		{
			short  m_nVolAmount;   //�ɽ�����
			short  m_nZeroVol;	   //�Ե��ɽ�����
		};
		long* m_pDataEx;		   // ����ǳ�Ȩ���� ChuQuanData �ṹ����ָ��
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

// �ֱʼ�¼
struct StockTick
{
	short		   m_nTime;			   // ��ǰʱ�䣨�࿪�̷�������

	union
	{

		short			    m_nBuyOrSellOld;	 // �ɵģ�����
		char			    m_nBuyOrSell;	     // �ǰ��۳ɽ����ǰ����۳ɽ�(1 ����� 0 ������)
		StockTickDetailTime m_sDetailTime;       // ����������
	};

	long		   m_lNewPrice;        // �ɽ���
	unsigned long  m_lCurrent;		   // �ɽ���

	long		   m_lBuyPrice;        // ί���
	long		   m_lSellPrice;       // ί����

	//
	unsigned long  m_nChiCangLiang;	   // �ֲ���,�����Ʊ���ʳɽ���,�۹ɳɽ��̷���(Y,M,X�ȣ���������Դ��ȷ����

	//long  m_lOutside;         // ����
	//long  m_lInside;          // ����

	void To(char cIntelToComputer )
	{

	}
};

//ͨ���ļ�ͷ�ṹ
struct HSCommonFileHead
{
	long 	m_lFlag;   // �ļ����ͱ�ʶ
	time_t 	m_lDate;   // �ļ���������(����:32bit)
	long 	m_lVersion; // �ļ��ṹ�汾��ʶ
	long 	m_lCount;  // �����ܸ���	 
};

struct CalcData_Share // ��Ʊ����
{
	long 			m_lMa10;			// 10�죬20�죬50�����̾���
	long 			m_lMa20;
	long 			m_lMa50;

	long 			m_lMonthMax;		// ��������
	long 			m_lMonthMin;		

	long 			m_lYearMax;			// ��������
	long 			m_lYearMin;

	long			m_lHisAmplitude;	// ��ʷ����(ʹ��ʱ��1000Ϊ�ٷֱ�����

	void To(char cIntelToComputer )
	{
	}
};


#pragma	pack()
