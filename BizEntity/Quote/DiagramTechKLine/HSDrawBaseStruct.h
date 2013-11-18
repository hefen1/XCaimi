/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�HSDrawBaseStruct.h
 * �ļ���ʶ����ͼ��Ҫ�ṹ����ͷ�ļ�
 * ժ    Ҫ����ͼ��Ҫ�ṹ����ͷ�ļ�
 *
 * ��ǰ�汾��1.0
 * ��    �ߣ�
 * ������ڣ�
 *
 * ��	 ע��
 *******************************************************************************/

#if !defined(_YLSDRAWSTRUCT_H_)
#define _YLSDRAWSTRUCT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//
#include "..\..\..\Common\FormulaManPublic.h"

// view����
enum CutViewType
{
	CutViewNone,	//��
	CutViewIn,		//Ƕ��
	CutViewPopup	//������ʾ,����ʷ����
};

// ���ƶ�������
enum CutAtTimePageType
{
	CutNoThing,		 //��

	CutReport,		 // ����
	CutPaiMingReport,// ��������

	CutFenShi,       // ���ɷ�ʱ
	CutTech,	     // ��������	

	CutBigTech,      // ��������
	CutBigFenShi,    // ���̷�ʱ
	CutBigADMFenShi, // ����ADM��ʱ
	CutBigNon,
	CutBigShSz,		 // ��֤��֤

	CutPubPage,		 // ����Сͼ		 

	CutAddTech,			 // ���Ӽ���ָ��
	CutChageTech,		 // change����ָ��
	CutAddPKLine,		 // ����PKLine
	CutAddByAlignTree,	 // ��ͣ���������
	CutAddMajor,       //yulx add 20091112���Ӵ���
	CutStarField,		// �ǿ�ͼ


	CutGraphicFinance,	//	����ͼʾ��������
	CutGannAnalisys     //  ��������
};
//��ϸ��������
enum DetailTableType
{
	DetailNone,
	DLFenShi,     // ��ʱ��ϸ
	DLRiXian,     // ������ϸ
	DLBig,	      // ������ϸ
	DLFenJiaTable // �ּ۱�
};
//��ϸ�������ݷ���
struct StockUserInfo;
struct DetailTableDataSend
{
	int   nType;	//����
	int   lDate;	//���� 
	void* pData;	//����ָ��	
	void* pSize;	//���ݳ���
	StockUserInfo* pStock;	//��Ʊ�ṹָ��
	CStringArray  strHisGeneralInfo;
	CArray<char*,char*>* p_ayReqData;
	CArray<short,short>* p_ayReqLen;
	CArray<StockUserInfo*,StockUserInfo*>* p_payStock;

	DetailTableDataSend()
	{
		memset(this,0,sizeof(DetailTableDataSend));
	}
};
//�з�������
enum SplitterType
{	
	SplitterX,   // ������
	SplitterY,   // ������
	SplitterBoth // �ݺ���
};
//���߹��߲���
enum DrawOperate
{
	OpProperty,   // ��������
	OpCopy,       // ���󿽱�
	OpDelete,	  // ɾ����һ
	OpDeleteAll,  // ɾ������
	OpObjList,	  // �����б�
	OpRectInfo,   // ������Ϣ
	OpDrawListObj // �����б���Ϣ
};
// ���� obj ����
class CCutTechAnalysis;
class CCutStockTrendEx;
class CDrawTechKLine;
struct AnalisysObjData
{
	BOOL			m_bRequest;			// ����
	BOOL			m_bRequestNext; 	// ����
	short			m_sObjtatus;		// ״̬	
	CodeInfo		m_Info;				// ��Ʊ������Ϣ

	long			m_lTotalVol;		// �ɽ�����
	long			m_lTotalMeney;		// �ɽ��ܶ�
	int				m_nLastTransDay;	// ����������

  union
  {
	CCutTechAnalysis* m_pParent;		// ���׹�����
  CCutStockTrendEx* m_pTrendEx;
  };
	CDrawTechKLine*	  m_pPKLine;		// ���ͼԪ

	int				m_nValueNum;		// ��Ȩ����ȡ������ݸ���
	StockDay *	    m_pData;    		// ��Ȩ�������

