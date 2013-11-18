/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�RegMsg.h
 * �ļ���ʶ��ע����Ϣ������
 * ժ    Ҫ��ע����Ϣ������ �����˳�����Ҫ��һЩע����Ϣ, �������, ������
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

#ifndef  _REGMSG_H_
#define _REGMSG_H_
/* ע�ͣ����ӱ�
   ���� : 2003-07-03
*/
/* �����������Ϣ
   ԭ�Ͳμ�CHSChildFrame::OnCompileData(WPARAM wp,LPARAM lp)
   ʹ��ʵ�����Բμ�������ģ��CHSDataSource��	 
*/
#define HS_COMPILE_DATA			_T("Compile Data")
/* �����������Ϣ
	����HS_COMPILE_DATA_WPARAM_??����,Ϊ��Ϣ��WPARAM����,ʵ����,����ʵ����,ȡֵ����������ֵ
*/
#define HS_COMPILE_DATA_WPARAM_BEGIN 0x10000
/* ��	   ��:�ɴ���õ���Ʊ
 * LPARAM����:const char * ��Ʊ������Ϣ
 * ��	   ��:LRESULT,ΪStockInitInfo*
 */
#define HS_COMPILE_DATA_WPARAM_GetStockUserInfo		(HS_COMPILE_DATA_WPARAM_BEGIN + 1) 
/* ��	   ��:ȡ�ù�Ʊ��λ
 * LPARAM����:HSMarketDataType* �г���Ϣ����ָ��
 * ��	   ��:LRESULT,Ϊshort
 */
#define HS_COMPILE_DATA_WPARAM_GetStockPriceUnit       (HS_COMPILE_DATA_WPARAM_BEGIN + 2) 
/* ��	   ��:��ƱС��λ
 * LPARAM����:HSMarketDataType* �г���Ϣ����ָ��
 * ��	   ��:LRESULT,Ϊshort
 */ 
#define HS_COMPILE_DATA_WPARAM_GetStockPriceDecimal	(HS_COMPILE_DATA_WPARAM_BEGIN + 3) 
/* ��	   ��:��ȡ�̺�����
 * LPARAM����:AskData* ���������,�˴�Ӧ�ð������ڵ���Ϣ
 * ��	   ��:LRESULT,Ϊvoid* ,��ȡ�����ݵ�ָ��
 */  
#define HS_COMPILE_DATA_WPARAM_GetDayData				(HS_COMPILE_DATA_WPARAM_BEGIN + 4) 
/* ��	   ��:��ȡ��������
 * LPARAM����:CodeInfo*
 * ��	   ��:LRESULT,ΪFinanceData* ,��ȡ�����ݵ�ָ��
 */ 
#define HS_COMPILE_DATA_WPARAM_GetFinaceData			(HS_COMPILE_DATA_WPARAM_BEGIN + 5) 
 /* ��	   ��:��ȡ��Ȩ����
 * LPARAM����:CodeInfo*
 * ��	   ��:LRESULT,ΪCArray<ChuQuanData*,ChuQuanData*>* ,��ȡ�����ݵ�ָ��
 */ 
#define HS_COMPILE_DATA_WPARAM_GetChuQuanData			(HS_COMPILE_DATA_WPARAM_BEGIN + 6) 
/* ��	   ��:��ȡʵʱ����
 * LPARAM����:char* - ��Ʊ����
 * ��	   ��:LRESULT,ΪRealTimeDataSource* ,��ȡ�����ݵ�ָ��
 */ 
#define HS_COMPILE_DATA_WPARAM_GetNowData				(HS_COMPILE_DATA_WPARAM_BEGIN + 7) 

#define HS_COMPILE_DATA_WPARAM_GetStockUserInfo2		(HS_COMPILE_DATA_WPARAM_BEGIN + 8)

/* ��	   ��:��ʽ�����������ݸı��,ˢ����ͼ
 * LPARAM����:һ��Ϊ���ܱ��
 * LPARAM����:��Ӧ������
 * ��	   ��:LRESULT,��Ϊ0
 * ԭ      ��:CHSChildFrame::OnCompileUpdateData
 * ʵ      ��:CDlgFinanceData::OnUserDragData
 */
#define HS_COMPILE_UPDATEDATA		_T("Compile Update Data")
/* ��	   ��:��ʽ���,������Ӧ��ʽ
 * WPARAM����:����ΪHX_USER_DRAGDATA-1
 * LPARAM����:ΪCExpression*����,��ǰѡ �еĹ�ʽ 
 * ��	   ��:LRESULT,��Ϊ0
 * ԭ      ��:CDlgFormulaBrowser::OnUserDragData;CHSCondition::OnUserDragData
 * ʵ      ��:
 */
