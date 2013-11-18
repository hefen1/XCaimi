
/*******************************************************************************
* Copyright (c)2010, ������Ϣ�������޹�˾
* All rights reserved.
*
* �ļ����ƣ�hsds_comudata.h
* �ļ���ʶ���°�����ͨѶ�ṹ
* ժ    Ҫ���°�����ͨѶ�ṹ
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

#include "hsstructnew.h"		

#pragma	pack(1)

//////////////////////////////////////////////////////////////////////////
/* ����/�������� DEFINE BEGIN*/

#define RT_BEGIN			    		0x0100
#define RT_END				    		0x8FFF

#define RT_COMPASKDATA					0x8FFE  /* �����������						*/
#define RT_ZIPDATA						0x8001  /* ѹ�����ذ�����					*/

#define RT_JAVA_MARK					0x0010	/* JAVA��¼ | RT_LOGIN_*			*/
#define RT_WINCE_MARK					0x0020	/* WINCE��¼ | RT_LOGIN_*			*/
#define RT_NOTZIP						0x0040	/* ��ʹ��ѹ�����ݴ���				*/
#define RT_DEBUG						0x0080	/* ��ʹ��ѹ�����ݴ���				*/

#define RT_INITIALINFO          		0x0101  /* �ͻ����������г�ʼ��				*/
#define RT_LOGIN						0x0102  /* �ͻ��˵�¼���������				*/
#define RT_SERVERINFO					0x0103  /* ��վ��Ϣ							*/
#define RT_BULLETIN						0x0104	/* ��������(����)					*/
#define RT_PARTINITIALINFO				0x0105	/* �ͻ������ݲ��ֳ�ʼ��				*/
#define RT_ETF_INIT		          		0x0106  /* 52ֻ��Ʊ����(ETF)				*/
#define RT_LOGIN_HK						0x0107	/* �ͻ��˵�¼�۹ɷ�����				*/
#define RT_LOGIN_FUTURES				0x0108	/* �ͻ��˵�¼�ڻ�������				*/
#define RT_LOGIN_FOREIGN				0x0109	/* �ͻ��˵�¼��������				*/
#define RT_LOGIN_WP						0x010A	/* �ͻ��˵�¼���̷�����				*/
#define RT_LOGIN_INFO					0x010B	/* �ͻ��˵�¼��Ѷ������				*/
#define RT_CHANGE_PWD					0x010C	/* �޸�����							*/
// ghm add level2 ��½����
#define RT_LOGIN_LEVEL					0x010D  /* level2��½����					*/
#define RT_SERVERINFO2					0x010E  /* ��վ��Ϣ2						*/
#define RT_VERIFYINFO					0x010F  /* �û���֤��Ϣ����					*/
//add by fangly
#define RT_LOGIN_CENTER				    0x0110  /* center��½����					*/
// add by lxqi 20081016
#define RT_LOGIN_FTP					0x0111  /* FTP��¼��������					*/
#define RT_MODIFY_PWD                   0x0112  /*�޸�����*/
#define RT_MODIFY_LV2PWD				0x0112  /* �޸�Level2�û����� */
#define RT_DISCONNLEVEL2				0x0113	/* level2���� */
#define RT_STOCKINFOLIST				0x0114	/* ��������б�						*/

#define RT_REALTIME						0x0201  /* ���鱨�۱�:1-6Ǭ¡������			*/
#define RT_DYNREPORT					0x0202  /* ǿ������;ָ������;���Ű�����;�������;����ƹ�Ʊ�б�����;Ԥ��	*/
#define RT_REPORTSORT					0x0203  /* �������۱�:61-66�����������		*/
#define RT_GENERALSORT					0x0204  /* �ۺ���������:81-86				*/
#define RT_GENERALSORT_EX				0x0205  /* �ۺ���������:81-86�����Զ����������	*/
#define RT_SEVER_EMPTY					0x0206  /* �����������ݷ��ؿհ�	*/
#define RT_SEVER_CALCULATE				0x0207  /* �������������ݰ�,������ͣ����ͣ	*/
#define RT_ANS_BYTYPE					0x0208  /* �������ͷ������ݰ�	*/
#define RT_QHMM_REALTIME				0x0209  /* �ڻ�������	*/
#define RT_LEVEL_REALTIME				0x020A  /* level	*/
#define RT_CLASS_REALTIME				0x020B  /* ���ݷ�������ȡ���鱨��*/
// ghm add level2�������ݲ�ѯ��Ϣ
#define RT_LEVEL_ORDERQUEUE				0x020C  /* level2��������*/
#define RT_LEVEL_TRANSACTION			0x020D  /* LEVEL2��ʳɽ�*/
#define RT_QHARB_REALTIME				0x020E	/* �ڻ��������� maxy add */
#define RT_REPORTSORT_EXT				0x0212  /* �������۱���չ:61-66�����������	*/

#define RT_TREND						0x0301  /* ��ʱ����							*/
#define RT_ADDTREND						0x0302  /* ����ͼ���ӡ����ͬ��				*/
#define RT_BUYSELLPOWER					0x0303  /* ��������							*/
#define RT_HISTREND						0x0304  /* ��ʷ����;���շ�ʱ;��Сͼ�·�ʱ����*/
#define RT_TICK							0x0305  /* TICKͼ							*/
#define RT_ETF_TREND					0x0306  /* ETF��ʱ����						*/
#define RT_ETF_NOWDATA					0x0307  /* ETFʱʱ����						*/
#define RT_ETF_TREND_TECH				0x0308  /* ETFtech��ʱ����						*/
#define RT_HISTREND_INDEX				0x0309  /* ���ڴ�������-��ʷ����;���շ�ʱ;��Сͼ�·�ʱ����*/
#define RT_AUTOARBPUSH					0x030A	/* �ڻ��������� maxy add */
#define RT_HISTREND_EXT                 0x030C


#define RT_TECHDATA						0x0400  /* �̺����							*/
#define RT_FILEDOWNLOAD					0x0401	/* �ļ������̺��������أ�			*/
#define RT_TECHDATA_EX					0x0402	/* �̺������չ -- ֧�ֻ���ֵ*/
#define RT_DATA_WEIHU					0x0403	/* ����ά������ */
#define RT_FILEDOWNLOAD2				0x0404	/* ���ط�����ָ��Ŀ¼�ļ�			*/
#define RT_FILED_CFG					0x0405	/* �����ļ�����/����			    */
#define RT_FILL_DATA					0x0406	/* ���ߴ��� */
#define RT_TECHDATA_BYPERIOD			0x0407	/* �̺������չ -- ֧�ֲ�ͬ����ת��*/
#define RT_ONE_BLOCKDATA				0x0408	/* һ������������� */
#define RT_TECHDATA_INCREMENT			0x0409	/* �̺������չ -- ������������    */
#define RT_TECHDATA_RANGE			    0x0410	/* �ֶ��̺����*/
#define RT_TECHDATA_BIG                 0x0411  /* �̺���������������Լ�����      */


#define RT_TEXTDATAWITHINDEX_PLUS		0x0501	/* ������Ѷ��������					*/
#define RT_TEXTDATAWITHINDEX_NEGATIVE	0x0502	/* ������Ѷ��������					*/
#define RT_BYINDEXRETDATA				0x0503  /* ��Ѷ��������						*/
#define RT_USERTEXTDATA		    		0x0504  /* �Զ�����Ѷ������˵��ȣ�		*/
#define RT_FILEREQUEST					0x0505  /* �����ļ��ļ�						*/
#define RT_FILESimplify					0x0506  /* �����ļ�����						*/
#define RT_ATTATCHDATA					0x0507  /* ��������							*/
#define RT_PROMPT_INFO					0x0508	/* ���������õ���ʾ��Ϣ				*/

#define RT_STOCKTICK					0x0601  /* ���ɷֱʡ�������ϸ�ķֱ�����		*/
#define RT_BUYSELLORDER 				0x0602  /* ����������						*/
#define RT_LIMITTICK            		0x0603  /* ָ�����ȵķֱ�����				*/
#define RT_HISTORYTICK					0x0604  /* ��ʷ�ķֱ�����					*/
#define RT_MAJORINDEXTICK				0x0605  /* ������ϸ							*/
#define RT_VALUE						0x0606	/* ��Сͼ��ֵ��						*/
#define RT_BUYSELLORDER_HK 				0x0607  /* ����������(�۹ɣ�				*/		
#define RT_BUYSELLORDER_FUTURES			0x0608  /* ����������(�ڻ���				*/		
#define RT_VALUE_HK						0x0609	/* ��Сͼ��ֵ��(�۹�),��СͼҲ��������*/		
#define RT_VALUE_FUTURES				0x060A	/* ��Сͼ��ֵ��(�ڻ�),����СͼҲ��������*/		
#define RT_TOTAL						0x060B	/* �ܳ������ */


#define RT_LEAD							0x0702  /* ��������ָ��						*/
#define RT_MAJORINDEXTREND				0x0703  /* ��������							*/
#define RT_MAJORINDEXADL				0x0704  /* �������ƣ�ADL					*/
#define RT_MAJORINDEXDBBI	    		0x0705  /* �������ƣ����ָ��				*/
#define RT_MAJORINDEXBUYSELL    		0x0706  /* �������ƣ���������				*/
#define RT_SERVERFILEINFO    			0x0707  /* �������Զ�����Ҫ���µ��ļ���Ϣ				*/
#define RT_DOWNSERVERFILEINFO    		0x0708  /* ����-�������Զ�����Ҫ���µ��ļ���Ϣ				*/

#define RT_CURRENTFINANCEDATA			0x0801  /* ���µĲ�������					*/
#define RT_HISFINANCEDATA				0x0802  /* ��ʷ��������						*/
#define RT_EXRIGHT_DATA					0x0803	/* ��Ȩ����							*/
#define RT_HK_RECORDOPTION				0x0804	/* �۹���Ȩ							*/
#define RT_BROKER_HK					0x0805	/* �۹ɾ���ϯλ��ί�����			*/				// �����ǵķ������Ƿ����ɴ�����
#define RT_BLOCK_DATA					0x0806	/* �������							*/
#define RT_STATIC_HK					0x0807	/* �۹ɾ�̬����						*/
#define RT_USER_BLOCK_DATA				0x0810	/* ��̨�Զ���������					*/		//add by lxqi 20090907
#define RT_ONE_BLOCKDATA				0x0408	/* һ������������� */


#define RT_MASSDATA             		0x0901  /* ��								*/
#define RT_SERVERTIME					0x0902  /* ��������ǰʱ��					*/
#define RT_KEEPACTIVE					0x0903  /* ����ͨ�Ű�						*/
#define RT_TEST							0x0904	/* ����ͨ�Ű�						*/
#define RT_TESTSRV						0x0905	/* ���Կͻ��˵��������Ƿ�ͨ��		*/
#define RT_PUSHINFODATA					0x0906	/* ��Ѷʵʱ����						*/

#define RT_AUTOPUSH             		0x0A01	/* ��������							*/		// ��RealTimeData Ϊ CommRealTimeData
#define RT_AUTOPUSHSIMP         		0x0A02	/* ��������				*/					// ��Ϊ��������
#define RT_REQAUTOPUSH					0x0A03  /* ��������,Ӧ���ڣ�Ԥ���������	*/		// ��RealTimeData Ϊ CommRealTimeData
#define RT_ETF_AUTOPUSH					0x0A04  /* ETF����	*/
#define RT_AUTOBROKER_HK				0x0A05	/* ��������							*/
#define RT_AUTOTICK_HK					0x0A06	/* �۹ɷֱ�����						*/
#define RT_AUTOPUSH_QH					0x0A07	/* �ڻ���С����						*/
#define RT_PUSHREALTIMEINFO				0x0A08	/* ʵʱ��������						*/
#define RT_RAW_AUTOPUSH					0x0A09  /* ����Դԭʼ��������	*/
#define RT_AUTOPUSH_RES					0x0A12 /* Ԥ���ĳ������� maxy add */
#define RT_AUTOPUSH_LV2RES				0x0A13 /* Ԥ����level2���� */
#define RT_QHMM_AUTOPUSH				0x0A0A  /* �ڻ�����������	*/
#define RT_LEVEL_AUTOPUSH				0x0A0B  /* level����	*/