	/*************************************************************
	* �����¸���
	* ʱ�䣺  2003��9��18��
	* ������  ���յ����Ǵ����ж�����ԭʼK�����ݣ��������λK�ߣ�
	*************************************************************/
	int				m_nOldValueNum;		// ԭʼ���ݳ���
	StockDay*	    m_pOldData; 		// ԭʼ����
	/**********�����¸��Ľ���*************/

	~AnalisysObjData();
	AnalisysObjData() { memset(this,0,sizeof(AnalisysObjData)); }
  //CHANGE BY YUHJ 2008.5.8
  int IsRiseOrFall(int pos,BOOL bNegative)
  {
    long lPrevClose = 0;
    StockDay *pData = m_pData + pos;

    if(pos > 0)
		  lPrevClose = Negative(m_pData[pos-1].m_lClosePrice,bNegative);

  	long	lClose;//   = m_pStock->m_lPrevClose;
	  lClose = Negative(lPrevClose,bNegative); // m_lNO1
	  if( lClose == 0 )
	  {
		  lClose = pData->m_lClosePrice;
	  }
  
    long lData = pData->m_lClosePrice ? (pData->m_lClosePrice - lClose) * 100 : 0;  

    if(!lData)
      return 0;

    return lData > 0 ? 1 : -1;
  }
  //END
	AnalisysObjData(CCutStockTrendEx* pParent,short sObjtatus,const CodeInfo* pInfo)
	{
		memset(this,0,sizeof(AnalisysObjData));

		m_pTrendEx  = pParent;
		m_bRequest = TRUE;
		m_bRequestNext = FALSE;

		m_sObjtatus = sObjtatus;
		if(pInfo != NULL)
		{
			memcpy(&m_Info,pInfo,sizeof(CodeInfo));
		}
	}

