#pragma once

#include "..\..\HelperLib\GeneralHelper.h"
#include "HSBizDialog.h"
#include "ScrollNewsBar.h"
#include "ScrollFrame.h"
#include "HSMacro.h"

#include "DataSourceDefine.h"
#include "DataSourceSinkDefine.h"
#include "DataSourceDefineEx.h"
#include "hsstructnew.h"
#include "hsds_comudata.h"

#include "ConfigInterface.h"
#include "TradeComm\TradeCoreIntf.h"
#include ".\XInfoTip\XInfoTip.h"

//�Զ�����Ϣ
#define WM_SCROLL_MOVE      (WM_USER+100)
// CStatuBar �Ի���

//Ҫ��ʾ���ݵĽṹ��
struct strStruct
{
	char name[strStructLenth];      //��Ʊ����
	char nowPrice[strStructLenth];  //�ּ�
	char ricePrice[strStructLenth]; //�ǵ�
	char riceRatio[strStructLenth]; //�Ƿ�
	char totalMoney[strStructLenth];//�ܶ�
	short   trend;     //��־�ǵ� -1���� 0��ƽ 1:��

	strStruct()
	{
		name[0] = '\0';
		nowPrice[0] = '\0';
		ricePrice[0] = '\0';
		riceRatio[0] = '\0';
		totalMoney[0] = '\0';
		trend = -2;
	}

	~strStruct()
	{

	}

	strStruct& operator=(const strStruct &temp)
	{
		strncpy(name,      temp.name,      strStructLenth);
		strncpy(nowPrice,  temp.nowPrice,  strStructLenth);
		strncpy(ricePrice, temp.ricePrice, strStructLenth);
		strncpy(riceRatio, temp.riceRatio, strStructLenth);
		strncpy(totalMoney,temp.totalMoney,strStructLenth);
		trend = temp.trend;
		return *this;
	}

	BOOL operator==(const strStruct &temp)
	{
		if (memcmp(this, &temp, sizeof(strStruct)))
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL operator!=(const strStruct &temp)
	{
		if (memcmp(this, &temp, sizeof(strStruct)))
		{
			return TRUE;
		}
		return FALSE;
	}

};

//��¼���ֹ�Ʊ���ݵĽṹ��
struct StatuBarData
{
	StockUserInfo       m_stock;
	long				m_lOpen;         		// ����
	long				m_lMaxPrice;     		// ��߼�
	long				m_lMinPrice;     		// ��ͼ�
	long				m_lNewPrice;     		// ���¼�
	unsigned long		m_lTotal;				// �ɽ���(��λ:��)
	float				m_fAvgPrice;			// �ɽ����
	long		        m_lPreJieSuanPrice;     // ������ ����ڻ�
	void CopyFromRealTimeData(HSStockRealTime* pData)
	{
		this->m_lOpen            = pData->m_lOpen;
		this->m_lMaxPrice        = pData->m_lMaxPrice;
		this->m_lMinPrice        = pData->m_lMinPrice;
		this->m_lNewPrice        = pData->m_lNewPrice;
		this->m_lTotal           = pData->m_lTotal;
		this->m_fAvgPrice        = pData->m_fAvgPrice;
		this->m_lPreJieSuanPrice = 0;
	}

	void CopyFromRealTimeData(HSQHRealTime* pData)
	{
		this->m_lOpen			 = pData->m_lOpen;
		this->m_lMaxPrice		 = pData->m_lMaxPrice;
		this->m_lMinPrice		 = pData->m_lMinPrice;
		this->m_lNewPrice		 = pData->m_lNewPrice;
		this->m_lTotal			 = pData->m_lTotal;
		this->m_fAvgPrice		 = 0;
		this->m_lPreJieSuanPrice = pData->m_lPreJieSuanPrice;
	}

	void GetStockData(StockUserInfo* pData)
	{
		pData->Copy(&this->m_stock);
	}

	void GetRealTimeData(HSStockRealTime* pData)
	{
		pData->m_lOpen     = this->m_lOpen;
		pData->m_lMaxPrice = this->m_lMaxPrice;
		pData->m_lMinPrice = this->m_lMinPrice;
		pData->m_lNewPrice = this->m_lNewPrice;
		pData->m_lTotal    = this->m_lTotal;
		pData->m_fAvgPrice = this->m_fAvgPrice;

	}

	void GetRealTimeData(HSQHRealTime* pData)
	{
		pData->m_lOpen			  = this->m_lOpen;
		pData->m_lMaxPrice		  = this->m_lMaxPrice;
		pData->m_lMinPrice		  = this->m_lMinPrice;
		pData->m_lNewPrice		  = this->m_lNewPrice;
		pData->m_lTotal			  = this->m_lTotal;
		pData->m_lPreJieSuanPrice = this->m_lPreJieSuanPrice;

	}