#define RT_AUTO_TOTALMAX        0x0A10
#define RT_AUTO_SINGLEMAX       0x0A11
#define RT_LEVEL_TOTALMAX       0x2001
#define RT_LEVEL_SINGLEMAX      0x2002

#define RT_AUTOPUSH_EXT         0x0A0F
#define RT_TREND_EXT            0x030B
#define	RT_REALTIME_EXT         0x020F
// ghm add level2��������������Ϣ
#define RT_LEVEL_BORDERQUEUE_AUTOPUSH	0x0A0C  /* LEVEL2����������������*/
#define RT_LEVEL_SORDERQUEUE_AUTOPUSH	0x0A0D  /* LEVEL2������������������*/
#define RT_LEVEL_TRANSACTION_AUTOPUSH	0x0A0E	/* LEVEL2��ʳɽ�����*/


#define RT_UPDATEDFINANCIALDATA			0x0B01	/* �����Ĳ��񱨱����� */
#define RT_SYNCHRONIZATIONDATA			0x0B02	/* ����ͬ������ */

#define RT_Level_OrderQueue				0x0B03	// �������� - level2 ������������
#define RT_Level_Cancellation			0x0B04	// ���� - ����ί����������10����/������
#define RT_Level_Consolidated			0x0B05	// �ۼ� - ��������/����ί���ۼ���������ǰ10ֻ��Ʊ
#define RT_Level_LargeTick				0x0B06	// �ɽ����
#define RT_HISTORYTICKEx		        0x0B07	//��ʷ�ֱʳɽ���չ����

// 
#define RT_Send_Notice				    0x0C01 // ������
#define RT_Send_ScrollText				0x0C02 // ���������Ϣ
#define RT_Change_program				0x0C03 // ���ķ���������
#define RT_Send_File_Data				0x0C04 // �����ļ���������
#define RT_RequestDBF					0x0C05 // ����DBF�ļ�

#define RT_InfoSend						0x0C06  // ������Ϣ
#define RT_InfoUpdateIndex				0x0C07  // ������Ϣ����
#define RT_InfoUpdateOneIndex			0x0C08  // ����һ����Ϣ����
#define RT_NoteMsgData					0x0C09	/* ���ƶ������ݴ��� */
#define RT_InfoDataTransmit				0x0C0A  // ��֤ת��
#define RT_InfoCheckPurview				0x0C0B  // ����ע����ϸ������Ϣ
#define RT_InfoClickTime				0x0C0C  // �������

#define RT_REPORTSORT_Simple			0x0D01  /* �������۱�:61-66����������򣨾���	*/
#define RT_PARTINITIALINFO_Simple		0x0D02  /* ���뷵��	*/
#define RT_RETURN_EMPTY					0x0D03  /* ���ؿյ����ݰ�	*/
#define RT_InfoDataRailing				0x0D04  // ������Ŀ

#define RT_Srv_SrvStatus				0x0F01 // ��̨��������״̬
#define RT_SRV_SYNC                     0x0F02//�ͻ��˺ͷ�����ά��һ��Sessionͬ��

// ������Ϣ����
#define Notice_Option_WinCE				0x0001 // ������Ϣֻ��WinCE�û�//
#define Notice_Option_SaveSrv			0x0002 // ������Ϣ�ڷ������Զ����档
#define Login_Option_Password			0x0004 // ��½ʱʹ���µļ��ܷ�ʽ��
#define Login_Option_NotCheck			0x0008 // ������û���

// AskData �� m_nOption ָ�������,Ϊ������
#define ByType_LevelStatic				0x1000  // �������� LevelStatic  
#define ByType_LevelRealTime			0x2000  // �������� LevelRealTime  

// �г�������ת��
#define Market_STOCK_MARKET				0x0001	 // ��Ʊ
#define	Market_HK_MARKET				0x0002   // �۹�
#define Market_WP_MARKET				0x0004   // ����
#define Market_FUTURES_MARKET			0x0008   // �ڻ�
#define Market_FOREIGN_MARKET			0x0010   // ���

#define Market_Address_Changed			0x0020   // ��ǰ��������Ҫ��ַ�л�
#define Market_Client_ForceUpdate		0x0040   // ��ǰ�ͻ��˱����������ܹ�ʹ��
#define Market_DelayUser				0x0080   // ��ǰ�û�Ϊ��ʱ�û����ڸ۹�����ʱʹ��
#define Market_TestSrvData				0x0100   // �Ƿ�֧�ֲ���
#define Market_UserCheck				0x0200   // ���������а����û���ѶȨ����Ϣ
#define Market_LOGIN_INFO				0x0400   // ��Ѷ

#define Market_STOCK_LEVEL				0x0800   // level2

// ������
//
#define RT_Srv_Sub_Restart		0x0001 // ������������
#define RT_Srv_Sub_Replace		0x0002 // �滻����

#define RT_Srv_Sub_DownCFG		0x0003 // ���������ļ�
#define RT_Srv_Sub_UpCFG		0x0004 // �ϴ������ļ�

#define RT_Srv_Sub_DownUserDB	0x0005 // �����û������ļ��ļ�
#define RT_Srv_Sub_UpUserDB		0x0006 // �ϴ��û������ļ��ļ�

#define RT_Srv_Sub_DownReport	0x0007 // ���غ�̨���򱨸��ļ�
#define RT_Srv_Sub_LimitPrompt	0x0008 // Ȩ�޴�����ʾ

#define RT_Srv_Sub_Succ			0x1000 // �����ɹ���ʾ

/* ����/���� DEFINE END*/

/* ʵʱ���ݰ��롡DEFINE BEGIN*/
#define MASK_REALTIME_DATA_OPEN					0X00000001      //����
#define MASK_REALTIME_DATA_MAXPRICE				0X00000002      //��߼�
#define MASK_REALTIME_DATA_MINPRICE				0X00000004      //��ͼ�
#define MASK_REALTIME_DATA_NEWPRICE				0X00000008      //���¼�

#define MASK_REALTIME_DATA_TOTAL				0X00000010      //�ɽ���(��λ:��)
#define MASK_REALTIME_DATA_MONEY				0X00000020	    //�ɽ����(��λ:Ԫ)

#define MASK_REALTIME_DATA_BUYPRICE1			0x00000040	    // �򣱼�
#define MASK_REALTIME_DATA_BUYCOUNT1			0x00000080		// ����
#define MASK_REALTIME_DATA_BUYPRICE2			0x00000100	    // �򣲼�
#define MASK_REALTIME_DATA_BUYCOUNT2			0x00000200		// ����
#define	MASK_REALTIME_DATA_BUYPRICE3			0x00000400	    // �򣳼�
#define MASK_REALTIME_DATA_BUYCOUNT3			0x00000800		// ����
#define MASK_REALTIME_DATA_BUYPRICE4			0x00001000	    // �򣴼�
#define MASK_REALTIME_DATA_BUYCOUNT4			0x00002000		// ����
#define MASK_REALTIME_DATA_BUYPRICE5			0x00004000	    // �򣵼�
#define MASK_REALTIME_DATA_BUYCOUNT5			0x00008000		// ����

#define MASK_REALTIME_DATA_SELLPRICE1			0x00010000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT1			0x00020000		// ������
#define MASK_REALTIME_DATA_SELLPRICE2			0x00040000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT2			0x00080000		// ������
#define MASK_REALTIME_DATA_SELLPRICE3			0x00100000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT3			0x00200000		// ������
#define MASK_REALTIME_DATA_SELLPRICE4			0x00400000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT4			0x00800000		// ������
#define MASK_REALTIME_DATA_SELLPRICE5			0x01000000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT5			0x02000000		// ������

#define MASK_REALTIME_DATA_PERHAND				0x04000000      //��/�� ��λ
#define MASK_REALTIME_DATA_NATIONAL_DEBT_RATIO  0x08000000      //��ծ����
//����Ϊ��32λ  m_lReqMask1 ��ӦStockOtherData�ṹ
#define MASK_REALTIME_DATA_TIME					0x00000001      //�࿪�̷�����
#define MASK_REALTIME_DATA_CURRENT				0x00000002	    //����
#define MASK_REALTIME_DATA_OUTSIDE				0x00000004		//����
#define MASK_REALTIME_DATA_INSIDE				0x00000008		//����
#define MASK_REALTIME_DATA_OPEN_POSITION		0x00000010      //�񿪲�
#define MASK_REALTIME_DATA_CLEAR_POSITION		0x00000020      //��ƽ��
#define MASK_REALTIME_DATA_CODEINFO				0x10000000		//����

// �����ж���˵��
#define MASK_REALTIME_DATA_BUYORDER1			0x00000080		// ��1����
#define MASK_REALTIME_DATA_BUYORDER2			0x00000100	    // ��2����
#define MASK_REALTIME_DATA_BUYORDER3			0x00000200		// ��3����
#define	MASK_REALTIME_DATA_BUYORDER4			0x00000400	    // ��4����
#define MASK_REALTIME_DATA_BUYORDER5			0x00000800		// ��5����

#define MASK_REALTIME_DATA_SELLORDER1			0x00001000	    // ��1����
#define MASK_REALTIME_DATA_SELLORDER2			0x00002000		// ��2����
#define MASK_REALTIME_DATA_SELLORDER3			0x00004000	    // ��3����
#define MASK_REALTIME_DATA_SELLORDER4			0x00008000		// ��4����
#define MASK_REALTIME_DATA_SELLORDER5			0x00010000		// ��5����

#define MASK_REALTIME_DATA_SELLCOUNT1			0x00020000		// ������
#define MASK_REALTIME_DATA_SELLPRICE2			0x00040000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT2			0x00080000		// ������
#define MASK_REALTIME_DATA_SELLPRICE3			0x00100000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT3			0x00200000		// ������
#define MASK_REALTIME_DATA_SELLPRICE4			0x00400000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT4			0x00800000		// ������
#define MASK_REALTIME_DATA_SELLPRICE5			0x01000000		// ������
#define MASK_REALTIME_DATA_SELLCOUNT5			0x02000000		// ������

/* ʵʱ���ݰ��롡DEFINE END*/

/* �ۺ����������������� DEFINE BEGIN*/
#define	RT_RISE					0x0001		//�Ƿ�����
#define	RT_FALL					0x0002		//��������
#define	RT_5_RISE				0x0004		//5�����Ƿ�����
#define	RT_5_FALL				0x0008		//5���ӵ�������
#define RT_AHEAD_COMM			0x0010		//��������(ί��)��������
#define RT_AFTER_COMM			0x0020		//��������(ί��)��������
#define	RT_AHEAD_PRICE			0x0040		//�ɽ��������������
#define	RT_AHEAD_VOLBI			0x0080		//�ɽ����仯(����)��������
#define	RT_AHEAD_MONEY			0x0100		//�ʽ�������������
/* �ۺ����������������� DEFINE END*/

/* K��������������� BEGIN*/
#define PERIOD_TYPE_DAY			0x0010		//�������ڣ���
#define PERIOD_TYPE_HISDAY		0x0020		//�������ڣ���
#define PERIOD_TYPE_MINUTE0		0x0000		//�ֱʳɽ�
#define PERIOD_TYPE_MINUTE1		0x00C0		//�������ڣ�1����
#define PERIOD_TYPE_MINUTE5		0x0030		//�������ڣ�5����
#define PERIOD_TYPE_MINUTE15	0x0040		//�������ڣ�15����
#define PERIOD_TYPE_MINUTE30	0x0050		//�������ڣ�30����
#define PERIOD_TYPE_MINUTE60	0x0060		//�������ڣ�60����
#define PERIOD_TYPE_MINUTE120   0x00B0      //
#define PERIOD_TYPE_WEEK		0x0080		//�������ڣ���
#define PERIOD_TYPE_MONTH		0x0090		//�������ڣ���/* K��������������� END*/
//////////////////////////////////////////////////////////////////////////

// level 2 Դ���ݽṹ

// ��ʳɽ�����
struct LevelTransaction
{
	//CodeInfo m_sCode;		//֤ȯ����,���ⲿ����

	int		DataStatus;	   //
	int		TradeIndex;	   // �ɽ����
	int		TradeChannel;  // �ɽ�ͨ��
	int		TradeRef;	   // �ɽ����
	int		TradeTime;	   // �ɽ�ʱ��
	float	TradePrice;	   // �ɽ��۸�
	float	TradeQty;      // �ɽ�����
	float	TradeMoney;    // �ɽ����
};

