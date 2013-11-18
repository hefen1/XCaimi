 /*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	FormulaManPublic.h
*	�ļ���ʶ�� 	
*	ժ	  Ҫ�� ��ʽ����ͷ�ļ�	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
***************************************************************/

#ifndef FORMULAMANPUBLIC
#define FORMULAMANPUBLIC

#include "ConfigStruct.h"
#include "hsds_comudata.h" 
#include "hsothercoldefine.h"
#pragma pack(1) 


 //ͼԪ����
#define HS_DRAWICON    1
#define HS_DRAWLINE    2
#define HS_DRAWTEXT    3
#define HS_POLYLINE    4
#define HS_STICKLINE   5 
#define HS_HLTHBQ	    6 
#define HS_BUYSELL	    7 
#define HS_DRAWSL	    8
 
#ifndef hx_DefMaxMin
#define hx_DefMaxMin
static long hx_DefMaxValues		 = 0x80000000;	   // ��Сֵ
static long hx_DefMinValues		 = 0x7fffffff;	   // ���ֵ
static long hx_ReturnPointValues    = 0x5fffffff;     // ������ֵ����ָ��
#endif

// ��ʽ����
#define HS_EXP_DEFAULT_TECH 0x0001 // ȱʡָ��/����ָ��
#define HS_EXP_USES		 0x0002 // ��ʹ����
#define HS_EXP_DELETE		 0x0004 // ����ɾ��
#define HS_EXP_MAINCHART	 0x0008 // ��ͼ����
#define HS_EXP_SUBCHART	 0x0010 // ��ͼ
#define HS_EXP_USEREDIT	 0x0020 // �Ա๫ʽ
#define HS_EXP_SYSTEM  	 0x0040 // ϵͳ��ʽ
#define HS_EXP_TEMPIN		 0x8000 // ��ʱ����Ĺ�ʽ
#define HS_EXP_FINANCE		 0x10000000 // ������������Ĺ�ʽ
#define HS_EXP_PROTECTED	 0x20000000 // ��ʽ�Ƿ�Ϊ������

#define HS_EXP_OUTTYPE(x)   ( (x) & 0xFF000000 ) // ��ʽ���mask

#define HS_HxDataSource_Left_ChuQuan	    0x00020000 // ��߳�Ȩ
#define HS_HxDataSource_Back_ChuQuan		0x00040000 // ����Ȩ
#define HS_HxDataSource_Read_CaiWu_Data     0x00060000 // ��ȡ��������

#define HX_NOTTABLE			   0x00080000  // Ϊ��ͼ���ʽ,���ǲ��ڱ������ʾ
#define HX_NOTEXPRESSDRAW	   0x00100000  // Ϊ��ͼ���ʽ,���ǲ���ͼ
#define HX_EXPRESSDRAW		   0x00200000  // Ϊ��ͼ���ʽ
#define HX_EXPRESSDRAW_NONAME  0x00400000  // Ϊ��ͼ���ʽ(û������)

#define HX_EXPRESSSENTENCE	   0x00800000  // Ϊ�����ʽ
#define HX_EXPRESSNUMBER	   0x01000000  // Ϊ��ֵ���ʽ
#define HX_EXPRESSFUNCTION	   0x02000000  // Ϊ�������ʽ
#define HX_EXPRESSREFERENCE	   0x04000000  // Ϊ���������ʽ
#define HX_UNION_EXPRESS	   0x08000000  // Ϊ��ϱ��ʽ
#define HX_UNKNOWFUNPARAM	   0x10000000  // Ϊ�������ʽ,���������
#define HX_NOTCALCMAXMIN	   0x40000000  // �����������Сֵ

#define hs_NameProperty_NotDrawName		0x0100 // ����������
#define hs_NameProperty_NotDrawExp		0x0200 // �����Ʋ���
#define hs_NameProperty_DrawRectangle   0x0400 // �����ı��߿�
#define hs_NameProperty_DrawDescribe	0x0800 // ������������
#define hs_NameProperty_NotCompile	    0x1000 // ��ǰ��ʽû�н���/����


#define hxOpenExpress_Edit				     0x0100    //�޸Ĺ�ʽ
#define hxOpenExpress_Notes					 0x0200	   //ע��
#define hxOpenExpress_Eidolon				 0x0300	   //����

#define hxShowAlignWindow					 0x0400
#define hxShowKeyboardWindow				 0x0600

#define hxShowByExpressType					 0x0700 // �ɹ�ʽ��������ʾ��ʽ
#define hxShowByExpressType_AddTech			 0x0010 // ָ�����
#define hxShowByExpressType_TechSort		 0x0020 // ָ������
#define hxShowByExpressType_AddTechSort		 0x0030 // ���ָ������

#define hxShowReturnByExpressType			 0x0800 // �ɹ�ʽ��������ʾ��ʽ,������ѡ�й�ʽ

#define hxOpenExpressByConditionStockType	 0x0900 // ����ѡ�ɴ�
#define hxOpenExpressByConditionDingZhi		 0x0910 // ����ѡ�ɴ�
#define hxOpenExpressByConditionZhiBiao		 0x0920 // ָ��ѡ�ɴ�
#define hxOpenExpressByConditionZhiNeng		 0x0930 // ����ѡ�ɴ�
#define hxOpenExpressByConditionChaJian		 0x0940 // ���ѡ�ɴ�
#define hxOpenExchangeSys            		 0x0950 // ����ϵͳ
#define hxOpenExchangeYouSel				 0x0960	// ������ѡ

#define hxOpenExpressBySortStockType		 0x0A00 // ָ�������
#define hxOpenBlockAnalysis					 0x0B00 // ������
#define	hxOpenUniteAnalysis					 0x0C00 // ��������
#define	hxOpenFindDataDlg					 0x0D00 // �򿪲���dlg
#define	hxOpenAlarmManagerDlg				 0x0E00 // ��Ԥ������Ի���
#define hxOpenAddStkToAlarm					 0x1000 // �򿪼��뵽Ԥ���Ի���

#define hsOpenExpress_Create				 0x0F00 // �����½�����ָ�� 

#define hxSaveLoadTree						 0x2000 // ���湫ʽ��
#define hxSettingExpressParent				 0x3000 // ���ù�ʽdll������

#define hsOpenStrategyTestSetting			 0x4000	// �򿪲��Խ��ײ����趨����


enum HSAllocType {charType,InfoIndexType,InfoIndexSubType,HSDrawOtherDataType,HSDoubleType,HSCString,HSCStringCopy};
enum ExpressType { none,
		Tech = 0x01,Condition = 0x02,Exchange = 0x04,MoreKLine = 0x08,
		ExpressBin = 0x10, Strategy = 0x20, Union,JiBenMian};

typedef void* ( *HSExternAlloc )( long nCurSize,int nType  );
typedef void  ( *HSExternFree )( void* pData, int nType  );



struct HSDrawOtherData
{
 	BYTE	 m_bType1;
 	BYTE	 m_bType2;
 	BYTE	 m_bType3;
 	BYTE	 m_bType4; 
	double	 m_dValue;    // ���� 
	char     m_cText[50];
	
 	HSDrawOtherData()
 	{
 		Empty();
	}