#define HS_COMPILE_DRAGDATA		_T("Compile Drag Data")
/* ��	   ��:���ҹ�ʽ
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_FIND_DATA_MSG			_T("HS_FIND_DATA_MSG")
/* ��	   ��:������Ѷ����
 * WPARAM����:�������ͨ�÷��͸�ʽchar*
 * LPARAM����:����
 * ��	   ��:0
 * ԭ      ��:CHSChildFrame::OnZiXunDataRequest
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_ZiXun_Data_Request		_T("HS_ZiXun_Data_Request")
/* ��	   ��:�������������Ϣ
 * WPARAM����:��ʾ״̬ ����MSG_DOWNLOAD_STATUS��
 * LPARAM����:����WPARAM��ͬ,������ͬ,��DOWNLOADSTATUS*,CString*��
 * ��	   ��:ʧ�ܷ��ط���ֵ,�ɹ�������ֱ
 * ԭ      ��:CDlgMainDownload::OnMsgPress
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_DOWNLOADDATA_MSG		_T("HS_DOWNLOADDATA_MSG")       // ����
/* ��	   ��:�������
 * WPARAM����:
 * LPARAM����:CNI_General *
 * ��	   ��:
 * ԭ      ��:CHSChildFrame::OnHttpCommNotify
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_DOWNLOADDATA_Other_MSG	_T("HS_DOWNLOADDATA_Other_MSG") // http ���� msg 
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_GET_HSMAIN          _T("HS_GET_HSMAIN")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:CHSChildFrame::OnCommNotify
 * ʵ      ��:
 * ˵      ��:
 */

#define HS_COMM_DATA			 _T("HS Comm Data")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_YDL_HQJYDATA         _T("HS YDL HqjyData") //yangdl 2008.04.09 ���齻����Ϣ
#define HS_REQUEST_DATA		 _T("HS REQUEST DATA")  // �������ݰ�
#define HS_CONNNECT_TCP		 _T("HS CONNNECT TCP")
#define HS_DISCONNNECT_TCP		 _T("HS DISCONNNECT TCP")
#define HS_INIT_DATA			 _T("HS Init Data")     // �õ���ʼ������
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_ALL_MSG_DATA		 _T("HS_ALL_MSG_DATA")
#define HS_WINDOW_DATA_EXCHANGE _T("HS WINDOW DATA EXCHANGE")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_NUMEDIT_EDITUPDATE   _T("HS NumEdit EditUpdate")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_OPEN_DATA_TIP        _T("Open DataTip")
#define HS_DATA_TIP_EXPLORER    _T("DataTip Explorer")
#define HS_DATA_TIP_Pre         _T("DataTip Pre")
#define HS_DATA_TIP_Next        _T("DataTip Next")
#define HS_DATA_TIP_Close       _T("DataTip Close")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_PRINT_ONPRINT        _T("HS PRINT ONPRINT")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_GET_HSMAIN          _T("HS_GET_HSMAIN")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_OPEN_DETAIL_REPORT   _T("HS_OPEN_DETAIL_REPORT")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_VERTSCROLL_LBUTTONUP	 _T("HS_VERTSCROLL_LBUTTONUP")
#define HS_HORZSCROLL_LBUTTONUP     _T("HS_HORZSCROLL_LBUTTONUP")
/* ��	   ��:ͨѶ���
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_PARENT_MSG				 _T("HS_PARENT_MSG")
/* ��	   ��:��̳���ַ�̵Ľ�����Ϣ
 * WPARAM����:�������ܱ��
 * LPARAM����:���չ��ܱ�Ų�ͬ,������ͬ,��Ӧ:
			  WPARAM :1, CString* -- Ϊ���ձ༭�������ı�
			  WPARAM :2, LPRECT   -- ���ô�������
			  WPARAM :2
			  WPARAM :4,		  -- ȷ��
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:CAddSysGroupWiz::OnManListPassMsg
			  CSendArticleDlg::OnManListPassMsg
			  CRecvListDlg::OnManListPassMsg
			  CQQSendFileDlg::OnManListPassMsg
 * ˵      ��:
 */
#define HS_MAN_LIST_PASS_MSG		 _T("HS_MAN_LIST_PASS_MSG")
/* ��	   ��:ע����
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:CMainFrame::OnQQWizardPassMsg
			  CUserIDRegDlg::OnQQWizardPassMsg
			  CHSLoginChart::OnQQWizardPassMsg
 * ʵ      ��:
 * ˵      ��:
 */
#define HS_QQ_WIZARD_PASS_MSG		 _T("HS_QQ_WIZARD_PASS_MSG")//ע����

/* ��	   ��:���ݴ����ȡ��Ʊ��Ϣ
 * WPARAM����:
 * LPARAM����:
 * ��	   ��:
 * ԭ      ��:
 * ʵ      ��:
 * ˵      ��:
 */


#endif // _REGMSG_H_