// ��������
struct LevelOrderQueue_Item
{
	float Price;		 // �۸�
	int	  NumOrders;	 // ί�б���
	float OrderQty[1];   // ��������,max 50
};

struct LevelOrderQueue
{
	int		 DataTimeStamp;	// 163025 stands for 16:30:25.
	int		 DataStatus;
	int		 Side;			//��������
	//CodeInfo m_sCode;		//֤ȯ����,���ⲿ����

	int m_nSize;
	LevelOrderQueue_Item data[1];   // max 10
};


// ��ҵ����
struct LevelTotalValue
{
	int			Ranking;			// �������
	CodeInfo	m_sCode;			// ֤ȯ����
	float		TotalValueTraded;   // �ɽ����
};

struct LevelByIndustry
{
	int   IndustryRanking;			// �������
	char  IndustryCode[4];			// ��ҵ����
	float IndustryTotalValueTraded;	// ��ҵ�ɽ����

	int   m_nSize;

	LevelTotalValue data[5];
};

struct LevelTransactionRankingByIndustry
{
	int		DataTimeStamp;			// 163025 stands for 16:30:25
	int		DataStatus;				//
	float   MarketTotalValueTraded; // �г��ܳɽ����

	char	CFICode[4];				// ֤ȯ���
	char	SecuritySubType[4];		// ֤ȯ�����

	int		m_nSize;

	LevelByIndustry data[1];
};

//
struct LevelWeiTuo
{
	int		DataTimeStamp;		// 163025 stands for 16:30:25
	int		DataStatus;			//
	int		Side;				// ��������
	char	CFICode[4];			// ֤ȯ���
	char	SecuritySubType[4];	// ֤ȯ�����
};

// ί����������ί��������������ǰʮֻ֤ȯ
struct LevelOrderRanking_Item
{
	int		 Ranking;			// rank in list

	CodeInfo m_sCode;			// ֤ȯ����

	float	 TotalOrderQty;		// ί������
	float	 TotalVolumeTraded; // �ѳɽ�����
	float	 TotalValueTraded;	// �ɽ����
};

struct LevelOrderRanking
{
	LevelWeiTuo m_sLevelWeiTuo;

	int		m_nSize;
	LevelOrderRanking_Item data[10];   // ǰ100��,������ǰ10��
};

// ����ί����������10����/������
struct LevelSingleOrderCancellationRanking_Item
{
	int		 Ranking;		// �������
	CodeInfo m_sCode;		// ֤ȯ����
	int		 OrderEntryTime;// ί��ʱ��
	float	 Quantity;		// ί������
	float	 Price;			// ί�м۸�
};

struct LevelSingleOrderCancellationRanking
{
	LevelWeiTuo m_sLevelWeiTuo;

	int		m_nSize;
	LevelSingleOrderCancellationRanking_Item data[10];
};

// ��������/����ί���ۼ���������ǰ10ֻ��Ʊ
struct LevelConsolidatedOrderCancellationRanking_Item
{
	int		 Ranking;				 // �������
	CodeInfo m_sCode;				 // ֤ȯ����
	float	 TotalWithdrawnQty;      // ����ί���ۼ�����
};

struct LevelConsolidatedOrderCancellationRanking
{
	LevelWeiTuo m_sLevelWeiTuo;

	int		m_nSize;
	LevelConsolidatedOrderCancellationRanking_Item data[10];
};

//////////////////////////////////////////////////////////////////////////
//��֤����
struct HSPrivateKey
{
	CodeInfo m_pCode;	// ��Ʒ����
	HSPrivateKey()
	{
		memset(this,0,sizeof(HSPrivateKey));
	}

	// ������ ��������
	const HSPrivateKey& operator=(const HSPrivateKey& data)
	{
		memcpy(&m_pCode,&data.m_pCode,sizeof(m_pCode));
		return *this;
	}
	// ������ ��������
	const HSPrivateKey& operator=(const StockUserInfo& pStockCode)
	{
		memcpy(&m_pCode,&pStockCode.m_ciStockCode,sizeof(m_pCode));
		return *this;
	}
	// ������ ��������
	const HSPrivateKey& operator=(const CodeInfo& pCode)
	{
		memcpy(&m_pCode,&pCode,sizeof(m_pCode));
		return *this;
	}
	// ������ ��������
	const HSPrivateKey& operator=(const long& pStockCode);
	// ��������
	void Copy(const HSPrivateKey& data)
	{
		memcpy(&m_pCode,&data.m_pCode,sizeof(m_pCode));
	}
	// ��������
	void Copy(const CodeInfo& pCode)
	{
		memcpy(&m_pCode,&pCode,sizeof(m_pCode));
	}
	// �Ƚϴ��� ����TRUE or FALSE
	BOOL Compare(const CodeInfo& pCode);
	BOOL Compare(const CodeInfo* pCode);
	// �Ƚϴ��� ����TRUE or FALSE
	BOOL Compare(const StockUserInfo& pCode);
	BOOL Compare(const StockUserInfo* pCode);
	// ���ض�Ӧ��StockUserInfo�ṹ��ַ
	const StockUserInfo* GetStock();
};

/*
ͨѶ����ʽ˵��:

����������ṹ
˵����
1��	m_nIndex��m_cOperor��m_lKey��m_nPrivateKeyΪ�ͻ���ר�õ�һЩ��Ϣ��
�������˴���ʱֱ�ӿ�������,��ͬ��
2��	�궨�壺HS_SUPPORT_UNIX_ALIGNΪʹ����UNIX������4�ֽڶ���ʹ�ã���ͬ��
3��	�������󶼷��ʹ˰�������m_nType���;��������ȷ��m_nSize��m_pCode[1]
������ʵ�ָ��ָ���������
4��	��������ָ����m_nSizeȡֵΪn����m_pCode[1]����ֻ��n��CodeInfo��
��ֻ����n����Ʊ���ݣ�����m_nType��ʶ���������͡�
*/
struct AskData
{
	unsigned short 		m_nType;	     // ��������
	char				m_nIndex;     	 // �������������������ݰ�һ��

	union
	{
		struct
		{
			char			m_cNotEmptyPacket:2;   // �ͻ���ʹ�ã���������=1ʱ����ǰ����������壬=0ʱ�����
			char			m_cOperator:6;	      // �ͻ���ʹ�ã���ǰ�����ķ��������ָ��CEV_Connect_HQ_�ȵĶ��塣
		};
		char			m_cSrv;				  // ������ʹ��
	};

	long				m_lKey;		 	 // һ����ʶ��ͨ��Ϊ���ھ��
	HSPrivateKey	    m_nPrivateKey;	 // ������ʶ

	short     			m_nSize;         // ����֤ȯ������С����ʱ��
	// �����ֵ���������ֽ���
	short 				m_nOption;       // Ϊ��4�ֽڶ������ӵ��ֶ�

	CodeInfo			m_pCode[1];      // ֤ȯ��Ϣ

	AskData()
	{
		Emtpy();
	}

	void Emtpy()
	{
		m_nType     = 0;
		m_cSrv      = 0;
		m_nSize     = 0;
		m_lKey      = 0;
		m_nOption   = 0;
	}
};

//��ϰ������ʽ
struct CompAskData               
{
	unsigned short	m_nType;	      // �������� = COMPASKDATA

	char			m_nIndex;		  // �������������������ݰ�һ��

	union
	{
		struct
		{
			char			m_cNotEmptyPacket:2;   // �ͻ���ʹ�ã���������=1ʱ����ǰ����������壬=0ʱ�����
			char			m_cOperator:6;   // �ͻ���ʹ�ã���ǰ�����ķ��������ָ��CEV_Connect_HQ_�ȵĶ��塣
		};
		char			m_cSrv;          // ������ʹ��
	};

	long			m_lKey;			  // һ����ʶ��ͨ��Ϊ���ھ��
	HSPrivateKey	m_nPrivateKey;    // ������ʶ

	short			m_nAsk;		      // �������ݰ��ĸ���
	short			m_nLength[1];     // ÿ���������ݰ��ĳ��ȣ��� m_nAsk	��
};

//ѹ�����ذ���ʽ
struct TransZipData
{
	unsigned short	m_nType;		// ��������,��ΪRT_ZIPDATA

	short 			m_nAlignment;	// Ϊ��4�ֽڶ������ӵ��ֶ�

	long			m_lZipLen;		// ѹ����ĳ���
	long			m_lOrigLen;		// ѹ��ǰ�ĳ���
	char		    m_cData[1];		// ѹ���������
};

//���ذ�ͷ�ṹ
struct DataHead
{
	unsigned short		m_nType;         // �������ͣ����������ݰ�һ��
	char				m_nIndex;     	 // �������������������ݰ�һ��

	union
	{
		struct
		{
			char			m_cNotEmptyPacket:2;   // �ͻ���ʹ�ã���������=1ʱ����ǰ����������壬=0ʱ�����
			char			m_cOperator:6;   // �ͻ���ʹ�ã���ǰ�����ķ��������ָ��CEV_Connect_HQ_�ȵĶ��塣
		};
		char			m_cSrv;          // ������ʹ��
	};

	long				m_lKey;		 	 // һ����ʶ��ͨ��Ϊ���ھ��
	HSPrivateKey		m_nPrivateKey;	 // ������ʶ
};

//////////////////////////////////////////////////////////////////////////
//����Ϊ����Ӧ����ṹ��ϸ˵��
// RT_NoteMsgData
// ����
struct NoteMsgDataReq
{
	int  m_nType;		// ��������
	int  m_nSize;		// ���ݳ���
	char m_cData[1];    // �������ݸ�����Ҫ����,�����������κδ���
};
// Ӧ��
struct NoteMsgDataAns
{
	DataHead m_dhHead;	// ���ݱ�ͷ

	int  m_nType;		// ��������
	int  m_nSize;		// ���ݳ���
	char m_cData[1];	// �������ݸ�����Ҫ����,�����������κδ���
};

/*
��������: RT_LOGIN��RT_LOGIN_INFO��RT_LOGIN_HK��RT_LOGIN_FUTURES��RT_LOGIN_FOREIGN
����˵��: �ͻ��˵�¼����
��	  ע: 
*/
/* ����ṹ */
struct ReqLogin
{
	char			m_szUser[64];	// �û���
	char			m_szPWD[64];	// ����(�������ģ�����ϸ�μ�Э��
	char			m_cLoginType;   //1���ʺŵ�¼ 2���ǳƵ�¼
};

struct AnsLoginLevel2
{
	short		m_nError;	// �����ʶ 0: У��ͨ�� 1:�û������������ 
	// 2:�ͻ��ѵ�½ 3:δ��ͨ 4:���ڵȵ�
	int			m_nGrant;	// Ȩ��λ ������λ��ÿλ����һ��Ȩ�ޣ�����Ȩ�ް�����
	int 		m_nValidDay;	// ��Ч����,�ṩ�ͻ��˿쵽������ʹ��
};

/*�޸�����*/
struct ReqModifyPwd
{
	char m_szUser[20];
	char m_szOldPwd[20];
	char m_szNewPwd[20];
};

/* ����ṹ */
struct ReqChangePassword
{
	char			m_szUser[64];	// �û���
	char			m_szOldPWD[64];	// ����(�������ģ�����ϸ�μ�Э��
	char			m_szNewPWD[64];	// ����(�������ģ�����ϸ�μ�Э��
};

#define AnsLogin_Normal 0x0001   // һ���û�
#define AnsLogin_Midlle 0x0002   // �м��û�
#define AnsLogin_Hight  0x0004   // �߼��û�

/* ���ؽṹ */
struct AnsLogin0
{
	DataHead		m_dhHead;	//���ݱ�ͷ

	long			m_nError:2;	// ����� ��RT_LOGIN˵��1
	long			m_nSize:30;	// ����

	char			m_szRet[1];	// �����ļ����ݻ��߷��ش�����Ϣ�ַ��� ��RT_LOGIN˵��1
};

struct AnsLogin
{
	DataHead		m_dhHead;	//���ݱ�ͷ

	//long			m_nError:2;	// ����� ��RT_LOGIN˵��1
	//long			m_nSize:30;	// ����

	short			m_nError;
	short 			m_nAlignment;	// Ϊ��4�ֽڶ������ӵ��ֶ�
	long			m_nSize;
	char			m_szRet[1];	// �����ļ����ݻ��߷��ش�����Ϣ�ַ��� ��RT_LOGIN˵��1
};
/*
RT_LOGIN˵��1:
����m_nError ����0����m_szRetΪ�����ļ����ݡ�
����m_nError С��0����m_szRetΪ���������ش�����Ϣ�ַ�����
����m_nError ����ERROR_*(����ID)����m_szRetΪ�ա�
*/

struct AnsPwdModify
{
	DataHead m_dhHead;
	long     m_nSize;
	char     m_szRet[1];
};

/*
��������: RT_INITIALINFO
����˵��: ��������֤�ͳ�ʼ��
��	  ע: 
*/

/*
����ṹ��
��������֤�ͳ�ʼ������
*/
struct ReqInitSrv
{
	short            m_nSrvCompareSize;		// �������Ƚϸ���
	short 			 m_nAlignment;    		// Ϊ��4�ֽڶ������ӵ��ֶ�
	ServerCompare    m_sServerCompare[1];   // �������Ƚ���Ϣ
};

/*
��������: RT_STOCKINFOLIST 
����˵��: �����Ʊ�����б�
��	  ע: 

����ṹ : ReqStockInfoList
���ؽṹ : 
*/
struct ReqStockInfoList
{
	int						m_nSize;				// �г�����
	HSMarketDataType	m_nMarketType[1];		// �����г����
};

struct AnsStockInfoList
{
	DataHead				m_dhHead;		 // ���ݱ�ͷ
	short					m_nSize;		 // ���۱����ݸ���
	short 					m_nAlignment;    // Ϊ��4�ֽڶ������ӵ��ֶ�
	StockUserInfo			m_pnowData[1];	 // ���۱�����
};
/*
���ؽṹ��
��������֤�ͳ�ʼ��Ӧ��
��Ʊ��ʼ����Ϣ
*/
/* ������Ʊ��Ϣ */
struct StockInitInfo
{
	char		m_cStockName[STOCK_NAME_SIZE];	// ��Ʊ����
	CodeInfo	m_ciStockCode;		// ��Ʊ����ṹ
	long		m_lPrevClose;		// ����
	long		m_l5DayVol;			// 5����(�Ƿ���ڴ˼���ɽ���λ����������
};

/* �����г���������Ϣ */
struct OneMarketData
{
	CommBourseInfo  m_biInfo;		 // �г���Ϣ

	short           m_nSize;        // ��Ʊ������
	short 		    m_nAlignment;    // Ϊ��4�ֽڶ������ӵ��ֶ�
	StockInitInfo   m_pstInfo[1];    // (m_biInfo����ָ������Ĵ�������
};

#define AnsInitialData_All 0x0002  // ��ǰ�г����з�������

/* ���ؽṹ */
struct  AnsInitialData  
{
	DataHead		m_dhHead;		// ���ݱ�ͷ

	short		    m_nSize;		// �г�����
	union
	{
		short 			m_nAlignment;   // �Ƿ�Ϊ���Ƴ�ʼ����(0�������ʼ��������0�����Ƴ�ʼ����)
		short			m_nOpertion;    // ����ѡ��,�μ�:AnsInitialData_All ����
	};
	OneMarketData   m_sOneMarketData [1]; // �г�����
};

struct CommRealTimeData
{	
	CodeInfo		m_ciStockCode;		// ��Ʊ����

	StockOtherData	m_othData;			// ʵʱ��������
	char			m_cNowData[1];		// ָ��ShareRealTimeData������һ��
	// ���ݴ���õ���һ��λ��ptr
	CommRealTimeData* GetNext(CodeInfo* pCode,int& nNowSize,int* pTotal = NULL,int nFixSize = 0)
	{
		if( pCode == NULL )
			return this;

		if( nFixSize != 0 )
			nNowSize = nFixSize;
		else
		{
			nNowSize = GetStructSize( pCode );
			if( nNowSize == 0 )
				return this;
		}

		short nSize = nNowSize + GetAddSize();
		if( pTotal )
			*pTotal += nSize;

		char* pp = (char*)this;
		pp += nSize;

		return (CommRealTimeData*)pp;
	}

	ShareRealTimeData* GetShareData()
	{
		return (ShareRealTimeData*)m_cNowData;
	}

	static int GetAddSize()
	{
		return (sizeof(CommRealTimeData) - sizeof(char));//(sizeof(CodeInfo) + sizeof(StockOtherData));
	}

	// �ṹ��С(Ŀǰֻ֧����ͬ�ṹ�����������)
	static int GetStructSize(CodeInfo* pCode,int nAdd = 0)
	{
		if( pCode == NULL )
			return 0;

		switch(MakeMarket(pCode->m_cCodeType))
		{
		case (HSMarketDataType)STOCK_MARKET:		     // ��Ʊ
			if( MakeIndexMarket(pCode->m_cCodeType) )
				return (sizeof(HSIndexRealTime) + nAdd); // ָ��ʵʱ��������
			return (sizeof(HSStockRealTime) + nAdd);
		case (HSMarketDataType)FUTURES_MARKET:			 // �ڻ�
		case (HSMarketDataType)WP_MARKET:
			return (sizeof(HSQHRealTime) + nAdd);
		default:
			break;
		}

		return 0;
	}
};

/* ���ؽṹ */
struct AnsRealTime 
{
	DataHead				m_dhHead;		 // ���ݱ�ͷ
	short					m_nSize;		 // ���۱����ݸ���
	short 					m_nAlignment;    // Ϊ��4�ֽڶ������ӵ��ֶ�
	CommRealTimeData	  	m_pnowData[1];	 // ���۱�����
}; 

/*
������ʼ��˵����
������ʼ��У�鷽ʽ��У��ÿ�������CRC�����༴��ͼ���Ʒ�֣���
������ʼ�����ݰ������������͵�������������֣�
1.�ͻ��˱��ϳ�ʼ������������֣�
a.ĳ���г�����Ʒ��У����ͬ���򲻷��ش����
b.ĳ���г���ĳЩƷ��У�鲻ͬ����ֻ���ز�ͬƷ�ֵ�Ʒ����Ϣ��Ʒ�ִ����
c.ĳ���г�����Ʒ��У�鲻ͬ���򷵻������г���Ϣ
2.�����������䶯ʱ�������̣������ƴ�������и��ͻ������ͱ�
��Ʒ�ֵ�Ʒ����Ϣ��Ʒ�ִ�������ң�����Ʒ�ְ��г����࣬ͬ���Ʒ��
���һ��OneMarketData���ء�

ע��˵�����ᵽ�ġ�Ʒ����Ϣ��ָStockType��
��Ʒ�ִ����ָCommBourseInfoָ��Ʒ�ְ��������д���,�����OneMarketData::m_pstInfo
*/


/*
��������: RT_SERVERINFO
����˵��: ��վ��Ϣ
��	  ע: 
*/
/* ����ṹ : ��������*/
/* 
���ؽṹ :
��վ������Ϣ�ṹ
*/

#define AnsServerInfo_SupportMarket  0x0001 // ֧�ַ����������

struct AnsServerInfo
{
	DataHead	m_dhHead;			// ���ݱ�ͷ
	char		m_pName[32];		// ��������
	char		m_pSerialNo[12];    // ���кţ���֤������
	long		m_lTotalCount;		// �����ӹ�������

	long		m_lToDayCount;		// ������������
	long		m_lNowCount;		// ��ǰ������
};


struct AnsServerInfo2
{
	DataHead	m_dhHead;			// ���ݱ�ͷ

	long		m_lTotalCount;		// �����ӹ�������

	char		m_strAddr[20];      // ip  127.123.123.123
	short		m_nPort;			// �¸����Ӷ˿�

	short		m_nMaxCount;

	long		m_lResever[2];		//
};

/*
��������: 
����ID							����
RT_TEXTDATAWITHINDEX_NEGATIVE	������Ѷ��������
RT_BYINDEXRETDATA				��Ѷ��������
RT_TEXTDATAWITHINDEX_PLUS		������Ѷ��������
RT_USERTEXTDATA					�Զ�����Ѷ������˵��ȣ�
RT_FILEREQUEST					�����ļ��������������ļ��ȣ�
����˵��: 
1��	������Ѷ�������ݣ�������Ϣ���׵�������Ӧ��
2��	��Ѷ��������������Ӧ��
3��	������Ѷ�������ݣ�Ŀǰһ��ΪF10��������������Ӧ��
4��	�Զ�����Ѷ������˵��ȣ���ĿǰΪȯ���Զ���˵�������Ӧ��
5��	�����ļ����󣺰�����Ҫ�����Լ������ļ�������������Ӧ��

*/
/* ������Ѷ��������Ѷ���������� */
struct TextMarkData
{
	long	m_lCRC;	      		//�ãң�ֵ
	long	m_lBeginPos;     	// ��ʼλ�ã�ֱ����������ļ��������ļ���ƫ�ƣ��ֽڣ�
	long	m_lEndPos;			// ��ֹλ��,ͬ��,�μ�˵��

	long	m_lCheckCRC;		//�Ƿ����У��ãңá��ǣ�����У�飬����У�飬 

	/*ָ���ִ�,�ͻ���ʹ��,�ִ���ʽΪ: aa;bb;cc;dd
	����aaΪ:
	#define INFO_PATH_KEY_F10			 "F10-%i"
	#define INFO_PATH_KEY_TREND			 "TREND-%i"
	#define INFO_PATH_KEY_TECH			 "TECH-%i"
	#define INFO_PATH_KEY_REPORT			 "REPORT-%i"
	����bbΪ: ���õĶ��� 
	����ccΪ: ���õ�ȡֵ��
	����ddΪ: �����ļ���*/
	char	m_szInfoCfg[128];	//���������ļ����Ƶ���Ϣ���ַ������ͻ���ʹ��
	char	m_cTitle[64];		// ����,�ͻ��˱���ʹ��
	char	m_cFilePath[192];	// �ļ���/·��
};

/*����ṹ*/
struct ReqTextData	  //���������������
{
	TextMarkData    m_sMarkData;		//У��������
};
/*���ؽṹ*/
struct AnsTextData		// ��ϸ�ı���Ϣ����
{
	DataHead			m_dhHead;	// ���ݱ�ͷ
	TextMarkData        m_sMarkData;// ����������
	unsigned long		m_nSize;	// ���ݳ���
	char				m_cData[1]; // ����
};

// ��Ѷ���
enum InfoClassType
{
	InfoClassType_D1    = 1,	// ��������Ѷ
	InfoClassType_D2    = 2,	// ���������
	InfoClassType_D3    = 3,	// ��ɳ�
	InfoClassType_D4    = 4,	// �Ƚ���Ͷ�����
	InfoClassType_D5    = 5,	// �����Ҫ
	InfoClassType_D6    = 6,	// ��Ϣ��
	InfoClassType_D7    = 7,	// ����ʦ�챨
	InfoClassType_E1    = 8,	// ���ᱨ��
	InfoClassType_E2    = 9,	// ��۲���
	InfoClassType_E3    = 10,	// ��ҵ�о�
	InfoClassType_E4    = 11,	// ��˾�о�
	InfoClassType_E5    = 12,	// Ȩ֤�о�
	InfoClassType_E6    = 13,	// �����о�
	InfoClassType_E7    = 14,	// ծȯ�о�
	InfoClassType_N1    = 15,	// ������Ѷ
	InfoClassType_N2    = 16,	// ������Ѷ
	InfoClassType_N3    = 17,	// ������Ѷ
	InfoClassType_N4    = 18,	// ������Ѷ
	InfoClassType_N5    = 19,	// ������Ѷ
	InfoClassType_N6    = 20,	// ������Ѷ
	InfoClassType_N7    = 21,	// ������Ѷ
	InfoClassType_N8    = 22,	// ������Ѷ
	InfoClassType_N9    = 23,	// ������Ѷ
	InfoClassType_NA    = 24,	// ������Ѷ
	InfoClassType_NB    = 25,	// ������Ѷ
	InfoClassType_NC    = 26,	// ������Ѷ
	InfoClassType_ND    = 27,	// ������Ѷ
	InfoClassType_NE    = 28,	// ������Ѷ
};

// ����״̬
enum InfoOperator
{
	InfoOperator_ChuShi  = 0x0000, // ��ʼ״̬
	InfoOperator_ShenHe  = 0x0001, // �����
	InfoOperator_ShanChu = 0x0002, // ��ɾ��
	InfoOperator_ZuoFei	 = 0x0004, // ������
	InfoOperator_TuiHui	 = 0x0008, // ���˻�

	InfoOperator_DianJiLv = 0x1000, // ���µ����

	InfoOperator_FengKongShenHe = 0x40000000,   // ���տ������
	InfoOperator_FengKong = 0x80000000, // ���տ�������

	//InfoOperator_TianJia = 0x0004, // ���
};

// �û�����
enum UserAttribute
{
	// ע��:1~n���û����Զ�Ӧ��������,����ʵ����Ҫ����
	//�����á������ƽ𡱡������𡱡�����ʯ��
	// ���֧��28���û�����
	UserAttribute_0     = 0x00000000, // �����û������Կ�
	UserAttribute_1     = 0x00000001, //  1���û�
	UserAttribute_2     = 0x00000002, //  2���û�
	UserAttribute_3     = 0x00000004, //  3���û�
	UserAttribute_4     = 0x00000008, //  4���û�
	UserAttribute_5     = 0x00000010, //  5���û�
	UserAttribute_6     = 0x00000020, //  6���û�
	UserAttribute_7     = 0x00000040, //  7���û�
	UserAttribute_8     = 0x00000080, //  8���û�
	UserAttribute_9     = 0x00000100, //  9���û�
	UserAttribute_10    = 0x00000200, // 10���û�
	UserAttribute_11    = 0x00000400, // 11���û�
	UserAttribute_12    = 0x00000800, // 12���û�
	UserAttribute_13    = 0x00001000, // 13���û�
	UserAttribute_14    = 0x00002000, // 14���û�
	UserAttribute_15    = 0x00004000, // 15���û�
	UserAttribute_16    = 0x00008000, // 16���û�
	UserAttribute_17    = 0x00010000, // 17���û�
	UserAttribute_18    = 0x00020000, // 18���û�
	UserAttribute_19    = 0x00040000, // 19���û�
	UserAttribute_20    = 0x00080000, // 20���û�
	UserAttribute_21    = 0x00100000, // 21���û�
	UserAttribute_22    = 0x00200000, // 22���û�
	UserAttribute_23    = 0x00400000, // 23���û�
	UserAttribute_24    = 0x00800000, // 24���û�
	UserAttribute_25    = 0x01000000, // 25���û�
	UserAttribute_26    = 0x02000000, // 26���û�
	UserAttribute_27    = 0x04000000, // 27���û�
	UserAttribute_28    = 0x08000000, // 28���û�

	UserAttribute_29    = 0x10000000, // �����û�
	UserAttribute_30    = 0x20000000, // ����û�
	UserAttribute_31    = 0x40000000, // �����û�
	UserAttribute_32    = 0x80000000, // ɾ���û�
};

// ��Ϣ״̬
struct InfoCheckStatus
{
	unsigned int	m_InfoClassType;	    // ��Ϣ���,�μ� InfoClassType ����
	unsigned int	m_InfoOperator;		    // ��Ѷ״̬,�μ� InfoOperator  ����,���Զ������
	unsigned int	m_UserAttribute;	    // �û�����,�μ� UserAttribute ����,���Զ������
	unsigned int	m_Atattch[5];		    // ��������,��CTime����
	unsigned int    m_nSalesDepartmentNum;  // Ӫҵ����  �μ� SalesDepartmentNum ����
	unsigned int	m_nSenderNum;			// ������(�û�ID)������ָ�򣺷����ˡ�������λ�������̣�,�μ�SenderNum����
	unsigned int	m_nAreaNum;			    // �������
	//add by lxqi 20090223
	union
	{
		unsigned int	m_nRankNo;				//��Ѷ�����Ǽ�
		unsigned int	m_nAuditNum;			// ���
	};
	//add by lxqi 20081016
	unsigned int    m_index_no;             //���������Ѷ�������
};

// ���������ص��û���Ϣ
struct SrvCheckInfo
{
	unsigned int	m_InfoClassType;	    // ��Ϣ���,�μ� InfoClassType ����
	unsigned int    m_InfoOperator;		    // ����״̬,�μ� InfoOperator  ����,���Զ������
	unsigned int    m_UserAttribute;	    // �û�����,�μ� UserAttribute ����,���Զ������
	unsigned int    m_nSalesDepartmentNum;  // Ӫҵ����,�μ� SalesDepartmentNum ����
	unsigned int	m_nSenderNum;			// ������(�û�ID)������ָ�򣺷����ˡ�������λ�������̣�,�μ�SenderNum����
	unsigned int	m_nAreaNum;			    // �������	

	char			m_szUserName[20];		// �û�����

	unsigned int	m_nClassNum;			// �ȼ���ʾ���
	char            m_cCurRank[20];         // yangdl 2008.05.23 �ȼ�����   
	char			m_cResever[30];
};

#define InfoIndexDataEx_mark (0x7F) // ����������־

// �����ļ���¼,����InfoIndex�ṹ
struct InfoIndexDataEx
{
	char m_cType; // Bit0��     ���ܱ�־ (0:������,1:����)
	// Bit1-Bit6������
	// Bit7�� ��Ч��¼��־λ(0:��Ч��¼��1:��Ч��¼)
	// 0x00	��ͨ��Ϣ����
	// 0x01	������Ϣ����
	// 0x80	��Ч��¼
	// 0x7F	��������Ѷ(ע��ʹ��)

	char m_cGroup;   // Bit0-Bit3Ϊ��һ���ࣨ��֧��16�����ͣ�
	//����4BitΪ��Ѷ���̱������xx����Ϣ��
	// Bit0-3ֵ	��Ϣ����
	// 0x00	    ����������
	// 0x01	    �ƾ�����
	// 0x02	    ������ʷ��Ѷ (��˾���¼�)
	// 0x03	    ����ʵʱ��Ѷ
	// 0x04	    F10����
	// 0x05-0x0e	����
	// 0x0f	    ����	

	char m_cMarket;  // Bit0-Bit7Ϊ�г����ࣨ��֧��8���г�����
	// �ͱ���Ϣ��ص��г�Bit��Ϊ1��
	// ����Ϣ�������κ��г�ʱ��Bit0-Bit7ȫΪ0��
	// 0	�Ϻ�
	// 1	����
	// 2	��ҵ
	// 3	���
	// 4	̨��
	// 5	����
	// 6	����
	// 7	����

	long m_lDate;	   // ��*10000+��*100+��
	long m_lTime;      // ʱ*10000+��*100+��
	char m_cTitle[64]; // �ַ���������0x00��β
	// ������ʷ��Ѷ	��ظ�����ʷ��Ѷ���ݵı��⡣�������63���ֽڣ�������0x00��β��
	// ����ʵʱ��Ѷ	��ظ���ʵʱ��Ѷ���ݵı��⡣�������63���ֽڣ�������0x00��β��
	// ����F10����	��ΪF10����ҳ�ϵķ������ƣ����֧��4�����֡�
	// ��Ѷ���̵���ɫ��Ѷ	�����ɫ��Ѷ���ݵı��⡣�������63���ֽڣ�������0x00��β��

	union
	{
		char m_CYlsFilePath[80]; // Դ�ļ�·�� // 130
		char m_cFilePath[80];    // Դ�ļ�·�� // 130
	};

	long m_lOffset;		  // ƫ��λ��
	long m_lLength;	      // ��Ϣ����
	//char m_cCode[4][9];   // ��ش���
	// ���֧��4����ظ��ɴ��롣ÿ����ظ��ɴ���ռ9�ֽڣ����е�1�ֽ��Ǹø��������г�����(�����������)������8���ֽ�������д��ظ��ɴ��룬������0x00��
	char m_write[8];    //��Ѷ����������
	char m_source[12];  //��Ѷ��Դ 
	char m_cCode[2][8];   // ��ش���


	//char m_cReserved[55]; // ���� // 5
	union
	{
		char FilePK[12];	  // ������ţ������������Ψһ������֤������(�׸��������)
		char m_cReserved[55]; // ���� // 5

		InfoCheckStatus m_sInfoCheckStatus; // ���������ض�����,ֻ��Է�������Ѷ
	};

	char m_cSubType;	  // ��Ϣ�ӷ��� 0-8
	// 0 	��ʾ
	// 1	˵��
	// 2	��˾��̬
	// 3	��������
	// 4	�ֺ����
	// 5	ҵ������
	// 6 	�й�����
	// 7	��˾��������

	// 100 �ļ�
	// 101 �ļ�����

	char m_cChksum;		  // У��
	char* m_pSubContent;  // add by lxqi 20081105 ��Ѷ���ݸ�Ҫ��ע�⣺��Ҫ�ڸ�����ʾʱʹ�ã�ʹ��ʱҪ����ռ䣬�����Ҫ�ֶ��ͷ�
};

/*
��������: RT_BULLETIN 
����˵��: ���ƽ�������
��	  ע: 
*/
/* ����ṹ : ������*/
/* ���ؽṹ */
struct AnsBulletin
{
	DataHead		m_dhHead;		//���ݱ�ͷ

	long			m_nSize;		//�������ݳ���	
	char			m_cData[1];		//��������
};


// ʵʱ����
struct HSStockRealTimeOther
{
	long				m_lExt1;		// Ŀǰֻ��ETFʱ���ã�Ϊ��IOPVֵ������510050ʱΪ510051�����¼ۣ�
	long				m_lStopFlag;	// ͣ�̱�־��0��������1��ͣ��
	union
	{
		struct 
		{
			float m_fSpeedUp; // ���������
			long m_lRes[2]; // Ԥ��
		};
		long m_lOther[3]; // Ԥ�� 
	};
};

struct HSStockRealTime_Ext 
{
	unsigned short			m_cSize;		// �ṹ�峤��
	unsigned short			m_nVersion;		// �汾��
	HSStockRealTime			m_stockRealTime;//ʱʱ����
	HSStockRealTimeOther	m_stockOther;	//��չ����
};

// ʱʱ���ݷ���
union ShareRealTimeData_Ext
{
	HSStockRealTime_Ext		m_nowDataExt;		// ����ʵʱ��������
	HSStockRealTime_Ext		m_stStockDataExt;

	HSIndexRealTime			m_indData;		// ָ��ʵʱ��������
	HSQHRealTime			m_qhData;		// �ڻ�ʵʱ��������
	HSQHRealTime_Min		m_qhMin;	
};

/*
��������: RT_AUTOPUSH_EXT
����˵��: ����������չ
��	  ע: 
*/
struct CommRealTimeData_Ext
{	
	unsigned short		m_cSize;		// �ṹ�峤��
	unsigned short		m_nVersion;		// �汾��
	CodeInfo		      m_ciStockCode;		// ��Ʊ����

	StockOtherData		m_othData;			// ʵʱ��������
	char			        m_cNowData[1];		// ָ��ShareRealTimeData_Ext������һ��

	ShareRealTimeData_Ext * GetShareData()
	{
		return (ShareRealTimeData_Ext *)m_cNowData;
	}

	CommRealTimeData_Ext* GetNext(CodeInfo* pCode,int& nNowSize,int* pTotal = NULL,int nFixSize = 0)
	{
		if( pCode == NULL )
			return this;

		if( nFixSize != 0 )
			nNowSize = nFixSize;
		else
		{
			nNowSize = GetStructSize( pCode );
			if( nNowSize == 0 )
				return this;
		}
		int nSize = nNowSize + GetAddSize();
		if( pTotal )
			*pTotal += nNowSize;

		char* pp = (char*)this;
		pp += nSize;

		return (CommRealTimeData_Ext*)pp;
	}
	static int GetAddSize()
	{
		return (sizeof(CommRealTimeData_Ext) - sizeof(char));//(sizeof(CodeInfo) + sizeof(StockOtherData));
	}

	// �ṹ��С(Ŀǰֻ֧����ͬ�ṹ�����������)
	static int GetStructSize(CodeInfo* pCode,int nAdd = 0)
	{
		if( pCode == NULL )
			return 0;

		switch(MakeMarket(pCode->m_cCodeType))
		{
		case (HSMarketDataType)STOCK_MARKET:		     // ��Ʊ
			return (sizeof(HSStockRealTime_Ext) + nAdd);
		case (HSMarketDataType)FUTURES_MARKET:			 // �ڻ�
		case (HSMarketDataType)WP_MARKET:
			return (sizeof(HSQHRealTime) + nAdd);
		default:
			break;
		}

		return 0;
	}
};

