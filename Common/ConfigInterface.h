/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�ConfigInterface.h
* ժ    Ҫ�����ýӿڶ���ͷ�ļ�
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-10
* ��	 ע��
*******************************************************************************/
#pragma once
#include "ConfigStruct.h"
#include "NetEngineInterface.h"


#define OBJ_COLORCFG	   _T("ConfigCore.CHsColor")		    // ��ɫ����
#define OBJ_FONTCFG        _T("ConfigCore.CHsFont")             // ��������
#define OBJ_QUOTESYS       _T("ConfigCore.CHsQuoteSys")         // ����ϵͳ����
#define OBJ_GRAPH          _T("ConfigCore.CHsGraph")            // ͼ���������
#define OBJ_MIS            _T("ConfigCore.CHsMiscellany")       // ��������
#define OBJ_SCROLLNEW      _T("ConfigCore.CHsScrollNews")       // ���������
#define OBJ_COLUMNPRO	   _T("ConfigCore.CHsColumnProperty")	// ��������Ϣ����
#define OBJ_TAB            _T("ConfigCore.CHsTabCfg")           // ���۱�ǩ����
#define OBJ_COMM           _T("ConfigCore.CHsCommuncation")     // ͨѶ����
#define OBJ_USERDATA       _T("ConfigCore.CHsUserDataCfg")      // �����е��û����ݶ�д����
#define OBJ_BLOCKMANAGER   _T("ConfigCore.CBlockManager")		// ������
#define OBJ_KEYBOADR       _T("ConfigCore.CHsKeyboardCfg")      // �ȼ�����
#define OBJ_INFO           _T("ConfigCore.CHsInfoCfg")          // ��Ѷ����
#define OBJ_KLINETAB       _T("ConfigCore.CHsKLineTab")         // K��tab
#define OBJ_KLINEPOSCTRL   _T("ConfigCore.CHsKLinePosCtrol")    // K��λ�ÿ���

interface INotify
{
// 	virtual void SendChangedNotify( /* ֪ͨ���͡�����������ͣ���ɫ�������(ö��) */ ) = 0;
// 	virtual void SetChangedNotify(/* �ص�����ָ�롪�������֪ͨ���� */) = 0;
};

interface ICfgBase
{
	virtual BOOL SaveToFile() = 0;                  //�����ļ�
};


///////////////////////////////////  HsColor ///////////////////////////////////
//��ɫ����
interface IHsColor : public INotify, public ICfgBase
{ //������ 

	////////////////////////////  ��ȡ����  ///////////////////////////////////////////
	virtual const COLORREF GetColumnColor(UINT nColumnId)=0; 
	virtual const COLORREF GetBlackgroundColor()=0;  // ��ȡ����ɫ
	virtual const COLORREF GetFixedCodeColor()=0;    // ��ȡ��������ı�ɫ
	virtual const COLORREF GetFixedNameColor()=0;    // ����֤ȯ����ɫ
	virtual const COLORREF GetFixedNameSelfColor()=0;// ����֤ȯ��ѡ������
	virtual const COLORREF GetUpColor()=0;           // ��ȡ������ɫ
	virtual const COLORREF GetDownColor()=0;         // ��ȡ�µ���ɫ
	virtual const COLORREF GetEqualColor()=0;        // ��ȡƽ����ɫ
	virtual COLORREF GetTotalColor()=0;              // ��ȡ������ɫ
	virtual COLORREF GetNowColor()=0;                // ��ȡ������ɫ
	virtual COLORREF GetDoneColor()=0;               // ��ȡ�ɽ���ɫ
	virtual COLORREF GetChangeColor()=0;             // ��ȡ������ɫ
	virtual COLORREF GetQuantitativeRateColor()=0;   // ��ȡ������ɫ 
	virtual COLORREF GetOrderBuyVolColor()=0;        // ��ȡί������ɫ
	virtual COLORREF GetOrderSellVolColor()=0;       // ��ȡί������ɫ
 
	virtual const COLORREF GetStatusTextColor()=0;   // ��ȡ״̬���ı���ɫ
	virtual const COLORREF GetScrollTestColor()=0;   // ��ȡ������ı���ɫ
	virtual const COLORREF GetScrollBKColor()=0;     // ��ȡ����Ʊ�����ɫ

	virtual const COLORREF GetTipTitleBk()=0;		 // tip��ʾ���ⱳ��
	virtual const COLORREF GetTipTitleFg()=0;		 // tip��ʾ����ǰ��
	virtual const COLORREF GetTipTextFg()=0;		 // tip��ʾ�ı�����
	virtual const COLORREF GetTipTextBk()=0;		 // tip��ʾ�ı�ǰ��

