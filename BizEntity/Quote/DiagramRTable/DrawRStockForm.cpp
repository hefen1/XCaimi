#include "StdAfx.h"
#include "resource.h"
#include "DrawRStockForm.h"
#include <atlcore.h>
#include "GeneralHelper.h"
#include "HSCalculate.h"
#include "..\..\..\ServiceEntity\HSFormulaManDll\FormulaCalc\ExpPublic.h"
#include "HSMessage.h"
#include "HSWinner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �Ҽ��˵�ID
#define ID_QUICKTRADE_BUY   0x0001
#define ID_QUICKTRADE_SELL  0x0002

CDrawRStockForm::CDrawRStockForm(IDataSourceEx* pDataSource, BOOL bLevel)
:CDrawTableBase()
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_HEIGHT_CHANGABLE | TABLE_WIDTH_CHANGABLE);
	m_preCodeInfo.SetEmpty();	
	m_sLevelData.m_ciStockCode.SetEmpty();
	m_pDataSource = pDataSource;
	m_bLevel = bLevel;
	m_nNewPrice = 0;
	//m_bLevel = TRUE;
}

CDrawRStockForm::~CDrawRStockForm()
{
	
}

BOOL CDrawRStockForm::CreateSelf()
{
	WORD nMarket = MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
	if (nMarket == STOCK_MARKET)
	{// ��Ʊ�г�
		WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
		switch (nSubMarket)
		{
		case KIND_BOND:
			return CreateBonds();
		case KIND_FUND:
		case KIND_LOF:
			return CreateFund();
		case KIND_ETF:
			return CreateEFT();
		case KIND_INDEX:
			return CreateIndex();
		default:
			return CreateStock();
		}
	}
	else if (nMarket == FUTURES_MARKET)
	{// �ڻ�
		return CreateFuture();
	}
	return FALSE;
}

