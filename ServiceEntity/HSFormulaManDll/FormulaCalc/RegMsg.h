/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：RegMsg.h
 * 文件标识：注册消息定义类
 * 摘    要：注册消息定义类 定义了程序主要的一些注册消息, 如编译器, 互动等
 *
 * 当前版本：2.0
 * 作    者：
 * 完成日期：
 *
 * 取代版本：
 * 原 作 者：
 * 完成日期：
 * 备	 注：
 *******************************************************************************/

#ifndef  _REGMSG_H_
#define _REGMSG_H_
/* 注释：熊钟彬
   日期 : 2003-07-03
*/
/* 编译器相关消息
   原型参见CHSChildFrame::OnCompileData(WPARAM wp,LPARAM lp)
   使用实例可以参见编译器模块CHSDataSource类	 
*/
#define HS_COMPILE_DATA			_T("Compile Data")
/* 编译器相关消息
	以下HS_COMPILE_DATA_WPARAM_??定义,为消息的WPARAM参数,实际上,函数实体中,取值尚有其它数值
*/
#define HS_COMPILE_DATA_WPARAM_BEGIN 0x10000
/* 功	   能:由代码得到股票
 * LPARAM输入:const char * 股票代码信息
 * 输	   出:LRESULT,为StockInitInfo*
 */
#define HS_COMPILE_DATA_WPARAM_GetStockUserInfo		(HS_COMPILE_DATA_WPARAM_BEGIN + 1) 
/* 功	   能:取得股票单位
 * LPARAM输入:HSMarketDataType* 市场信息类型指针
 * 输	   出:LRESULT,为short
 */
#define HS_COMPILE_DATA_WPARAM_GetStockPriceUnit       (HS_COMPILE_DATA_WPARAM_BEGIN + 2) 
/* 功	   能:股票小数位
 * LPARAM输入:HSMarketDataType* 市场信息类型指针
 * 输	   出:LRESULT,为short
 */ 
#define HS_COMPILE_DATA_WPARAM_GetStockPriceDecimal	(HS_COMPILE_DATA_WPARAM_BEGIN + 3) 
/* 功	   能:获取盘后数据
 * LPARAM输入:AskData* 数据请求包,此处应该包含周期等信息
 * 输	   出:LRESULT,为void* ,读取的数据的指针
 */  
#define HS_COMPILE_DATA_WPARAM_GetDayData				(HS_COMPILE_DATA_WPARAM_BEGIN + 4) 
/* 功	   能:获取财务数据
 * LPARAM输入:CodeInfo*
 * 输	   出:LRESULT,为FinanceData* ,读取的数据的指针
 */ 
#define HS_COMPILE_DATA_WPARAM_GetFinaceData			(HS_COMPILE_DATA_WPARAM_BEGIN + 5) 
 /* 功	   能:获取除权数据
 * LPARAM输入:CodeInfo*
 * 输	   出:LRESULT,为CArray<ChuQuanData*,ChuQuanData*>* ,读取的数据的指针
 */ 
#define HS_COMPILE_DATA_WPARAM_GetChuQuanData			(HS_COMPILE_DATA_WPARAM_BEGIN + 6) 
/* 功	   能:获取实时数据
 * LPARAM输入:char* - 股票代码
 * 输	   出:LRESULT,为RealTimeDataSource* ,读取的数据的指针
 */ 
#define HS_COMPILE_DATA_WPARAM_GetNowData				(HS_COMPILE_DATA_WPARAM_BEGIN + 7) 

#define HS_COMPILE_DATA_WPARAM_GetStockUserInfo2		(HS_COMPILE_DATA_WPARAM_BEGIN + 8)

/* 功	   能:公式或者其它数据改变后,刷新视图
 * LPARAM输入:一般为功能编号
 * LPARAM输入:相应的数据
 * 输	   出:LRESULT,恒为0
 * 原      型:CHSChildFrame::OnCompileUpdateData
 * 实      例:CDlgFinanceData::OnUserDragData
 */
#define HS_COMPILE_UPDATEDATA		_T("Compile Update Data")
/* 功	   能:公式相关,激活相应公式
 * WPARAM输入:必须为HX_USER_DRAGDATA-1
 * LPARAM输入:为CExpression*数据,当前选 中的公式 
 * 输	   出:LRESULT,恒为0
 * 原      型:CDlgFormulaBrowser::OnUserDragData;CHSCondition::OnUserDragData
 * 实      例:
 */