	AnalisysObjData(CCutTechAnalysis* pParent,short sObjtatus,const CodeInfo* pInfo)
	{
		memset(this,0,sizeof(AnalisysObjData));

		m_pParent  = pParent;
		m_bRequest = TRUE;
		m_bRequestNext = FALSE;

		m_sObjtatus = sObjtatus;
		if(pInfo != NULL)
		{
			memcpy(&m_Info,pInfo,sizeof(CodeInfo));
		}
	}
	//���û���ͼԪָ��
	void SetPKLine(CDrawTechKLine* pPKLine) {m_pPKLine = pPKLine;}
	//�ж�״̬
	BOOL IsStatus(short	sObjtatus){return m_sObjtatus == sObjtatus;}
	//��������״̬
	void SetRequest(BOOL bRequest){m_bRequest = bRequest;}
	//����m_bRequest
	BOOL IsRequest(AnalisysObjData* pFirstData)
	{
		return m_bRequest;
	}
	//��������״̬
	void SetRequestNext(BOOL bRequestNext){m_bRequestNext = bRequestNext;}
	//����m_bRequestNext
	BOOL IsRequestNext()                  {return m_bRequestNext;}
	//���ù�Ʊ��Ϣ
	void SetInfo(const CodeInfo* pInfo)
	{
		if(pInfo != NULL)
		{
			memcpy(&m_Info,pInfo,sizeof(CodeInfo));
		}
		else
		{
			ZeroMemory(&m_Info, sizeof(CodeInfo));
		}
	}
	//����m_nLastTransDay
	int GetLastTransDay()
	{
		m_nLastTransDay;
	}
	//���K�����ݰ������ڿ�������
	void SizeData(AnalisysObjData* pFirstData);
	/********************************************************************************
	 * �������� : ��������
	 * �������� : StockDay* pKData - K������ָ�� ��NULL int nSize - ���ݸ���
	 *			  AnalisysObjData* pKData - �������ݵ�ָ�� ��NULL
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-5-7
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void CopyData(StockDay* pKData, int nSize);
	void CopyData(AnalisysObjData* pKData);
	//�ͷ��ڴ�
	void Free();
	//�ͷ��ڴ沢��ʼ��
	void SetNULL();
	/********************************************************************************
	 * �������� : ������������(����Сͼ), �������巢�������
	 * �������� : CodeInfo& info,short nTAnali,BOOL bNext,int nRequestCount - ��ʷ��������, δʹ��
	 *  ����ֵ  : void
	 * �������� : �û�����������ʱ��, ����; ����GetRequestData()ȡ�������
	 * ��    �� :����ʯ ��RequestData����
	 * ������� :2003-5-1
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	void RequestData(short nTAnali = -1,BOOL bNext = FALSE,int nRequestCount = -1);
	//���ع�Ʊ����
	CString GetName();
	/********************************************************************************
	 * �������� : ��ȡ����, ������Ȩ���ݵ�
	 * �������� : void* pAsk - ��������� ��NULL
	 *			  AnalisysObjData* pPreData - ǰһ��������ָ�� ��NULL
	 *  ����ֵ  : �ɹ�����TRUE, ʧ�ܷ���FALSE
	 * �������� : 
	 * ��    �� :����ʯ
	 * ������� :2003-5-7
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	BOOL    ReadData(void* pAsk,AnalisysObjData* pPreData);
	//�ж����������߻�������
	BOOL IsUp(int nPosition)
	{
		if( m_pData == NULL ||
			nPosition < 0   ||
			nPosition >= m_nValueNum)
			return FALSE;
		return (m_pData[nPosition].m_lOpenPrice < m_pData[nPosition].m_lClosePrice
			|| (m_pData[nPosition].m_lOpenPrice == m_pData[nPosition].m_lClosePrice
			   && m_pData[nPosition].m_lClosePrice >= m_pData[(nPosition - 1)>0?(nPosition - 1) : 0].m_lClosePrice));
	}
	//��ʱ���Ʊ�������
	void SaveCur();
	void LoadCur();
  void Append(StockDay &day);//ADD BY YUHJ 2008.6
	BOOL IsSave();
	//����ԭʼ������
	StockDay* GetSrcData();

	// ���ݵĵ���
	void Convert(int nPriceUnit);
};



/////////////////////////////////////////////////////////////////////////////
// CDoublePoint - A 2-D point, similar to Windows POINT structure.

class CDoublePoint;
class CDoubleRect;
//�߾��ȵ�ṹ
class CDoublePoint : public CObject
{
public:

// Constructors
	CDoublePoint() { x = 0 ; y = 0; }
	CDoublePoint(double initX, double initY)
	{
		x = initX;
		y = initY;
	}
	CDoublePoint(CDoublePoint& point)
	{
		x = point.x;
		y = point.y;
	}
// Operations
	//ƫ��
	void Offset(double xOffset, double yOffset) { x += xOffset; y += yOffset; }
	//������=����
	void operator=(const CDoublePoint& point)
	{
		x = point.x;
		y = point.y;
	}
	//������==����
	BOOL operator==(CDoublePoint point) const
	{
		return (x == point.x && y == point.y); 
	}
	//������!=����
	BOOL operator!=(CDoublePoint point) const
	{ 
		return (x != point.x || y != point.y); 
	}
	//������+=����
	void operator+=(CDoublePoint point)	{ x += point.x; y += point.y; }
	//������-=����
	void operator-=(CDoublePoint point) { x -= point.x; y -= point.y; }
	//������+����
	CDoublePoint operator+(CDoublePoint point) const
	{ 
		return CDoublePoint(x + point.x, y + point.y); 
	}
	//������-����
	CDoublePoint operator-(CDoublePoint point) const
	{ 
		return CDoublePoint(x - point.x, y - point.y); 
	}
	//������-����
	CDoublePoint operator-() const 	{ return CDoublePoint(-x, -y); }

public:
	double	x, y;
};

/////////////////////////////////////////////////////////////////////////////
// CDoubleRect - A 2-D rectangle, similar to Windows RECT structure.
//�߾��Ⱦ�������
class CDoubleRect : public tagRECT
{
public:

// Constructors
	CDoubleRect()
	{
		left = right = top = bottom = 0;
	}
	CDoubleRect(double l, double t, double r, double b)
	{ 
		left = l; top = t; right = r; bottom = b; 
	}
	CDoubleRect(const CDoubleRect& srcRect)	{ CopyRect(&srcRect); }
	CDoubleRect(CDoublePoint point, SIZE size)
	{ 
		right = (left = point.x) + size.cx; 
		bottom = (top = point.y) + size.cy; 
	}
	CDoubleRect(CDoublePoint topLeft, CDoublePoint bottomRight)
	{ 
		left = topLeft.x; 
		top = topLeft.y;
		right = bottomRight.x; 
		bottom = bottomRight.y; 
	}

// Attributes (in addition to RECT members)
	//���ؿ��,�߶�
	double Width() const { return right - left; }
	double Height() const{ return bottom - top; }
	//����(TOP,LEFT)
	CDoublePoint& TopLeft() { return *((CDoublePoint*)this); }
	//����(BOTTOM, RIGHT)
	CDoublePoint& BottomRight() { return *((CDoublePoint*)this+1); }
	//����(TOP,LEFT)
	const CDoublePoint& TopLeft() const { return *((CDoublePoint*)this); }
	//����(BOTTOM, RIGHT)
	const CDoublePoint& BottomRight() const { return *((CDoublePoint*)this+1); }
	//�����е�
	CDoublePoint CenterPoint() const { return CDoublePoint((left+right)/2, (top+bottom)/2); }
	//�ж��Ƿ�Ϊ��
	BOOL IsRectEmpty() const
	{
		return (left >= right || top >= bottom);
	}
	//�ж������Ƿ�Ϊ��
	BOOL IsRectNull() const
	{ 
		return (left == 0 && right == 0 && top == 0 && bottom == 0); 
	}
	//�жϵ��Ƿ���������
	BOOL PtInRect(CDoublePoint point) const
	{
		return (point.x >= left && point.x < right && point.y >= right && point.y < bottom);
	}

// Operations
	//��������
	void SetRect(double x1, double y1, double x2, double y2)
	{
		left = x1; top = y1;
		right = x2; bottom = y2;
	}
	//��������
	void SetRect(CDoublePoint topLeft, CDoublePoint bottomRight)
	{
		SetRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	}
	//�����ÿ�
	void SetRectEmpty()
	{
		left = top = right = bottom = 0;
	}
	//��������
	void CopyRect(const CDoubleRect* lpSrcRect)
	{
		memcpy(this, lpSrcRect, sizeof(CDoubleRect));
	}
	//�ж������Ƿ����
	BOOL EqualRect(CDoubleRect* lpRect) const
	{
		return memcmp(this, lpRect, sizeof(CDoubleRect)) == 0;
	}
	//Increases the width and height of CRect
	void InflateRect(double x, double y)
	{
		left -= x; right += x;
		top  -= y; bottom += y;
	}
	//Increases the width and height of CRect
	void InflateRect(SIZE size) { InflateRect((double)size.cx, (double)size.cy); }
	//Moves CRect by the specified offsets
	void OffsetRect(double x, double y)
	{
		left += x; right += x;
		top  += y; bottom += y;
	}
	//Moves CRect by the specified offsets
	void OffsetRect(SIZE size) { OffsetRect((double)size.cx, (double)size.cy); }
	//Moves CRect by the specified offsets
	void OffsetRect(CDoublePoint point) { OffsetRect(point.x, point.y); }
	//Standardizes the height and width of CRect
	void NormalizeRect();
	// Additional Operations
	operator RECT()
	{
		CRect rect;
		rect.left   = (int)left;
		rect.right  = (int)right;
		rect.top    = (int)top;
		rect.bottom = (int)bottom;
		return rect;
	}
	//Copies the dimensions of a rectangle to CRect
	void operator=(const RECT& srcRect)
	{
		left   = (double)srcRect.left;
		right  = (double)srcRect.right;
		top    = (double)srcRect.top;
		bottom = (double)srcRect.bottom;
	}
	//Copies the dimensions of a rectangle to CRect
	void operator=(const CDoubleRect& srcRect)
	{
		CopyRect(&srcRect);
	}
public:
	double left, top, right, bottom;
};

// �����������
struct InterzoneData
{
	int		m_nDateBegin;		// ��ʼ����
	int		m_nDateEnd;			// ��������
	int		m_nTimeBegin;		// ��ʼʱ��
	int		m_nTimeEnd;			// ����ʱ��
	
	CodeInfo	m_tagCode;		// ����
	int		m_nCount;		// ��������
	long	m_lOpenPrice;		//��
	time_t  m_tOpen;			

	long	m_lMaxPrice;		//��
	time_t  m_tMax;

	long	m_lMinPrice;		//��
	time_t  m_tMin;

	long	m_lClosePrice;		//��
	time_t  m_tClose;

	long	m_lPreClosePrice;		//����
	
	double		m_dbMoney;			//�ɽ����
	double		m_dbTotal;			//�ɽ���   ��λ����
	double		m_dbSumClose;		//���̼ۺ�

	HSDrawData	m_dbCAPITAL;		//��ͨ��
};
// Serialization
/********************************************************************************
* �������� : ����ʹ���ڻ����ṹ�Ĵ��л�ʵ��                                                                    	 
* �������� : CArchive& ar - ���л���         
*			  CDoublePoint point - �߾��ȵ�
*			CDoubleRect& rect - �߾�������
*  ����ֵ  :  CArchive&  - ���л���                                                                   	 
* �������� :                                                                     	 
* ��    �� :����ʯ                                                               	 
* ������� :2003-5-7                                                             	 
* �޸ļ�¼ :                                                                     	 
*******************************************************************************/ 	 
inline CArchive& AFXAPI operator<<(CArchive& ar, CDoublePoint point)
{ 
	ar.Write(&point, sizeof(CDoublePoint)); 
	return ar; 
}
inline CArchive& AFXAPI operator<<(CArchive& ar, const CDoubleRect& rect)
{ 
	ar.Write(&rect, sizeof(CDoubleRect)); 
	return ar; 
}
inline CArchive& AFXAPI operator>>(CArchive& ar, CDoublePoint& point)
{ 
	ar.Read(&point, sizeof(CDoublePoint)); 
	return ar; 
}
inline CArchive& AFXAPI operator>>(CArchive& ar, CDoubleRect& rect)
{ 
	ar.Read(&rect, sizeof(CDoubleRect)); 
	return ar; 
}


// �Ϸ�����,���ڽ����ⲿ���������
class CDragAndDropData
{
public:
	CutAtTimePageType m_lType;			 //ҳ������
	void*			  m_pIdentifier;	 //��ʶ,ָʾ����ͼָ��
	void*			  m_pData;			 //ָʾ����,���繫ʽ,��Ʊ������ָ��
	char			  m_cUserType;		 //�Զ�������,ö��
	char			  m_cAddKlineAndDel; //���Ӻ�ɾ��ԭ����ָ��

public:
	enum {none,MouseMove,DbClicked};

