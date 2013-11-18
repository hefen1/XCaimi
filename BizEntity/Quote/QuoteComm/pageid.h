/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	pageid.h
*	��	  �ߣ�	������
*  ��ǰ�汾��	4.0
*  �������ڣ�	2003��5��19��
*  ������ڣ�	2003��5��19��
*  ��    ����	ҳ��ID�����ļ�
*
*	�޸���ʷ��	
*          ���ڣ�  
*          ���ߣ�  
*          �Ķ���  
*
***************************************************************/

#ifndef _PAGEID_H_
#define _PAGEID_H_
  
#define  TextMenuWindow           0x0001    //���ֲ˵� 
/////////////////////////////////////////////////////////////////
//���̷����ļ���
  #define  Sh30Exp            0x0010    //��֤������ָ��
  #define  ShTrend            0x0011    //��֤����
  #define  ShLeadExp          0x0012    //��֤����ָ��
  #define  ShATrend           0x0013    //��֤A������ 
  #define  ShBTrend           0x0014    //��֤B������
  #define  ShADL              0x0015    //��֤ADLָ��
  #define  ShDuoKong          0x0016    //��֤���ָ��
  #define  ShLiDao            0x0017    //��֤��������
  #define  ShPerExpTrend      0x0018    //��֤����ָ������
  #define  SzTotalExpTrend    0x0019    //��֤�ۺ�ָ������
  #define  SzTranExpTrend     0x001A    //��֤�ɽ�ָ������
  #define  SzLeadExp          0x001B    //��֤����ָ��
  #define  SzATrend           0x001C    //��֤A������
  #define  SzBTrend           0x001D    //��֤B������
  #define  SzADL              0x001E    //��֤ADLָ��
  #define  SzDuoKong          0x001F    //��֤���ָ��
  #define  SzLiDao            0x0020    //��֤��������
  #define  SzPerExpTrend      0x0021    //��֤����ָ������
  #define  ShSzTrend          0x0022    //��֤��֤����
  #define  ShSzTotal          0x0023    //��֤��֤�ۺ�
  #define  BigTchAnalisys     0x0024    //���̼�������
  #define  ExpReportForm      0x0025    //ָ�����鱨��5��15��30��60��

  #define  StockBigAnalisys   0x0028

  #define  BigBegin			  Sh30Exp
  #define  BigEnd			  ShSzTrend
/////////////////////////////////////////////////////////////////
//�ı��������
  #define  TextView			  0x0050
/////////////////////////////////////////////////////////////////
//���ɷ����ļ���
  #define  StockAnalisys      0x0060    //���ɼ�ʱ���������ȣ���ʱ��������

  #define  StockTrDetail      0x0061    //���ɳɽ���ϸ
  #define  StockFenPrice      0x0062    //���ɷּ۱�
  #define  Stock1TrDetail     0x0063    //����һ���ӳɽ���ϸ
  #define  StockDayDetail     0x0065    //���߳ɽ���ϸ
  #define  Big1TrDetail       0x0066    //����1���ӳɽ���ϸ
  #define  Big5TrDetail       0x0067    //��������ӳɽ���ϸ
  #define  StockMaiMaiChart   0x0068    //����������ͼ

  #define  StockTchAnalisys   0x0069    //���ɼ������� 
  #define  StockReportForm    0x0070    //�������鲥��
  #define  StockTickChart     0x0071    //����tickͼ
  #define  StockStarField     0x0072    //�ǿ�ͼ
  #define  StockMulTrend	  0x0073	//���շ�ʱͼ 	
  #define  StockGannAnalisys  0x0074	//����ͼ����
  #define  HS_PAGEID_GRAPHICS_FINANCE	0x0075	//	����ͼʾҳ��
  #define  TechTickChart	  0x0076	//add by lxqi 20090622  ����Tickͼ
  


///////////////////////////////////////////////////////////////////////  
//���۷����ļ���
  #define  PriceAnalisys1     0x0082    //���۷���1
  #define  PriceAnalisys2     0x0083    //���۷���2
  #define  PriceAnalisys3     0x0084    //���۷���3
  #define  PriceAnalisys4     0x0085    //���۷���4

  #define  PriceTechSort      0x0086    //ָ�����򱨼۷���
  #define  PriceQiangRuoSort  0x0087    //ǿ�����򱨼۷���
  #define  PriceJieDuanSort   0x0088    //�׶����򱨼۷���
  #define  PriceSelStock      0x0089    //����ѡ�ɱ��۷���
  #define  PriceBlockStock    0x0090    //���Ű�鱨�۷���

  #define  PriceAnalisys5     0x0099    //���۷���5

  #define  PA_BEGIN			  PriceAnalisys1
  #define  PA_END			  PriceAnalisys5
///////////////////////////////////////////////////////////////////////  
  
  #define  MultiStock         0x0100    	//���ͬ��
  #define  SpecialReport      0x0110    	//�ر𱨵�

  #define  StockChoose        0x0120    	//��Ʊѡ��                  