	virtual COLORREF GetQuoteFixedRowColBKColor()=0;			    // ��ȡ���鱨�۹̶��й̶��б���ɫ
	virtual COLORREF GetQuoteFixedRowBKColor()=0;					// ��ȡ���鱨�۹̶��б���ɫ
	virtual COLORREF GetQuoteFixedColBKColor()=0;					// ��ȡ���鱨�۹̶��б���ɫ
	virtual COLORREF GetQuoteHighLightFgColor()=0;					// ��ȡ���鱨�۸���ǰ��ɫ
	virtual COLORREF GetQuoteHighLightBKColor()=0;					// ��ȡ���鱨�۸�������ɫ
	virtual COLORREF GetQuotePriceChangeBKColor()=0;				// ��ȡ���鱨�۸���.�����ı�����
	virtual COLORREF GetQuoteTabBKColor()=0;							// ��ȡ���鱨�۱�ǩҳ������ɫ
	virtual COLORREF GetQuoteTabTextColor()=0;							// ��ȡ���鱨�۱�ǩҳ������ɫ
	virtual COLORREF GetQuoteGuideTabBKColor()=0;						// ��ȡ���鱨���򵼱�ǩҳ������ɫ
	virtual COLORREF GetQuoteGuideTabTextColor()=0;						// ��ȡ���鱨���򵼱�ǩҳ������ɫ
	virtual COLORREF GetQuoteActiveTabBKColor()=0;						// ��ȡ���鱨�ۼ����ǩҳ������ɫ
	virtual COLORREF GetQuoteActiveTabTextColor()=0;					// ��ȡ���鱨�ۼ����ǩҳ������ɫ
	virtual const ColorProperty* GetQuoteTabFrameColorProp()=0;			// ��ȡ���鱨�۱�ǩҳ�߿���������ɫ
	virtual const ColorProperty* GetQuoteGuideTabFrameColorProp()=0;	// ��ȡ���鱨���򵼱�ǩҳ�߿���������ɫ
	virtual COLORREF GetQuoteTabButtonBKColor()=0;						// ��ȡ���鱨�۱�ǩҳ��ť������ɫ

	virtual COLORREF GetRTitleCode()=0;          // ��Сͼ�����Ʊ������ɫ
	virtual COLORREF GetRTitleCodeName()=0;      // ��Сͼ�����Ʊ������ɫ
	virtual COLORREF GetRTableFixedName()=0;     // ��Сͼ�̶��ı���ɫ
	virtual COLORREF GetRTableTime()=0;          // ��Сͼʱ����ɫ
	virtual COLORREF GetRTableVer()=0;           // ��Сͼ������ɫ
	virtual COLORREF GetRTableHor()=0;           // ��Сͼ������ɫ
	virtual COLORREF GetTableTitle()=0;          // ������ͼԪ������ɫ
	virtual COLORREF GetTableTitleBK()=0;        // ������ͼԪ������ɫ
	virtual COLORREF GetTradeTableFixedName()=0; // ����ͼԪ������

	virtual COLORREF GetZiXunBK()=0;             // ��Ѷ����ɫ
	virtual COLORREF GetZiXunFG()=0;             // ��Ѷ�ı�ǰ��ɫ

	virtual COLORREF GetDiagramLayoutBKColor() = 0;              // ͼ���ǩҳ����
	virtual COLORREF GetDiagramLayoutActiveFillColor() = 0;      // ͼ���ǩҳ����
	virtual COLORREF GetDiagramLayoutFillColor() = 0;            // ͼ���ǩҳ����
	virtual COLORREF GetDiagramLayoutForeColor() = 0;            // ͼ���ǩҳǰ��
	virtual COLORREF GetDiagramLayoutBorderColor() = 0;          // ͼ���ǩҳ�߿�


	virtual COLORREF GetTechLineColor(int nID) = 0;   // ��ȡ��������ɫ

	////////////////////////////  д�벿��  ///////////////////////////////////////////
	virtual void  SetScrollTextColor(const COLORREF textColor)=0; // ����������ı���ɫ
	virtual void  SetScrollBKColor(const COLORREF bkColor)=0;     // ��������Ʊ�����ɫ
};

///////////////////////////////////  HsFont ///////////////////////////////////
//��������
interface IHsFont: public INotify, public ICfgBase
{
	////////////////////////////  ��ȡ����  ///////////////////////////////////////////
	virtual CFont* GetTipTitleFont()=0;       // ��ȡTIP��ʾ��������
	virtual CFont* GetTipTxtFont()=0;         // ��ȡTIP��ʾ�ı�����

	virtual CFont* GetQuoteTabFont()=0;		  // ��ȡ���鱨�۱�ǩҳ����
	virtual CFont* GetQuoteGuideTabFont()=0;  // ��ȡ���鱨���򵼱�ǩҳ����
	virtual CFont* GetStatusFontProt()=0;	  // ��ȡ״̬��ָ������
	virtual CFont* GetScrollFontProt()=0;     // ��ȡ�������������
	 
	virtual CFont* GetQuoteRefreshFont()=0;	  // ��ȡ���鱨�۸�������
	virtual CFont* GetQuoteColHeadFont()=0;	  // ��ȡ���鱨�۱�ͷ����
	virtual CFont* GetQuoteStringFont()=0;	  // ��ȡ���鱨����������(������)
	virtual CFont* GetQuoteCodeFont()=0;	  // ��ȡ���鱨�۹�Ʊ��������
	virtual CFont* GetQuoteDynamicFont()=0;	  // ��ȡ���鱨�۶�̬�ı�����
	virtual CFont* GetQuoteFixedFont()=0;	  // ��ȡ���鱨�۹̶��ı����壨��

	virtual CFont* GetRTableFixedFont()=0;        // ��ȡ��Сͼ�ֶ���������
	virtual CFont* GetRTableDynamicalFont()=0;    // ��ȡ��Сͼ��̬�ֶ�����
	virtual CFont* GetRTableTitleCodeFont()=0;    // ��ȡ��Сͼ�����Ʊ��������
	virtual CFont* GetRTableTitleNameFont()=0;    // ��ȡ��Сͼ�����Ʊ��������
	virtual CFont* GetRTableFinanceFont()=0;      // ��ȡ��Сͼ���ֵҳ�ı�
	virtual CFont* GetRTableTimeFont()=0;         // ��ȡ��Сͼʱ��������
	virtual CFont* GetDiagramLayoutTabFont()=0;   // ��ȡͼ���ǩҳ����
	virtual CFont* GetDiagramTableTitleFont()=0;  // ��ȡ������ͼԪ�ı�����

	virtual CFont* GetZiXunFont()=0;              // ��ȡ��Ѷ����
	virtual CFont* GetDiagramTradeFont()=0;       // ��ȡ����ͼԪ����
	////////////////////////////  д�벿��  ///////////////////////////////////////////
	virtual BOOL SetScrollFontProt(CFont* pFontScroll)=0;             // �����������������
};

///////////////////////////////////  HsSystem ///////////////////////////////////
//�����������Լ�����������
interface IHsQuoteSys: public INotify, public ICfgBase
{
	virtual BOOL IsSupportLevel2() = 0; // �Ƿ�֧��Level2
	virtual BOOL IsSupportFuture() = 0;//�Ƿ�֧���ڻ�����
	virtual CString GetQuoteServerInitMarket() = 0;
	virtual CString GetFutureServerInitMarket() = 0;
};

//ͼ��������
interface IHsGraph: public INotify, public ICfgBase
{
	virtual int GetIndexTrendAxisDecimal() =0;		// ָ����ʱͼ����������ʾ��С��λ
	virtual int GetIndexKLineAxisDecimal() =0;		// ָ��K��ͼ����������ʾ��С��λ

	//��ʱ��K��Tabҳ��ʾ��ָ����Ϣ
	virtual CString GetTabTechIndexDispOrder() =0;	// ָ����ʾ�Ķ���˳��
};

//��Ѷ������
interface IHsInfo: public INotify, public ICfgBase
{
	virtual CString GetInfoProvider() =0;   // ��ȡ��Ѷ�ṩ��
};

//���������(����״̬��)
interface IHsScrollNews: public INotify, public ICfgBase
{
	virtual int		IsDispThirdIndex()=0;		// �������Ƿ���ʾ
	virtual int		IsDispFourthIndex()=0;		// �������Ƿ���ʾ 
	virtual int		IsScrollFixed() =0;			// ������Ƿ�̶���״̬����
	virtual int		IsScrollMultiLine() =0;		// ������Ƿ������ʾ  	
 	virtual int		GetShowScrollAlone() =0;	// �������ʾ��ʽ 0������ƺ�״̬����ͬһ�У�1������Ƶ���һ����״̬�����棻2��״̬������һ�У��������������һ��

	virtual CString GetStatusName()=0;          // ��ȡ����Ʊ�������
	virtual int		GetStatusScrollSpeed()=0;	// ������� ��Ϊ��λ
	virtual int     GetStockNewName(CStringArray &ayStockList)=0; // ��ȡ��Ʊ������            
	virtual int     GetUpDownRate()=0;          // �Ƿ���ʾ�ǵ���
	virtual int		GetTotal()=0;               // �Ƿ���ʾ�ܶ�
	virtual int		GetShowNewPirce()=0;        // �Ƿ���ʾ���³ɽ���
	virtual int		GetUpDown()=0;              // �Ƿ���ʾ�ǵ�

	virtual int		GetScrollSpeed() =0;			// ��������ٶ� ����Ϊ��λ
	virtual int		GetHorizonScrollWay() =0;		// ����ˮƽ�������� 3: ����; 4������
	virtual int		GetVerticalScrollWay() =0;		// ���鴹ֱ�������� 1: ����; 2������
	virtual int		GetClickWay() =0;				// ��ʾ���鷽ʽ 0������; 1��˫�� 
	virtual void	GetScrollText(CStringArray &ayScrollText)=0;  // ��ȡ����ƹ�����ѯ

	//��ȡÿ��ָ������Ӧ�Ĵ����б�һ����¼��Ӧһ��CStringArray��ÿ��CStringArray�����Ⱥ����֤ȯ������г����
	//���ô˺�����ÿ��CStringArray������ڴ��ͷ�
	virtual int		GetFirstListStockInfo(CStringArray &ayStockList) =0;
	virtual int		GetSecondListStockInfo(CStringArray &ayStockList) =0;
	virtual int		GetThirdListStockInfo(CStringArray &ayStockList) =0;
	virtual int		GetFourthListStockInfo(CStringArray &ayStockList) =0;
	//////////////////////////////////////////////////////////////////////////

	virtual BOOL	SetDispThirdIndex(int nValue) =0;    // �����������ʾ���� 
	virtual BOOL	SetDispFourthIndex(int nValue) =0;   // �����������ʾ����
	virtual BOOL	SetScrollFixed(int nValue) =0;       // ��������ƹ̶�����
	virtual BOOL	SetScrollMultiLine(int nValue) =0;   // ��������ƶ��п���

	virtual BOOL	SetStatusScrollSpeed(int nValue) =0; // ����ָ����ʾ�ٶ� ��Ϊ��λ
	virtual BOOL	SetScrollSpeed(int nValue) =0;       // ������������ٶ�
	virtual BOOL	SetHorizonScrollWay(int nValue) =0;  // ��������ˮƽ��������
	virtual BOOL	SetVerticalScrollWay(int nValue) =0; // �������鴹ֱ��������
	virtual BOOL	SetClickWay(int nValue) =0;          // ����������ʾ��ʽ

	virtual	BOOL	SetFirstListStockInfo(const CStringArray &ayStockList) =0;
	virtual BOOL	SetSecondListStockInfo(const CStringArray &ayStockList) =0;
	virtual BOOL	SetThirdListStockInfo(const CStringArray &ayStockList) =0;
	virtual BOOL	SetFourthListStockInfo(const CStringArray &ayStockList) =0;
};

//��������:	��ϵͳ������������ Tabҳǩ ���������ԣ���ʱ��k�ߣ���Ѷ����Ϣ���ף����������֮���ϵͳ��������
//          �����Ƿ���ʾ��������״̬�����������߶ȣ�״̬����ƣ����Ͻǰ�ť���õ�
interface IHsMiscellany: public INotify, public ICfgBase
{
	//virtual CString GetRYZQEQWebSite()=0;		//�����о�����EQ�û����ӵ�ַ
	//virtual int IsLevel2Must()=0;					//1���߱�level2�˺�ʱ�����¼level2�������ʹ��ϵͳ��Ĭ��Ϊ0��������level2�˺ſ���ʹ��level1����

	//virtual int IsDispBulletin()=0;					//��¼ϵͳʱ�Ƿ���ʾ�������棬Ĭ��1����ʾ��0������ʾ����ʾ������������ݴӷ�����ȡ(welcome.htm)
	//virtual int IsDispTradeTime()=0;				//���Ƴɽ���ϸ��ʱ���Ƿ���ȫ��ʾ��Ĭ��0���ֱʳɽ�ʱ�䲻��ȫ��ʾ��1���ֱʳɽ�ʱ����ȫ��ʾ
	//virtual int IsDispSubTitle()=0;					//�Ƿ��ڿ�ܱ�������ʾ�Ӵ��ڱ��⣬Ĭ��1������ʾ��0����ʾ
	//virtual int IsCommunicationModified()=0;		//��ֹ�޸�ͨѶ���ã�Ĭ��0�������ͨѶ���ý���ɾ�����޸ģ�1���������ͨѶ���ý���ɾ�����޸�

	//virtual int IsDispSelfBlockTip()=0;				//������ѡ��֮���Ƿ���ʾ��Ϣ��Ĭ��1����ʾ��0������ʾ
	//virtual int IsDispIPAndPort()=0;				//1��ͨѶ�����ﲻ��ʾIP��ַ��0����ʾ

	//virtual int IsDispZixun()=0;					//�Ƿ���ʾ��Ѷ
	//virtual int IsNotShowWindowList()=0;			//���˵��С����ڡ��˵����Ƿ���ʾ�򿪹��Ĵ����б�0��д�ü�ֵ���򿪴����б�1�����򿪴����б�
	//virtual int IsShowLoginStatur()=0;				//��ܱ������Ƿ���ʾ���׵�¼״̬��Ĭ��1����ʾ��0������ʾ
	//virtual int IsTradeMaximal()=0;					//Ĭ��0������+���׷�ʽ��¼ϵͳʱ���׽��治�������ʾ��1������+���׷�ʽ��¼ϵͳʱ���׽��������ʾ
	//virtual int IsLevel2AccountSave()=0;			//�Ƿ��ѱ���Level2�˺�

	//virtual int IsSupportMainMenu()=0;				//Ĭ��0��д����֧�����˵����棻1��֧�����˵�����
	//virtual int IsHQUseBestSite()=0;				//�����¼ʹ�����վ��
	//virtual int IsDispLevel2()=0;					//�Ƿ���ʾLevel2���
	//virtual int IsSupportLevel2()=0;                //�Ƿ�֧��Level2,��֮ǰ�ϰ汾�ı��뿪��
	//virtual int IsGetLevel2AccoutFromServer()=0;	//�Ƿ��ط�����ȡLevel2�˺�

	//virtual int IsOpenTipInKeyBoard();				//����000����������;Ĭ��0��д����֧�ִ򿪽�����ʾ��1��֧�ִ򿪽�����ʾ(welcome.htm)��
	//virtual int IsDispDailyBulletin()=0;			//��¼��ɺ���ʾ��ȯ�̵�ÿ����Ϣ����
	//virtual int IsShowUseTip()=0;					//Ͷ��Ӯ�ҿͻ���ʹ�ý���Tip
	//virtual int GetUseTipFilePos()=0;				//Ͷ��Ӯ�ҿͻ���ʹ�ý���Tip���ļ�λ��

	//virtual int IsEnableStrategeTrade()=0;			//���Խ����Ƿ���Ч
	//virtual int IsStrategeTradeRelateKline()=0;		//�Ƿ񽫲��Բ���������������ָ�꣬1������������ָ�ꣻ0��������������ָ��

	//virtual int QuoteExcludeIsUsed()=0;				//���ͻ����鱨���Ƿ�ʹ�ù��˹���
	//virtual int QuoteExcludeIsNowNew()=0;			//�Ƿ��޳������¹�
	//virtual int QuoteExcludeIsNotUpStop()=0;		//�Ƿ��޳����ǵ�ͣ��Ʊ
	//virtual int QuoteExcludeIsNotSmall()=0;			//�Ƿ��޳������¹�
	//virtual int QuoteExcludeIsNotGrow()=0;			//�Ƿ��޳���ҵ���Ʊ
	//virtual int QuoteExcludeIsNotSCapt()=0;			//�Ƿ��޳���ͨ��С��һ����ֵ��Ʊ
	//virtual int QuoteExcludeIsShowSmall()=0;		//�Ƿ��޳���С���Ʊ
	//virtual int QuoteExcludeAmtRatio()=0;			//���ճɽ���/��ͣ�ҵ��� ��ֵ:
	//virtual int QuoteExcludeAmtCapital()=0;			//��ͨ�̹�Ʊ����


	//virtual int GetLoginType()=0;			 //1����ģʽ��¼����¼�����ڳ�ʼ������ɺ�����ʧ������Ҫ��̨������֧�֣�0:Ϊ��ģʽ����ʼ���ȹ����ڵ�¼������ʧ�����
	//virtual int GetTabBarPos()=0;			 //���۽����Tab��������ʾλ�ã�Ĭ��0���ײ���1������
	//virtual int GetReportScrollBarType()=0;  //���۱�ǩҳ�߿���ѡ��Ĭ��1����ɫ�߿���0��ԭ���

	//virtual int GetMultiMenu()=0;			//ϵͳ����а����ʾ��ʽ��Ĭ��0��ԭ����ʽ��������ʾ��1��������ʾ
	//virtual int GetShowScrollAlone()=0;		//�Ƿ�����Ƶ���һ����ʾ��Ĭ��2��״̬������һ�У��������������һ�У�0������ƺ�״̬����ͬһ�У�1������Ƶ���һ����״̬������

	//virtual int GetBulletinWidth()=0;		//BulletinWidth������������ʾ����
	//virtual int GetBulletinHeight()=0;		//BulletinHeight������������ʾ��߶�
	//virtual int GetReportGridWidth()=0;		//����и߶ȣ������б���и�
	//virtual int GetLogStockCount()=0;		//�˸��֧�ֹ�Ʊ�����ڷ�ʱ��K�߽����л��������룬Ȼ���˸��(Backspace��)���Է�����һ�����룬���ñ�ʾ�����˵�֮ǰ���ٸ�����
	//virtual float  GetCalculator()=0;			//����������˰��

	//virtual int GetExportFileType()=0;			//�����ļ����ܣ������ļ�������
	//virtual CString GetExportFilePath()=0;		//�����ļ����ܣ������ļ���·��
	//virtual CString GetPanHouPath()=0;			//���ݶ�ȡ·�������ػ������ݴ��·����Ĭ���̺�����Ŀ¼Ϊdata��Ĭ����������Ŀ¼ΪLOND\dat
	//virtual CString GetPanzhongPath()=0;		//���ݶ�ȡ·�������ػ������ݴ��·����Ĭ����������Ŀ¼Ϊdata��Ĭ����������Ŀ¼ΪLOND\dat
	//virtual CString GetVersionNumber();			//���ڶԻ�����ʾ�İ汾��

	//virtual int GetCurrentPageID()=0;			//ҳ��ID
	//virtual CString GetCurrentPageType()=0;		//ʹ��ҳ������
	//virtual CString GetFontTemplet()=0;			//����ģ��
	//virtual CString GetColorTemplet()=0;		//��ɫģ��
	//virtual CString GetLineTemplet()=0;			//����ʹ��ģ��

	virtual  int SetSectionValue(CString lpszSection, CString lpszValue) = 0;		//���������ļ���ʽ���ݣ�����section��Ӧ��ֵ�� 
	virtual  CString GetSectionValue(CString lpszSection) = 0;		//���������ļ���ʽ���ݣ���ȡsection��Ӧ��ֵ�� 
};


///////////////////////////////////  HsColumnProperty ///////////////////////////////////
//����������
interface IHsColumnProperty: public INotify, public ICfgBase
{
	virtual TableColProperty*	GetColumn(int nID)=0;	    // ��ȡ������
 	virtual CString		GetDefDispColGroupName()=0;		    // ��ȡĬ����ʾ��������
 	virtual CString		GetDefaultColGroupName()=0;		    // ��ȡĬ���������� ��ɾ���ȱ���
 	virtual long		GetAllColumnGroupName(CStringArray& ayGroup)=0;		// ��ȡ������������
 	virtual CColPropArray* GetColumnGroupProp(CString strGroupName)=0;		// ��ȡָ����������������
	virtual BOOL           GetColumnGroupPropEx(CColPropIDIndex &mapProp, CString strGroupName) = 0;    // ����ͬ�� ����ֵ��ͬ
	virtual CString		GetFinanceGroupName()=0;			// ��ȡ����������������
	virtual long		GetAllDispColTemplName(CStringArray& ayGroup)=0;	// ��ȡ������ʾ��ģ������
 
	virtual RefreshDispTextProp* GetQuoteRefreshProp()=0; // ��ȡˢ�����Խṹ

	virtual CString		GetColumnName(int nID)=0;     // ��ȡ������
	virtual long		GetColumnMask(int nID)=0;     // ��ȡ������
	virtual long		GetColumnWidth(int nID)=0;    // ��ȡ�п��

	//��ȡ��ʾ���еĸ����е����� ayGroup����������ColumnID��ż������ColumnWidth
	virtual long        GetDispColGroup(CString strGroup, CUIntArray*& ayGroup)=0; 
	virtual void		SetDispColGroup(CString strGroup, CUIntArray* ayGroup)=0;	// ������ʾ��������
	virtual BOOL		AddDispColGroup(CString strGroup, CUIntArray* ayGroup)=0;	// �����ʾ����
	virtual BOOL		DelDispColGroup(CString strGroup)=0;		// ɾ����ʾ����
};

///////////////////////////////////  HsTabCfg ///////////////////////////////////
//Tabҳǩ�ӿ�
interface IHsTabCfg : public INotify, public ICfgBase
{
	virtual CString		GetDefaultTabGroup()=0;							// ��ȡĬ����ʾ�ı�ǩҳ������
	virtual int 		GetDefaultTabItemID()=0;						// ��ȡĬ��ѡ�еı�ǩҳID