struct AnsHSAutoPushData_Ext
{
	DataHead	 	m_dhHead;				// ���ݱ�ͷ

	long		 	m_nSize;				// ���ݸ���
	CommRealTimeData_Ext 	m_pstData[1];	// ����ʵʱ���͵�����
};

/*
��������: RT_DYNREPORT 
����˵��: 
1.	ǿ������
2.	ָ������
3.	���Ű�����
4.	�������
5.	����ƹ�Ʊ�б�����
6.	Ԥ��
7.	�ǿ�ͼ
��	  ע: 
*/
/* ����ṹ */
struct ReqDynReportData
{
	long                m_lReqMask0;	// ���� 0-31λ
	long                m_lReqMask1;	// ���� 32-63λ 

	HSMarketDataType	m_cCodeType;	// ��𣬼�����˵��
	short				m_nSize;		// �������볤�ȶ���

	CodeInfo			m_pCode[1];		// ��Ʊ�б�
};
/* ���ؽṹ ��
˵��:
1��	m_cCodeType �ɲο�RT_REPORTSORT˵��
2��	m_lReqMask0Ϊ0-31bit��m_lReqMask1Ϊ32-63bit
3��	ÿһ�������Դ���CodeInfo��ʼ��������ݣ�
���ݵ�λΪ4�ֽڳ��ȣ�����4�ֽ��������㴦��;
�������ݰ��յ�λ����λ��˳����������
4��	����Ϊ�̶�������������

*/
struct AnsDynamicData
{
	DataHead			m_dhHead;		//���ݱ�ͷ
	long				m_lReqMask0; 	//��ǰҪ���صĽṹ����
	long				m_lReqMask1; 	//��ǰҪ���صĽṹ����

	unsigned long		m_nSize;		//���ݳ���
	char				m_cData[1];		//���ݵ�ַ
};

/*
��������: RT_REPORTSORT 
����˵��: 
1���������۱�61-66Ǭ¡����������ͬ��
2�����������
��	  ע: 
*/
/*���������۱�--61-66�����������
����ṹ
˵����
m_cCodeType�����¼��ַ��ࣺ
1��	��׼���ࣺ��A����A
m_nSize = 0��m_sAnyReportData��ָ���κζ���
2��	ϵͳ��飺SYSBK_BOURSE
m_sAnyReportDataָ��ReqOneBlock
ע��Ŀǰ�汾�����а��CRCУ�飬ֱ�ӷ��ͱ������ݡ�
3��	��ѡ�ɺ��Զ����飨USERDEF_Bourse�� 
m_sAnyReportDataָ��AnyReportData
4��	������A�ɡ���
HSMarketDataTypeΪ��������ȡ�����㣬���硰����A�ɡ����Ա�ʾΪ��
STOCK_MARKET| SH_BOURSE| SZ_BOURSE| KIND_STOCKA
*/
/* ����ṹ */
struct AnyReportData
{
	CodeInfo		m_ciStockCode;	//��Ʊ����
};

struct ReqAnyReport
{
	HSMarketDataType	m_cCodeType; 		// ��𣬼�RT_REPORTSORT˵��1
	short				m_nBegin;			// ��ʾ��ʼ
	short				m_nCount; 			// ��ʾ����
	char				m_bAscending;		// ����/����
	char 				m_cAlignment;		//Ϊ��4�ֽڶ������ӵ��ֶ�
	int					m_nColID;			// ������id 

	short				m_nSize;			// ����
	short				m_nAlignment;		//Ϊ��4�ֽڶ������ӵ��ֶ�
	AnyReportData		m_sAnyReportData[1];//�ͻ��˸��ο����ݸ���������
};

// RT_REPORTSORT_Simple

struct CommRealTimeData_Simple
{	
	CodeInfo m_ciStockCode;		// ��Ʊ����

	long	 m_lPrevClose;		// ����
	char	 m_szValue[12];

	short	 m_nSize;			// ��С
	char	 m_szName[8];		// ����	
};

struct AnsReportData_Simple 
{
	DataHead					m_dhHead;			// ����ͷ

	short						m_nSize;			// ���ݸ���
	CommRealTimeData_Simple 	m_prptData[1];		// ����
};


/*
RT_REPORTSORT˵��1:
m_cCodeType�����¼��ַ��ࣺ
1��	��׼���ࣺ��A����A
m_nSize = 0��m_sAnyReportData��ָ���κζ���
2��	ϵͳ��飺SysBK_Bourse -- ��ǰ�汾��ʱ��֧����֤��顣
m_sAnyReportDataָ��ReqOneBlock
����ļ����������ļ��������ļ���ʽ���ͻ�������ʱ�����CRCУ���뷢�͸���������������	�����������CRC��ͻ��˰��CRC���м�飬������߲�ƥ�䣬����ϵͳ���ѹ�����ݰ���Ȼ���ͱ������ݣ�����ֱ�ӷ��ͱ������ݡ�
3��	��ѡ�ɺ��Զ����飺UserDefBk_Bourse  
m_sAnyReportDataָ��AnyReportData
*/

#define BLOCK_NAME_LENGTH		32			// ������Ƴ���
/* ����ṹ �����������*/
struct ReqOneBlock
{
	long   m_lCRC;            // ���CRC
	char   m_szBlockName[BLOCK_NAME_LENGTH];  // �����
};

/* ���ؽṹ */
struct AnsReportData 
{
	DataHead		m_dhHead;			// ����ͷ

	short			m_nSize;			// ���ݸ���
	short 			m_nAlignment;		// Ϊ��4�ֽڶ������ӵ��ֶ�
	CommRealTimeData 	m_prptData[1];		// ����	
};

/*
��������: RT_GENERALSORT
����˵��: �ۺ���������
��	  ע: 
*/
/* ����ṹ */
struct ReqGeneralSort
{
	HSMarketDataType m_cCodeType;   // �г����
	short            m_nRetCount;   // ��������
	short            m_nSortType;   // �������� ��RT_GENERALSORT˵��1
	short 			 m_nAlignment;  // Ϊ��4�ֽڶ������ӵ��ֶ�
	// Ŀǰ(2004��3��11��)�����������ܵ�ʹ��
	// �����ۺ������п����������ڵļ������������á�
};
/*
RT_GENERALSORT˵��1:
m_nSortType�������£�����ȡ��ֵ��
1��RT_RISE			�Ƿ�����
2��RT_FALL			��������
3��RT_5_RISE		5�����Ƿ�����
4��RT_5_FALL		5���ӵ�������
5��RT_AHEAD_COMM	�������ί�ȣ���������
6��RT_AFTER_COMM	�������ί�ȣ���������
7��RT_AHEAD_PRICE	�ɽ��������������
8��RT_AHEAD_VOLBI	�ɽ����仯�����ȣ���������
9��RT_AHEAD_MONEY	�ʽ�������������
*/

/* �ۺ��������������� */
struct GeneralSortData
{
	CodeInfo		m_ciStockCode;		// ��Ʊ����
	long			m_lNewPrice;		// ���¼�
	long			m_lValue;			// ����ֵ
};

/*���ؽṹ*/
struct AnsGeneralSort
{
	DataHead			m_dhHead;		// ���ݱ�ͷ
	short				m_nSortType;    // ��������
	short				m_nSize;		// �����Ӱ���������GeneralSortData����
	// ��m_ nSortTypeΪ��ֵʱ��ֻ��һλΪ1��,
	// ��ʾm_prptData����ĸ���Ϊm_nSize����
	// ��m_ nSortTypeΪ��ֵ���ʱ����NλΪ1������ʾ�˷��ذ����N���Ӱ���
	// ÿ���Ӱ�����GeneralSortData����Ϊm_nSize������m_prptData�ĳ���ΪN*m_nSize��
	GeneralSortData		m_prptData[1];	// ����		
};

/* 
RT_SEVER_CALCULATE
���ؼ����������ݰ���ʹ�ó��������
*/

struct AnsSeverCalculate
{
	DataHead m_dhHead;

	int		 m_nSize;
	char     m_cData[1];
};

// ��������
struct SeverCalculateData
{
	//CodeInfo m_sCodeInfo;
	HSMarketDataType	m_cCodeType;	// ֤ȯ����
	char				m_cCode[6];		// ֤ȯ����

	float				m_fUpPrice;     // ��ͣ���
	float				m_fDownPrice;   // ��ͣ���
};

/*
RT_ANS_BYTYPE
����ָ�����͵�����,�������ڲ�����ʲô����
�����,ע��:AskData �� m_nOption ָ�������
*/

struct AnsDataByType
{
	DataHead m_dhHead;

	short	 m_nSubType;   // �������ͣ����ݴ�������ȷ�� m_cData �������,�μ�ByType_*����
	short	 m_nResever;

	int		 m_nSize;
	char     m_cData[1];
};

/* 
RT_SEVER_EMPTY
������û������ʱ���ؿհ������һ������������ʱ
*/
struct AnsSeverEmpty
{
	DataHead			m_dhHead;		// ���ݱ�ͷ
	unsigned short		m_nOldType;     // ������������
	unsigned short		m_nAlignment;   // 
	int					m_nSize;
	char				m_cData[1];
};

/*
��������: RT_GENERALSORT_EX
����˵��: �ۺ���������(�����˼�������������)
��	  ע: 
*/
/* ����ṹ */
struct ReqGeneralSortEx
{
	HSMarketDataType m_cCodeType;   // �г����
	short            m_nRetCount;   // ��������
	short            m_nSortType;   // �������� ��RT_GENERALSORT˵��1
	short 			 m_nMinuteCount;// �����ۺ������п����������ڵļ������������á�
	// 0 ʹ�÷�����Ĭ�Ϸ�����
	// 1 ... 15Ϊ�Ϸ�ȡֵ(һ����ܵ�ȡֵΪ1,2,3,4,5,10,15)
};

/*���ؽṹ*/
struct AnsGeneralSortEx
{
	DataHead			m_dhHead;		// ���ݱ�ͷ
	short				m_nSortType;    // ��������
	short				m_nSize;		// �����Ӱ���������GeneralSortData����
	// ��m_ nSortTypeΪ��ֵʱ��ֻ��һλΪ1��,
	// ��ʾm_prptData����ĸ���Ϊm_nSize����
	// ��m_ nSortTypeΪ��ֵ���ʱ����NλΪ1������ʾ�˷��ذ����N���Ӱ���
	// ÿ���Ӱ�����GeneralSortData����Ϊm_nSize������m_prptData�ĳ���ΪN*m_nSize��
	short				m_nAlignment;   // �ֽڶ���
	short				m_nMinuteCount;	// �����ۺ������п����������ڵļ������������á�
	// 0 ʹ�÷�����Ĭ�Ϸ�����
	// 1 ... 15Ϊ�Ϸ�ȡֵ(һ����ܵ�ȡֵΪ1,2,3,4,5,10,15)
	GeneralSortData		m_prptData[1];	// ����		
};

#define Market_SrvCheckError			0x1000   // ��½��֤������ʧ��
#define Market_SrvLoad					0x0800   // ��Ҫ���������ظ�����Ϣ,�μ��ṹ��TestSrvLoadData
#define Market_SrvInfo					0x2000   // ������������Ϣ,�μ��ṹ��TestSrvInfoData

struct TestSrvLoadData
{
	long		m_lTotalCount;		// �����ӹ�������
	long		m_nMaxCount;		// ��������������
	long		m_nCheckDelay;		// ��֤��½ʹ��ʱ��
	long		m_nCheckErrors;		// ��֤��½ʧ�ܴ������
	long		m_nCpu;				// cpu���
	long		m_nMemory;			// �ڴ�ʹ�����

	TestSrvLoadData()
	{
		memset(this,0,sizeof(TestSrvLoadData));
	}
};
struct TestSrvInfoData
{
	unsigned long	m_lSrvDate;			// ��������ǰ���ڣ���ʽΪYYYYMMDDhh
	unsigned long	m_lSrvTime;			// ��������ǰʱ�䣬��ʽΪhhmmss
	unsigned long	m_lReserve[2];		// ����
};


/*
���ƣ�
���ܣ�
��ɷ���������ͻ����������ݣ�
���������ƣ�������������ƣ��͹̶����ƣ����������Զ����͵ģ���ָ������

����
1�������������ƶ����أ����������ƵĹ�Ʊ����Ϊ0ʱ���������ɾ���˴����ơ�
2������ʵʱ���͵����ݣ�ֻ������ָ����
*/