//�����������б�ǵĶ��� 
//��m_nInPageTag��ֵ
// 31          23                15         7              0
// ---------------------------------------------------------
// |    ����    |   ����ָ������ֵ  | ֤ȯ����  |  �����ڲ���־ |
// ---------------------------------------------------------
////////////////////////////////////////////////////////////////////
//֤ȯ���͵Ķ���
////////////////////////////////////////////////////////////////////
//m_nInPageTag��ֵ
 #define PT_LIST		  		0x8000		//��Ʊ�б�
 #define PT_MULITSEL		  	0x4000		//��ѡ
 #define PT_TYPE		  		0x0100		//����ɣ��б�ʱHSMarketDataTypeָ������
 #define PT_BLOCK	  			0x0200		//���ɣ��б�ʱCODEָ��������
 #define PT_USERDEFINE 			0x0300		//��ѡ��
 #define PT_INDEX				0x0400		//ָ����
 #define PT_GOOD				0x0500		//��Ʒ˳��
 #define PT_TCHTYPE				0x0600		//����ָ��
 #define PT_CONDITION			0x0700		//����ѡ��
 #define PT_USRBLOCK			0x0800		//��ѡ��/����
 #define PT_ALLUSRTYPE			0x0900		//���з���
 #define PT_CW_GBQX				0x0A00		//����-�ɱ�ȨϢ
 #define PT_CW_CWSJ				0x0B00		//����-��������
 #define PT_CW_CWZB				0x0C00		//����-����ָ��
 #define PT_CW_JJZB				0x0D00		//����-�����ܱ�v
 #define PT_BLOCK_INDEX			0x0E00		//���ָ��	add by lxqi 20090702
 #define PT_USER_BLOCK_INDEX	0x0F00		//��չ���ָ��	add by lxqi 20090702
/////////////////////////////////////////////////////////////// //
////////////////////////////////////////////////////////////////////
//���ɼ�����������ָ���������ڷ������ڱ�ǵĶ���
////////////////////////////////////////////////////////////////////
 #define  AnalisysFor0             0x0000  //�ֱʳɽ�
 #define  AnalisysFor1             0x0001  //1����
// gbq add 20060217
//#ifdef Support_DHJR
// #define  AnalisysFor3			   0x000A  //3����
//#endif 
// gbq add end;
 #define  AnalisysFor5             0x0002  //�����
 #define  AnalisysFor15            0x0003  //ʮ�����
 #define  AnalisysFor30            0x0004  //��ʮ����
 #define  AnalisysFor60            0x0005  //��ʮ����
 #define  AnalisysForDay           0x0006  //����
 #define  AnalisysForWeek          0x0007  //����
 #define  AnalisysForMonth         0x0008  //����
 #define  AnalisysForMoreDay       0x0009  //������
// gbq add 20060217, �����޸� 20060220
 #define  AnalisysFor180           0x000A  //180����
/*
#ifdef Support_DHJR
 #define  AnalisysFor180           0x000B  //180����
#else
 #define  AnalisysFor180           0x000A  //180����
#endif
 */
// gbq end;

 #define  AnalisysBegin			   AnalisysFor1
// gbq add
//#ifdef Support_DHJR
// #define  AnalisysEnd         	   AnalisysFor3
//#else
// #define  AnalisysEnd         	   AnalisysForMoreDay
//#endif 
 #define  AnalisysEnd         	   AnalisysForMoreDay
// gbq end;

/////////////////////////////////////////////////////////////// //
//���ɼ�ʱ�������ڵļ�������
/////////////////////////////////////////////////////////////// //
 #define  StockFenTime             0x0030  //��ʱ����
 #define  StockLiDao               0x0031  //��������
 #define  StockLiangBi             0x0032  //����ָ?
 #define  StockTotalHolding		   0x0033  //�ܳ�
 #define  StockETFTech			   0x0034  //etfָ��

//�ر𱨵��ļ������
/////////////////////////////////////////////////////////////// //
 
 #define  SpecialReport1          0x0040   //�ǵ�������
 #define  SpecialReport2          0x0041   //�ɽ����������
 #define  SpecialReport3          0x0042   //�ɽ�������
 #define  SpecialReport4          0x0043   //�ɽ����仯����
 #define  SpecialReport5          0x0044   //����ǿ�ƹ�
 #define  SpecialReport6          0x0045   //�������ƹ�
 #define  SpecialReport7          0x0046   //�����ʺ������
 #define  SpecialReport8          0x0047   //�����ʺ����չ�
 #define  SpecialReport9          0x0048   //�����ʽ���������
 #define  SpecialReport10         0x0049   //������������
 #define  SpecialReport11         0x004A   //�ۺ�ָ������

 #define  SR_BEGIN				  SpecialReport1
 #define  SR_END				  SpecialReport11
/////////////////////////////////////////////////////////////// // 
//�ı��������
/////////////////////////////////////////////////////////////// // 
 #define  StockMessage            0x0060  //���ɻ�������
 #define  ShStockInfo             0x0061  //�Ϻ�֤������Ϣ
 #define  SzStockInfo             0x0062  //��֤֤������Ϣ
 #define  StockerInfo1            0x0063  //ȯ����Ϣ1
 #define  StockerInfo2            0x0064  //ȯ����Ϣ2
 #define  StockNews               0x0065  //�ƾ���Ϣ
 #define  Notice	              0x0066  //��������
 #define  GraphicFinance		  0x0067  //����ͼʾ��������
 #define  HtmlView				  0x0068  //���HTMLҳ��		
///////////////////////////////////////////////////////////////////
// ��������ͼ
///////////////////////////////////////////////////////////////////
 #define GannSquare				  0x0070	//��������
 #define GannHexagon			  0x0071	//��������
// �߼���Ѷ��صļ���ҳ��
#define  TreeView				  0x00A0	//����ͼ
#define  GraphAnalyseView		  0x00A1	//����ͼ�η�����ͼ
#define  FinancialTableiew		  0x00A2	//�������������ͼ
#define  InfoTextView			  0x00A3	//�ı������ͼ

// ��Ʊ����
#ifndef SC_StockZ
#define SC_StockZ			0x04 // ծȯ
#endif
#ifndef SC_StockJ
#define SC_StockJ			0x05 // ����
#endif

// by spec 2003.04.16
//#define Yls_Split_Status    0x1000 // ���״̬

/////////////////////////////////////////////////////////////// // 
#endif // _PAGEID_H_