	virtual long		GetAllTabGroups(CStringArray& ayTabGroup)=0;	// ��ȡ���б�ǩҳ������
	virtual long		GetSysTabGroups(CStringArray& ayTabGroup)=0;	// ��ȡϵͳĬ�ϱ�ǩҳ������
	virtual long		GetUserTabGroups(CStringArray& ayTabGroup)=0;	// ��ȡ�û��Զ����ǩҳ������

	virtual long		GetUserDefMenuID(int nID)=0;

	virtual long		GetTabList(CString strGroupName,CTabItemArray& ayTabItem)=0;	// ��ȡTab�б�
	virtual long		GetSysTabList(CString strGroupName,CTabItemArray& ayTabItem)=0;	// ��ȡϵͳTab�б�
	virtual BOOL		SetTabList(CString strGroupName,CTabItemArray* ayTabItem)=0;	// ����ָ������Tab�б�
	virtual BOOL		DelTabList(CString strGroupName)=0;				// ɾ��ָ����ǩҳ����
};

///////////////////////////////////  IHsCommunication ///////////////////////////////////
//ͨѶ���������
interface IHsCommunication : public INotify, public ICfgBase
{
	virtual int GetSysServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1) = 0;    // ��ȡָ�����͵�ϵͳĬ��վ��
 	virtual int GetUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1) = 0;    // ��ȡָ�����͵��û��Զ���վ��
	virtual int GetAllSysServer(CCommInfoArray *pAy, int nCellID) = 0;   // ��ȡ�������͵�ϵͳĬ��վ��
	virtual int GetAllUsrServer(CCommInfoArray *pAy, int nCellID) = 0;   // ��ȡ�������͵��û��Զ���վ��
 	virtual void SaveUsrServer(Server_type type, CCommInfoArray *pAy, int nCellID = -1) = 0;  // ����ָ�����͵��û��Զ���վ��
 	virtual void SaveAllUsrServer(CCommInfoArray *pAy, int nCellID) = 0;  // �����������͵��û��Զ���Ĭ��վ��
 	virtual BOOL GetProxy(ProxyInfo *proxy) = 0; // ��ȡ������Ϣ
	virtual void SaveProxy(ProxyInfo *proxy) = 0;// ���������Ϣ

 	virtual BOOL GetAuto() = 0;                  // ��ȡ�Զ����ٿ���
 	virtual BOOL SetAutoChooseBest(UINT au) = 0; // ��ȡ�Զ��������վ�㿪��

	virtual int  GetHeartBeat(Server_type type) = 0; // ��ȡ�������

 	virtual BOOL GetSwitch(Server_type type) = 0; // ��ȡָ��������ʾ����
	virtual void GetAllSwitch(CMap<Server_type,Server_type,BOOL,BOOL> *pAy) = 0;  // ��ȡ����������ʾ����
 	virtual CString GetDefaultSite(Server_type type, int nCellID = -1) = 0; // ��ȡָ������Ĭ��վ��
	virtual void    GetAllDefaultSite(CMap<Server_type,Server_type,CString,CString> *pAy, int nCellID) = 0; // ��ȡ��������Ĭ��վ��
	virtual void    SetDefaultSite(Server_type type, CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID = -1) = 0; // ����ָ������Ĭ��վ��
	virtual void    SetAllDefaultSite(CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID) = 0;     // ������������Ĭ��վ��
	virtual void    RemoveSysMap(Server_type type, int nCellID) = 0; // �Ƴ�map������
	virtual void    RemoveUsrMap(Server_type type, int nCellID) = 0; // �Ƴ�map������

	virtual int     GetSaveAccount() = 0;             // ��ȡ�����˺ſ���
	virtual BOOL    SetSaveAccount(UINT unSave) = 0;  // ���ñ����˺ſ���
	virtual int     GetHideAccount() = 0;             // ��ȡ�����˺ſ���
	virtual BOOL    SetHideAccount(UINT unHide) = 0;  // ���������ʺſ���

	virtual BOOL    GetLevelAcc(CString &accName, CString &accPwd) = 0; // ��ȡ����Level2�˺š�����
	// ��ȡ/�����˺���Ϣ ayAccInfo�а��˺����ơ��˺����͡�Ӫҵ��ID��� nAccCount��ʾ�洢�˺Ÿ���
	virtual int     GetAccount(CStringArray& ayAccInfo) = 0;
	virtual BOOL    SaveAccount(CStringArray& ayAccInfo, int nAccCount) = 0; 
	virtual BOOL    RemoveAllAccount() = 0; // ɾ�����д洢���˺�
};