/*
˵��: ��������  ���ڿͻ�������������ҳ�棩
����: RT_AUTOPUSH

˵��: �������ƣ�Ŀǰ����Ԥ��
����: RT_REQAUTOPUSH 
*/
/*���ؽṹ*/
struct AnsHSAutoPushData
{
	DataHead	 m_dhHead;				// ���ݱ�ͷ

	long		 m_nSize;				// ���ݸ���
	CommRealTimeData m_pstData[1];	// ����ʵʱ���͵�����
};

// RT_SERVERTIME˵�� BEGIN
// ˵�������������ʱ��
// ���ͣ�RT_SERVERTIME
// ����ṹ: ��������:AskData
// ���ؽṹ
struct AnsServerTime
{
	DataHead				m_dhHead;		// ���ݱ�ͷ
	int						m_nDateTime;	// ��ǰʱ��(time_t,��1970/1/1 0:0:0��ʼ�������)
};
// RT_SERVERTIME˵�� END

// RT_KEEPACTIVE˵�� BEGIN
// ˵��������ͨ��
// ���ͣ�RT_KEEPACTIVE
// ����ṹ:
struct ReqKeepActive
{
	unsigned short m_nType;			 // �������ͣ����������ݰ�һ��

	char			m_nIndex;     	 // �������������������ݰ�һ��
	char			m_cOperator;   	 // ������0����� 1:�����)
};
// ���ؽṹ
struct AnsKeepActive
{
	unsigned short  m_nType;			// �������ͣ����������ݰ�һ��
	char			m_nIndex;     	 // �������������������ݰ�һ��
	char			m_cOperator;   	 // ������0����� 1:�����)
	int			    m_nDateTime;		// ��ǰʱ��(time_t,��1970/1/1 0:0:0��ʼ�������)
};
/*
˵��:
1���������յ�RT_KEEPACTIVE����ʱ�����Ӧ�𻺳������з������ݣ���ɲ�����˰������÷��ء�
2���������յ��˰�ʱ�����ɰ�ͨ������ķ�ʽ�������յ�������ʱ����Ӧ������󻺳�����
*/
// RT_KEEPACTIVE˵�� END


//RT_TESTSRV	0x0905	/* ���Կͻ��˵��������Ƿ�ͨ�� */
typedef unsigned long  SessionID ;
struct ReqSrvSync
{
	SessionID    client_sessionId ;
};

struct AnsSrvSync
{
	DataHead	m_dhHead;
	int        session_stat;
	SessionID  srv_sessionid ;

} ;

struct TestSrvData
{
	unsigned short m_nType;				// �������ͣ����������ݰ�һ��
	char		   m_nIndex;     		// �������������������ݰ�һ��
	char		   m_cOperator;   		// ������0����� 1:�����)

	TestSrvData()
	{
		m_nType = RT_TESTSRV;
	}
};


// RT_TEST˵�� BEGIN
// ˵��������ͨ�Ű�
// ���ͣ�RT_TEST
// ����ṹ:
struct ReqTest
{
	unsigned short m_nType;				// �������ͣ����������ݰ�һ��
	char		   m_nIndex;     		// �������������������ݰ�һ��
	char		   m_cOperator;   		// ������0����� 1:�����)
	long		   m_nSize;				// ���ݳ���
	long 		   m_nCSendTick;		// �ͻ�������ʼ����ʱ��
	long		   m_nCReceiveTick;		// �ͻ��˽����귵�ذ�ʱ��
	char		   m_szData[1];			// �������ݰ�
};
// ���ؽṹ:
struct AnsTest
{
	unsigned short m_nType;				// �������ͣ����������ݰ�һ��
	char		   m_nIndex;     		// �������������������ݰ�һ��
	char		   m_cOperator;   		// ������0����� 1:�����)
	long		   m_nSize;				// ���ݳ���
	long 		   m_nCSendTick;		// �ͻ�������ʼ����ʱ��
	long		   m_nCReceiveTick;		// �ͻ��˽����귵�ذ�ʱ��

	long		   m_nSReceiveTick;		// ����������������ʱ��
	long		   m_nSBeginCalcTick;	// �������˿�ʼ����ʱ��
	long		   m_nSEndCalcTick;		// �������˼������ʱ��
	long		   m_nSSendTick;		// �������˿�ʼ����ʱ��

	char		   m_szData[1];			// �������ݰ�
};
/*
˵��:
1������ʱ����Ժ���Ϊ��λ
2��m_szData����ָ���������͵�����/Ӧ�����Ƴ��������ݰ�
*/
// RT_TEST˵�� END

/*
��������: RT_CURRENTFINANCEDATA
����˵��: ���µĲ�������
*/
/* ����ṹ */
struct ReqCurrentFinanceData
{
	long 	m_lLastDate;	//��������

};
/* ���ؽṹ */
struct CurrentFinanceData
{
	CodeInfo		m_ciStockCode;	 	// ��Ʊ����	
	int	   			m_nDate;		 	// ����
	float   		m_fFinanceData[39]; // ������
};

// �����Ҳ������ݽṹ
struct FxjCaiWu
{
	char   cType[2];
	char   cUnknow[2];
	char   cCode[6];
	float  fCwdata[39];
	char   cNext[1];
};

struct AnsCurrentFinance
{
	DataHead				m_dhHead;			//���ݱ�ͷ

	short            		m_nSize;			//�������ݸ���
	short 	        		m_nAlignment;		//Ϊ��4�ֽڶ������ӵ��ֶ�
	CurrentFinanceData      m_sFinanceData[1]; // ��������
};

// ��Ȩ�����ļ�������Ϣ
struct HSExRight 
{
	CodeInfo	m_CodeInfo;
	long 		m_lLastDate;	//��������
	long	    m_lCount;		//�ο�ʹ��
};
// ��Ȩ���ݽṹ
struct HSExRightItem
{
	unsigned long  m_nTime;			// ʱ��
	float 	m_fGivingStock;		// �͹�
	float 	m_fPlacingStock;	// ���
	float 	m_fGivingPrice;		// �͹ɼ�
	float 	m_fBonus;			// �ֺ�
};

// �����ҳ�Ȩ���ݽṹ
struct FxjChuQuan
{
	char   cCode[8];
	int	   nUnknow[2];
	char   cNext[1];
};
/*
struct FxjChuQuanDataItem
{
	unsigned long   nTime;//--2010.11.12 litao����2012ʱ������
	float fSg;
	float fPg;
	float fPgPrice;
	float fGive;
};*/
/* ���ؽṹ - ��Ȩ*/
struct CurrentExRightData
{
private:
	char*			m_pszRightData;
	int				m_nDataLen;
public:
	/*	struct
	{
		CodeInfo		m_ciStockCode;	 	// ��Ʊ����	
		long			m_lCount;			//item��
		char			m_cData[1];			//�������� -- �ṹ��HSExRightItem
	};*/

	CurrentExRightData()
	{
		m_pszRightData = NULL;
		m_nDataLen = 0;
	}

	BOOL Init(int nLen)
	{
		Release();
		m_pszRightData = (char*)malloc(nLen);
		if (m_pszRightData != NULL)
		{
			m_nDataLen = nLen;
			memset(m_pszRightData, 0, nLen);
			return TRUE;
		}
		return FALSE;
	}

	void SetCode(CodeInfo* pCode)
	{
		if (m_pszRightData)
		{
			memcpy(m_pszRightData, pCode, sizeof(CodeInfo));
		}
	}

	CodeInfo* GetCode()
	{
		return (CodeInfo*)m_pszRightData;
	}

	void SetCount(long lCount)
	{
		if (m_pszRightData)
		{
			long* pCount =  (long*)(m_pszRightData + sizeof(CodeInfo));
			*pCount = lCount;
		}		
	}

	long GetCount()
	{
		long* pCount =  (long*)(m_pszRightData + sizeof(CodeInfo));
		return *pCount;
	}

	void SetExRightDataItem(HSExRightItem* pItem, int nItemIndex)
	{
		if (m_pszRightData)
		{
			HSExRightItem* pItemSet = (HSExRightItem*)(m_pszRightData + sizeof(CodeInfo) + sizeof(long));
			pItemSet += nItemIndex;
			memcpy(pItemSet, pItem, sizeof(HSExRightItem));

			CTime tm = CTime(time_t(pItemSet->m_nTime));
			long lTime = tm.GetYear() * 10000 + tm.GetMonth() * 100 + tm.GetDay();
			pItemSet->m_nTime = lTime;
		}
	}

	HSExRightItem* GetExRightDataItem(int nItemIndex)
	{
		HSExRightItem* pItemGet = (HSExRightItem*)(m_pszRightData + sizeof(CodeInfo) + sizeof(long));
		pItemGet += nItemIndex;
		return pItemGet;
	}

	void Release()
	{
		if (m_pszRightData)
		{
			free(m_pszRightData);
			m_pszRightData = NULL;
		}
	}
};
/*
��������: RT_HISFINANCEDATA
����˵��: ��ʷ��������
����ṹ: ��������:AskData
*/
/*���ؽṹ*/
struct HisFinanceData
{
	int		m_nDate;		  // ����
	float   m_fFinanceData[39];  // ������
};

struct AnsHisFinance
{
	DataHead		m_dhHead;		//���ݱ�ͷ

	short           m_nSize;			//�������ݸ���
	short 	        m_nAlignment;	//Ϊ��4�ֽڶ������ӵ��ֶ�
	HisFinanceData  m_sFinanceData[1]; // ��������
};

/*��������:RT_EXRIGHT_DATA
����:��Ȩ������������
*/
// ����ṹ��
struct ReqExRightData
{
	long 	m_lLastDate;	//��������
};

// ���ؽṹ��
struct AnsExRightData
{
	DataHead		m_dhHead;		//���ݱ�ͷ
	long           	m_nSize;		//��������������
	char        	m_cData[1]; 	//��Ȩ����������ʽͬ�ļ����μ�����Ȩ�ļ��ṹ
};

struct HSHKStockRealTime 
{
	long m_lOpen;          // ����
	long m_lMaxPrice;      // ��߼�
	long m_lMinPrice;      // ��ͼ�
	long m_lNewPrice;      // ���¼�
	unsigned long m_lTotal; // �ɽ������ɣ�
	float m_fAvgPrice; // �ɽ����(Ԫ)
	long m_lBuyPrice; // �������
	long m_lBuySpread; // ��۲�
	long m_lSellPrice; // ��������
	long m_lSellSpread; // ���۲�
	long m_lBuyCount1; // ��һ��
	long m_lBuyCount2; // �����
	long m_lBuyCount3; // ������
	long m_lBuyCount4; // ������
	long m_lBuyCount5; // ������
	long m_lSellCount1; // ��һ��
	long m_lSellCount2; // ������
	long m_lSellCount3; // ������
	long m_lSellCount4; // ������
	long m_lSellCount5; // ������
	long m_lHand;//m_lIEP; // �ο�ƽ���
	long m_lIEV; // �ο�ƽ����
	void To(char cIntelToComputer )
	{
	}
};

// ���ʵʱ����
struct HSWHRealTime 
{
	long		m_lOpen;         	// ����(1/10000Ԫ)
	long		m_lMaxPrice;     	// ��߼�(1/10000Ԫ)
	long		m_lMinPrice;     	// ��ͼ�(1/10000Ԫ)
	long		m_lNewPrice;     	// ���¼�(1/10000Ԫ)

	long		m_lBuyPrice;		// ���(1/10000Ԫ)
	long		m_lSellPrice;		// ����(1/10000Ԫ)

	//long		m_lPrevClose;	    // ��������
	//long		m_lPriceTimes;	    // ��������һ���Ӽ۸�仯����

	void To(char cIntelToComputer )
	{
	}
};

// ��ʷ��ʱ��������
struct StockHistoryTrendHead 
{
	long				m_lDate;		// ����
	long				m_lPrevClose;	// ����

	union
	{
		HSStockRealTime			m_stStockData;		// ����ʵʱ��������
		HSIndexRealTime			m_stIndexData;		// ָ��ʵʱ��������
		HSHKStockRealTime		m_hkData;		// �۹�ʵʱ��������
		HSQHRealTime			m_qhData;		// �ڻ�ʵʱ��������
		HSWHRealTime			m_whData;		// ���ʵʱ��������
	};