#define HS_COMPILE_DRAGDATA		_T("Compile Drag Data")
/* 功	   能:查找公式
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_FIND_DATA_MSG			_T("HS_FIND_DATA_MSG")
/* 功	   能:发送资讯请求
 * WPARAM输入:请求包的通用发送格式char*
 * LPARAM输入:长度
 * 输	   出:0
 * 原      型:CHSChildFrame::OnZiXunDataRequest
 * 实      例:
 * 说      明:
 */
#define HS_ZiXun_Data_Request		_T("HS_ZiXun_Data_Request")
/* 功	   能:数据下载相关消息
 * WPARAM输入:表示状态 例如MSG_DOWNLOAD_STATUS等
 * LPARAM输入:根据WPARAM不同,参数不同,有DOWNLOADSTATUS*,CString*等
 * 输	   出:失败返回非正值,成功返回正直
 * 原      型:CDlgMainDownload::OnMsgPress
 * 实      例:
 * 说      明:
 */
#define HS_DOWNLOADDATA_MSG		_T("HS_DOWNLOADDATA_MSG")       // 下载
/* 功	   能:下载相关
 * WPARAM输入:
 * LPARAM输入:CNI_General *
 * 输	   出:
 * 原      型:CHSChildFrame::OnHttpCommNotify
 * 实      例:
 * 说      明:
 */
#define HS_DOWNLOADDATA_Other_MSG	_T("HS_DOWNLOADDATA_Other_MSG") // http 数据 msg 
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_GET_HSMAIN          _T("HS_GET_HSMAIN")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:CHSChildFrame::OnCommNotify
 * 实      例:
 * 说      明:
 */

#define HS_COMM_DATA			 _T("HS Comm Data")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_YDL_HQJYDATA         _T("HS YDL HqjyData") //yangdl 2008.04.09 行情交易消息
#define HS_REQUEST_DATA		 _T("HS REQUEST DATA")  // 发送数据包
#define HS_CONNNECT_TCP		 _T("HS CONNNECT TCP")
#define HS_DISCONNNECT_TCP		 _T("HS DISCONNNECT TCP")
#define HS_INIT_DATA			 _T("HS Init Data")     // 得到初始化数据
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_ALL_MSG_DATA		 _T("HS_ALL_MSG_DATA")
#define HS_WINDOW_DATA_EXCHANGE _T("HS WINDOW DATA EXCHANGE")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_NUMEDIT_EDITUPDATE   _T("HS NumEdit EditUpdate")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_OPEN_DATA_TIP        _T("Open DataTip")
#define HS_DATA_TIP_EXPLORER    _T("DataTip Explorer")
#define HS_DATA_TIP_Pre         _T("DataTip Pre")
#define HS_DATA_TIP_Next        _T("DataTip Next")
#define HS_DATA_TIP_Close       _T("DataTip Close")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_PRINT_ONPRINT        _T("HS PRINT ONPRINT")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_GET_HSMAIN          _T("HS_GET_HSMAIN")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_OPEN_DETAIL_REPORT   _T("HS_OPEN_DETAIL_REPORT")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_VERTSCROLL_LBUTTONUP	 _T("HS_VERTSCROLL_LBUTTONUP")
#define HS_HORZSCROLL_LBUTTONUP     _T("HS_HORZSCROLL_LBUTTONUP")
/* 功	   能:通讯相关
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */
#define HS_PARENT_MSG				 _T("HS_PARENT_MSG")
/* 功	   能:论坛与地址铺的交互消息
 * WPARAM输入:操作功能编号
 * LPARAM输入:按照功能编号不同,参数不同,对应:
			  WPARAM :1, CString* -- 为接收编辑框设置文本
			  WPARAM :2, LPRECT   -- 设置窗体区域
			  WPARAM :2
			  WPARAM :4,		  -- 确定
 * 输	   出:
 * 原      型:
 * 实      例:CAddSysGroupWiz::OnManListPassMsg
			  CSendArticleDlg::OnManListPassMsg
			  CRecvListDlg::OnManListPassMsg
			  CQQSendFileDlg::OnManListPassMsg
 * 说      明:
 */
#define HS_MAN_LIST_PASS_MSG		 _T("HS_MAN_LIST_PASS_MSG")
/* 功	   能:注册向导
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:CMainFrame::OnQQWizardPassMsg
			  CUserIDRegDlg::OnQQWizardPassMsg
			  CHSLoginChart::OnQQWizardPassMsg
 * 实      例:
 * 说      明:
 */
#define HS_QQ_WIZARD_PASS_MSG		 _T("HS_QQ_WIZARD_PASS_MSG")//注册向导

/* 功	   能:根据代码获取股票信息
 * WPARAM输入:
 * LPARAM输入:
 * 输	   出:
 * 原      型:
 * 实      例:
 * 说      明:
 */


#endif // _REGMSG_H_