	CDragAndDropData(){memset(this,0,sizeof(CDragAndDropData));}
	CDragAndDropData(CutAtTimePageType lType,void* pIdentifier,void* pData)
	{
		m_lType = lType;
		m_pIdentifier = pIdentifier;
		m_pData = pData;
	}
	~CDragAndDropData(){}
	//������Ч��
	BOOL IsValidate()
	{
		return ( (m_pIdentifier != NULL) && (m_pData != NULL) && (m_lType != 0) );
	}
	//�Ƚϱ�ʶ
	BOOL Compare(void* pIdentifier)
	{
		if(!IsValidate())
			return FALSE;
		return (m_pIdentifier == pIdentifier);
	}
	//��������ָ��
	void* GetData() {return m_pData;}
	//���ر�ʶָ��
	void* GetIdentifier() {return m_pIdentifier;}
	//����ҳ������
	CutAtTimePageType GetType() {return m_lType;}

};

//K�����ݽ��սṹ
struct StockDay;
struct CStockDayDataSequence
{
	short m_nType;	//��������
	int	  m_nSize;	//���ݸ���
	int   m_nStart;	//������ʼλ��
	StockDay * m_pData;	//����ָ��

	CStockDayDataSequence()
	{
		memset(this,0,sizeof(CStockDayDataSequence));
	}
};


//////////////////////////////////////////////////////////////////////////////////
////// struct CTPoint
//�������͵�
template <class T> 
struct CTPoint
{
	T x;
	T y;
	DWORD  m_dwMarker;	// ��ǩ����,�ɶ�����ϣ��μ�CMarkerType��

	CTPoint()							   { m_dwMarker = 0; x = 0; y = 0; }
	CTPoint(T initX, T initY)			   { x = initX; y = initY; }
	void operator = (const CTPoint& pt)	   { x = pt.x;	y = pt.y;  }

	// ��ǩ����:���,ɾ��,�ж�
	void AddMarker(DWORD dwMarkerType)     { m_dwMarker |= dwMarkerType;    }	
	void RemoveMarker(DWORD dwMarkerType)  { m_dwMarker &= (~dwMarkerType); }
	BOOL IsMarker()						   { return m_dwMarker != 0; }
	BOOL IsMarker(DWORD dwMarkerType)      { return (m_dwMarker & dwMarkerType);  }

};
//�������͵��ı���,��Ҫ����ע�ͱ���
class CTechKLineImp;
template <class ToptextType>
struct CToptextType
{
	CString		 m_strName;		//�ı�����
	ToptextType  m_Value;		//ֵ
	COLORREF	 m_cl;			//�ı���ɫ
	short		 m_nDecimal;	//С��λ��
	BYTE		 m_bArrow;		//���Ƽ�ͷ����
	short		 m_nImageIndex;	//����imagelist
	CTechKLineImp*	m_pTechLine;

	CToptextType() {m_Value = 0;m_cl = 0;m_bArrow = 0;}
	CToptextType(CString strName, ToptextType Value, COLORREF cl, CTechKLineImp* pImp, short nDecimal = 0,BYTE bArrow = 0, short nImageIndex = -1)
	{
		m_strName  = strName;
		m_Value    = Value;
		m_cl       = cl;
		m_nDecimal = nDecimal;
		m_bArrow   = bArrow;
		m_nImageIndex = nImageIndex;
		m_pTechLine = pImp;
	}
	~CToptextType()
	{
	}
	//�жϻ��Ƽ�ͷ�����Ƿ�Ϊ��ͷ
	BOOL IsArrow() { return (BOOL)(m_bArrow&01); }
	//�жϻ��Ƽ�ͷ�����Ƿ�Ϊ����
	BOOL IsAddHig() { return (BOOL)(m_bArrow&02); }
};


// ������
struct DrawPointInfo
{
	float m_fCoordinate;	//��Ӧ����Ļ���ص�
	int	  m_lPoint;			//����������

	DrawPointInfo()
	{
		memset(this,0,sizeof(DrawPointInfo));
	}
	//������	
	void operator = (float fCoordinate)
	{
		m_fCoordinate = fCoordinate;
	}
	//������
	void operator = (int lPoint)
	{
		m_lPoint = lPoint;
	}
	//������
	operator float() { return m_fCoordinate; }
	//������
	operator int()	 { return (int)m_fCoordinate; }

};

//ʵʱ����-��ͼ
struct DrawFenShiData
{
	long   m_lValue;		//ֵ
	short  m_nMessageMask;	//��Ϣ����
	union
	{
		long  m_lTime;		//ʱ��
		long* m_pDataEx;	//��չ����ָ��
	};

	DrawFenShiData()
	{
		memset(this,0,sizeof(DrawFenShiData));
	}
	//��ֵ������
	void operator = (long lValue)
	{
		m_lValue = lValue;
	}
	void operator = (unsigned long lValue)
	{
		m_lValue = lValue;
	}
	//��ֵ������
	void operator = (int lPoint)
	{
		m_lValue = lPoint;
	}
	//ǿ������ת��������
	operator long()		   { return m_lValue; }
	operator int()		   { return (int)m_lValue; }
	operator double long() { return (double long)m_lValue; }
};

//��������
struct ZoomInAndOutData
{
	char  m_nType;			//��������,ö��ֵ
	float m_nCountPoint;	//��ǰ��Ļ�����ݵ�����	
	float m_nCurPoint;		//��ǰ�ο���
	ZoomInAndOutData* m_pPreZoom;//ָ��ǰһ����������ָ��
	//? ö�ٺ��壿��
	enum ZoomType {none,BigIn,BigOut,SmallIn,SmallOut,DelAll,ZoomOut = WM_USER,ZoomIn,MoreChartUnion};
	ZoomInAndOutData()
	{
		memset(this,0,sizeof(ZoomInAndOutData));
	}
	~ZoomInAndOutData()
	{
		if(m_pPreZoom != NULL)
		{
			delete m_pPreZoom;
			m_pPreZoom = NULL;
		}
	}
	ZoomInAndOutData(char nType,float nCountPoint,float nCurPoint)
	{
		m_nType		  = nType;
		m_nCountPoint = nCountPoint;
		m_nCurPoint   = nCurPoint;
		m_pPreZoom    = NULL;
	}
	//������ֵ
	void Set(char nType,float nCountPoint,float nCurPoint)
	{
		m_nType		  = nType;
		m_nCountPoint = nCountPoint;
		m_nCurPoint   = nCurPoint;
		//m_pPreZoom    = NULL;
	}
	//�ж�����
	BOOL IsType(char nType) { return (m_nType == nType); }
};

//�����Ϣ���ͽṹ
struct hxRefMouseMsg
{
	char m_cType;		//����

	enum Type 
	{
		none,
		RMove,RMoveDone,	// �ƶ�
		RSel,RSelDone,		// ѡ��

		StartPlanMove,		// ����ƽ��
		CancelPlanMove,		// ȡ��ƽ��
		DBClickPlanMove,	// ˫��ƽ��
		VerPlanMove,		// ����ƽ��
		RightHoriPlanMove,	// �Ҽ�����ƽ��
		RightHoriPlanMoveDone,	// �Ҽ�����ƽ�ƽ���
		RightVerPlanMove,	    // �Ҽ�����ƽ��

		RightZoomIn,		// �Ҽ��Ŵ�
		RightZoomRect,		// �õ��Ҽ��Ŵ�����
		ClickInExplainRect, // �����ͼ��������ʾ����
		ClickInExplainDownRect,
		NextRefresh,		// �´���Ҫˢ��
		LButtonMove,		// ��������ƶ�(�ڷ�ͼ������)
		RButtonMove,		    // �Ҽ������ƶ�(�ڷ�ͼ������)
		LButtonRightOperator    // ͼ��������
	};

	hxRefMouseMsg()
	{
		m_cType = -1;
	}
	hxRefMouseMsg(Type cType)
	{
		m_cType = (char)cType;
	}
	//�ж�����
	BOOL IsType(Type cType) { return (m_cType == cType); }

};

// ��ͼ����
struct CDrawParam
{
	short m_nValidCount;     // ��Ч��ͼ����
	short m_nSingleWidth;    // ������ͼ���
	CDrawParam()
	{
		memset(this,0,sizeof(CDrawParam));
	}
};

struct CLineProperty;
//��ǰѡ���ָ���߽ṹ
struct yls_CurSelLineData
{
	int				m_nSelPos;	//��ǰѡ��,ָʾ����ѡ�����ݵ��±�
	CLineProperty*  m_pData;	//ָ��
	yls_CurSelLineData()
	{
	}
	yls_CurSelLineData(int nSelPos,CLineProperty* pData)
	{
		m_nSelPos = nSelPos;
		m_pData   = pData;
	}
};

#define TimeCurrentData StockOtherData

//// �̸�ʽ��other����
//struct TimeCurrentData
//{
//   long  m_nTime;		// ����ʱ��
//   long  m_lCurrent;    // ��������
//   long	 m_lInside;		// ����
//   long	 m_lOutside;	// ����
//   TimeCurrentData(StockOtherData* pOtherData)
//   {
//	   m_nTime = pOtherData->m_nTime;
//	   m_lCurrent = pOtherData->m_lCurrent;
//	   m_lInside  =	pOtherData->m_lInside;
//	   m_lOutside =	pOtherData->m_lOutside;
//   };
//   TimeCurrentData()
//   {
//	   memset(this, 0, sizeof(TimeCurrentData));
//   };
//};
//���ݽ��սṹ
struct CMulQuantitySequence
{
	int  m_nSize;	//���ݴ�С
	int  m_nStart;	//��ʼ
	long m_lReference;//���߲ο�
	long m_lMax;		//���
	long m_lMin;		//��С
	int  m_nSeqs;		/*��ʾӦ�����ݸ���,
						0��ʾֻ��DrawFenShiData,ָ��m_ppValues[0],
						1��ʾm_ppValues[1]Ϊ��������ָ��,
						2��ʾm_ppValues[2]Ϊ��������ָ��*/
	long **m_ppValues;	//����ָ��
};
//�ּ�ͼ���ݽṹ
struct CFenJiaTuData
{
	long m_lPrice;		//�¼�
	long m_lBuyVol;		//����
	long m_lSellVol;	//����
	CFenJiaTuData()
	{
		m_lPrice    = 0;
		m_lBuyVol   = 0;
		m_lSellVol  = 0;
	};
	
};
//�ֱ�����
struct CStockTraceDataSequence
{
	int m_nSize;		//���ݸ���
	int m_nDataType;	//����
	StockTick  *m_pData;	//����ָ�� 
};
struct DrawFenShiData;
//��֤��֤�������ݽṹ
struct CMyBigMainIndexFigInfo
{
	long  m_lTotalVol;				//�ɽ�����
	long  m_lRefPrice;				//���߲ο��۸�
	int   m_nValueNum;				//���ݸ���
	DrawFenShiData* m_pPriceValues;	//��ʱ����ָ��
	long* m_pVolValues;				//�ɽ�������ָ��
	int   m_nPoints;				//����
};
//��֤��֤����ָ�����ݽṹ
struct CMyBigIndexFigInfo
{
	long  m_lTotalVol;				//�ɽ�����     
	long  m_lRefPrice;				//���߲ο��۸� 
	int   m_nValueNum;				//���ݸ���     
	DrawFenShiData* m_pPriceValues;	//��ʱ����ָ�� 
	int   m_nPoints;				//����
};
//�����ʾ��Ϣ���ݽṹ
struct MouseTipTextInfo
{
	DWORD	m_dwMask;	//����

	CString m_strTitle; //����
	CString m_strData;  //����

	CString m_strX;		//X����ʾ
	CString m_strY;		//Y����ʾ
	
	CPoint* m_pPoint;	//��ָ��
	MouseTipTextInfo()
	{
		m_dwMask = 0;
		m_pPoint = NULL;
	}
	//����ı�
	void EmptyTip()
	{
		m_strTitle = "";
		m_strData  = "";
	}
};
//�򵼶Ի���
class CExpression;
struct YlsWizardData
{
	CString		 m_pMainTech;	//��ָ���������
	CStringArray m_AySub;		//��ָ���������
	YlsWizardData(){}
	YlsWizardData( CExpression* pMainTech,CArray<CExpression*,CExpression*>* AySub );
};

//K�߲������ݽṹ
struct CKLineChartParam
{ 
	DWORD   m_dwStyle;	//���
	DWORD   m_dwType;	//����
	double  m_nValues;  // sum vol,close first ���ڻ��ͳɽ����йص���
	CPoint  m_pointFistpoint;	//���ڻ����̼��ߣ�������
	int		m_nPreUpDown;	//���ڼ�¼ǰ���������ǵ�����1Ϊ��һ�㣬1Ϊ���ǣ�0Ϊ�µ�
	int		m_nPreHigh;		//����ߵ�
	int		m_nPreLow;		//����͵�
	float   m_fChange;		//�¼��ߵ����õ�
	CKLineChartParam()
	{
		m_dwStyle = 0;
		m_dwType  = -1;
		m_nValues = 0;
		m_pointFistpoint.x = -1;
		m_pointFistpoint.y = -1;
		m_nPreUpDown = -1;
		m_nPreHigh   = 0;
		m_nPreLow    = 0;
		m_fChange    = (float)0.05;
	}
	//�ж�����
	BOOL IsType(DWORD dwType) { return (dwType == m_dwType); }
	
};
/*
�ṹ����
�ṹ����:StarFieldItem
��������:�ǿ�ͼһ�����ǵ����������
��	  ��:���ӱ�
��    ��:2003-08-12
*/

struct StarFieldItem
{	
	HSDouble		m_dbX;		// X������
	HSDouble		m_dbY;		// Y������
	
	int				m_nPos;		// ������X�������λ��

	char			m_cStatus;	// �ǵ���־
	short			m_nMask;	// ����, ��־�������,����ѡ��״̬��
    
	const StockUserInfo*		m_pStockInfo;	// ����ָ��,ָ���Ʊ����
public:	
	enum {Up, Down, Holding};	// �ǵ���־
	enum {Selected = 0x0100};	// ����ȡֵ 	
public:
	StarFieldItem()
	{
		memset(this, 0, sizeof(StarFieldItem));
	};
	int GetImage();
};
#endif // _YLSDRAWSTRUCT_H_