 	~HSDrawOtherData()
 	{
 	}
 	void Copy( HSDrawOtherData* pData );
 	void Empty()
 	{
 		m_bType1 = 0;
 		m_bType2 = 0;
 		m_bType3 = 0;
 		m_bType4 = 0; 
 		m_dValue = 0;
		memset(m_cText,0,sizeof(m_cText));
 	}
 };
 
 struct HSDrawData
 {
	 // ����
	 double   m_dData;       
	 HSDrawOtherData*   m_pOtherData;  
	 static HSExternAlloc m_pHSExternAlloc;
	 static HSExternFree  m_pHSExternFree;

	 //����
	 HSDrawData()
	 {
		 m_dData = hx_DefMinValues;
		 m_pOtherData = NULL;
	 }
	 //���� ������������
	 HSDrawData(HSDrawData& sData)
	 {
		 m_dData = sData.m_dData;
		 m_pOtherData = NULL;
	 }

	 //���� ����double��������
	 HSDrawData(double dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }

	 ~HSDrawData()
	 {
		FreeOther();
	 }
	 void init()
	 {
		 m_dData = hx_DefMinValues;
	 }
	 void FreeOther();
	 HSDrawOtherData* AllocOther(HSDrawOtherData* pData,BOOL bForce = FALSE);

	 void Copy(HSDrawData* pData,BOOL bMsg = FALSE);

#ifdef VC_7_0
	 //���� ����long��������
	 HSDrawData(long lData)
	 {
		 m_dData   = lData;
		 m_pOtherData = NULL;
	 }
	 //���� ����int��������
	 HSDrawData(int dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }
	 //���� ����BYTE��������
	 HSDrawData(BYTE dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }
	 //���� ����unsigned long��������
	 HSDrawData(unsigned long dData)
	 {
		 m_dData   = dData;
		 m_pOtherData = NULL;
	 }
#endif

	 //������= ������double ��������
	 const HSDrawData& operator=(double dData)
	 {
		 m_dData = dData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

	 //������=, ������HSDrawData ��������
	 const HSDrawData& operator=(HSDrawData& dData)
	 {
		 m_dData	= dData.m_dData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

 #ifdef VC_7_0

	 //������=, ������long ��������
	 const HSDrawData& operator=(long lData)
	 {
		 m_dData = lData;
		 //m_pOtherData = NULL;
		 return *this;
	 }
	 //������=, ������int ��������
	 const HSDrawData& operator=(int nData)
	 {
		 m_dData = nData;
		 //m_pOtherData = NULL;
		 return *this;
	 }
	 //������=, ������BYTE ��������
	 const HSDrawData& operator=(BYTE bData)
	 {
		 m_dData = bData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

	 const HSDrawData& operator=(unsigned long bData)
	 {
		 m_dData = bData;
		 //m_pOtherData = NULL;
		 return *this;
	 }

#endif

	 ///////////
	 //������ +����:m_dData += dData.m_dData;
	 const HSDrawData& operator+=(const HSDrawData& dData)
	 {
		 m_dData += dData.m_dData;
		 return *this;
	 }
	 //������ +
	 const HSDrawData& operator+=(double dData)
	 {
		 m_dData += dData;
		 return *this;
	 }


	 //������  - ����:m_dData -= dData.m_dData;
	 const HSDrawData& operator-=(const HSDrawData& dData)
	 {
		 m_dData -= dData.m_dData;
		 return *this;
	 }
	 const HSDrawData& operator-=(double dData)
	 {
		 m_dData -= dData;
		 return *this;
	 }

	 //������ * ����:m_dData *= dData.m_dData;
	 const HSDrawData& operator*=(const HSDrawData& dData)
	 {
		 m_dData *= dData.m_dData;
		 return *this;
	 }
	 const HSDrawData& operator*=(double dData)
	 {
		 m_dData *= dData;
		 return *this;
	 }

	 //������ / ����:m_dData /= dData.m_dData;
	 const HSDrawData& operator/=(const HSDrawData& dData)
	 {
		 m_dData /= dData.m_dData;
		 return *this;
	 }
	 const HSDrawData& operator/=(double dData)
	 {
		 m_dData /= dData;
		 return *this;
	 }

	 bool operator!=(double dData)
	 {
		 return (m_dData != dData);
	 }

	 bool operator!=(HSDrawData dData)
	 {
		 return (m_dData != dData.m_dData);
	 }

	 //������ ��������ת��
	 operator double()
	 {
		 return m_dData;
	 }
	 //������ ��������ת��
	 operator long()
	 {
		 return (long)m_dData;
	 }
	 //������ ��������ת��
	 operator int()
	 {
		 return (int)m_dData;
	 }
	 //������ ��������ת��
	 operator BYTE()
	 {
		 return (BYTE)m_dData;
	 }
	 //������ ��������ת��
	 operator bool()
	 {
		 return m_dData?true:false;
	 }
	 operator unsigned long()
	 {
		 return (unsigned long)m_dData;
	 }	
 };

typedef  HSDrawData HSDouble;

 //���������� == ����true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData == s2.m_dData);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData == s2);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(double s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData == s2);
 }
 inline bool AFXAPI operator==(const HSDrawData& s1, unsigned long s2)
 {
 	return (s1.m_dData == s2);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(long s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData == s2);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(int s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData == s2);
 }
 //���������� == ����true or false
 inline bool AFXAPI operator==(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 == s2.m_dData);
 }
 
 //���������� != ����true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData != s2.m_dData);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData != s2);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(double s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData != s2);
 }
 inline bool AFXAPI operator!=(const HSDrawData& s1, unsigned long s2)
 {
 	return (s1.m_dData != s2);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(long s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData != s2);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(int s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData != s2);
 }
 //���������� != ����true or false
 inline bool AFXAPI operator!=(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 != s2.m_dData);
 }
 
 //���������� < ����true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData < s2.m_dData);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData < s2);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(double s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData < s2);
 }
 inline bool AFXAPI operator<(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData < s2);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(long s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData < s2);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(int s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData < s2);
 }
 //���������� < ����true or false
 inline bool AFXAPI operator<(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 < s2.m_dData);
 }
 
 //���������� > ����true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData > s2.m_dData);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData > s2);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(double s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData > s2);
 }
 inline bool AFXAPI operator>(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData > s2);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(long s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData > s2);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(int s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData > s2);
 }
 //���������� > ����true or false
 inline bool AFXAPI operator>(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 > s2.m_dData);
 }
 
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData <= s2.m_dData);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(double s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData <= s2);
 }
 inline bool AFXAPI operator<=(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(long s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(int s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData <= s2);
 }
 ///���������� <= ����true or false
 inline bool AFXAPI operator<=(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 <= s2.m_dData);
 }
 
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData >= s2.m_dData);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData >= s2);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(double s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData >= s2);
 }
 inline bool AFXAPI operator>=(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData >= s2);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(long s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData >= s2);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(int s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData >= s2);
 }
 //���������� >= ����true or false
 inline bool AFXAPI operator>=(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 >= s2.m_dData);
 }
 
 //���������� + ����HSDrawData
 inline HSDrawData AFXAPI operator+(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData + string2.m_dData);
 }
 //���������� + ����HSDrawData
 inline HSDrawData AFXAPI operator+(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData + ch);
 }
 //���������� + ����HSDrawData
 inline HSDrawData AFXAPI operator+(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch + string.m_dData);
 }
 //���������� + ����HSDrawData
 inline HSDrawData AFXAPI operator+(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData + lpsz);
 }
 inline HSDrawData AFXAPI operator+(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData + lpsz);
 }
 //���������� + ����HSDrawData
 inline HSDrawData AFXAPI operator+(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz + string.m_dData);
 }
 
 //���������� - ����HSDrawData
 inline HSDrawData AFXAPI operator-(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData - string2.m_dData);
 }
 //���������� - ����HSDrawData
 inline HSDrawData AFXAPI operator-(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData - ch);
 }
 //���������� - ����HSDrawData
 inline HSDrawData AFXAPI operator-(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch - string.m_dData);
 }
 //���������� - ����HSDrawData
 inline HSDrawData AFXAPI operator-(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData - lpsz);
 }
 inline HSDrawData AFXAPI operator-(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData - lpsz);
 }
 //���������� - ����HSDrawData
 inline HSDrawData AFXAPI operator-(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz - string.m_dData);
 }
 
 //���������� * ����HSDrawData
 inline HSDrawData AFXAPI operator*(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData * string2.m_dData);
 }
 //���������� * ����HSDrawData
 inline HSDrawData AFXAPI operator*(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData * ch);
 }
 //���������� * ����HSDrawData
 inline HSDrawData AFXAPI operator*(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch * string.m_dData);
 }
 //���������� * ����HSDrawData
 inline HSDrawData AFXAPI operator*(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData * lpsz);
 }
 inline HSDrawData AFXAPI operator*(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData * lpsz);
 }
 //���������� * ����HSDrawData
 inline HSDrawData AFXAPI operator*(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz * string.m_dData);
 }
 
 //���������� / ����HSDrawData
 inline HSDrawData AFXAPI operator/(const HSDrawData& string1,const HSDrawData& string2)
 {
 	return HSDrawData(string1.m_dData / string2.m_dData);
 }
 //���������� / ����HSDrawData
 inline HSDrawData AFXAPI operator/(const HSDrawData& string, double ch)
 {
 	return HSDrawData(string.m_dData / ch);
 }
 //���������� / ����HSDrawData
 inline HSDrawData AFXAPI operator/(double ch, const HSDrawData& string)
 {
 	return HSDrawData(ch / string.m_dData);
 }
 //���������� / ����HSDrawData
 inline HSDrawData AFXAPI operator/(const HSDrawData& string, long lpsz)
 {
 	return HSDrawData(string.m_dData / lpsz);
 }
 inline HSDrawData AFXAPI operator/(const HSDrawData& string,unsigned long lpsz)
 {
 	return HSDrawData(string.m_dData / lpsz);
 }
 //���������� / ����HSDrawData
 inline HSDrawData AFXAPI operator/(long lpsz, const HSDrawData& string)
 {
 	return HSDrawData(lpsz / string.m_dData);
 }
 
 ///////////////
 //���������� ! ����true or false
 inline bool AFXAPI operator!(const HSDrawData& s1)
 {
 	return !s1.m_dData;
 }
 
 //���������� || ����true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData || s2.m_dData);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData || s2);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(double s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData || s2);
 }
 inline bool AFXAPI operator||(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData || s2);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(long s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData || s2);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(int s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData || s2);
 }
 //���������� || ����true or false
 inline bool AFXAPI operator||(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 || s2.m_dData);
 }
 
 //���������� && ����true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, const HSDrawData& s2)
 {
 	return (s1.m_dData && s2.m_dData);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, double s2)
 {
 	return (s1.m_dData && s2);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(double s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, long s2)
 {
 	return (s1.m_dData && s2);
 }
 inline bool AFXAPI operator&&(const HSDrawData& s1,unsigned long s2)
 {
 	return (s1.m_dData && s2);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(long s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, int s2)
 {
 	return (s1.m_dData && s2);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(int s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(const HSDrawData& s1, BYTE s2)
 {
 	return (s1.m_dData && s2);
 }
 //���������� && ����true or false
 inline bool AFXAPI operator&&(BYTE s1, const HSDrawData& s2)
 {
 	return (s1 && s2.m_dData);
 }
 
// ����ʱ�����ⲿ����
 struct CCalcExternParam
 {
 	long		m_nBeginDate; // ��ʼ����,��19901020
 	long	    m_nEndDate;   
 	CodeInfo    m_sCode;	  // ����
 	char 		m_szBlock[BLOCK_NAME_LENGTH]; // �������
 
 	CCalcExternParam()
 	{
 		EmptyBlock();
 	}
 
 	void    EmptyBlock() { memset(m_szBlock,0,sizeof(m_szBlock)); }
 	CString GetBlockName() { return CString(m_szBlock).Left(BLOCK_NAME_LENGTH); }
 	void    SetBlockName(CString pData)
 	{
 		EmptyBlock();
 		if( pData.IsEmpty() )
 			return;
 
 		strncpy(m_szBlock,pData,sizeof(m_szBlock));
 	}
 };

/// *************************************************************
// *	�� �� �ƣ�	CLineProperty
// *	�� �� ����	��ͼ����(���̺���������õ�)
// ***************************************************************/
 struct CLineProperty
 {
 public:
 	CCalcExternParam* m_pCalcParam; // �ⲿָ�룬���಻��ɾ��
 public:
 	HSDouble *		m_pData;    		// ����
 	int				m_nSize;			// ���ݳ���
 
	char		    m_strName[20];  		// ���� 
 	DWORD		    m_dwStyle;  		// ���Ʒ��
 	ColorProperty   m_lColor;   		// ��Ϊ������ʱ����ɫ
 
 	HSDouble		m_lMinValue;		// ��С
 	HSDouble		m_lMaxValue;		// ���
 
  	void*		    m_pCurVal;			 // ��ǰ�Ĺ�ʽ
  	void*	        m_pDataSource;		 // ���ݰ�װ 
 	int				m_nColID;			 // ��id���ڹ�ʽ�����ʹ�� 
 	char			m_strFormat[64];		 // �������ʹ�ã������ʽ��

	void			init();
 	void			FreeStr();
 	void			AllocStr(CString* pStr);
 	CString			GetFormat(HSDouble& dValue);
 
 public:
 	// ��ʼ��
 	CLineProperty();
 	// �����ռ�õ���Դ
 	~CLineProperty();
 
 	/******************************************************************
 	*	�������ܣ�	����m_pData[nDataPos]��BOOLֵ
 	*	����������	int nDataPos : [in] Ҫ�󷵻�BOOLֵ������λ��
 	*	�� �� ֵ��	BOOL : ��Ӧ��������Ƿ�Ϊ��
 	*	����������	
 	*	��    �ߣ�	
 	*	������ڣ�
 	*	�޸�˵����	
 	*****************************************************************/
 	BOOL IsTrue(int nDataPos) ;
 	BOOL IsEmpty();
 
 	/******************************************************************
 	*	�������ܣ�	����nCurSize*sizeof(HSDouble)���ռ��m_pData
 	*	����������	int nCurSize: [in] �µĳ���
 	*				int nOldSize: [in] �ɵĳ���
 	*	�� �� ֵ��	void 
 	*	����������	
 	*	��    �ߣ�	
 	*	������ڣ�
 	*	�޸�˵����	
 	*****************************************************************/
 	void Alloc(int nCurSize,int nOldSize);
 
 	void Free();
 	// ��Ĭ�ϵ������Сֵ
 	void DefMinMax();
 
 public:
 	// ����ж�
 	BOOL	IsStyle(long dwStyle);
 	// �ӷ��
 	void	AddStyle(long dwStyle);
 	// ɾ�����
 	void	RemoveStyle(long dwStyle);
 	// ���÷��
 	void    SetStyle(long dwStyle);
 	// ���������С�ٷֱ�
 	// ע�⣺ʹ�ô˺���ǰ�������ж�nBegin��nEnd�ĺϷ���
 	BOOL CalcUpDownPercent(int nBegin, int nEnd, double& dUp, double& dDown, double dBaseIntput = (double)hx_DefMinValues);
 };


 /*************************************************************
 *	�� �� �ƣ�	CTValues
 *	�� �� ����	�����������ķ�װ��ģ��
 ***************************************************************/
 template <class TValues>
 struct CTValues
 {
	 TValues* m_pValue;	// ָ���ⲿ�ռ�,���ﲻ�����
	 long	 m_lStart;	// ��ʼ�ڵ�
	 long	 m_lSize;	// �ܳ���

	 CTValues()									     { m_pValue = NULL;m_lStart = 0;m_lSize = 0; }
	 CTValues(TValues* pValue,long lSize)		     { m_pValue = pValue;m_lStart = 0;m_lSize = lSize; }
	 CTValues(TValues* pValue,long lStart,long lSize) { m_pValue = pValue;m_lStart = lStart;m_lSize = lSize; }
	 void operator = (const CTValues& values)	     { m_pValue = values.m_pValue;m_lStart = values.m_lStart;m_lSize = values.m_lSize; }
	 void Empty()									 { m_pValue = NULL;m_lStart = 0;m_lSize = 0; }
	 BOOL IsEmpty()									 { return (m_pValue != NULL && m_lSize > 0); }	
 };


 // �������ݷ���
 struct RetDiskData
 {
	 void* m_pData;
	 long  m_lSize;

	 RetDiskData()
	 {
		 memset(this,0,sizeof(RetDiskData));
	 }

	 ~RetDiskData()
	 {
		 Free();
	 }

	 BOOL IsValid() { return ( m_pData != NULL && m_lSize > 0 ); }
	 BOOL IsEmpty() { return ((m_pData == NULL) || (m_lSize <= 0)); }

	 void Alloc( int nCurSize  )
	 {
		 if( nCurSize <= 0 )
		 {
			 Free();
			 return;
		 }		
		 if( m_lSize != nCurSize )
		 {
			 Free();
			 m_pData = new char[sizeof(StockDay)*nCurSize];
			 m_lSize = nCurSize;
		 }
		 memset( m_pData,0,sizeof(StockDay)*m_lSize);
	 }
	 void Free()
	 {
		 if ( m_pData != NULL )
		 {
			 delete[] m_pData;
			 m_pData = NULL;
		 }
		 m_lSize = 0;
	 }
 };

 struct HSRetFileData
 {
	 char* m_pData;
	 long  m_lSize;

	 HSRetFileData()
	 {
		 memset(this,0,sizeof(HSRetFileData));
	 }
	 ~HSRetFileData()
	 {
		 Free();
	 }

	 inline void Alloc( int nCurSize  )
	 {		
		 if( nCurSize <= 0 )
		 {
			 Free();
			 return;
		 }		
		 if( m_lSize != nCurSize )
		 {
			 Free();

			 if( HSDrawData::m_pHSExternAlloc )
			 {
				 m_pData = (char*)(*HSDrawData::m_pHSExternAlloc)
					 ( (long)nCurSize,charType );
				 if( m_pData )
				 {
					 m_lSize = nCurSize;
				 }
			 }
			 else
			 {
				 m_pData = new char[nCurSize];
				 memset( m_pData,0,m_lSize);
				 m_lSize = nCurSize;
			 }
		 }
		 else
		 {
			 memset( m_pData,0,m_lSize);
		 }
	 }

	 inline void Free()
	 {
		 if( HSDrawData::m_pHSExternFree )
		 {
			 (*HSDrawData::m_pHSExternFree)(m_pData,charType);
			 m_pData = NULL;
			 m_lSize = 0;
		 }
		 else if ( m_pData != NULL )
		 {
			 delete[] m_pData;
			 m_pData = NULL;
			 m_lSize = 0;
		 }
	 }
	 inline BOOL IsValid() { return (m_pData != NULL && m_lSize > 0); }

	 inline void Append(HSRetFileData* pRetData) //׷������,Xiongzb,2002-03-15
	 {
		 if(!pRetData )
			 return;
		 Append(pRetData->m_pData, pRetData->m_lSize);
	 }

	 void Append(char* pData, long lDataSize) //׷������,Xiongzb,2004-03-30
	 {
		 if(!pData || lDataSize <= 0)
			 return;

		 if(IsValid())
		 {
			 long lSize = m_lSize;

			 char* pTotalData = new char[lSize];
			 memcpy(pTotalData, m_pData, lSize);

			 Alloc(lSize + lDataSize);

			 memcpy(m_pData, pTotalData, lSize);
			 memcpy(m_pData + lSize, pData, lDataSize);

			 delete[] pTotalData;
			 return;
		 }

		 Alloc(lDataSize);
		 memcpy(m_pData, pData, m_lSize);
	 }
 };

 struct   RealTimeDataSource
 {
	 StockUserInfo	  m_stStockInfo;
	 StockOtherData    m_sOthData;
	 union
	 {
		 HSIndexRealTime  m_stIndexData;
		 HSStockRealTime  m_stStockData;
		 HSQHRealTime	 m_qhNowData;
	 };
 };
//��ʽ���� 
 struct ExpPropery
 {
	 CString		    m_strName;		// ����
	 CString		    m_strDescribe;  // ��������
	 char				m_dExpType;     // ��ʽ����
	 DWORD				m_bStyle;       //��ʽ�ָ� ������ͼ���ӣ�
 };
 //��Ȩ��۸� = ��Ȩǰ�۸� * m_fMulit + m_fAdd
 struct ChuQuanData
 {
	 enum { NotCalc,Calc/* ����*/ };

	 char	m_cMask;		//ѡ��

	 unsigned long	m_lDate;	//��Ȩ��  
	 float	m_fMulit;			//��˵�����
	 float	m_fAdd;				//��ӵ�����
	 float	m_fSGBL;			//�͹ɱ���
	 float	m_fPGBL;			//��ɱ���
	 float	m_fPGJ;				//��ɼ�
	 float	m_fXJHL;			//�ֽ����
 };


 // �����Сֵ
 struct CDispMaxMinValue
 {
	 long	  m_nPos; // λ��
	 HSDouble  m_lValue; // ������С��ֵ
	 CDispMaxMinValue();
	 void Empty();
	 BOOL IsFind(long nPos);
 };

 /*************************************************************
 *	�� �� �ƣ�	CNameProperty
 *	�� �� ����	��������
 ***************************************************************/
 struct CNameProperty
 {
 public:
	 CString		    m_strName;		// ����
	 CString		    m_strDescribe;  // ��������
	 CString		    m_strExp;		// ����
	 WORD				m_dStyle;		// ���Ʒ��
	 char				m_dExpType;     // ��ʽ����
	 ColorProperty		m_lColor;		// ��ɫ

	 CNameProperty();

	 // �����Ƿ�Ϊ��
	 BOOL IsEmpty();
	 // ���ֱȽ�
	 int  CompareNoCase( LPCTSTR lpsz );

	 // ������
 public:
	 BOOL	IsStyle(WORD dStyle);
	 void	AddStyle(WORD dStyle);
	 void	RemoveStyle(WORD dStyle);
	 void   SetStyle(WORD dStyle);
 };


 class CHSFinanceData : public CurrentFinanceData
 {
 public:
	 CHSFinanceData();
	 CHSFinanceData(CurrentFinanceData* pFinanceData);

	 //���ݲ������ݵ�ID�ŷ������Ӧ�Ĳ�������
	 HSDouble GetDataByID(UINT nID);
	 //���ݲ������ݵ����λ�÷������Ӧ�Ĳ�������
	 HSDouble GetDataByPos(int nPos);

	 HSDouble CAPITAL(CodeInfo* pCode);

	 //��ձ��ṹ
	 void Empty()   {	memset(this,0,sizeof(HisFinanceData));	 }

	 //��������Ƿ�Ϊ��
	 BOOL IsEmpty() { return (m_nDate == 0) ; }
 };

//	��Ʊ��ʵʱ����
class CRealTimeDataSource : public RealTimeDataSource
{
public:
	//	���캯��
	CRealTimeDataSource();
	

	/******************************************************************
	*	�������ܣ�	�������ݵ�λ�÷��ظ�ʵʱ����
	*	����������	int			nPos		��ָ��������λ��
	*				double		nPriceUnit	���۸񾫶ȵ�λ
	*				char		cType		��֤ȯ����
	*	�� �� ֵ��	HSDouble�������ص�ʵʱ����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	HSDouble GetDataByPos(int nPos,double nPriceUnit,char cType = 0);
	
	//	�������
	void Empty()	
	{	
		memset(this,0,sizeof(RealTimeDataSource));

		m_dHuanShoulv = 0; // ������
		m_dShiYinglv  = 0;  // ��ӯ��
		m_nTolalTime  = 0;  // ��ʱ��
	}

	//	����Ƿ�Ϊ������
	BOOL IsEmpty();

	// ������������
public:
	double m_dHuanShoulv; // ������
	double m_dShiYinglv;  // ��ӯ��
	int    m_nTolalTime;  // ��ʱ��
	double m_dQuarter;	  // ���� //20090605 YJT �޸���ӯ���㷨

};
#pragma pack() 
 #endif // _DRAWDATA_H_

