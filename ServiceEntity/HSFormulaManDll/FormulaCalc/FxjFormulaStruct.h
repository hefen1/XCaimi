/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�FxjFormulaStruct.h
 * �ļ���ʶ��
 * ժ    Ҫ��
 *
 * ��ǰ�汾��2.0
 * ��    �ߣ�
 * ������ڣ�
 *
 * ȡ���汾��
 * ԭ �� �ߣ�
 * ������ڣ�
 * ��	 ע��
 *******************************************************************************/

#if !defined(_FXJFORMULASTRUCT_H_)
#define _FXJFORMULASTRUCT_H_

#pragma	pack(1)

#include "ExportDll.h"

//#include "afxtempl.h"

//�����ҹ�ʽ�ļ���ʽ

/*
//�ļ�ͷ�ṹ
typedef struct tagSUPERSTK_HEAD
{
	BYTE		chSign[16];		//��ʽ�ļ���־,	3.1�汾Ϊ 10 00 00 00 7D CC 34 C0
	long		lCount;			//��ʽ�ļ��й�ʽ�ĸ���
	long		lUnknown;		//δ֪�ֶ�
//!!������ lCount �� long ������!!
} SSTK_HEAD;

//!!����������Ϣ���ⲿ�����ڹ�ʽͷ�ṹ���������ļ�ͷ�ṹ!!
typedef struct	STRMSG
{
	short	nMsgUnknown;
	short	nMsgWeek;			//����������Ϣ��enumMsgWeek
} strMsg;

typedef union unionItemMsg
{
	long		lMsg;
	strMsg      sMsg;
} SSTK_ITEMMSG;

//��ʽͷ�ṹ
typedef struct tagSUPERSTK_ITEMHEAD
{
	long			lUnknown1;		//δ֪�÷�
	SSTK_ITEMMSG	unionMsg;		//����������Ϣ
	long			lIcon;			//��ʽͼ��(���Ʒ�ʽ)��
									//Ϊ3ʱ����ͼ���ӣ�Ϊ4ʱ��ͼ
	char 			pszItemName[10];//��ʽ����, �˴��������л���
	long			lUnknown2;		//��;δ֪�����ƺ�����йء�
	BYTE			chKey[4];		//��ʽ����Ľ�����Կ(��ȫ���ܵ���Կ���ڴ˴�)
									//���ֽ�˳��ѭ��������
	long			lParameterCount;//��������
									//���½�����SSTK_ITEMPARA para[lParameterCounter];
} SSTK_ITEMHEAD;

//�����ṹ
typedef struct tagSUPERSTK_ITEMPARA
{
	float		fMin;				//��������Сֵ
	float		fMax;				//���������ֵ
	float		fDefault;			//������Ĭ��ֵ
	float		fStep;				//�����Ĳ���
	BYTE		chParameterName[15];//��������
} SSTK_ITEMPARA;

//�˽ṹ���Ժ��ԣ�������ȫ���ܵ�ʱ���Ҫ������ṹ���⿪��ʽ���ʽ
//����δ����������������Ϊ��λ�������������͵�ֵΪ 0x44 0x? 
//("?"�ο�����˵��)��ʱ��ṹ�����
//Ȼ����һ�������͵�0��ʾ������( "?" Ϊ��ʽ�Ĳ��������ͱ��ʽ�ж���Ĳ���֮��
//��Ϊ�����жϣ�ֻҪ "?" С��0xffff�Ϳ�����Ϊ�ṹ���Ѿ�������һ����˵
//"?"�����ܴ���0xff)
//����ı��ʽ���ò��������ñ�ʾ��������ר����һ�������洢
//��һ������������˵�������������ͣ��������ǲ�����
struct tagUnknown
{
	//!!��Ҫ�����Ӧ���������Ի������ʽ
};

//��ʽ��ṹ
typedef struct tagSUPERSTK_ITEMBODY
{
	struct tagPoints				
	{									//���Ʒ�ʽΪ��ͼʱ��7������
		long		nCount;				//�������
		float		nPoint[7];			//���7������
	} strPoints;
	CString				pszDepiction;	//��ʽ����
	CString				pszContent;		//��ʽ���ݣ���ȫ����ʱΪ��
	CString				pszExegesis;	//��ʽע��
	CString				pszHelp;		//��������
	CString				pszClass;		//������������ı�
	long				lEnd;			//������־��ͨ����20, 80֮��仯
}SUPERSTK_ITEMBODY;

//��������
char*	chMsgWeek[] = {
	"�ֱʳɽ�", //0x20
	"1������",	//0x40
	"5������",	//0x80
	"10������",	//0x100
	"15������",	//0x200
	"30������",	//0x400
	"60������",	//0x800
	"����",		//0x1000
	"����",		//0x2000
	"����",		//0x4000
	"������"	//0x8000
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
struct FxjParam  // ����
{
	float m_fMin;
	float m_fMax; 
	float m_fDefault;
	float m_fStep;
	char  m_strName[16]; // ������
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

struct FxjPerhaps  // �Ժ����
{
	int    m_nExpressLen;     // m_nCount*int
	char   m_pNextData[1];
};

struct FxjPerhaps1  // �Ժ����
{
	char m_cData[52];
	char m_pNextData[1];
};

struct FxjHead
{
    char   m_cFlag[8];      // ��־ 8
    int    m_nCount;		// ��ʽ���� 8+4
    int    m_nUnknown1[2];  // 
	char   m_pNextData[1];

	int  GetLen()
	{
		return sizeof(FxjHead) - sizeof(char);
	}
};

struct FxjName
{
    int   m_nPeriod;       // ���� 24+4
    int   m_nChart;        // ��ͼ/��ͼ // 28+4
    char  m_strName[10];   // ��ʽ���� // 32+10
	int   m_nUnknown;      // 42+4
	char  m_cKey[4];       // ��Կ46+4
	int   m_nParamCount;   // ��������42+4
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

struct FxjExpressEnd  // һ����ʽ��β
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
									//���Ʒ�ʽΪ��ͼʱ��7������
	long	m_nCount;				//�������
	float	m_fPoint[7];			//���7������

	int		GetOffset();

	int  GetLen()
	{
		return sizeof(FxjUserDefCoordinate);
	}
};

class BCGCONTROLBARDLLEXPORT FxjEidolon  // ��������
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

//��ʽ��ṹ
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
	FxjName*					 m_pName;      // ����
	CArray<FxjParam*,FxjParam*>* m_ayParam;    // ����
	
	FxjSerialString*			 m_pDepiction; // ����
	FxjSerialString*			 m_pBody;      // ��ʽ��
	FxjSerialString*			 m_pNotes;     // ע��
	FxjEidolon*					 m_pEidolon;   // ��������
	FxjSerialString*			 m_pType;	  // ��������

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


//�������̺������ļ���ʽ///////////////////////////////////////////////
//�����һ������ݽṹ,28�ֽ�
struct FXJStockData 
{ 
	time_t		m_time;		//ʱ��,UCT 
	float		m_fOpen;	//���� 
	float		m_fHigh;	//��� 
	float		m_fLow;		//��� 
	float		m_fClose;	//���� 
	float		m_fVolume;	//�ɽ��� 
	float		m_fAmount;	//�ɽ���

};
//׷���˽�����־�ķ����һ������ݽṹ 32�ֽ�
class FXJAStockData
{
public:
	FXJStockData	m_FXJStockData;
	float			m_fADataEnd;	// ������־ 0
public:	
	FXJAStockData()
	{
		m_fADataEnd = 0L;
	};
};
//ĳ֧��Ʊ������  32 + 32 * N �ֽ�
//#define A_PERIOD_TYPE_DAY_A_STOCK_LEN 64
//#define A_PERIOD_TYPE_DAY_A_STOCK_NODATA_LEN 32

class FXJStockDataHead
{
public:	
	long	m_lStart;	//Must be -1
	char	m_sStockCode[8];	//��Ʊ���ţ�SH(SZ)123456
	long	m_lSplit0;	//����0
	long	m_lSplit1;	//����1
	char	m_lpsStockName[8];	//��Ʊ����
	long	m_lDataStart; //Must Be 0
	
	FXJStockDataHead()
	{
		 m_lStart = -1;
		 m_lSplit0 = 0;
		 m_lSplit1 = 0;
		 m_lDataStart = 0;
	}

};
//���ο�,δʹ��
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