BOOL CDrawRStockForm::CreateBonds() // ����ծȯ�嵵
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  ���⿪ʼ  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// ��5~��2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //�ɽ�
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //�ǵ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //�ܶ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lCellStyle = CELL_BORDER_LEFT;
	::LoadString(instance, IDS_STRING_LIXI, buf, nBufLenth);       //��Ϣ
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 Ҫ��ʾ�ܱ�
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth);  // �ܱ�
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //��ͣ
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //��ӯ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //��ͨ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("��һ�� %d \n", m_ayCell.GetSize());
	////////////////////////////////////// �ڶ���  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //ί��

	if (!m_bLevel)
	{// level1��ʾ�嵵����
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//��5~��2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // �ɽ���

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // �ǵ�

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // ����

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // ����

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // �ܶ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ��Ϣ

	if (m_bLevel)
	{// level2 ��ʾ�ܱ�                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // �ܱ�
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // ��ӯ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // ��ͨ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ��ֵ
	//		TRACE1("�ڶ��� %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵���鱨��
		lItemStyle = ITEM_TEXT_RIGHT;   
		for (int i = 0; i < 4; i++)
		{	// ��5~��2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // ��1~��4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_HUANSHOU, buf, nBufLenth);    // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_QUANJIA, buf, nBufLenth);     // ȫ��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 ��ʾÿ��
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // ÿ��
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // ��ͣ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // �о�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // �ܹ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // ��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("������ %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // ί��

	if (!m_bLevel)
	{// level1 ��ʾί�����仯
		//ί�����仯
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// ��5~��2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_HUANSHOU, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ����

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // ȫ��

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // ÿ�� 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �о�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �ܹ�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // ��ֵ
	//		TRACE1("������ %d \n", m_ayCell.GetSize());

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixRowCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateStock() // ������Ʊ�嵵
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  ���⿪ʼ  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// ��5~��2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //�ɽ�
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //�ǵ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //�ܶ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, nBufLenth);     //����
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 Ҫ��ʾ�ܱ�
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth); // �ܱ�
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //��ͣ
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //��ӯ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //��ͨ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("��һ�� %d \n", m_ayCell.GetSize());
	////////////////////////////////////// �ڶ���  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //ί��

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//��5~��2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // �ɽ���

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // �ǵ�

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // ����

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // ����

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // �ܶ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ����

	if (m_bLevel)
	{// level2 ��ʾ�ܱ�                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // �ܱ�
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // ��ӯ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // ��ͨ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ��ֵ
	//		TRACE1("�ڶ��� %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵���鱨��
		lItemStyle = ITEM_TEXT_RIGHT ;   
		for (int i = 0; i < 4; i++)
		{	// ��5~��2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // ��1~��4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_HUANSHOU, buf, nBufLenth);    // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_NEIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 ��ʾÿ��
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // ÿ��
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // ��ͣ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // �о�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // �ܹ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // ��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("������ %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // ί��

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		//ί�����仯
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// ��5~��2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_HUANSHOU, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ����

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // ����

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // ÿ�� 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �о�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �ܹ�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // ��ֵ
	//		TRACE1("������ %d \n", m_ayCell.GetSize());

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}


BOOL CDrawRStockForm::CreateFund()  // ���������嵵
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  ���⿪ʼ  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// ��5~��2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //�ɽ�
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //�ǵ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //�ܶ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, nBufLenth);     //����
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 Ҫ��ʾ�ܱ�
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth); // �ܱ�
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //��ͣ
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //��ӯ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //��ͨ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("��һ�� %d \n", m_ayCell.GetSize());
	////////////////////////////////////// �ڶ���  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //ί��

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//��5~��2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // �ɽ���

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // �ǵ�

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // ����

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // ����

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // �ܶ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ����

	if (m_bLevel)
	{// level2 ��ʾ�ܱ�                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // �ܱ�
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // ��ӯ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // ��ͨ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ��ֵ
	//		TRACE1("�ڶ��� %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_RIGHT ;   
		for (int i = 0; i < 4; i++)
		{	// ��5~��2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // ��1~��4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}
	
	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_JINGZHI, buf, nBufLenth);    // ��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_NEIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 ��ʾÿ��
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // ÿ��
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // ��ͣ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // �о�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // �ܹ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // ��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("������ %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // ί��

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		//ί�����仯
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// ��5~��2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ��ֵ

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // ����

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // ÿ�� 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �о�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �ܹ�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // ��ֵ
	//		TRACE1("������ %d \n", m_ayCell.GetSize());

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateEFT() // ����ETF�嵵
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	////////////////////////////  ���⿪ʼ  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);    //ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_SELL, buf, nBufLenth);
		strTitle = buf;					
		for (int i = 4; i >= 1; i--)
		{
			// ��5~��2
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_BUY, buf, nBufLenth);
		strTitle = buf;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			::LoadString(instance, IDS_STRING_NUM1+i, buf, nBufLenth);
			strName = buf;
			CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_LEFT;
		lCellStyle = CELL_BORDER_BOTTOM;
		::LoadString(instance, IDS_STRING_NUM1+4, buf, nBufLenth);
		strName = buf;
		CreateCell(strTitle+strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT ;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);  //�ɽ�
	strName = buf;  
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);   //�ǵ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_FUDU, buf, nBufLenth);       //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_XIANSHOU, buf, nBufLenth);   //����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZONGE, buf, nBufLenth);      //�ܶ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, nBufLenth);     //����
	strName = buf;                    
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (m_bLevel)
	{// level2 Ҫ��ʾ�ܱ�
		lCellStyle = CELL_BORDER_LEFT;
		::LoadString(instance, IDS_STRING_ZONGBI, buf, nBufLenth); // �ܱ�
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, nBufLenth);  //��ͣ
	strName = buf;                            
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIYING, buf, nBufLenth);    //��ӯ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIUTONG, buf, nBufLenth);    //��ͨ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_SHIZHI, buf, nBufLenth);     //��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//		TRACE1("��һ�� %d \n", m_ayCell.GetSize());
	////////////////////////////////////// �ڶ���  ////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("",  0, TEXT_FONT_DYNAMICAL, lItemStyle,lCellStyle);                  //ί��

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			//��5~��2				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4				
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;                                   
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                     // �ɽ���

	lItemStyle = ITEM_TEXT_RIGHT ;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // �ǵ�

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	                   // ����

	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // ����

	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // �ܶ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ����

	if (m_bLevel)
	{// level2 ��ʾ�ܱ�                                           
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_ZONGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);         // �ܱ�
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  	           // ��ӯ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);	               // ��ͨ

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                 // ��ֵ
	//		TRACE1("�ڶ��� %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);       // ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		lItemStyle = ITEM_TEXT_RIGHT ;   
		for (int i = 0; i < 4; i++)
		{	// ��5~��2
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}  
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT ;
		for (int i = 0; i < 4; i++)
		{   // ��1~��4
			CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_LEFT;  
	::LoadString(instance, IDS_STRING_JUNJIA, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);      // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);       // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_LIANGBI, buf, nBufLenth);     // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_IOPV, buf, nBufLenth);        // LOPV
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);	

	lItemStyle = ITEM_TEXT_LEFT;                                
	::LoadString(instance, IDS_STRING_NEIPAN, buf, nBufLenth);      // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	if (m_bLevel)
	{// level2 ��ʾÿ��
		lItemStyle = ITEM_TEXT_LEFT;
		::LoadString(instance, IDS_STRING_MEIBI, buf, nBufLenth);   // ÿ��
		strName = buf;
		CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, lItemStyle, 0);
	}

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_DIETING, buf, nBufLenth);     // ��ͣ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHIJING, buf, nBufLenth);     // �о�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGGU, buf, nBufLenth);      // �ܹ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	::LoadString(instance, IDS_STRING_ZONGZHI, buf, nBufLenth);     // ��ֵ
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);
	//		TRACE1("������ %d \n", m_ayCell.GetSize());
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // ί��

	if (!m_bLevel)
	{// level1 ��ʾ�嵵����
		//ί�����仯
		lItemStyle = ITEM_TEXT_RIGHT;  
		for (int i = 0; i < 4; i++)
		{
			// ��5~��2
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��1
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);

		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < 4; i++)
		{
			// ��1~��4
			CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0); 
		}
		// ��5
		lItemStyle = ITEM_TEXT_RIGHT;
		lCellStyle = CELL_BORDER_BOTTOM ;
		CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT ;               
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ����

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);                // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0); // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ��ֵ

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);          // ����

	if (m_bLevel)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		CreateCell("", TEXT_COLOR_MEIBI, TEXT_FONT_DYNAMICAL, lItemStyle, 0);     // ÿ�� 
	}

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;               
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle); // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;               
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �о�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // �ܹ�

	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);            // ��ֵ
	//		TRACE1("������ %d \n", m_ayCell.GetSize());

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateIndex()   // ����ָ��
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	//////////////////////////////////////// ��һ�п�ʼ //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_CHENGJIAO, buf, nBufLenth);        // �ɽ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, nBufLenth);         // �ǵ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZHANGFU, buf, nBufLenth);          // �Ƿ�
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, nBufLenth);         // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_JINE, buf, nBufLenth);             // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEIBI, buf, nBufLenth);            // ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_BUYSHOUSHU, buf, nBufLenth);       // ί������
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_SELLSHSOUSHU, buf, nBufLenth);     // ί������
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_SHANGZHANGJIASHU, buf, nBufLenth); // ���Ǽ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_PINGPANJIASHU, buf, nBufLenth);    // ƽ�̼���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_XIADIEJIASHU, buf, nBufLenth);     // �µ�����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	//////////////////////////////////////// �ڶ��п�ʼ //////////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // �ɽ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // �ǵ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // �Ƿ�

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);   // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);      // ί��

	CreateCell("");                                                      // ί������

	CreateCell("", 0, 0, 0, CELL_BORDER_BOTTOM);                         // ί������

	CreateCell("");                                                      // ���Ǽ���

	CreateCell("");                                                      // ƽ�̼���

	CreateCell("");                                                      // �µ�����

	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////	
	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUOSHOU, buf, nBufLenth);          // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_KAIPAN, buf, nBufLenth);           // ����
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUIGAO, buf, nBufLenth);          // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	::LoadString(instance, IDS_STRING_ZUIDI, buf, nBufLenth);           // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, 0);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_ZHENFU, buf, nBufLenth);          // ���
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_WEICHA, buf, nBufLenth);          // ί��
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);
	
	CreateCell("");                                                      // ί������

	CreateCell("", 0, 0, 0, CELL_BORDER_BOTTOM);                         // ί������

	CreateCell("");                                                      // ���Ǽ���

	CreateCell("");                                                      // ƽ�̼���

	CreateCell("");                                                      // �µ�����
	
	//////////////////////////////////////// �����п�ʼ //////////////////////////////////////////	
	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, 0);               // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);      // ί��

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);   // ί������

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);  // ί������

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, 0);  // ���Ǽ���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, lItemStyle, 0);// ƽ�̼���

	lItemStyle = ITEM_TEXT_RIGHT;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, 0);// �µ�����

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::CreateFuture() // �����ڻ�
{
	CString strTitle = "";
	CString strName = "";
	DWORD lItemStyle(0); // ��ԪС���ʽ
	DWORD lCellStyle(0); // ��Ԫ���ʽ

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	//

	//////////////////////////////////////////////////////////////////////////
	// ��һ��
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_SELLPRICE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_BUYPRICE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ���

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_NEW, buf, 1000);                    
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZHANGDIE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // �ǵ�

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZHANGFU, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // �Ƿ�

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZONGSHOU, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_XIANSHOU, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WAIPAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZHANGTING, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ��ͣ

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_JIESUAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZONGCHI, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // �ܳ�

	//////////////////////////////////////////////////////////////////////////
	// �ڶ���
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // ����

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // ���

	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // �ǵ�

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // �Ƿ�

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);       // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);              // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);              // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                          // �ܳ�

	//////////////////////////////////////////////////////////////////////////
	// ������
	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_SELLCOUNT, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = CELL_BORDER_BOTTOM;
	::LoadString(instance, IDS_STRING_BUYCOUNT, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_JUNJIA, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_KAIPAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZUIGAO, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ���

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_ZUIDI, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ���

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_WEIBI, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ί��

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_NEIPAN, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_DIETING, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ��ͣ

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_QIANJIE, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ǰ��

	lItemStyle = ITEM_TEXT_LEFT;
	lCellStyle = 0;
	::LoadString(instance, IDS_STRING_RIZENG, buf, 1000);
	strName = buf;
	CreateCell(strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyle, lCellStyle);      // ����

	//////////////////////////////////////////////////////////////////////////
	// ������
	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_SELLCOUNT, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);        // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = CELL_BORDER_BOTTOM;
	CreateCell("", TEXT_COLOR_BUYCOUNT, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);         // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ���

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ί��

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);             // ����

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);             // ��ͣ

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ǰ��

	lItemStyle = ITEM_TEXT_RIGHT;
	lCellStyle = 0;
	CreateCell("", 0, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);                           // ����

	// ��ʼ���������
	SetColCount(4);
	SetRowCount(m_ayCell.GetCount() / 4);
	SetFixColCount(0);
	SetFixColCount(0);

	delete[] buf;
	return TRUE;
}