///////////////////////////////////////// IHsKeyboardCfg /////////////////////////////////
// ���̾����ȼ� ֻ��
interface IHsKeyboardCfg : public INotify 
{
	virtual int GetOrderMarket(CStringArray &ay, CString strKey) = 0;  //��ȡ�г�����
	virtual int GetCutShort(CStringArray &ay) = 0; //��ȡ�ȼ�
};


///////////////////////////////////  IHsUserDataCfg ///////////////////////////////////
// �����е��û����ݶ�д�ӿ�
interface IHsUserDataCfg : public INotify, public ICfgBase
{
	virtual const HsCodeMark* SearchMark(IN CString& strKeyWord)=0; //���ݹؼ��ֲ��ұ������
	virtual BOOL DelMark(IN CString& strKeyWord)=0;		        // ɾ��ָ�����
	virtual BOOL SetMark(IN const HsCodeMark& CodeMark)=0;		// �趨���
};

interface IBlockManager : public INotify, public ICfgBase
{
	// ������
	virtual long	GetAllBlockGroupName(OUT CStringArray& ayBlockGroupName)=0;	// ��ȡ���а���������
	virtual long	GetAllSysBlockGroupName(OUT CStringArray& aySysBlockGroupName)=0;
	virtual long	GetAllUserBlockGroupName(OUT CStringArray& ayUserBlockGroupName)=0;
	virtual long	GetBlockName(IN CString& strBlockGroupName, OUT CStringArray& ayBlockName)=0;	// ��ȡָ������������а������
	virtual long	GetAllBlockName(OUT CBlockGroupArray& ayBlockName)=0;	    // ��ȡ���а������

	// ��ȡ�ɷֹ���Ϣ
	virtual long	GetStockList(IN CString strBlockName, OUT CArray<CodeInfo>& ayStockList)=0;	// ��ȡָ�����ĳɷֹ��б�
	virtual long    GetStockList(IN CString strBlockName, OUT CMapStringToPtr& mapStockList)=0; // ��ȡָ�����ĳɷֹ��б� �ӵ�map��
	virtual long	GetStockListCount(IN CString strBlockName)=0;		// ��ȡָ�����ɷֹɸ���
	virtual long	GetStockAdscription(IN CodeInfo* pCode,OUT CStringArray& ayBlockName)=0;		// �����������

	// �����ɷֹ��б�
	virtual BOOL	AddStockToBlock(IN CString strBlockName, IN CodeInfo* code)=0;		// ���һ���ɷֹɵ����ĩβ
	virtual BOOL	InsertStockToBlock(IN CString strBlockName, IN CodeInfo* code, IN long nIndex)=0;	// ����һ���ɷֹɵ�ָ������ָ��λ��
	virtual long	DelStockFromBlock(IN CString strBlockName, IN CodeInfo* code)=0;	// ��ָ�����ɾ��ָ���ɷֹ�

};

// k��tab������
interface IHsKLineTab
{
	virtual CString GetDefaultGroupName()=0;   // ��ȡĬ��ѡ�е�����

	virtual int  GetTechIndexAyCount()=0;                 // ��ȡָ���������
	virtual CString GetTechIndexName(IN int nIndex)=0;    // ��ȡĳһָ���������
	virtual int  GetTechIndex(IN int nIndex, OUT CTechIndexArray& ayTechIndex)=0;          // ��ȡָ����
	virtual int  GetTechTempleteAyCount()=0;              // ��ȡģ���������
	virtual CString GetTechTempleteName(IN int nIndex)=0; // ��ȡĳһģ���������
	virtual int  GetTechTemplete(IN int nIndex, OUT CTechTempleteArray& ayTechTemplete)=0; // ��ȡģ����
};

//K�߷�ʱ����ͼԪλ�ù�����
interface IHSKLinePosition
{
	virtual int				GetTechLines(IN int nType, CStringArray& ayStrRet, BOOL bResearch = FALSE) = 0;	//��ȡλ�ÿ���ǰ������ͼԪ
	virtual float			GetPosCtrolRatio(IN int nType) = 0;												//��ȡλ�ÿ��Ƶı���
	virtual int				SetTechLines(IN int nType, IN CStringArray& strKLIneName, 
								BOOL bFirst = FALSE, BOOL bLast = FALSE) = 0;								//д��ͼԪ
	virtual int				SetPosCtrolRatio(IN int nType, IN float fRatio) = 0;							//д��λ�ÿ���
};