	short			    m_nSize;		//  ÿ�������ܸ���
	short				m_nAlignment;   //  ������

	void To(char cIntelToComputer )
	{
	}
};

//��ʷ��ʱ1��������
struct StockCompHistoryData
{
	long	m_lNewPrice;		// ���¼�
	unsigned long	m_lTotal;			/* �ɽ��� //���ڹ�Ʊ(��λ:��)
										����ָ��(��λ:�ٹ�) */
	float	m_fAvgPrice;		/*�ɽ���� */
	long	m_lBuyCount;        // ί����
	long	m_lSellCount;       // ί����

	void To(char cIntelToComputer)
	{
	}
};

/*����ṹ*/
struct ReqHisTrend
{
	CodeInfo	 m_ciStockCode;	// ��Ʊ����
	long         m_lDate; 		// ���� ��RT_HISTREND˵��1

	void To(char cIntelToComputer )
	{
	}
};

/*
RT_HISTREND˵��1:
m_lDate��
��Ϊ������ָ��������,��ʽ��20030701
��Ϊ��������m_lDate�����ĵ����ʱ���ƣ���-10�򷵻ص�����10��ģ�
�����0,Ϊ����
*/
struct HisTrendData
{
	StockHistoryTrendHead	m_shHead;		// ��ʷ��ʱ��������(2004��6��23�� �Ķ��� �ṹ��ͬ��
	StockCompHistoryData	m_shData[1];	// ������ʷ����

	void To(char cIntelToComputer )
	{

	}
}; 

/*���ؽṹ*/
struct AnsHisTrend
{
	DataHead			m_dhHead;	// ���ݱ�ͷ
	HisTrendData		m_shTend;	// ��ʱ����

	void To(char cIntelToComputer )
	{
	}
};

/*
��������: RT_TECHDATA / RT_TECHDATA_EX
����˵��: �̺����
*/

/*����ṹ*/
struct ReqDayData 	
{
	short				m_nPeriodNum;		// ���ڳ���,����������
	unsigned short		m_nSize;			// �������ݵ�ǰ�Ѿ���ȡ������ʼ����,����������
	long				m_lBeginPosition;	// ��ʼ������-1 ��ʾ��ǰλ�á� �����������Ѿ����صĸ�����
	unsigned short		m_nDay;				// ����ĸ���
	short				m_cPeriod;			// ��������
	CodeInfo			m_ciCode;			// ����Ĺ�Ʊ����

	void To(char cIntelToComputer )
	{
	}
};

struct DateRange
{
	long m_nBeginDate;
	long m_nEndDate;
};

struct ReqRangeDayData: public ReqDayData	
{
	long                     m_nDataRangCount;          // m_dataRange�������
	DateRange                m_dataRange[1];
};

/* ���ؽṹ */
// ���RT_TECHDATA���󷵻�
struct StockCompDayData
{
	long	m_lDate;		// ����
	long	m_lOpenPrice;	// ��
	long	m_lMaxPrice;	// ��
	long	m_lMinPrice;	// ��
	long	m_lClosePrice;	// ��
	long	m_lMoney;		// �ɽ����
	unsigned long	m_lTotal;		// �ɽ���

};
struct AnsDayData
{
	DataHead			m_dhHead;			//����ͷ

	long				m_nSize;			//�������ݸ���	
	//short 			    m_nAlignment;  	 	//Ϊ��4�ֽڶ������ӵ��ֶ�
	StockCompDayData	m_sdData[1];		//��������
};

// ���RT_TECHDATA_EX���󷵻�
struct StockCompDayDataEx
{
	long	m_lDate;		// ����
	long	m_lOpenPrice;	// ��
	long	m_lMaxPrice;	// ��
	long	m_lMinPrice;	// ��
	long	m_lClosePrice;	// ��
	long	m_lMoney;		// �ɽ����
	unsigned long	m_lTotal;		// �ɽ���
	long	m_lNationalDebtRatio; // ��ծ����(��λΪ0.1��),����ֵ(��λΪ0.1��), ������ʱ���뽫����Ϊ0 2004��2��26�ռ���

	void To(char cIntelToComputer )
	{
	}
};

// ���RT_TECHDATA_EX���󷵻�

struct StockCompDayDataExBig
{
	long m_lDate;          // ����
	long m_lOpenPrice;          // ��
	long m_lMaxPrice;  // ��
	long m_lMinPrice;   // ��
	long m_lClosePrice;         // ��
	float        m_lMoney;                // �ɽ����
	float        m_lTotal;          // �ɽ��� 
	long m_lNationalDebtRatio; // ��ծ����(��λΪ0.1��),����ֵ(��λΪ0.1��), ������ʱ���뽫����Ϊ0 2004��2��26�ռ���
	void To(char cIntelToComputer )
	{
	}
};

struct AnsDayDataEx 
{
	DataHead			m_dhHead;			//����ͷ

	long				m_nSize;			//�������ݸ���	
	//short 			    m_nAlignment;  	 	//Ϊ��4�ֽڶ������ӵ��ֶ�
	StockCompDayDataEx	m_sdData[1];		//��������

	void To(char cIntelToComputer )
	{
	}
};

/*����ṹ����������AskData */
struct PriceVolItem_Ext
{
	unsigned short		m_cSize;		// �ṹ�峤��
	unsigned short		m_nVersion;		// �汾��
	long			    m_lNewPrice;	// ���¼�
	unsigned long		m_lTotal;		// �ɽ���(�����ʱ����������)

	long				m_lExt1;		// Ŀǰֻ��ETFʱ���ã�Ϊ��IOPVֵ������510050ʱΪ510051�����¼ۣ�
	long				m_lStopFlag;	// ͣ�̱�־��0��������1��ͣ��
};

/*���ؽṹ*/
struct AnsTrendData_Ext
{
	DataHead		    m_dhHead;		// ���ݱ�ͷ
	short				m_nHisLen;		// ��ʱ���ݸ���
	short 			    m_nAlignment;	// Ϊ��4�ֽڶ������ӵ��ֶ�
	StockOtherData		m_othData;		// ʵʱ��������
	//HSStockRealTime		m_nowData;		// ʵʱ��������
	union
	{
		HSStockRealTime_Ext		m_nowDataExt;	// ����ʵʱ��������
		HSIndexRealTime			m_indData;		// ָ��ʵʱ��������
		HSHKStockRealTime		m_hkData;		// �۹�ʵʱ��������
		HSQHRealTime			m_qhData;		// �ڻ�ʵʱ��������
		HSWHRealTime			m_whData;		// ���ʵʱ��������
	};
	PriceVolItem_Ext		m_pHisData[1];	// ��ʷ��ʱ����
};

/*  
��������: RT_LIMITTICK
����˵��: ָ�����ȵķֱ�����
*/
//����ṹ��
struct ReqLimitTick
{
	CodeInfo     m_pCode;		// ����
	short        m_nCount;		// ��Ҫ���صĳ���
	short 		 m_nAlignment;  // Ϊ��4�ֽڶ������ӵ��ֶ�
};
//���ؽṹ��ͬ�ֱʽṹ
/*
��������: RT_STOCKTICK
����˵��: ���ɷֱʡ�������ϸ�ķֱ�����
*/
//����ṹ���������� AskData
//���ؽṹ�����ɷֱʷ��ذ�
struct AnsStockTick
{
	DataHead	m_dhHead;		// ���ݱ�ͷ
	long		m_nSize;		// ���ݸ���
	//short 		m_nAlignment;   // Ϊ��4�ֽڶ������ӵ��ֶ�
	StockTick	m_traData[1];	// �ֱ�����
};

/*
��������: RT_LEAD
����˵��: ��������ָ��
����ṹ: ��������AskData
*/
//���ؽṹ��
struct StockLeadData
{
	long			m_lNewPrice;	// ���¼�(ָ��)
	unsigned long	m_lTotal;		// �ɽ���

	short	m_nLead;		// ����ָ��
	short	m_nRiseTrend;	// ��������
	short	m_nFallTrend;	// �µ�����
	short 	m_nAlignment;   // Ϊ��4�ֽڶ������ӵ��ֶ�

	void To(char cIntelToComputer )
	{

	}
};
struct AnsLeadData  
{
	DataHead		m_dhHead;		// ����ͷ
	HSIndexRealTime	m_indData;		// ָ��ʵʱ����

	short			m_nHisLen;		// �������ݸ���
	short 			m_nAlignment;   // Ϊ��4�ֽڶ������ӵ��ֶ�
	StockLeadData	m_pHisData[1];	// ��������

	void To(char cIntelToComputer )
	{
	}
};

/*��������: 	RT_VALUE
����:������Ӧ����ʾ�ڿͻ�����Сͼ��ֵ��������(��Ʊ)
*/
// ����ṹ����������
// ���ؽṹ��
struct AnsValueData
{
	DataHead		m_dhHead;			// ���ݱ�ͷ

	long			m_nTime;			// ʱ�䣬���뿪�̷�����
	unsigned long	m_lTotal;			// ����
	float			m_fAvgPrice;		// �ܽ��
	long			m_lNewPrice;		// ���¼�
	//long			m_lPreMaxPrice;		// ������߼�
	//long			m_lPreMinPrice;		// ������ͼ�
	long			m_lTickCount;		// �ɽ�����	

	union
	{
		struct
		{
			long 			m_lMa10;			// 10�죬20�죬50�����̾���
			long 			m_lMa20;
			long 			m_lMa50;
			long 			m_lMonthMax;		// ��������
			long 			m_lMonthMin;		
			long 			m_lYearMax;			// ��������
			long 			m_lYearMin;
			long			m_lHisAmplitude;	// ��ʷ����(ʹ��ʱ��1000Ϊ�ٷֱ�����
		};
		CalcData_Share m_Share;
	};

	void To(char cIntelToComputer )
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// Level2���ݽṹ
// ����ƱLevel2��������
struct LevelStockOtherData
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
};

struct RealTimeDataLevel
{	
	CodeInfo			m_ciStockCode;		// ����
	LevelStockOtherData	m_othData;			// ʵʱ��������
	LevelRealTime		m_sLevelRealTime;	// 

	// ���ݴ���õ���һ��λ��ptr
	RealTimeDataLevel* GetNext(CodeInfo* pCode,int& nNowSize,int* pTotal = NULL,int nFixSize = 0)
	{
		if( pCode == NULL )
			return this;

		if( nFixSize != 0 )
			nNowSize = nFixSize;
		else
		{
			nNowSize = sizeof(RealTimeDataLevel);
			if( nNowSize == 0 )
				return this;
		}
		if( pTotal )
			*pTotal += nNowSize;

		char* pp = (char*)this;
		pp += nNowSize;

		return (RealTimeDataLevel*)pp;
	}
};

// level����
struct AnsHSAutoPushLevel
{
	DataHead			 m_dhHead;			// ���ݱ�ͷ

	long				 m_nSize;			// ���ݸ���
	RealTimeDataLevel	 m_pstData[1];		// ����ʵʱ���͵�����

	void To(char cIntelToComputer )
	{

	}
};


//��ʳɽ� ��ѯ����
struct ReqLevelTransaction
{
	CodeInfo 	m_Codeinfo;
	int			m_nSize;
	int			m_nPostion;
};

// ��ʳɽ���ѯ���ذ�
struct AnsLevelTick
{
	DataHead				m_dhHead;		// ����ͷ
	long					m_nSize;			// ���ش������
	HSLevelTransaction		m_Data[1];
};

// ��ʳɽ��ṹ
struct LevelTransactionData
{
	CodeInfo			m_CodeInfo;			// ����
	long          m_nSize;
	HSLevelTransaction	m_Data[1];			// ��ʳɽ�����

	LevelTransactionData* GetNext(int& nNowSize,int* pTotal = NULL)
	{
		nNowSize = sizeof(LevelTransactionData);
		if( nNowSize == 0 )
			return this;

		if( pTotal )
			*pTotal += nNowSize;

		char* pp = (char*)this;
		pp += nNowSize;

		return (LevelTransactionData*)pp;
	}
};

// ��ʳɽ����ķ��ذ�
struct AnsHsLevelTransactionAuto
{
	DataHead				m_dhHead;		// ����ͷ
	long					m_nSize;			// ���ش������
	LevelTransactionData	m_Data[1];			// ��ʳɽ��������� 
};

#pragma	pack()