BOOL CDrawRStockForm::GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}
	if (pStockInfo->m_ciStockCode.m_cCodeType != m_preCodeInfo.m_cCodeType)
	{
		// ��ͬ�г���Ҫ�������ɵ�Ԫ��
		RemoveAllCell();
		CreateSelf();
		CalCulateSize();
	}
	m_preCodeInfo = pStockInfo->m_ciStockCode;

	//////////////////////////////////////////////////////////////////////////
	// ��һЩ�����ָ���ʼֵ
	m_sellMap.RemoveAll();
	m_buyMap.RemoveAll();
	m_nNewPrice = 0;
	memset(&m_sRealTime, 0, sizeof(HSStockRealTime));
	//////////////////////////////////////////////////////////////////////////
	WORD wMask = m_bLevel?RT_LEVEL_REALTIME:RT_REALTIME_EXT;
	
	int lAskLen = sizeof(AskData);
	char* pData = new char[lAskLen]; 
	AskData*  pAskData = (AskData*)pData;
	memset(pAskData, 0, lAskLen);
	pAskData->m_nType  = wMask;
	if (MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == KIND_INDEX)
	{// �����ָ�� ��������ָ��
		pAskData->m_nType = RT_LEAD;
	}
	pAskData->m_nSize  = 1;
	memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)pAskData,lAskLen);
	
	wMask = m_bLevel?RT_LEVEL_AUTOPUSH:RT_AUTOPUSH_EXT;
	pAskData->m_nType  = wMask;
	if (MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == KIND_INDEX)
	{// �����ָ�� ��������ָ��
		pAskData->m_nType = RT_AUTOPUSH;
	}
	pReqCollector->AddRequest((char*)pAskData,lAskLen);
	delete[] pData;

	return TRUE;
}
int	CDrawRStockForm::OnUpdateData(DataHead* pData)
{
	
	//////////////////////////////////////////////////////////////////////////
	// ָ��
	if (pData->m_nType == RT_LEAD)
	{
		AnsLeadData* pRealData = (AnsLeadData*)pData;
		UpdateIndexData(&pRealData->m_indData);
		InValidate(NULL, FALSE);
		return 1;
	}
	else if (pData->m_nType == RT_AUTOPUSH)
	{
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if (pEveryData->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CommRealTimeData* pRealData = (CommRealTimeData*)pEveryData->m_pstData;
			if (pRealData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode))
			{
				UpdateIndexData(&pRealData->GetShareData()->m_indData);
			}
			return 1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Level1
	if (!m_bLevel)
	{
		if( pData->m_nType == RT_REALTIME_EXT)
		{// level1��չ����
			AnsRealTime*  pNowData  = (AnsRealTime*)pData;
			if(pNowData->m_nSize <= 0 )
			{
				return -1;
			}
			else
			{
				CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pNowData->m_pnowData;
				HandleNowData( (DataHead*)pData, pRealData, pNowData->m_nSize);
				return 1;
			}
		}
		else if (pData->m_nType == RT_AUTOPUSH_EXT)
		{// level1��������
			AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
			if (pEveryData->m_nSize <= 0)
			{
				return -1;
			}
			else
			{
				CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pEveryData->m_pstData;
				HandleNowData( (DataHead*)pData, pRealData, pEveryData->m_nSize);
				return 1;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Level2
	else
	{
		if (pData->m_nType == RT_LEVEL_AUTOPUSH || pData->m_nType == RT_LEVEL_REALTIME)
		{
			// level2��չ���� �� ���ư�
			AnsHSAutoPushLevel* pLevelAuto = (AnsHSAutoPushLevel*)pData;
			if (pLevelAuto->m_nSize <= 0)
			{
				return -1;
			}
			else
			{
				if (memcmp(&m_sLevelData, pLevelAuto->m_pstData, sizeof(RealTimeDataLevel)) == 0)
					return 0;
				memcpy(&m_sLevelData, pLevelAuto->m_pstData, sizeof(RealTimeDataLevel));
				
				CodeInfo* pCodeInfo = &(pLevelAuto->m_pstData->m_ciStockCode);
				if (pCodeInfo->CompareCode(&m_preCodeInfo))
				{
					LevelRealTime* pLevelReal = &pLevelAuto->m_pstData->m_sLevelRealTime;
					LevelStockOtherData* pOtherData = &pLevelAuto->m_pstData->m_othData;

					WORD nMarket = MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
					if (nMarket == STOCK_MARKET)
					{// ��Ʊ�г�
						WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
						if (nSubMarket == KIND_BOND)
						{// ծȯ
							UpdateBondsData(pLevelReal, pOtherData);
						}
						else if (nSubMarket == KIND_FUND || nSubMarket == KIND_LOF)
						{// ���� LOF
							UpdateFundData(pLevelReal, pOtherData);
						}
						else if (nSubMarket == KIND_ETF)
						{// ETF
							UpdateETFData(pLevelReal, pOtherData);
						}
						else if (MakeIndexMarket(m_stkInfo.m_ciStockCode.m_cCodeType))
						{// ָ��		

						}
						else
						{// ��Ʊ
							UpdateStockData(pLevelReal, pOtherData);
						}
					}
					else if (nMarket == FUTURES_MARKET)
					{// �ڻ��г�


					}
					InValidate(NULL, FALSE);
					return 1;
				}
			}
		}
	}
	return 0;
}

void CDrawRStockForm::HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		
		if (pData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode))
		{
			WORD nMarket = MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
			if (nMarket == STOCK_MARKET)
			{// ��Ʊ�г�
				WORD nSubMarket = MakeSubMarket(m_stkInfo.m_ciStockCode.m_cCodeType);
				if (nSubMarket == KIND_BOND)
				{// ծȯ
					if(UpdateBondsData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
				else if (nSubMarket == KIND_FUND || nSubMarket == KIND_LOF)
				{// ���� LOF
					if(UpdateFundData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
				else if (nSubMarket == KIND_ETF)
				{// ETF
					if(UpdateETFData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
				else if (MakeIndexMarket(m_stkInfo.m_ciStockCode.m_cCodeType))
				{// ָ��		
				}
				else
				{// ��Ʊ
					if(UpdateStockData(pData->GetShareData(), &pData->m_othData) == FALSE)
					{
						pData = pNextData;
						continue;
					}
					else
						break;
				}
			}
		}
		pData = pNextData;		
	}
	InValidate(NULL, FALSE);
}

BOOL  CDrawRStockForm::UpdateBondsData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// ������ͬ�����ư�
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum; 
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
		pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// ��ʼ���۸�map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);

	double dValue = 0.0;
	
	//�ӵ�2����ʼ
	// ί��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	if (!m_bLevel)
	{
		//��5~��1 ��
		for (int i = 4; i >= 0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//��1~��5 ��
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}

	//�ɽ�
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo,pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��Ϣ
	dValue = pRealTime->m_lNationalDebtRatio;
	CGeneralHelper::LongToString(dValue, 0, 1000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice < 0 ? 0:m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	////////////////////////// ������ ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//��5 ~ ��1��
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// ��1 ~ ��5��
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//��5 ~ ��1 ���仯
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//��1 ~ ��5 ���仯
	for (int i = 0; i < 5; i++)
		pos.y++;

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	pos.y++;
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	pos.y++;
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	SetCellText(pos, 0, strText);

	//����
	pos.y++;
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//ȫ��
	pos.y++;
	dValue = CHSCalculate::GetQuanjia(&m_stkInfo, pRealTime, strText);
	CGeneralHelper::LongToString(dValue, 0, nUnit, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice < 0 ? 0:m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	pos.y++;
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//�ܹ�
	pos.y++;
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//��ֵ
	pos.y++;
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateBondsData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)  // ����ծȯ����
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum;
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//�ӵ�2����ʼ
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//�ɽ�
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��Ϣ
	dValue = pLevelData->m_IPOV;
	CGeneralHelper::LongToString(dValue, 0, 1000, 3, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܱ�
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice < 0 ? 0:m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pLevelData, 1, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//ȫ��
	dValue = CHSCalculate::GetQuanjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//ÿ��
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice < 0 ? 0:m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܹ�
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateStockData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// ������ͬ�����ư�
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum;
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
	    pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// ��ʼ���۸�map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);

	double dValue = 0.0;
	//�ӵ�2����ʼ
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);
	
	if (!m_bLevel)
	{
		//��5~��1 ��
		for (int i = 4; i >=0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//��1~��5 ��
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}

	//�ɽ�
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	pos.y++;
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	pos.y++;
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	SetCellText(pos, 0, strText);

	//����
	pos.y++;
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	pos.y++;
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//��ͨ
	pos.y++;
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//��ֵ
	pos.y++;
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	////////////////////////// ������ ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//��5 ~ ��1��
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// ��1 ~ ��5��
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//��5 ~ ��1 ���仯
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//��1 ~ ��5 ���仯
	for (int i = 0; i < 5; i++)
		pos.y++;

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	pos.y++;
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	pos.y++;
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	SetCellText(pos, 0, strText);
	
	//����
	pos.y++;
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	pos.y++;
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//�ܹ�
	pos.y++;
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	SetCellText(pos, 0, strText);

	//��ֵ
	pos.y++;
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateStockData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData) // ���¹�Ʊ����
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum;
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//�ӵ�2����ʼ
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//�ɽ�
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܱ�
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pLevelData, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetHuanshou(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//ÿ��
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܹ�
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateFundData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// ������ͬ�����ư�
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum;  // ����һ��100��
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
		pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// ��ʼ���۸�map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);

	double dValue = 0.0;
	//�ӵ�2����ʼ
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	if (!m_bLevel)
	{
		//��5~��1 ��
		for (int i = 4; i >=0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//��1~��5 ��
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}

	//�ɽ�
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	////////////////////////// ������ ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//��5 ~ ��1��
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// ��1 ~ ��5��
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//��5 ~ ��1 ���仯
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//��1 ~ ��5 ���仯
	for (int i = 0; i < 5; i++)
		pos.y++;

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetJingZhi(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);


	//�о�
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܹ�
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateFundData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum;  // ����һ��100��
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;
	
	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//�ӵ�2����ʼ
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);
	
	//�ɽ�
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);
	
	//�ܱ�
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pLevelData, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetJingZhi(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//ÿ��
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܹ�
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;

}

BOOL CDrawRStockForm::UpdateETFData(ShareRealTimeData_Ext* pStock, StockOtherData* pOther)
{
	if ( pStock == NULL || ::IsBadReadPtr(pStock, 1) || pOther == NULL || ::IsBadReadPtr(pOther, 1))
	{
		return FALSE;
	}

	HSStockRealTime_Ext* pRealTimeExt = &pStock->m_nowDataExt;
	if (pRealTimeExt == NULL || ::IsBadReadPtr(pRealTimeExt,1))
	{
		return FALSE;
	}

	HSStockRealTime* pRealTime = &pRealTimeExt->m_stockRealTime;
	if (pRealTime == NULL || ::IsBadReadPtr(pRealTime,1))
	{
		return FALSE;
	}

	if ( memcmp(pRealTime, &m_sRealTime, sizeof(HSStockRealTime)) == 0)
	{// ������ͬ�����ư�
		return FALSE;
	}
	memcpy(&m_sRealTime, pRealTime, sizeof(HSStockRealTime));

	int nHand = m_stkInfo.m_nHandNum; // EFT 1�� = 100һ��
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pRealTimeExt->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	long BuyCount[] = {pRealTime->m_lBuyCount1, pRealTime->m_lBuyCount2, pRealTime->m_lBuyCount3,
		pRealTime->m_lBuyCount4, pRealTime->m_lBuyCount5};
	long SellCount[] = {pRealTime->m_lSellCount1, pRealTime->m_lSellCount2, pRealTime->m_lSellCount3,
		pRealTime->m_lSellCount4, pRealTime->m_lSellCount5};
	long BuyPrice[] = {pRealTime->m_lBuyPrice1, pRealTime->m_lBuyPrice2, pRealTime->m_lBuyPrice3,
		pRealTime->m_lBuyPrice4, pRealTime->m_lBuyPrice5};
	long SellPrice[] = {pRealTime->m_lSellPrice1, pRealTime->m_lSellPrice2, pRealTime->m_lSellPrice3,
		pRealTime->m_lSellPrice4, pRealTime->m_lSellPrice5};
	// ��ʼ���۸�map
	InitPriceMap(m_buyMap,  BuyPrice);
	InitPriceMap(m_sellMap, SellPrice);
	
	//�ӵ�2����ʼ
	double dValue = 0.0;
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��	
	dValue = CHSCalculate::GetWeiBi(pRealTime, strText);
	pos.x = 1;
	pos.y = 0; 
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	if (!m_bLevel)
	{
		//��5~��1 ��
		for (int i = 4; i >=0; i--)
		{
			dValue = SellPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}

		//��1~��5 ��
		for (int i = 0; i < 5; i++)
		{
			dValue = BuyPrice[i];
			CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
			pos.y++;
			lClr = GetTextColor(dValue, nClose);
			SetCellText(pos, 0, strText, lClr);
		}
	}
	
	//�ɽ�
	dValue = pRealTime->m_lNewPrice;
	m_nNewPrice = pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pRealTime, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pRealTime->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	////////////////////////// ������ ////////////////////////////////////////
	pos.x = 2;
	pos.y = 0;
	//��5 ~ ��1��
	for (int i = 4; i >= 0; i--)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, SellCount[i], strText);		
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetSellChange(SellPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	// ��1 ~ ��5��
	for (int i = 0; i < 5; i++)
	{
		pos.y++;
		dValue = CHSCalculate::GetBuySellCount(&m_stkInfo, BuyCount[i], strText);
		SetCellText(pos, 0, strText);

		// ͬʱ����õ��ĳɽ���
		lClr = GetBuyChange(BuyPrice[i], (long)dValue, strText);
		SetCellText(CPoint(pos.x+1, pos.y), 0, strText, lClr);
	}

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pRealTime, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//��5 ~ ��1 ���仯
	for (int i = 4; i >= 0; i--)
		pos.y++;

	//��1 ~ ��5 ���仯
	for (int i = 0; i < 5; i++)
		pos.y++;

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pRealTime, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pRealTime->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pRealTime->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pRealTime, pOther, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//IOPV
	dValue = CHSCalculate::GetIOPV(&m_stkInfo, pRealTimeExt, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOther, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	dValue = CHSCalculate::GetShijing(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܹ�
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetZongzhi(pRealTime, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateETFData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData)          // ����EFT����
{
	if (pLevelData == NULL || pOtherData == NULL)
		return FALSE;

	int nHand = m_stkInfo.m_nHandNum; // EFT 1�� = 100һ��
	//////////////////////////////////////////////////////////////////////////
	// �õ���������
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//

	BOOL bIsStop = TRUE;//�Ƿ�ͣ��
	if (pLevelData->m_lOpen > 0)
		bIsStop = FALSE;

	CString str;
	char    strText[64];
	CPoint  pos(0,0);
	WORD    lClr;

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;

	double dValue = 0.0;
	//�ӵ�2����ʼ
	//ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	pos.x = 1;
	pos.y = 0; 
	dValue = CHSCalculate::GetWeiBi(pLevelData, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//�ɽ�
	dValue = pLevelData->m_lNewPrice;
	m_nNewPrice = pLevelData->m_lNewPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//�ǵ�
	dValue = CHSCalculate::GetZhangdie(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����(�Ƿ�=(�ּ�-����)/����)
	dValue = CHSCalculate::GetFudu(bIsStop, &m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetZongshou(&m_stkInfo, pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetXianshou(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܶ�
	dValue = pLevelData->m_fAvgPrice;
	CGeneralHelper::LongToString(dValue, 0, 10000, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetWaipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܱ�
	dValue = pLevelData->m_lTickCount;
	pos.y++;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fUpPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ӯ 
	dValue = CHSCalculate::GetShiying(&m_stkInfo, pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͨ
	dValue = CHSCalculate::Getliutong(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetShizhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	/////////////////////////////////  ������ /////////////////////////////////////
	//ί��=(ί��������ί������)
	dValue = CHSCalculate::GetWeicha(pLevelData, nHand, strText);
	pos.x = 3;
	pos.y = 0;
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);

	//���� ����E��ʱ�ɽ��������ɽ��ۣ�/�ܳɽ�����
	dValue = CHSCalculate::GetJunjia(&m_stkInfo, pLevelData, strText);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = pLevelData->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMaxPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//���
	dValue = pLevelData->m_lMinPrice;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	//����
	dValue = CHSCalculate::GetLiangbi(&m_stkInfo, pLevelData, pOtherData, m_pDataSource, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//IOPV
	dValue = pLevelData->m_IPOV;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//����
	dValue = CHSCalculate::GetNeipan(&m_stkInfo, pOtherData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//ÿ��
	dValue = CHSCalculate::GetMeiBi(pLevelData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ͣ
	dValue = m_stkInfo.m_fDownPrice;
	CGeneralHelper::LongToString(dValue, 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�о�
	dValue = CHSCalculate::GetShijing(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//�ܹ�
	dValue = CHSCalculate::GetZonggu(pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	//��ֵ
	dValue = CHSCalculate::GetZongzhi(pLevelData, pFinanceData, strText);
	pos.y++;
	SetCellText(pos, 0, strText);

	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}

	return TRUE;
}

BOOL CDrawRStockForm::UpdateIndexData(HSIndexRealTime* pIndex)   
{
	if ( pIndex == NULL || ::IsBadReadPtr(pIndex, 1))
	{
		return FALSE;
	}

	long    nClose   = m_stkInfo.m_lPrevClose;
	int     nUnit    = m_stkInfo.m_nPriceUnit;
	int     nDecimal = m_stkInfo.m_nDecimal;
	int     nHand    = m_stkInfo.m_nHandNum; 

	CString str;
	char    strText[64];
	double dValue(0);
	CPoint pos(0,0);
	WORD    lClr;
	////////////////////////// �ڶ��� ////////////////////////////////////////
	pos.x = 1;
	pos.y = 0;
	
	dValue = pIndex->m_lNewPrice;
	lClr = GetTextColor(dValue, nClose);
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText, lClr);                // �ɽ�

	pos.y = 1;
	dValue = pIndex->m_lNewPrice;
	if (dValue <= 0)
		CGeneralHelper::LongToString((long)0, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	else
		CGeneralHelper::LongToString((dValue - nClose), 0, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue - nClose, 0);
	SetCellText(pos, 0, strText, lClr);                // �ǵ�

	pos.y = 2;
	if (nClose == 0)
		CGeneralHelper::LongToString((long)0, 0, 0, 0, strText, LTOA_MASK_SHOW_PERSENT);
	else
		CGeneralHelper::LongToString((dValue-nClose)*100, 0, nClose, nDecimal, strText, LTOA_MASK_SHOW_PERSENT);
	SetCellText(pos, 0, strText, lClr);                // �Ƿ�

	pos.y = 3;
	CHSCalculate::GetZongshou(pIndex, nHand, LTOA_MASK_SHOWEX_UNITAUTO, strText);
	SetCellText(pos, 0, strText);                      // ����

	pos.y = 4;
	dValue = pIndex->m_fAvgPrice * 100;
	CGeneralHelper::LongToString(dValue, 0, 1, 2, strText, LTOA_MASK_SHOWEX_UNITAUTO | LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                      // ���

	pos.y = 5;
	dValue = CHSCalculate::GetWeiBi(pIndex, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);                   // ί��

	////////////////////////// ������ ////////////////////////////////////////
	pos.x = 3;
	pos.y = 0;

	dValue = nClose;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);                       // ����

	pos.y = 1;
	dValue = pIndex->m_lOpen;
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);                // ����
	SetCellText(pos, 0, strText, lClr);

	pos.y = 2;
	dValue = pIndex->m_lMaxPrice;                   // ���
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	pos.y = 3;
	dValue = pIndex->m_lMinPrice;                   // ���
	CGeneralHelper::LongToString(dValue, nClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, nClose);
	SetCellText(pos, 0, strText, lClr);

	pos.y = 4;
	if (nClose == 0)
		dValue = 0.0;
	else
		dValue = (pIndex->m_lMaxPrice - pIndex->m_lMinPrice) * 100;
	CGeneralHelper::LongToString(dValue, 0, nClose, 2, strText, LTOA_MASK_SHOW_PERSENT | LTOA_MASK_HORLINE_IFZERO);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);                        // ���

	pos.y = 5;
	dValue = CHSCalculate::GetWeicha(pIndex, nHand, strText);
	lClr = GetTextColor(dValue, 0);
	SetCellText(pos, 0, strText, lClr);                        // ί��

	pos.y = 6;
	dValue = pIndex->m_lBuyCount / nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // ί������

	pos.y = 7;
	dValue = pIndex->m_lSellCount / nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // ί������

	pos.y = 8;
	dValue = pIndex->m_nRiseCount;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // ���Ǽ���

	pos.y = 9;
	long nTotalStock = pIndex->m_nTotalStock2;
	if(nTotalStock == 0)
		nTotalStock = pIndex->m_nTotalStock1;
	dValue = nTotalStock - pIndex->m_nFallCount - pIndex->m_nRiseCount; 
	dValue = dValue<0?0:dValue;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // ƽ�̼���

	pos.y = 10;
	dValue = pIndex->m_nFallCount;                        
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pos, 0, strText);                        // �µ�����
	
	return TRUE;
}

void  CDrawRStockForm::InitPriceMap(CMap<long,long,long,long> &lMap, long lPrice[5])
{
	long lKey = 0;
	long lValue = 0;
	// ȥ��map�й�ʱ�ļ۸�
	POSITION pos = lMap.GetStartPosition();
	while(pos)
	{
		lMap.GetNextAssoc(pos, lKey, lValue);

		BOOL bExist = FALSE;
		for (int i = 0; i < 5; i++)
		{
			if (lKey == lPrice[i])
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist == FALSE)
			lMap.RemoveKey(lKey);
	}
}

WORD CDrawRStockForm::GetTextColor( double newValue, double preValue )
{
	double diff = newValue - preValue;
	if (diff > 0)
	{
		return TEXT_COLOR_UP;   // �ǵ���ɫ
	}
	else if (diff < 0)
	{
		return TEXT_COLOR_DOWN; // ������ɫ
	}
	else
	{
		return TEXT_COLOR_FLAT; // ƽ����ɫ
	}
}

BOOL CDrawRStockForm::GetQuickTradeDirection(CPoint pos, double dPrice)
{
	double dNewPrice = (double)m_nNewPrice / m_stkInfo.m_nPriceUnit;
	if (!m_bLevel)
	{// level1 ���
		if (pos.x == 1)
		{
			if (pos.y >= 1 && pos.y <= 5)
			// �嵵����
				return 1;
			else if (pos.y >= 6 && pos.y <= 10)
			// �嵵����
				return 0;
		}
		// ��ȥ������ �����µ��ķ��������³ɽ��۱�
		if (dPrice >= dNewPrice)
		// ���ڳɽ��� ����	
			return 1;
		else 
			return 0;
	}
	else
	{// level2 ���
		if (dPrice >= dNewPrice)
			// ���ڳɽ��� ����	
			return 1;
		else 
			return 0;
	}
}

WORD CDrawRStockForm::GetSellChange(long lSellPrice, long lNowSellCount, char* strText)
{
	double dValue = 0.0;
	if (lSellPrice)
	{// ���˼۸�Ϊ0
		long lPreSellCount = -1;
		if (m_sellMap.Lookup(lSellPrice, lPreSellCount))
			dValue = lNowSellCount - lPreSellCount;
		m_sellMap.SetAt(lSellPrice, lNowSellCount);
	}

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_NOTHING_IFZERO);
	return GetTextColor(dValue, 0);
}

WORD CDrawRStockForm::GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText)
{
	double dValue = 0.0;
	if (lBuyPrice)
	{// ���˼۸�Ϊ0
		long lPreBuyCount = -1;
		if (m_buyMap.Lookup(lBuyPrice, lPreBuyCount))
			dValue = lNowBuyCount - lPreBuyCount;
		m_buyMap.SetAt(lBuyPrice, lNowBuyCount);
	}

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_NOTHING_IFZERO);
	return GetTextColor(dValue, 0);
}

BOOL CDrawRStockForm::MouseDBDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	int nActiveCellID = GetActiveCellID();
	CDrawCell* pCell = GetCell( nActiveCellID );
	if ( pCell && GetSignal() == NEED_TEXTHIGHLIGHT)
	{
		CString str("");
		GetCellText(GetActiveCellID(), 0, str);
		double dPrice = strtod(str, NULL);
		if (dPrice - 0.0001 > 0)
		{// �۸�Ϊ0 
			CPoint pos = IndexToPoint(nActiveCellID);
			BOOL bBuyDirection = GetQuickTradeDirection(pos, dPrice);
			
			CString strFormat("%");
			strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
			if (bBuyDirection)
			{// ��������
				//////////////////////////////////////////////////////////////////////////
				// ��Ӳ���
				CDrawBaseParam param;
				str.Format(strFormat, dPrice);
				param.AddParam(PARAM_QUICKTRADE_PRICE, str);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
			}
			else
			{// ��������
				//////////////////////////////////////////////////////////////////////////
				// ��Ӳ���
				CDrawBaseParam param;
				str.Format(strFormat, dPrice);
				param.AddParam(PARAM_QUICKTRADE_PRICE, str);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
			}
		}
	}
	return TRUE;
}

BOOL CDrawRStockForm::RButtonDown( UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData /*= NULL*/ )
{
	int nActiveCellID = GetActiveCellID();
	CDrawCell* pCell = GetCell( nActiveCellID );
	if ( pCell && GetSignal() == NEED_TEXTHIGHLIGHT)
	{
		CString str("");
		GetCellText(GetActiveCellID(), 0, str);
		double dPrice = strtod(str, NULL);
		if (dPrice - 0.0001 > 0)
		{// �۸�Ϊ0
				
			ResetCell(pCell);
			//////////////////////////////////////////////////////////////////////////
			// ��ȡ��Դ�ַ���
			CAtlBaseModule _Module;
			//CComModule _Module;
			HINSTANCE instance = _Module.GetResourceInstance();
			char* buf = new char[1000];
			int   nBufLenth = 1000;
			//

			CMenu popMenu;
			popMenu.CreatePopupMenu();
			::LoadString(instance, IDS_STRING_QUICKBUY, buf, nBufLenth);  // ��������
			str = buf;
			popMenu.AppendMenu(0, ID_QUICKTRADE_BUY, str);

			::LoadString(instance, IDS_STRING_QUICKSELL, buf, nBufLenth); // ��������
			str = buf;
			popMenu.AppendMenu(0, ID_QUICKTRADE_SELL, str);

			int nMenuID(-1);
			if (popMenu.GetMenuItemCount() > 0)
			{
				::ClientToScreen(hWndParent, &point);
				CWnd* pWnd = CWnd::FromHandle(hWndParent);
				nMenuID = popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y, pWnd);
			}

			if (nMenuID > 0)
			{
				BOOL bBuyDirection = (nMenuID == ID_QUICKTRADE_BUY)?1:0;
				
				CString strFormat("%");
				strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);
				if (bBuyDirection)
				{// ��������
					//////////////////////////////////////////////////////////////////////////
					// ��Ӳ���
					CDrawBaseParam param;
					str.Format(strFormat, dPrice);
					param.AddParam(PARAM_QUICKTRADE_PRICE, str);
					param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
					m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
				}
				else
				{// ��������
					//////////////////////////////////////////////////////////////////////////
					// ��Ӳ���
					CDrawBaseParam param;
					str.Format(strFormat, dPrice);
					param.AddParam(PARAM_QUICKTRADE_PRICE, str);
					param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
					m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
				}
			}
			popMenu.DestroyMenu();
			delete[] buf;	
		}
	}

	return TRUE;
}