	void SetEmpty()
	{
		m_stock.SetEmpty();
		this->m_lOpen            = 0;
		this->m_lMaxPrice        = 0;
		this->m_lMinPrice        = 0;
		this->m_lNewPrice        = 0;
		this->m_lTotal           = 0;
		this->m_fAvgPrice        = 0;
		this->m_lPreJieSuanPrice = 0;
	}
	
};



struct TIndexData
{
	short	m_nRiseCount;	//���Ǽ���
	short	m_nFallCount;	//�µ�����
	short	m_nLead;            //����ָ��
	short	m_nRiseTrend;       //��������
	short	m_nFallTrend;       //�µ�����
	short   m_nTotalStock;		//��Ʊ����
	short   m_nPreRiseTrend;	//��һ�ε���������ֵ
	short   m_nPreFallTrend;	//��һ�ε��µ�����ֵ
	short   m_nFlags[8];

	TIndexData()
	{
		m_nRiseCount = 0;
		m_nFallCount = 0;
		m_nLead      = 0;
		m_nRiseTrend = 0;
		m_nFallTrend = 0;
		m_nTotalStock   = 0;
		m_nPreRiseTrend = 0;
		m_nPreFallTrend = 0;

		for( int i = 0; i < 8;i++ )
		{
			m_nFlags[i] = 3;
		}
	}
};


class CStatuBar : public CHSBizDialog,
	public IDataSourceSink, //ʵ����������Ļص�����
	public IAccountChange   //ʵ���˺Ź������Ļص�����
{
	DECLARE_DYNAMIC(CStatuBar)

public:
	CStatuBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStatuBar();

// �Ի�������
	enum { IDD = IDD_STATUEBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
//��Ϣ����
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnInit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//���ܺ���
public:	

	BOOL ReadCfg();   //�������ļ����ȡ���� 
	BOOL ReadColor(); //����ɫ�����ļ��ж�ȡ���ǡ��µ���ƽ�̡���Ʊ�������Ƶ���ɫ
	BOOL ReadFont();  //�����������ļ��ж�ȡ����

	void WriteCfg();    //д�����ļ�

	//ͨ���˺Ź����� ��ȡ��ǰ�˺���Ϣ ��ת��Ϊ�ַ�����(���������� ���������� ����״̬)���˳����
	void GetAccountMng(); 
	//������������װ�����ַ�����Ϣ bSwitch=trueƥ���˺Ź�����;bSwitch=falseƥ��ͨ�ſ�
	void SwitchTypeToString(int nType, CString &strServer, BOOL bSwitch);  
	void AddAccountMng(IAccount* pAccount); //����˺���Ϣ

	//���Ʋ���
	void DrawIndexPanel(CDC* dc);  //����״̬��ָ��
	void DrawTimePanel(CDC* dc);   //����ʱ�� 
	void DrawProgress(CDC* dc);    //����ͨѶ״̬
	void DrawStatusName(CDC* dc);  //����״̬������

	void DrawRedGreenLine(CDC* dc);//���ƺ��̵�
	void DrawRedGreenFlag(CDC* pDC, CRect rc, short offset );//���ƺ��̵������ͼƬ
	short DrawRGRectOffset(short flags ); //����flagֵȷ��ƫ����
	short CalWidth(short total, short value,short width);

	void FillDockBar(CDC* dc, CRect& rc);
	void SetSize();
	
	void GetShowString();           //���Ҫ��ʾ������
	void ReplaceName(char* name, char* code);   //����ȯ��Ҫ����ʾ����
	void MoveScrollNewsBarWnd();    //��������Ƶ�λ��

	//��������ӿ�
	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);
	void RequestRealTimeData(BOOL bNeedAutoPush=TRUE);  //����ʵʱ����
	
	void GetServerTime();                               //���������ʱ��


	BOOL OnRecvData( void* pData, int nLen );           //��Ϣ������
	void HandleAutoPushData(DataHead* pHead, CommRealTimeData* pnowData, int nSize);
	void HandleNowData(DataHead* pHead, CommRealTimeData* pnowData, int nSize);
	
	short SetFlags( short lead, short risetrend,short falltrend, short prerise, short prefall);//��֤ ��֤���ݴ�����
	
	void UpdateDataEntry(  StatuBarData* pStatus, int index, int nType = MAKELONG(0,LTOA_MASK_DEFAULT) );
	void UpdateIndexData(  StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, HSStockRealTime* pData, int index, int nType = 0);
	void UpdateStockData(  StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, HSStockRealTime* pData, int index, int nType = 0);
	void UpdateFuturesData(StockUserInfo* pStock, long nUnit, short nDecimal, HSQHRealTime* pData,    int index, int nType = 0);
	void GetStockInfo();  //����codeInfo����stockUserInfo

	BOOL UpdateRealTimeData(CodeInfo* pCode,ShareRealTimeData* pStock); //�����ص����ݱ�������
	
	//�˺Ź���ص�����
	void OnAccountChange(IAccount* lpValue, EAccountChange nChangeType); //�˻������ı�
	void OnAccountStatus(IAccount* lpValue); //�˻�״̬�����ı�
	//��Ա����
private:
	//����Ʊ���
	CScrollNewsBar m_scrollNewsBarWnd;
	CRect m_rectScroll;       //����Ƶľ���

	//����ƿ�ܱ���
	CScrollFrame m_scrollFrame;

    CRect m_rectScrollFloat;  //����Ƹ����ľ���
	CRect m_rectIndex;        //ָ���ľ���
	CRect m_rectRedGreen;     //���̾��ľ���
	CRect m_rectTimePanel;    //ʱ��ľ���
	CRect m_rectProgress;     //ͨѶ״̬����
	CRect m_rectHomeUrl;      //״̬��������Ƶľ���
	long  m_lRed_Green_Panel;

	CImageList   m_lstCommStatusImage;   // ͨ��״̬ͼ��
	CBitmap      m_bmpRedFlag;           // ���̾�ͼƬ

	CString m_strTime;        //���ڵ�ʱ��
	long    m_lServerTime;    //������ʱ��
	DWORD   m_dwTick;         //�������֮���Tickֵ

//	CTimeSpan m_timeSpace;    //�������ʱ��ļ��
//	CTime     m_timeServer;   //������ʱ��

	CString   m_strStatusName;   //������״̬�������ʾ������ ��happy.ini��[�����]�¡������⡱

	int m_nIsUpDownRate;    // ����ǵ����Ƿ���ʾ
	int m_nIsTotal;         // ����ܶ��Ƿ���ʾ
	int m_nIsShowNewPirce;  // ��ǳɽ����Ƿ���ʾ
	int m_nIsUpDown;        // ����ǵ��Ƿ���ʾ

	BOOL m_threeShow; //��ǵ������Ƿ���ʾ
	BOOL m_fourShow;  //��ǵ������Ƿ���ʾ
	unsigned int m_scrollSpeed; //��¼�������ʱ��
	//�������ͱ���
	CArray<StatuBarData> m_oneList;  //��һ�й�Ʊ
	CArray<StatuBarData> m_twoList;  //�ڶ��й�Ʊ
	CArray<StatuBarData> m_threeList;//�����й�Ʊ
	CArray<StatuBarData> m_fourList; //�����й�Ʊ

	CStringArray m_newNameList;   //��¼����ȯ��Ҫ�� ��ʾ�Ĺ�Ʊ����

	unsigned int m_nShowIndex;    //��ǵ�ǰ������ʾ���к�  
	int          m_nIndexOff[INDEX_MAX_COUNT];	 //ָ������������λ��
	
	// �ӿ�ָ��
	IBlockManager*  m_pBlockManager;    // ������ָ��
	IDataSourceEx*	m_pDataSource;		// ��������ָ��
	long			m_nDataID;			// ��������ID

	IHsScrollNews*    m_pScrollCfg;   // ����������ļ���д�ӿ�
	IHsCommunication* m_pCommConf;    // ͨѶ�����ļ���д�ӿ�
	ITradeCoreObj*	  m_pTradeCore;   // ��ȡ���׽ӿڶ���
	IHsColor*         m_pColorCfg;    // ��ɫ��д�ӿ�
	IHsFont*          m_pFontCfg;     // �����д�ӿ�

	TIndexData    m_TSHIndex; // ��֤����
	TIndexData    m_TSZIndex; // ��֤����

   	//��ʱ���¼����
	int m_nTimeIndexID;   // ����ʱ���¼�  
	int m_nScrollIndexID; // ������ʾ�¼�

	CXInfoTip  m_tip;
	CStringArray m_ayConnectNotify; //ͨѶ��Ϣ ����(���������� ���������� ����״̬)���˳����
//	CRITICAL_SECTION m_lock; //��ͨ����Ϣ���м��� ��ֹ���ֶ�д��ͻ

public:
	strStruct m_indexString[INDEX_MAX_COUNT];//��Ҫ��ʾ���ַ�

	COLORREF         m_clrUp;       // ������ɫ
	COLORREF         m_clrDown;     // �µ���ɫ
	COLORREF         m_clrEqual;    // ƽ����ɫ
	COLORREF         m_clrCodeName; // ����������ɫ
	CFont*           m_pStatusFont; // ״̬��ָ������
	//static CString   m_HomeUrlName;

};

