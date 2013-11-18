
#if !defined(_HEXINBASERES_H_)
#define _HEXINBASERES_H_

// 菜单ID /////////////////////////////////////////////////////////////////////
// 
#define hs_BEGIN		   (26001)

#define IDR_hexinCOMMONLYREPORT_MENU		26002  // 一般报价菜单
#define IDR_hexinBIGANALYSE_MENU			26003  // 大盘分析
#define IDR_hexinSpacialREPORT_MENU			26004  // 报价排名
#define IDR_hexinZoomRight_MENU				26005  // 日线状态下，以放大方式按鼠标右键弹出菜单
#define IDR_hexinSelectedCanvas_MENU		26006  // 日线状态下，以选中方式按鼠标右键弹出菜单
#define IDR_hexinRightTabPopup_MENU			26007  // 右边小图tab弹出菜单
#define IDR_hexinSpacialREPORT_SUB_MENU		26008  // 报价子排名:涨幅、量比、总值...
#define IDR_hexinKLinePopup_MENU			26009  // K线切换菜单
#define IDR_hexinAXISMENU				    26010  // 坐标变换

#define IDR_TELETEXT_MENU					26011  // 大利市页菜单
#define IDR_SPLIT_MENU						26012  // 拆分窗口

#define IDR_hexinCUTPAGE_MENU				26013  //  
#define IDR_hexinGENERAL_MENU				26014
#define DRAW_LINE_MENU				    26015  // 画线菜单
#define IDR_COLHEADSORT_POPUP				26016  // 
#define IDR_REPORTDETAILMENU				26017

#define IDR_TECH_MENU						26018  // 图表分析
#define IDR_REPORT_MAIN_MENU			    26019  // 报表
#define IDR_MULTI_CHART_MENU			    26020  // 多图
#define IDR_MAIN_MENU						26021  // 主菜单
#define IDR_INFOBROW_MENU					26022  // 信息浏览

#define IDR_DETAIL_MENU						26023  // 明细报表
#define IDR_REPORT_ANYSIS_MENU				26024  // 报表分析
#define IDR_CAIWU_ANYSIS_MENU				26025  // 财务分析

#define LOOBY_IDM_FUNCTIONTREE_MENU			26026  // 功能树"功能"菜单
#define IDR_MENUPAGE					26027  // 钱龙页面菜单

#define IDR_MAIN_MENU_EX					26028  // 主菜单ID

#define IDR_MULTI_POPUP_MENU			    26030  // 多图弹出
#define IDR_hexinTechPage_MENU				26031  // 技术分析-主图
#define IDR_hexinTechPage_MENU_Sub			26032  // 技术分析-副图
#define IDR_hexinTechPage_MENU_Period		26033  // 技术分析-周期
#define IDR_hexinTechPage_MENU_Finance		26034  // 技术分析-财务分析

#define IDR_hexinFenshiPage_MENU			26035  // 分时走势
#define IDR_hexinBigPage_MENU				26036  // 大盘分析
#define IDR_hexinBrowse_MENU				26037  // browse
#define HOME_URL_MENU					26038  // 状态条左边右菜单
#define IDR_HS_STARFIELD_POPUP_MENU			26039  // 星空图右健弹出菜单

#define SPEC_INFO_BROWSE				26040  // 特色资讯
#define DATA_SEARCH_PAGE				26041  // 数据搜索页面

#define SPLIT_PAGE_ID					26042  // 拆分页面
#define SPLIT_PAGE_MENU					26043  // 拆分页面-主菜单

#define Info_Advance_PAGE_ID				26044  // 高级资讯页面
#define Info_Advance_PAGE_MENU				26045  // 高级资讯页面-主菜单

#define IDR_STOCKTYPE_MENU_SS				26046
#define IDR_STOCKTYPE_MENU_HK				26047
#define IDR_STOCKTYPE_MENU_FUTURES			26048
#define IDR_STOCKTYPE_MENU_WP				26049
#define IDR_STOCKTYPE_MENU_FOREIGN			26050

#define IDR_SELECT_STOCK					26051	// 选取板块


// 外汇主菜单
#define IDR_WH_Report						26052	// 报价
#define IDR_WH_Tech							26053	// 技术分析
#define IDR_WH_Info							26054	// 资讯

#define IDR_hexinTechPage_MENU_Compare		26055  // 技术分析-股票对比

// gbq add, 20060216 支持东航K线图弹出周期菜单
#define IDR_dhTechPage_MENU_Period			26060	// 技术分析-周期(东航专属, 增加3分钟线)
#define IDR_TOOLBAR_MENU_Period				26061	// 东航专属 改变工具栏形式, 周期以按钮点击菜单形式弹出			
// gbq add end;

#define IDR_DieJiaKLinePopup_MENU			26080  // k线,股票叠加
#define IDR_CHUQIANMENU						26081  // 除权探出

#define IDR_hexinGannHexagon_MENU			26090	// 江恩六角
#define IDR_hexinGannSquare_MENU			26091	// 江恩正方

// dialog
#define IDD_PRINTTEMPLET				    26100
#define IDC_TITLEEDIT					    26101
#define IDC_LEFTMARGINEDIT				    26102
#define IDC_TOPMARGINEDIT                   26103
#define IDC_WIDTHEDIT                       26104
#define IDC_HEIGHTEDIT                      26105

// version dlg
#define IDD_ABOUTBOX                    26106
#define IDC_VERLIST                     26107
#define IDC_APP_NAME                    26108
#define IDC_CURVER                      26109
#define IDC_EMAIL                       26110
#define IDC_ADDRESS                     26111
#define IDC_VERSIONINFO                 26112
#define IDC_SMALL_APP_NAME              26113

//
#define IDD_SELPATH_DLG					26114
#define IDC_BROWSE_TREE					26115
#define IDC_EDIT_PATH					26116
#define IDC_MKDIR						26117

// TOOLBAR
#define DRAW_TOOLBAR					26150
#define OPERATION_TOOLBAR				26151

// 配置
#define IDR_YLSCONTROL				    26152
#define IDR_COLORTEMPL                  26153
#define IDR_TABLECOLGROUP               26154
#define IDR_FUNCTIONHELP                26155
#define IDR_TIPS                        26156
#define IDR_MOREPKLINE                  26157
#define IDR_CONDITION                   26158
#define IDR_TECH                        26159
#define IDR_EXCHANGE                    26160
#define IDR_PYJC                        26161
#define IDR_UNION                       26162
#define IDR_FINANCE                     26163
#define IDR_DEFAULT                     26164

#define USER_PAGE_TPL                   26165

#define IDD_SYSTEMPARAM_EDIT_BEGIN		26166

#define IDD_SYSTEMPARAM_EDIT_END		(IDD_SYSTEMPARAM_EDIT_BEGIN + 20)

#define IDD_AUTOPAGE                    IDD_SYSTEMPARAM_EDIT_BEGIN		  //"自动换页时间", 	
#define IDD_UPSPEEDTIME                 (IDD_SYSTEMPARAM_EDIT_BEGIN + 1)  //"涨速比较时间", 
#define IDD_NOTICENEW                   (IDD_SYSTEMPARAM_EDIT_BEGIN + 2)  //"公告新闻保存", 	
#define IDD_BUYEXCHANGE                 (IDD_SYSTEMPARAM_EDIT_BEGIN + 3)  //"买入交易费率", 
#define IDD_SELLEXCHANGE                (IDD_SYSTEMPARAM_EDIT_BEGIN + 4)  //"卖出交易费率", 
#define IDD_MULTIDAY                    (IDD_SYSTEMPARAM_EDIT_BEGIN + 5)  //"多日线天数",       
#define IDD_PREWARNTIME                 (IDD_SYSTEMPARAM_EDIT_BEGIN + 6)  //"预警时间间隔", 
#define IDD_5MUNITE                     (IDD_SYSTEMPARAM_EDIT_BEGIN + 7)  //"5分钟资料存储",	
#define IDD_RT_TECHDATA                 (IDD_SYSTEMPARAM_EDIT_BEGIN + 8)  //"日线资料存储", 	
#define IDD_DETAILSAVE                  (IDD_SYSTEMPARAM_EDIT_BEGIN + 9)  //"分笔成交存储", 
#define IDD_TIP_MAXLINES                (IDD_SYSTEMPARAM_EDIT_BEGIN + 10) //TIP提示显示最大行数
#define IDD_TIP_AUTOHIDE_TIME           (IDD_SYSTEMPARAM_EDIT_BEGIN + 11) //TIP提示自动隐藏时间（秒）
// gbq add 20060515
#define IDC_BIGVOL							(IDD_SYSTEMPARAM_EDIT_BEGIN + 12) //主力大单门限值
#define IDC_LOWESTTESTTIME		            (IDD_SYSTEMPARAM_EDIT_BEGIN + 13) //登陆自动获取最快站点单服务器最长测试时间
//
#define IDD_SYSTEMPARAM_COMBOX_BEGIN	(IDD_SYSTEMPARAM_EDIT_END + 1)		
#define IDD_SYSTEMPARAM_COMBOX_END		(IDD_SYSTEMPARAM_COMBOX_BEGIN + 20)

#define IDD_COMBOMAINCHART              (IDD_SYSTEMPARAM_COMBOX_BEGIN )    //"默认主图",     
#define IDD_COMBOCHART                  (IDD_SYSTEMPARAM_COMBOX_BEGIN + 1) //"多图组合",
#define IDD_COMBOTABLELINERpt           (IDD_SYSTEMPARAM_COMBOX_BEGIN + 2) //"1~6报表格线",
#define IDD_COMBOTABLELINERptSort       (IDD_SYSTEMPARAM_COMBOX_BEGIN + 3) //"61~66报表格线",
#define IDD_COMBOTABLELINEDetail        (IDD_SYSTEMPARAM_COMBOX_BEGIN + 4) //"明细报表格线",     
#define IDD_COMBOXSHOUYI				(IDD_SYSTEMPARAM_COMBOX_BEGIN + 5) //"未完成的收益测试:"
#define IDD_COMBOMULTIFENSHICHART		(IDD_SYSTEMPARAM_COMBOX_BEGIN + 6) //"多分时组合",
#define IDD_COMBOXMIDDLEPRICE			(IDD_SYSTEMPARAM_COMBOX_BEGIN + 7) //"中价计算方法:"
#define IDD_SYSPRM_WINDOW_STYLE			(IDD_SYSTEMPARAM_COMBOX_BEGIN + 8) //窗口风格

#define IDD_SYSTEMPARAM_CHECKBOX_BEGIN	(IDD_SYSTEMPARAM_COMBOX_END + 1)
#define IDD_SYSTEMPARAM_CHECKBOX_END	(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 20)

#define IDD_SELSTOCKCHECK				IDD_SYSTEMPARAM_CHECKBOX_BEGIN		 //"计算条件选股,交易系统时权息校正"
#define IDD_CHECKVOLMENEY				(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 1) //"权息校正时调整成交量成交额",
#define IDD_AUTOREADLOCALDATA			(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 2) //"自动从本地盘读取数据",
#define IDD_AUTOSTARTPREWARN			(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 3) //"开机自动启动预警",
#define IDD_MSGBOX_WHENEXIT				(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 4) //关闭系统时候显示提示
#define IDD_UPDATE_BLKCHUQCWDATA		(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 5) //系统登录连接后自动更新:系统板块/除权数据/财务数据			
#define IDD_MSGBOX_WHENMIN				(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 6) 
#define IDD_STATUSBAR_WINDOWS			(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 7) //状态栏使用Windows标准风格
#define IDD_MULFENSHI_NOTMULCHART		(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 8) //多分时组图中，不使用多图组合

#define IDD_WINDOWS_STYLE				(IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 9) //界面风格，分港股风格，期货风格及传统风格
#define IDC_Support_ThunderTrade            (IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 10) //双击报价界面的价格数字时是否支持闪电交易
#define IDC_HightLightWuShiDang            (IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 11) //五档十档是否整行高亮显色
#define IDC_DISPLAYINFOTREE                 (IDD_SYSTEMPARAM_CHECKBOX_BEGIN + 12) //登录系统有资讯权限时显示资讯树

#define IDD_SYSTEMPARAM_HOTKEY_BEGIN	(IDD_SYSTEMPARAM_CHECKBOX_END	+ 1)	
#define IDD_SYSTEMPARAM_HOTKEY_END		(IDD_SYSTEMPARAM_HOTKEY_BEGIN  + 10)

#define IDD_FASTMIN_HOTKEY				IDD_SYSTEMPARAM_HOTKEY_BEGIN		 //快速隐藏热键

// 报价
#define hs_REPORT_BEGIN       (hs_BEGIN + 200)//(IDD_SYSTEMPARAM_HOTKEY_END+1) // hs_BEGIN
#define hs_REPORT_END	     (hs_REPORT_BEGIN + 2200)

#define IDM_hexinSHIndex		 (hs_REPORT_BEGIN+1)   //"上海指数"
#define IDM_hexinSHStockA		 (hs_REPORT_BEGIN+2)   //"上海Ａ股"
#define IDM_hexinSHStockB		 (hs_REPORT_BEGIN+3)   //"上海Ｂ股"
#define IDM_hexinSHBond			 (hs_REPORT_BEGIN+4)   //"上海债券"
#define IDM_hexinSHFund			 (hs_REPORT_BEGIN+5)   //"上海基金"
#define IDM_hexinSHOther		 (hs_REPORT_BEGIN+6)   //"上海其他"

#define IDM_hexinSZIndex		 (hs_REPORT_BEGIN+10)  //"深圳指数"
#define IDM_hexinSZStockA		 (hs_REPORT_BEGIN+11)  //"深圳Ａ股"
#define IDM_hexinSZStockB		 (hs_REPORT_BEGIN+12)  //"深圳Ｂ股"
#define IDM_hexinSZBond			 (hs_REPORT_BEGIN+13)  //"深圳债券"
#define IDM_hexinSZFund			 (hs_REPORT_BEGIN+14)  //"深圳基金"
#define IDM_hexinSZOther		 (hs_REPORT_BEGIN+15)  //"深圳其他"

#define IDM_HISBSTRAY_ANYDAYE	 (hs_REPORT_BEGIN+16)  //历史买卖盘任何日

#define IDM_hexinCYIndex		 (hs_REPORT_BEGIN+20)  //"创业指数"
#define IDM_hexinCYStockA		 (hs_REPORT_BEGIN+21)  //"创业Ａ股"
#define IDM_hexinCYStockB		 (hs_REPORT_BEGIN+22)  //"创业Ｂ股"
#define IDM_hexinCYBond			 (hs_REPORT_BEGIN+23)  //"创业债券"
#define IDM_hexinCYFund			 (hs_REPORT_BEGIN+24)  //"创业基金"
#define IDM_hexinCYOther		 (hs_REPORT_BEGIN+25)  //"创业其他"

#define IDM_PT_CW_JBZL			 (hs_REPORT_BEGIN+26)  //财务-基本资料
#define IDM_PT_CW_CWTS			 (hs_REPORT_BEGIN+27)  //财务-财务图示
#define IDM_PT_CW_GBQX			 (hs_REPORT_BEGIN+28)  //财务-股本权息
#define IDM_PT_CW_CWSJ			 (hs_REPORT_BEGIN+29)  //财务-财务数据
#define IDM_PT_CW_CWZB			 (hs_REPORT_BEGIN+30)  //财务-财务指标
#define IDM_PT_CW_JJZB			 (hs_REPORT_BEGIN+31)  //财务-基金周报

#define IDM_PST_PHASESORT		 (hs_REPORT_BEGIN+32)  //打开历史买卖盘	
#define IDM_HISBSTRAY_PERIOD	 (hs_REPORT_BEGIN+33)  //历史买卖盘前一日
#define IDM_HISBSTRAY_NEXT		 (hs_REPORT_BEGIN+34)  //历史买卖盘后一日

//#define IDM_PST_PHASESORT			 (hs_REPORT_BEGIN+32)			//区间分析报表-阶段排序结果	

#define IDM_hexinOptional			 (hs_REPORT_BEGIN+35)			//"自选"
#define IDM_hexinOptional20			 (IDM_hexinOptional+20)			//"自选" // 40吕连新扩，现在最大自定义板块为20个

/* hs_REPORT_BEGIN + 41 -- 46  */
#define IDM_hexinOptionalAdd		 (IDM_hexinOptional20+1)			//"添加自选"
#define IDM_hexinOptionalAdd20		 (IDM_hexinOptionalAdd+5)			//"添加自选"

/* hs_REPORT_BEGIN + 47 -- 347  */
// 自动加入板块分类到菜单
#define IDM_BlockAddToMenu			 (IDM_hexinOptionalAdd20+1)
#define IDM_hexinBlock				 (IDM_hexinOptionalAdd20+2)			//"板块"
#define IDM_hexinBlock200			 (IDM_hexinBlock+300)				//"板块"

/* hs_REPORT_BEGIN + 348 -- 349  */
#define IDM_hexinBlockAdd			 (IDM_hexinBlock200+1)				//"添加板块"
#define IDM_hexinBlockAdd200		 (IDM_hexinBlockAdd+10)				//"添加板块"

/* hs_REPORT_BEGIN + 350 -- 370  */
#define IDM_hexinQihuoBlockBegin	 (IDM_hexinBlockAdd200+1)			//"期货板块菜单-begin"
#define IDM_hexinQihuoBlockEnd		 (IDM_hexinQihuoBlockBegin+30)		//"期货板块菜单-end"

/* hs_REPORT_BEGIN + 351 -- 401  */
#define IDM_hexinStockBlockBegin	 (IDM_hexinQihuoBlockEnd+1)		 //"股票板块菜单-begin"
#define IDM_hexinStockBlockEnd		 (IDM_hexinStockBlockBegin+500)  //"股票板块菜单-end"

#define IDM_hexinStock_BeginGG		 (IDM_hexinStockBlockBegin+80)   // 港股
#define IDM_hexinStock_BeginWP		 (IDM_hexinStockBlockBegin+200)  // 外盘
#define IDM_hexinStock_BeginWH		 (IDM_hexinStockBlockBegin+300)  // 外汇
#define IDM_hexinStockBlockEndBegin	 (IDM_hexinStockBlockBegin+400)  //"股票板块菜单-end"

/* hs_REPORT_BEGIN + 402 -- 428  */
#define Yls_StockSortBegin			 (IDM_hexinStockBlockEnd+1)			//"股票排名菜单-begin"
#define Yls_StockSortEnd			 (Yls_StockSortBegin+80)			//"股票排名菜单-end"

// 条件选股(由服务器返回)
#define Yls_SrvConditionUpdate		 (Yls_StockSortEnd+1)				// 服务器选股结果配置文件
#define Yls_StockPool				 (Yls_StockSortEnd+2)				// 股票池,就是服务器自动更新的股票列表
#define Yls_CatchBitmap				 (Yls_StockSortEnd+3)				// 截图处理

// 此处有空余id，可以使用
#define Yls_Empty_Begin				 (Yls_StockSortEnd+4)
#define Yls_Empty_End			     (Yls_StockSortEnd+20)			

//以下向后推16
/* hs_REPORT_BEGIN + 413 -- 423  */
#define IDM_hexinBlockTypeBegin		 (Yls_Empty_End+1)				//"板块分类-begin"
#define IDM_hexinBlockTypeEnd		 (IDM_hexinBlockTypeBegin+10)		//"板块分类-end"

/* hs_REPORT_BEGIN + 424 -- 484  */
#define Yls_ReportSortBegin			 (IDM_hexinBlockTypeEnd+1)			//"排名"
#define Yls_ReportSortEnd			 (Yls_ReportSortBegin+60)			//"排名"

/* hs_REPORT_BEGIN + 483 -- 493  */
#define IDM_hexinCondition			 (Yls_ReportSortEnd+1)				//"条件"
#define IDM_hexinCondition20		 (IDM_hexinCondition+10)			//"条件"

/* hs_REPORT_BEGIN + 494 --   */
#define IDM_hexinSpacialSHIndex			 (IDM_hexinCondition20+1)   //"上海指数排名"
#define IDM_hexinSpacialSHStockA		 (IDM_hexinCondition20+2)   //"上海Ａ股排名"
#define IDM_hexinSpacialSHStockB		 (IDM_hexinCondition20+3)   //"上海Ｂ股排名"
#define IDM_hexinSpacialSHBond			 (IDM_hexinCondition20+4)   //"上海债券排名"
#define IDM_hexinSpacialSHFund			 (IDM_hexinCondition20+5)   //"上海基金排名"
#define IDM_hexinSpacialSHOther			 (IDM_hexinCondition20+6)   //"上海其他排名"

#define IDM_hexinSpacialSZIndex			 (IDM_hexinCondition20+10)  //"深圳指数排名"
#define IDM_hexinSpacialSZStockA		 (IDM_hexinCondition20+11)  //"深圳Ａ股排名"
#define IDM_hexinSpacialSZStockB		 (IDM_hexinCondition20+12)  //"深圳Ｂ股排名"
#define IDM_hexinSpacialSZBond			 (IDM_hexinCondition20+13)  //"深圳债券排名"
#define IDM_hexinSpacialSZFund			 (IDM_hexinCondition20+14)  //"深圳基金排名"
#define IDM_hexinSpacialSZOther			 (IDM_hexinCondition20+15)  //"深圳其他排名"

#define IDM_hexinSpacialCYIndex			 (IDM_hexinCondition20+20)  //"创业指数排名"
#define IDM_hexinSpacialCYStockA		 (IDM_hexinCondition20+21)  //"创业Ａ股排名"
#define IDM_hexinSpacialCYStockB		 (IDM_hexinCondition20+22)  //"创业Ｂ股排名"
#define IDM_hexinSpacialCYBond			 (IDM_hexinCondition20+23)  //"创业债券排名"
#define IDM_hexinSpacialCYFund			 (IDM_hexinCondition20+24)  //"创业基金排名"
#define IDM_hexinSpacialCYOther			 (IDM_hexinCondition20+25)  //"创业其他排名"

//请保持以下11个ID连续以1递增
#define IDM_hexinSpacialSpecialReport1   (IDM_hexinCondition20+30)   //涨跌幅排名
#define IDM_hexinSpacialSpecialReport2   (IDM_hexinCondition20+31)   //成交加震幅排名
#define IDM_hexinSpacialSpecialReport3   (IDM_hexinCondition20+32)   //成交量排名
#define IDM_hexinSpacialSpecialReport4	 (IDM_hexinCondition20+33)   //成交量变化排名
#define IDM_hexinSpacialSpecialReport5	 (IDM_hexinCondition20+34)   //今日强势股
#define IDM_hexinSpacialSpecialReport6	 (IDM_hexinCondition20+35)   //今日弱势股
#define IDM_hexinSpacialSpecialReport7	 (IDM_hexinCondition20+36)   //今日适合做多股
#define IDM_hexinSpacialSpecialReport8	 (IDM_hexinCondition20+37)   //今日适合做空股
#define IDM_hexinSpacialSpecialReport9   (IDM_hexinCondition20+38)   //今日资金流向排名
#define IDM_hexinSpacialSpecialReport10	 (IDM_hexinCondition20+39)   //买卖量差排名
#define IDM_hexinSpacialSpecialReport11  (IDM_hexinCondition20+40)   //综合指标排名

/* hs_REPORT_BEGIN + 494 --   */
#define	IDM_REPORT_LARGE_FONT			 (IDM_hexinCondition20 + 41)//大字报价

/* hs_REPORT_BEGIN + 694 --  709 */
#define REPORT_HEAD_BEGIN			 (IDM_hexinCondition20 + 141)//(IDM_REPORT_LARGE_FONT+100)
#define REPORT_HEAD_END				 (IDM_hexinCondition20 + 156)//(REPORT_HEAD_BEGIN+15)

#define IDM_COLHEADASC_SORT              (REPORT_HEAD_BEGIN+1)
#define IDM_COLHEADDES_SORT              (REPORT_HEAD_BEGIN+2)
#define IDM_COLHEADSHOW_HGRID            (REPORT_HEAD_BEGIN+3)
#define IDM_COLHEADSHOW_VGRID            (REPORT_HEAD_BEGIN+4)
#define IDM_COLHEADSHOW_HVGRID           (REPORT_HEAD_BEGIN+5)
#define IDM_COLHEADSHOW_NONEGRID		 (REPORT_HEAD_BEGIN+6)
#define IDM_COLHEADANIGN_LEFT            (REPORT_HEAD_BEGIN+7)
#define IDM_COLHEADANIGN_RIGHT			 (REPORT_HEAD_BEGIN+8)
#define IDM_COLHEADANIGN_CENTER		     (REPORT_HEAD_BEGIN+9)

#define IDM_COLHEADSET                   (REPORT_HEAD_BEGIN+10)
#define IDM_SETCOLUMEFORMAT				 (REPORT_HEAD_BEGIN+11)


/* hs_REPORT_BEGIN + 710 --  729 */
#define	IDM_COLHEAD_GROUP1				 (REPORT_HEAD_END+1)
#define	IDM_COLHEAD_GROUP20				 (IDM_COLHEAD_GROUP1+19)
#define	IDM_COLHEAD_GROUPDEF			 (IDM_COLHEAD_GROUP20+1)

/* hs_REPORT_BEGIN + 730 --  735 */
#define REPORT_DETAIL_BEGIN			 (IDM_COLHEAD_GROUPDEF+1)
#define REPORT_DETAIL_END			 (REPORT_DETAIL_BEGIN+5)
#define	IDM_DETAILCLOSE					 REPORT_DETAIL_BEGIN
#define	IDM_DETAILPRINT					 (REPORT_DETAIL_BEGIN+1)

/* 分类菜单 熊钟彬 BEGIN*/
/* hs_REPORT_BEGIN + 736 --  741 */
#define IDM_ClassOptional			 (REPORT_DETAIL_END + 1)		//"自选"
#define IDM_ClassOptional20			 (IDM_ClassOptional + 20)			//"自选"   // 吕连新扩，现在最大自定义板块为20个

/* hs_REPORT_BEGIN + 742 --  1042 */
#define IDM_ClassBlock				 (IDM_ClassOptional20 + 1)			//"板块"
#define IDM_ClassBlock200			 (IDM_ClassBlock + 300)				//"板块"

/* hs_REPORT_BEGIN + 1043 --  1063 */
#define IDM_ClassQihuoBlockBegin	 (IDM_ClassBlock200 + 1)			//"期货板块菜单-begin"
#define IDM_ClassQihuoBlockEnd		 (IDM_ClassQihuoBlockBegin + 20)	//"期货板块菜单-end"

/* hs_REPORT_BEGIN + 1044 --  1094 */
#define IDM_ClassStockBlockBegin	 (IDM_ClassQihuoBlockEnd+1)		//"股票板块菜单-begin"
#define IDM_ClassStockBlockEnd		 (IDM_ClassStockBlockBegin+50)  //"股票板块菜单-end"
#define IDM_ClassStockBlockEndBegin	 (IDM_ClassStockBlockBegin+26)  //"股票板块菜单-end"

#define IDM_OtherClassIDBegin        (IDM_ClassStockBlockEnd+1)     // 其它开始

#define IDM_MARKSET_BEGIN			 (IDM_OtherClassIDBegin+1)	//TAB设置开始
#define IDM_MARKSET_END				 (IDM_MARKSET_BEGIN+10)		//TAB设置结束
#define IDM_MARKSET_DEF				 (IDM_MARKSET_END+1)

#define IDM_MARKSET					 (IDM_MARKSET_DEF)			//设置按钮
//#define hs_REPORT_END			 IDM_OtherClassIDBegin //(hs_REPORT_BEGIN + 2200)

// 目前用到1124,最大为1200，加ID时请注意
/* 分类菜单 熊钟彬 END*/



// 分时
#define hs_FENSHI_MENU_BEGIN		  (hs_REPORT_END + 1)
#define hs_FENSHI_MENU_END		  (hs_FENSHI_MENU_BEGIN + 10)

#define hs_FENSHI_DETAILTABLE      (hs_FENSHI_MENU_BEGIN)     // 明细表
#define hs_FENSHI_StockFenTime	  (hs_FENSHI_MENU_BEGIN + 1) // 分时走势
#define hs_FENSHI_StockLiangBi	  (hs_FENSHI_MENU_BEGIN + 2) // 量比
#define hs_FENSHI_StockLiDao		  (hs_FENSHI_MENU_BEGIN + 3) // 买卖力道
#define IDM_TREND_TOTAL_HOLDING 	  (hs_FENSHI_MENU_BEGIN + 4) // 总持指标
#define hs_FENSHI_FenJIaoBao  	  (hs_FENSHI_MENU_BEGIN + 5) // 分价表

//	杨朝惠添加，20040305
#define	IDM_STOCK_MASS_DATA			  (hs_FENSHI_MENU_BEGIN + 6) // 大单菜单项，
//	杨朝惠添加结束


// 技术分析
#define hs_TECH_MENU_BEGIN    (hs_FENSHI_MENU_END + 1)
#define hs_TECH_MENU_END	     (hs_TECH_MENU_BEGIN + 50)

// 大盘分析
#define hs_BIG_MENU_BEGIN     (hs_TECH_MENU_END + 1)
#define hs_BIG_MENU_END	     (hs_BIG_MENU_BEGIN + 25)

#define IDM_hexinSh30Exp         (hs_BIG_MENU_BEGIN + 1)  // "上证１８０指数",    
#define IDM_hexinShTrend         (hs_BIG_MENU_BEGIN + 2)  // "上证走势",        
#define IDM_hexinShLeadExp       (hs_BIG_MENU_BEGIN + 3)  // "上证领先指标",    
#define IDM_hexinShATrend        (hs_BIG_MENU_BEGIN + 4)  // "上证Ａ股走势",    
#define IDM_hexinShBTrend        (hs_BIG_MENU_BEGIN + 5)  // "上证Ｂ股走势",    
#define IDM_hexinShADL           (hs_BIG_MENU_BEGIN + 6)  // "上证ＡＤＬ指标",  
#define IDM_hexinShDuoKong       (hs_BIG_MENU_BEGIN + 7)  // "上证多空指标",    
#define IDM_hexinShLiDao         (hs_BIG_MENU_BEGIN + 8)  // "上证买卖力道",    
#define IDM_hexinShPerExpTrend   (hs_BIG_MENU_BEGIN + 9)  // "上证分类指数走势",
#define IDM_hexinSzTotalExpTrend (hs_BIG_MENU_BEGIN + 10) // "深证综指走势",    
#define IDM_hexinSzTranExpTrend  (hs_BIG_MENU_BEGIN + 11) // "深证成指走势",    
#define IDM_hexinSzLeadExp       (hs_BIG_MENU_BEGIN + 12) // "深证领先指标",    
#define IDM_hexinSzATrend        (hs_BIG_MENU_BEGIN + 13) // "深证Ａ股走势",    
#define IDM_hexinSzBTrend        (hs_BIG_MENU_BEGIN + 14) // "深证Ｂ股走势",    
#define IDM_hexinSzADL           (hs_BIG_MENU_BEGIN + 15) // "深证ＡＤＬ指标",  
#define IDM_hexinSzDuoKong       (hs_BIG_MENU_BEGIN + 16) // "深证多空指标",    
#define IDM_hexinSzLiDao         (hs_BIG_MENU_BEGIN + 17) // "深证买卖力道",    
#define IDM_hexinSzPerExpTrend   (hs_BIG_MENU_BEGIN + 18) // "深证分类指数走势",
#define IDM_hexinShSzTrend       (hs_BIG_MENU_BEGIN + 19) // "上证深证走势",  
#define IDM_hexinShSz300Trend    (hs_BIG_MENU_BEGIN + 20) // "沪深300指数"  

// 新闻
#define BROWSE_IE_ID	 	 (hs_BIG_MENU_END+1) // 浏览器
#define hs_NEWS_MENU_BEGIN    (BROWSE_IE_ID+1)
#define hs_NEWS_MENU_END	     (hs_NEWS_MENU_BEGIN+200)

#define ID_TV_SH_NEWS		 (hs_NEWS_MENU_BEGIN+1)  // "上海证交所信息",			  
#define ID_TV_SZ_NEWS		 (hs_NEWS_MENU_BEGIN+2)  // "深圳证交所信息",              
//#define ID_TV_STOCKER		 (hs_NEWS_MENU_BEGIN+3)  // "券商信息1",                   
//#define ID_TV_STOCKER2	 (hs_NEWS_MENU_BEGIN+4)  // "券商信息2",                   
//#define ID_TV_MESSAGE		 (hs_NEWS_MENU_BEGIN+5)  // "金融资讯",                    
//#define ID_TV_NOTICE		 (hs_NEWS_MENU_BEGIN+6)  // "紧急公告",
//#define ID_HSINFO			 (hs_NEWS_MENU_BEGIN+7)  // "恒生信息",

//#define ZX_SPEC_MENU_BEGIN   (ID_HSINFO+5)

#define ZX_SPEC			     (hs_NEWS_MENU_BEGIN+3)   // "特色资讯",
#define ZX_SPEC_F10			 (hs_NEWS_MENU_BEGIN+4)   // "特色f10资讯",
#define ZX_LUNTAN_SYS		 (hs_NEWS_MENU_BEGIN+5)   // "论坛系统",
#define ZX_LUNTAN			 (hs_NEWS_MENU_BEGIN+6)   // "论坛资讯",
#define ZX_HOME_INFO		 (hs_NEWS_MENU_BEGIN+7)   // "主页资讯",
#define ZX_COMP_INFO		 (hs_NEWS_MENU_BEGIN+8)   // "公司资料",

#define MAIL_SYSTEM		     (hs_NEWS_MENU_BEGIN+9)   // "邮件系统",
#define MAIL_NEW			 (hs_NEWS_MENU_BEGIN+10)  // "新建邮件",
#define MAIL_SETTING		 (hs_NEWS_MENU_BEGIN+11)  // "邮件设置",
#define OPEN_QICQ			 (hs_NEWS_MENU_BEGIN+12)  // "QICQ",
#define QICQ_REGISTER_WIZARE (hs_NEWS_MENU_BEGIN+13)  // "QICQ注册向导",
#define QICQ_CHANGE_ACCOUNT  (hs_NEWS_MENU_BEGIN+14)  // "更改用户",

#define DELIVER_DILEI_INFO			  (hs_NEWS_MENU_BEGIN+15)  //  "弹出发表地雷的对话框",
#define DELIVER_LUNTAN_INFO			  (hs_NEWS_MENU_BEGIN+16)  //  "弹出发表论坛的对话框",

#define INFO_TREE						  (hs_NEWS_MENU_BEGIN+17)  //  资讯树
#define INFO_SEARCH						  (hs_NEWS_MENU_BEGIN+18)  //  资讯查找
#define INFO_FAVORITES					  (hs_NEWS_MENU_BEGIN+19)  //  资讯收藏夹
#define BASE_FINANCIAL_STATEMENT_ANALYSIS (hs_NEWS_MENU_BEGIN+20)

#define IDM_REALTIME_INFO				  (hs_NEWS_MENU_BEGIN+21) // 实时解盘

#define ADD_NEWS_MENU_BEGIN			  (hs_NEWS_MENU_BEGIN+22)   //  "",

#define HS_HELPER_ONLINE				  (hs_NEWS_MENU_BEGIN+23)	//add by lxqi 20090611	添加在线帮助的ID

#define IDM_GetZixunUserInfo                  (hs_NEWS_MENU_BEGIN+24)    //qinhn 瑞银工具栏--研究报告 20100604

#define HS_ADD_MAIN_MENU_BEGIN			  (hs_NEWS_MENU_BEGIN+82)  //  100


// 添加其他菜单
#define ADD_Ohter_BEGIN				  (HS_ADD_MAIN_MENU_BEGIN+1)   // 网页动态菜单
#define ADD_Ohter_END				  (ADD_Ohter_BEGIN+5)       

#define ADD_Weituo_BEGIN			  (ADD_Ohter_END + 1)      // 委托动态菜单
#define ADD_Weituo_END				  (ADD_Weituo_BEGIN+50)  

#define ADD_Ohter_BEGIN2			  (ADD_Weituo_END + 1)     // 自定义其他菜单
#define ADD_Ohter_END2				  (ADD_Ohter_BEGIN2+5) 


// 新闻
#define hs_SINGLE_STOCK_NEWS_MENU_BEGIN    (hs_NEWS_MENU_END+1)
#define hs_SINGLE_STOCK_NEWS_MENU_END	  (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+20)

#define ID_BOTTOM_STOCK_ALERT			 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+1)  // "信息地雷",
#define ID_BOTTOM_EMIT_ALERT			 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+2)  // "发表地雷",
#define ID_BOTTOM_ZX_SPEC			     (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+3)  // "特色资讯",
#define ID_BOTTOM_STOCK_INFO			 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+4)  // "f10资讯",
#define ID_BOTTOM_STOCK_LUNTAN			 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+5)  // "论坛系统",
#define ID_BOTTOM_QICQ					 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+6)  // "OICQ",
#define ID_BOTTOM_EMAIL					 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+7)  // "邮件系统",

#define ID_NEW_ARTICLE_STOCK			 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+8)  // 发表
#define ID_RE_ARTICLE_STOCK			     (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+9)  // 回复
#define ID_SHOW_ARTICLE_STOCK			 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+10) // 查看
#define ID_FRESH_LIST_STOCK				 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+11) // 刷新

#define ID_CLEAR_DISP_DATA				 (hs_SINGLE_STOCK_NEWS_MENU_BEGIN+12) // 清除所有显示数据


// 图形选中菜单
#define hs_SelectedChart_MENU_BEGIN    (hs_SINGLE_STOCK_NEWS_MENU_END+1)
#define hs_SelectedChart_MENU_END	  (hs_SelectedChart_MENU_BEGIN+38)

// 日线状态下，以放大方式按鼠标右键弹出菜单
#define IDR_hexinZoomIn					  (hs_SelectedChart_MENU_BEGIN+1)  // "放大"
#define IDR_hexinInCome					  (hs_SelectedChart_MENU_BEGIN+2)  // "交易系统收益"
#define IDR_hexinInTimeSum			      (hs_SelectedChart_MENU_BEGIN+3)  // "时段统计..."
#define IDR_hexinExplorerParam			  (hs_SelectedChart_MENU_BEGIN+4)  // "探索最优参数..."
#define IDR_hexinExSelectParam			  (hs_SelectedChart_MENU_BEGIN+5)  // "优选交易系统..."

#define IDR_hexinSelectedCanvasCOPY       (hs_SelectedChart_MENU_BEGIN+6)  // "复制数据",    
#define IDR_hexinSelectedCanvasNOTES	  (hs_SelectedChart_MENU_BEGIN+7)  // "指标用法注释",
#define IDR_hexinSelectedCanvasParam	  (hs_SelectedChart_MENU_BEGIN+8)  // "调整指标参数",
#define IDR_hexinSelectedCanvasDelete	  (hs_SelectedChart_MENU_BEGIN+9)  // "删除指标",    
#define IDR_hexinSelectedCanvasExpress	  (hs_SelectedChart_MENU_BEGIN+10) // "修改指标公式",

#define IDR_hexinKLineChart				  (hs_SelectedChart_MENU_BEGIN+11) // "Ｋ线图",  
#define IDR_hexinCloseChart				  (hs_SelectedChart_MENU_BEGIN+12) // "收盘价线",
#define IDR_hexinAmericaChart			  (hs_SelectedChart_MENU_BEGIN+13) // "美国线",  
#define IDR_hexinTowerChart				  (hs_SelectedChart_MENU_BEGIN+14) // "宝塔线",  
#define IDR_hexinAxisCndeqv			      (hs_SelectedChart_MENU_BEGIN+15) // 压缩线
#define IDR_hexinOXChart				  (hs_SelectedChart_MENU_BEGIN+16) // "ＯＸ图",
#define IDR_hexinChangeData				  (hs_SelectedChart_MENU_BEGIN+17) // "修改数据",  
#define IDR_hexinNewPriceChart			  (hs_SelectedChart_MENU_BEGIN+18) // "新价线图",

#define IDR_hexinAxisMenuComm			  (hs_SelectedChart_MENU_BEGIN+19) // 普通坐标
#define IDR_hexinAxisMenuBFB			  (hs_SelectedChart_MENU_BEGIN+20) // 百分比坐标
#define IDR_hexinAxisMenuLog			  (hs_SelectedChart_MENU_BEGIN+21) // 反汇率
//#define IDR_hexinAxisBackChuQuan		  (hs_SelectedChart_MENU_BEGIN+22) // 向后除权
// 吕连新加入 
#define IDR_hexinSelectedCanvasSpeed	  (hs_SelectedChart_MENU_BEGIN+22) // 快速新建公式

#define IDR_hexinAxisExchange			  (hs_SelectedChart_MENU_BEGIN+23) // 坐标翻转
#define IDR_hexinAxisMenuLeft			  (hs_SelectedChart_MENU_BEGIN+24) // 左边
#define IDR_hexinAxisMenuRight			  (hs_SelectedChart_MENU_BEGIN+25) // 右边

#define IDR_hexinSelectedCanvasAreaCOPY   (hs_SelectedChart_MENU_BEGIN+26)  // "区间复制数据",    
//#define IDR_NOT_PRICE_POWER				  (hs_SelectedChart_MENU_BEGIN+26)// 不除权
//#define IDR_Advance_PRICE_POWER			  (hs_SelectedChart_MENU_BEGIN+27)// 高级除权
#define  IDR_DATA_OUTPUT				(hs_SelectedChart_MENU_BEGIN+27)//yulx add 20091126
#define IDR_hexinAxisOperatorChart		  (hs_SelectedChart_MENU_BEGIN+28)    // 操作小图表
#define IDR_OBJECT_MOVE_LEFT			  (hs_SelectedChart_MENU_BEGIN+29)    // 图元左移
#define IDR_OBJECT_MOVE_RIGHT			  (hs_SelectedChart_MENU_BEGIN+30)    // 图元右移
#define IDR_hexinAxislog10				  (hs_SelectedChart_MENU_BEGIN+31)	 // 对数坐标
//addby yuhj 2008.3.22
#define IDR_hexinAxisBisectrix      (hs_SelectedChart_MENU_BEGIN+32) //等分线
#define IDR_hexinAxisLogE           (hs_SelectedChart_MENU_BEGIN+33) //自然对数座标
#define IDR_hexinAxisGolden         (hs_SelectedChart_MENU_BEGIN+34) //黄金分割座标

#define IDR_hexinTechAnaNormal      (hs_SelectedChart_MENU_BEGIN+35) //普通周期图
#define IDR_hexinTechAnaRiseFall    (hs_SelectedChart_MENU_BEGIN+36) //涨跌周期图
#define IDR_hexinClearIndicator     (hs_SelectedChart_MENU_BEGIN+37)
#define IDR_hexinShowHideTech       (hs_SelectedChart_MENU_BEGIN+38)
//end

// 分类菜单ID 空余很多ID，可另用
#define IDR_STOCKTYPE_BEGIN				(hs_SelectedChart_MENU_END+1)
#define IDR_STOCKTYPE_END				(IDR_STOCKTYPE_BEGIN+40)

#define IDR_STOCKTYPE_MENU_SS_UP		(IDR_STOCKTYPE_BEGIN+11)
#define IDR_STOCKTYPE_MENU_HK_UP		(IDR_STOCKTYPE_BEGIN+12)
#define IDR_STOCKTYPE_MENU_FUTURES_UP	(IDR_STOCKTYPE_BEGIN+13)
#define IDR_STOCKTYPE_MENU_WP_UP		(IDR_STOCKTYPE_BEGIN+14)
#define IDR_STOCKTYPE_MENU_FOREIGN_UP	(IDR_STOCKTYPE_BEGIN+15)

#define IDT_STOCKTYPE_SHSZ              (IDR_STOCKTYPE_BEGIN+16)  // 上海深证股票分类
#define IDT_STOCKTYPE_HK                (IDR_STOCKTYPE_BEGIN+17)  // 港股分类
#define IDT_STOCKTYPE_FUTURES           (IDR_STOCKTYPE_BEGIN+18)  // 期货分类
#define IDT_STOCKTYPE_WP	            (IDR_STOCKTYPE_BEGIN+19)  // 外盘分类
#define IDT_STOCKTYPE_FOREITH           (IDR_STOCKTYPE_BEGIN+20)  // 外汇分类

#define IDT_ZOOM_OUT		            (IDR_STOCKTYPE_BEGIN+21)  // 缩小
#define IDT_ZOOM_IN						(IDR_STOCKTYPE_BEGIN+22)  // 放大

#define IDR_Trend_TechList				(IDR_STOCKTYPE_BEGIN+23)  // 趋势指标（下拉）
#define IDR_BaiDong_TechList			(IDR_STOCKTYPE_BEGIN+24)  // 摆动指标（下拉）
#define IDR_LianCang_TechList		    (IDR_STOCKTYPE_BEGIN+25)  // 量仓指标（下拉）
#define IDR_ZhouQi_TechList				(IDR_STOCKTYPE_BEGIN+26)  // 周期（下拉）
#define IDR_Report_TechList				(IDR_STOCKTYPE_BEGIN+27)  // 报表分析（下拉）

// gbq add, 20060217 增加东航特殊工具栏按钮
//#ifdef Support_DHJR
#define IDT_TECHPAGE_MOVE_RIGHT			(IDR_STOCKTYPE_BEGIN+30)	//技术分析页面右移
#define IDT_TECHPAGE_MOVE_LEFT			(IDR_STOCKTYPE_BEGIN+31)	//技术分析页面左移
#define IDT_TECHPAGE_ZOOM_OUT			(IDR_STOCKTYPE_BEGIN+32)	//技术分析页面水平放大
#define IDT_TECHPAGE_ZOOM_IN			(IDR_STOCKTYPE_BEGIN+33)	//技术分析页面水平压缩
#define IDT_TECHPAGE_TZOOM_IN			(IDR_STOCKTYPE_BEGIN+34)	//技术分析页面上方垂直放大
#define IDT_TECHPAGE_TZOOM_OUT			(IDR_STOCKTYPE_BEGIN+35)	//技术分析页面上方垂直压缩
#define IDT_TECHPAGE_BZOOM_IN			(IDR_STOCKTYPE_BEGIN+36)	//技术分析页面下方垂直放大
#define IDT_TECHPAGE_BZOOM_OUT			(IDR_STOCKTYPE_BEGIN+37)	//技术分析页面下方垂直压缩

#define IDT_STOCKTYPE_GZQH	            (IDR_STOCKTYPE_BEGIN+38)  // 股指期货
#define IDT_RENDIAN_LHB		            (IDR_STOCKTYPE_BEGIN+39)  // 热点追踪——龙虎榜
#define IDT_STOCK_TO_HKSTOCK            (IDR_STOCKTYPE_BEGIN+40)  // HA股价差比较
//#endif
// gbq add end

// 自定义
#define USER_DEFINE_BEGIN				 (IDR_STOCKTYPE_END + 1)
#define USER_DEFINE_END					 (USER_DEFINE_BEGIN + 1000)

// browse id rang
#define HOME_ADDRESS_URL			 (USER_DEFINE_BEGIN + 1)
#define USER_DEFINE_IE_BEGIN			 (HOME_ADDRESS_URL + 1)
#define USER_DEFINE_IE_END				 (USER_DEFINE_IE_BEGIN + 30)//32

// default tech rang 
#define USER_DEFAULT_TECH_BEGIN			 (USER_DEFINE_IE_END + 1)
#define USER_DEFAULT_TECH_END			 (USER_DEFAULT_TECH_BEGIN + 30)//63

// 自定义指标图ID
#define USER_DEFAULT_UNION_BEGIN		 (USER_DEFAULT_TECH_END + 1)
#define USER_DEFAULT_UNION_END			 (USER_DEFAULT_UNION_BEGIN + 20)//84

// default Style rang
#define USER_DEFAULT_STYLE_BEGIN_Font	 (USER_DEFAULT_UNION_END + 1)
#define USER_DEFAULT_STYLE_BEGIN		 (USER_DEFAULT_UNION_END + 2)
#define USER_DEFAULT_STYLE_END			 (USER_DEFAULT_STYLE_BEGIN + 10)//95

#define USER_DEFAULT_FONT_BEGIN			 (USER_DEFAULT_STYLE_END + 1)
#define USER_DEFAULT_FONT_END			 (USER_DEFAULT_FONT_BEGIN + 10)//95

// 快捷程序
#define XIADAN_BEGIN				 (USER_DEFAULT_FONT_END + 1)  
#define XIADAN_GROUP1				 (XIADAN_BEGIN + 1)
#define XIADAN_END					 (XIADAN_BEGIN + 10)
#define XIADAN_DEFAULT				 (XIADAN_END+1)//106								  // 默认下单程序

#define FIRST_FAVORITE_COMMAND			 (XIADAN_DEFAULT + 1)
#define LAST_FAVORITE_COMMAND			 (FIRST_FAVORITE_COMMAND+50)//297

#define IDM_TECH_INDEX_BEGIN			 (LAST_FAVORITE_COMMAND + 1) 
#define IDM_TECH_INDEX_END				 (IDM_TECH_INDEX_BEGIN + 139)

#define UserDefPage_MENU_BEGIN			 (IDM_TECH_INDEX_END+1)				  // 保存页面(用户自定义)
#define UserDefPage_MENU_END			 (UserDefPage_MENU_BEGIN+10)//308

/*
#define INSERT_MODULE_1_6					  (INSERT_MODULE_BEGIN + 1)    // 1~6报表
#define INSERT_MODULE_61_66				      (INSERT_MODULE_BEGIN + 2)    // 61~66报表

#define INSERT_MODULE_81_86_RT_RISE			  (INSERT_MODULE_BEGIN + 3)    // 今日涨幅排名
#define INSERT_MODULE_81_86_RT_5_RISE		  (INSERT_MODULE_BEGIN + 3)    // 5分钟涨幅排名
#define INSERT_MODULE_81_86_RT_AHEAD_COMM	  (INSERT_MODULE_BEGIN + 3)    // 今日委比前几名
#define INSERT_MODULE_81_86_RT_FALL			  (INSERT_MODULE_BEGIN + 3)    // 今日跌幅排名
#define INSERT_MODULE_81_86_RT_5_FALL		  (INSERT_MODULE_BEGIN + 3)    // 5分钟跌幅排名
#define INSERT_MODULE_81_86_RT_AFTER_COMM     (INSERT_MODULE_BEGIN + 3)    // 今日委比后几名
#define INSERT_MODULE_81_86_RT_AHEAD_PRICE    (INSERT_MODULE_BEGIN + 3)    // 今日震幅排名
#define INSERT_MODULE_81_86_RT_AHEAD_VOLBI    (INSERT_MODULE_BEGIN + 3)    // 今日量比排名
#define INSERT_MODULE_81_86_RT_AHEAD_MONEY    (INSERT_MODULE_BEGIN + 3)    // 今日总金额排名

#define INSERT_MODULE_HOT_BLOCK 			  (INSERT_MODULE_BEGIN + 4)    // 报表分析（热门板块）
#define INSERT_MODULE_QIAN_RUO   			  (INSERT_MODULE_BEGIN + 5)    // 报表分析（强弱分析）
#define INSERT_MODULE_TECH_SORT 			  (INSERT_MODULE_BEGIN + 6)    // 报表分析（指标排列）
#define INSERT_MODULE_PHASE_SORT 			  (INSERT_MODULE_BEGIN + 7)    // 报表分析（阶段排名）
#define INSERT_MODULE_SELSTOCKRESULT		  (INSERT_MODULE_BEGIN + 7)    // 报表分析（选股结果）

#define INSERT_MODULE_FENSHI				  (INSERT_MODULE_BEGIN + 1)    // 分时走势图
#define INSERT_MODULE_FENSHI_TICK			  (INSERT_MODULE_BEGIN + 1)    // 分时TICK图
#define INSERT_MODULE_FENSHI_FENJIA			  (INSERT_MODULE_BEGIN + 1)    // 分时分价表
#define INSERT_MODULE_FENSHI_1DETAIL		  (INSERT_MODULE_BEGIN + 1)    // 分时分笔明细

//#define INSERT_MODULE_PERIOD_TYPE_DAY_1				  (INSERT_MODULE_BEGIN + 1)    // 1分钟K线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_5				  (INSERT_MODULE_BEGIN + 1)    // 5分钟K线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_15				  (INSERT_MODULE_BEGIN + 1)    // 15分钟K线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_60				  (INSERT_MODULE_BEGIN + 1)    // 60分钟K线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_MULMINUTE		  (INSERT_MODULE_BEGIN + 1)    // 多分钟K线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_PERIOD_TYPE_DAY				  (INSERT_MODULE_BEGIN + 1)    // 日线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_WEEK			  (INSERT_MODULE_BEGIN + 1)    // 周线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_MONTH			  (INSERT_MODULE_BEGIN + 1)    // 月线图
//#define INSERT_MODULE_PERIOD_TYPE_DAY_MULDAY			  (INSERT_MODULE_BEGIN + 1)    // 多日线图
#define INSERT_MODULE_PERIOD_TYPE_DAY					  (INSERT_MODULE_BEGIN + 1)    // 日线图
#define INSERT_MODULE_PERIOD_TYPE_DAY_DETAIL			  (INSERT_MODULE_BEGIN + 1)    // 日线明细

#define INSERT_MODULE_INFO					  (INSERT_MODULE_BEGIN + 1)    // 个股资料
#define INSERT_MODULE_BIGANYSIS				  (INSERT_MODULE_BEGIN + 1)    // 大盘分析

#define INSERT_MODULE_FENJIA_TU				  (INSERT_MODULE_BEGIN + 1)    // 分价图（右）
#define INSERT_MODULE_18FENBI				  (INSERT_MODULE_BEGIN + 1)    // 18个时时分笔（右）
#define INSERT_MODULE_BAOJIA				  (INSERT_MODULE_BEGIN + 1)    // 时时报价（右）
#define INSERT_MODULE_CAIWU					  (INSERT_MODULE_BEGIN + 1)    // 财务数据摘要（右）
#define INSERT_MODULE_PEIGU					  (INSERT_MODULE_BEGIN + 1)    // 分红/配股（右）
#define INSERT_MODULE_JIANYIPAN				  (INSERT_MODULE_BEGIN + 1)    // 盘（右）
#define INSERT_MODULE_COST					  (INSERT_MODULE_BEGIN + 1)    // 成本分布（右）
#define INSERT_MODULE_HUOYANSHAN			  (INSERT_MODULE_BEGIN + 1)    // 火焰山（右）
#define INSERT_MODULE_HUOYAODU				  (INSERT_MODULE_BEGIN + 1)    // 活跃度（右）
*/

#define PAGE_DYN_MENU					(UserDefPage_MENU_END + 1)	   // 页面动态菜单开始
#define PAGE_DYN_MENU_BEGIN				(PAGE_DYN_MENU + 1)		   // 页面动态菜单开始ID
#define PAGE_DYN_MENU_END				(PAGE_DYN_MENU_BEGIN + 40)   // 页面动态菜单终止ID 670

#define PAGE_WINDOW_MENU				(PAGE_DYN_MENU_END + 1)	       // 已经打开页面动态菜单开始
#define PAGE_WINDOW_MENU_BEGIN			(PAGE_WINDOW_MENU + 1)		   // 页面动态菜单开始ID
#define PAGE_WINDOW_MENU_END			(PAGE_WINDOW_MENU_BEGIN + 20)    // 页面动态菜单终止ID	700

#define	IDM_STARFIELD_BEGIN					(PAGE_WINDOW_MENU_END + 1)	// 星空图相关起始ID
#define	IDM_STARFIELD_END					(IDM_STARFIELD_BEGIN + 60)		// 星空图相关结束ID

#define	IDM_STARFIELD_TYPE_BEGIN			(IDM_STARFIELD_BEGIN )			// 星空组合类型起始
#define	IDM_STARFIELD_TYPE_PRICE_ORDER		(IDM_STARFIELD_BEGIN )			// 星空组合类型:价盘
#define	IDM_STARFIELD_TYPE_UP_ORDER			(IDM_STARFIELD_BEGIN + 1)		// 星空组合类型:涨盘
#define	IDM_STARFIELD_TYPE_END				(IDM_STARFIELD_BEGIN + 10)		// 星空组合类型结束
#define IDM_STARFIELD_X_BEGIN				(IDM_STARFIELD_TYPE_END + 1)	// 星空X菜单选择字段起始
#define IDM_STARFIELD_X_END					(IDM_STARFIELD_X_BEGIN + 20)	// 星空X菜单选择字段结束	
#define IDM_STARFIELD_Y_BEGIN				(IDM_STARFIELD_X_END + 1)		// 星空Y菜单选择字段起始
#define IDM_STARFIELD_Y_END					(IDM_STARFIELD_Y_BEGIN + 20)	// 星空Y菜单选择字段结束
#define IDM_STARFIELD_SETTING				(IDM_STARFIELD_Y_END + 1)		// 选项

#define IDM_USERDEF_MAIN_MACD_BEGIN			(IDM_STARFIELD_Y_END + 2)			 //MA切换ID范围
#define IDM_USERDEF_MAIN_MACD_END			(IDM_USERDEF_MAIN_MACD_BEGIN + 11)   // 最大十个
#define IDM_USERDEF_MAIN_MACK_NULL			(IDM_USERDEF_MAIN_MACD_END)			 // 无均线设置


// 弘历通 ID 区间
#define ID_HongLiTong_Begin					(IDM_USERDEF_MAIN_MACD_END + 1)   // 弘历通开始ID
//#define IDR_TOOLBAR_HLT                     (ID_HongLiTong_Begin)
#define ID_HLT_INFO                         (ID_HongLiTong_Begin + 2) //信息
#define ID_HLT_SUMMARY                      (ID_HongLiTong_Begin + 3) //综述
#define ID_HLT_SIGNAL                       (ID_HongLiTong_Begin + 4) //信号集
#define ID_HLT_CHANNEL                      (ID_HongLiTong_Begin + 5) //通道集
#define ID_HLT_INDI                         (ID_HongLiTong_Begin + 6) //指标仓库
#define IDR_HLT_ZhouQiList                  (ID_HongLiTong_Begin + 7) //周期 
#define ID_BUTTON_HLTBLD                    (ID_HongLiTong_Begin + 8) //红绿灯
#define ID_BUTTON_HLTHBQ                    (ID_HongLiTong_Begin + 9) //红白圈
#define ID_BUTTON_HLTHLTD                   (ID_HongLiTong_Begin + 10) //弘历通道
#define ID_BUTTON_HLTXSTD                   (ID_HongLiTong_Begin + 11) //薛斯通道
#define ID_BUTTON_HLTHLCH                   (ID_HongLiTong_Begin + 12) //弘历彩虹
#define ID_HongLiTong_End	  				(IDM_USERDEF_MAIN_MACD_END + 20)  // 弘历通终止ID

#define IDM_TechIndex_Begin              (ID_HongLiTong_End + 100)  //K线图选指标的菜单号，此整个范围已被定义为菜单号范围，用于此相关的需注意 IDM_TechIndex_End值
#define IDM_TechIndex_End                (IDM_TechIndex_Begin + 101)//应比USER_DEFINE_END的小，qinhn 20090629 重新定义K线图选指标的菜单范围（先前和其他菜单的id冲突了）
//IDM_TechIndex_End当前值为29465，USER_DEFINE_END为29793（20091209 号）


#define IDM_HisInfo_Deal_Begin              (IDM_TechIndex_End + 1)   //历史回忆窗口 
#define IDM_HisInfo_Deal_Stat               (IDM_HisInfo_Deal_Begin + 1)   //阶段统计
#define IDM_HisInfo_Deal_PreDay             (IDM_HisInfo_Deal_Begin + 2)   //前一天
#define IDM_HisInfo_Deal_NextDay            (IDM_HisInfo_Deal_Begin + 3)   //后一天
#define IDM_HisInfo_Deal_End                (IDM_HisInfo_Deal_Begin + 10) 


// 主菜单
#define GLOBAL_BEGIN		 (USER_DEFINE_END+1)
#define GLOBAL_END			 (GLOBAL_BEGIN+500)

// 查看
#define hs_LOOKUP_BEGIN       (GLOBAL_BEGIN+1)
#define hs_LOOKUP_END	     (hs_LOOKUP_BEGIN+30) //

#define MENUTOOLBAR			 (hs_LOOKUP_BEGIN)    // 菜单条
#define MAINTOOLBAR			 (hs_LOOKUP_BEGIN+1)  // 主工具条
#define DRAWTOOLBAR			 (hs_LOOKUP_BEGIN+2)  // 画线工具条
#define SCROLLNEWS		     (hs_LOOKUP_BEGIN+3)  // 滚动信息
#define STATUS				 (hs_LOOKUP_BEGIN+4)  // 状态条
#define ONE_CURSOR			 (hs_LOOKUP_BEGIN+5)  // 一字光标
#define CROSS_CURSOR		 (hs_LOOKUP_BEGIN+6)  // 十字光标
#define STOCK_INFO			 (hs_LOOKUP_BEGIN+7)  // 股票信息
#define AUTO_CHANGEPAGE		 (hs_LOOKUP_BEGIN+8)  // 自动换页
#define MEMO				 (hs_LOOKUP_BEGIN+9)  // 备忘录
#define OPTION				 (hs_LOOKUP_BEGIN+10) // 选项
#define RPT_CHANGE_TYPE		 (hs_LOOKUP_BEGIN+11) // 类型切换(报表)
//#define QUOTE_FINANCE		 (hs_LOOKUP_BEGIN+12) // 行情/财务数据
#define CLOSE_FAVORATE		 (hs_LOOKUP_BEGIN+12) // 关闭收藏
#define BLOCK_ANALYSIS		 (hs_LOOKUP_BEGIN+13) // 1、	板块分析
#define LIANDONG_ANALYSIS	 (hs_LOOKUP_BEGIN+14) // 2、	联动分析
#define BLOCK_SUM			 (hs_LOOKUP_BEGIN+15) // 2、	板块计算
#define PROFONLINE			 (hs_LOOKUP_BEGIN+16) // 专家在线

#define Llx_HqAlarm_Menu_Off	 (hs_LOOKUP_BEGIN+17) // 预警开关
#define Llx_HqAlarm_Menu_Set	 (hs_LOOKUP_BEGIN+18) // 预警设置
#define	Llx_HqAlarm_Menu_List	 (hs_LOOKUP_BEGIN+19) // 预警列表

#define	LOOBY_PLACE_AND_LOST_CAL (hs_LOOKUP_BEGIN+20) // 盈亏计算


//	专家在线菜单项，杨朝惠添加20040412
#define	ID_VIEW_EXPERTONLINE_STATUSBAR	(hs_LOOKUP_BEGIN + 21)
//	杨朝惠添加结束
#define	IDR_STOCK_TYPE_TOOLBAR			(hs_LOOKUP_BEGIN + 22)	// 分类工具条

#define	IDR_FUTURES_CALC				(hs_LOOKUP_BEGIN + 23)   // 期货计算器

#define CLEARDRAWTOOLBAR			(hs_LOOKUP_BEGIN+24)     // 清除画线
//yangdl 2007.07.25 新华社资讯相关资讯...菜单
#ifdef Support_XHX_Dll
#define ID_XHSNew                       (hs_LOOKUP_BEGIN+25)     // 相关资讯... 
#define ID_XHSDownData                  (hs_LOOKUP_BEGIN+26)     // 导出数据..
#else
#define ID_XIADAN_NowBuy                   (hs_LOOKUP_BEGIN+25)     // 闪电买入... 
#define ID_XIADAN_NowSell                  (hs_LOOKUP_BEGIN+26)     // 闪电买出..
#define ID_XIADAN_Buy                    (hs_LOOKUP_BEGIN+27)     //快速买入
#define ID_XIADAN_Sell                   (hs_LOOKUP_BEGIN+28)     //快速卖出
#define MAINBAR                      (hs_LOOKUP_BEGIN+29)    //主工具栏 qinhn 2008-12-15号添加
#define ASSISTANTBAR                 (hs_LOOKUP_BEGIN+30)    //小工具条（目前为左边的竖状工具条） qinhn 2008-12-15号添加
#endif
// 个性服务/文件
#define PAGE_BEGIN			 (hs_LOOKUP_END+1)
#define PAGE_END			 (PAGE_BEGIN+5)

#define FIRST_PAGE			 (PAGE_BEGIN)		// 首页
#define ADD_PAGE			 (PAGE_BEGIN+1)
#define UPDATE_PAGE			 (PAGE_BEGIN+2)
#define SETCURRENT_PAGE		 (PAGE_BEGIN+3)

#define SYSTEM_PAGE_BEGIN	 (PAGE_END+1)       // 系统页面
#define SYSTEM_PAGE_END		 (SYSTEM_PAGE_BEGIN+10)

#define USER_PAGE_BEGIN		 (SYSTEM_PAGE_END+1)// 自定义页面
#define USER_PAGE_END		 (SYSTEM_PAGE_BEGIN+10)

#define CHART_BEGIN			 (USER_PAGE_END+1)  // 自定义页面
#define CHART_END			 (CHART_BEGIN+130)

#define FENBI				 (CHART_BEGIN)      // 分笔成交(&0)",						   32771
#define ONEMINUTE			 (CHART_BEGIN+1)    // MENUITEM "1分钟图(&1)",                 32772
#define FIVEMINUTE			 (CHART_BEGIN+2)    // MENUITEM "5分钟图(&5)",                 32773
#define FIFTYMINUTE			 (CHART_BEGIN+3)    // MENUITEM "15分钟图(&Q)",                32774
#define THIRTYMINUTE		 (CHART_BEGIN+4)    // MENUITEM "30分钟图(&3)",                32775
#define SIXTYMINUTE			 (CHART_BEGIN+5)    // MENUITEM "60分钟图(&6)",                32776
#define PERIOD_DAY			 (CHART_BEGIN+6)    // MENUITEM "日线图(&D)",                  32777
#define WEEK				 (CHART_BEGIN+7)    // MENUITEM "周线图(&W)",                  32778
#define MONTH				 (CHART_BEGIN+8)    // MENUITEM "月线图(&M)",                  32779
#define MOREDAY				 (CHART_BEGIN+9)    // MENUITEM "多日图(&U)",                  32780
#define MINUTE180			 (CHART_BEGIN+10)   // MENUITEM "180分钟图(&6)",

#define PERIOD_EXCHANGE      (CHART_BEGIN+11)
#define BIG_EXCHANGE	     (CHART_BEGIN+12)

#define MULTI_STOCK_CHART	 (CHART_BEGIN+13)   // MENUITEM "多股票组合\tAlt+1",           32781
#define MULTI_PERIOD_CHART	 (CHART_BEGIN+14)   // MENUITEM "多周期组合\tAlt+2",           32782
#define MULTI_FENSHI_CHART	 (CHART_BEGIN+15)   // MENUITEM "多分时组合\tAlt+3",           32783

#define TECH_FILTER_BEGIN (CHART_BEGIN+18)
#define TECH_TREND        (CHART_BEGIN+18) //分时指标
#define TECH_ANALYSIS		 (CHART_BEGIN+19)   // 技术指标
#define DEL_SHOW_ANALYSIS	 (CHART_BEGIN+20)   // 删除系统指示

#define SELECTSTOCK_ANALYSIS  (CHART_BEGIN+21)  // 条件选股
#define SELECTSTOCK_DingZhi   (CHART_BEGIN+22)  // 定制选股
#define SELECTSTOCK_ZhiBiao   (CHART_BEGIN+23)  // 指标选股
#define SELECTSTOCK_ZhiNeng   (CHART_BEGIN+24)  // 智能选股
#define SELECTSTOCK_ChaJian   (CHART_BEGIN+25)  // 插件选股

#define ALARM_ANALYSIS		 (CHART_BEGIN+26)   // 预警

#define SORT_ANALYSIS		 (CHART_BEGIN+27)   // 指标排序
#define ADD_SORT_ANALYSIS	 (CHART_BEGIN+28)   // 添加指标排序

#define APPRAISE_ANALYSIS	 (CHART_BEGIN+29)   // 评价
#define EXCELLENT_ANALYSIS	 (CHART_BEGIN+30)   // 优选
#define SHOW_ANALYSIS		 (CHART_BEGIN+31)   // 系统指示
#define INFO_ANALYSIS		 (CHART_BEGIN+32)   // 资讯
#define EXCHANGE_SYS		 (CHART_BEGIN+33)   // 交易系统
// 吕连新 2003-09-04 加入
#define IDM_GANN_RANGE			 (CHART_BEGIN+34)   // 江恩正方

#define STOCKSELECT_ANALYSIS (CHART_BEGIN+35)   // 选择股票

#define CHART_ADD			 (CHART_BEGIN+36)   // 主图叠加
#define TECH_ADD			 (CHART_BEGIN+37)   // 指标叠加

#define STANDARD_AXES		 (CHART_BEGIN+38)   // 10%分时线坐标

#define IDR_DelAllAddStock		 (CHART_BEGIN+39)		// 删除叠加股票
//#define PRICE_POWER		(CHART_BEGIN+39)    // 价格还权
#define SELFDIDINE_CHART	 (CHART_BEGIN+40)   // 自定义指标图...
#define OPEN_TOOLTREE		 (CHART_BEGIN+41)   // 打开工具...
#define OPEN_USRDEFINEKEY    (CHART_BEGIN+42)   // 自定义键盘...
#define FIND_DATA		     (CHART_BEGIN+43)   // 在公式里查找...

//请保持以下3个ID连续以1递增
#define REPORT_SELFSTOCK		     (CHART_BEGIN+44)  // 自选股...
#define REPORT_SYSSTOCK		         (CHART_BEGIN+45)  // 分类股...
#define REPORT_BOLCKSTOCK		     (CHART_BEGIN+46)  // 板块股...
#define SETTING_COMM_BLOCK		     (CHART_BEGIN+47)  // 设置常用板块...


//请保持以下3个ID连续以1递增
#define RTA_FENSHI				     (CHART_BEGIN+48)  //即时分析-分时走势图
#define RTA_TECH				     (CHART_BEGIN+49)  //即时分析-买卖力道
#define RTA_MEASURE 			     (CHART_BEGIN+50)  //即时分析-量比指标

#define Yls_ReInitData					 (CHART_BEGIN+51)  //重新初始化数据
#define LOOBY_SORT_PHASE				 (CHART_BEGIN+52)  //阶段排序，技术菜单

#define ALLBLOCK_ANALYSIS			 (CHART_BEGIN+53)  // 热门板块分析
#define QIANGRUO_ANALYSIS_REPORT	 (CHART_BEGIN+54)  // 强弱分析报表

#define FINANCE_ADD					 (CHART_BEGIN+55)  // 财务数据分析指标叠加

#define STOCK_COMPARE_CHART			 (CHART_BEGIN+56)  // 股票对比

//ADD BY YUHJ
#define IDR_hexinTradeSystem        (CHART_BEGIN+57)
#define IDR_hexinColorKLine         (CHART_BEGIN+58)
//END
#define ID_AUTOHIDEMAINBAR               (CHART_BEGIN+59)    //是否自动隐藏主工具栏  qinhn Add
#define ADDMAJOR						(CHART_BEGIN+60)   //大盘叠加  yulx add 20091111
#define ID_PAINT12						(CHART_BEGIN+61)//yulx add 20091126多股票组合图
#define ID_PAINT13						(CHART_BEGIN+62)
#define ID_PAINT14						(CHART_BEGIN+63)
#define ID_PAINT21						(CHART_BEGIN+64)
#define ID_PAINT22						(CHART_BEGIN+65)
#define ID_PAINT23						(CHART_BEGIN+66)
#define ID_PAINT24						(CHART_BEGIN+67)
#define ID_PAINT31						(CHART_BEGIN+68)
#define ID_PAINT32						(CHART_BEGIN+69)
#define ID_PAINT33						(CHART_BEGIN+70)
#define ID_PAINT34						(CHART_BEGIN+71)
#define ID_PAINT41						(CHART_BEGIN+72)
#define ID_PAINT42						(CHART_BEGIN+73)
#define ID_PAINT43						(CHART_BEGIN+74)
#define ID_PAINT44						(CHART_BEGIN+75)
#define ID_WELCOMETIP					(CHART_BEGIN+76)
#define ID_GANNCHANGESTOCK			(CHART_BEGIN+77)//end
#define STOCKMARK_DEL			(CHART_BEGIN+78)	//取消标记
#define STOCKMARK_1				(CHART_BEGIN+79) //标记1
#define STOCKMARK_2				(CHART_BEGIN+80)
#define STOCKMARK_3				(CHART_BEGIN+81)
#define STOCKMARK_4				(CHART_BEGIN+82)
#define STOCKMARK_5				(CHART_BEGIN+83)
#define STOCKMARK_6				(CHART_BEGIN+84)
#define STOCKMARK_TEXT			(CHART_BEGIN+85) //文字标记
#define STOCKMARK_MANAGE		(CHART_BEGIN+86)//标记管理
#define QUJIAN_ANALYSIS_REPORT_BEGIN (CHART_BEGIN+90)  // 区间分析报表
#define QUJIAN_ANALYSIS_REPORT_END   (QUJIAN_ANALYSIS_REPORT_BEGIN+10)  // 区间分析报表

#define QUJIAN_ANALYSIS_REPORT_ZhangFu   (QUJIAN_ANALYSIS_REPORT_BEGIN)     // 区间分析报表-涨幅
#define QUJIAN_ANALYSIS_REPORT_HuanShou  (QUJIAN_ANALYSIS_REPORT_BEGIN+1)   // 区间分析报表-换手
#define QUJIAN_ANALYSIS_REPORT_LiangBi   (QUJIAN_ANALYSIS_REPORT_BEGIN+2)	// 区间分析报表-量变
#define QUJIAN_ANALYSIS_REPORT_ZhenFu    (QUJIAN_ANALYSIS_REPORT_BEGIN+3)   // 区间分析报表－振幅
#define IDM_QUJIAN_ANALYSIS_REPORT_USERDEF	 (QUJIAN_ANALYSIS_REPORT_BEGIN+4)	// 区间分析报表-自定义
#define ID_PHASEREPORT_PARAM				 (QUJIAN_ANALYSIS_REPORT_BEGIN+5)	// 区间分析参数设置(分析区间

#define IDM_REPORT_AYALYSE_PARAM_SETEX		 (QUJIAN_ANALYSIS_REPORT_BEGIN+6)	// 添加指标
#define IDM_DATA_AND_TIME_SET				 (QUJIAN_ANALYSIS_REPORT_BEGIN+7)   // 报表分析的时间及股票设置
#define IDM_REPORT_AYALYSE_PARAM_SET		 (QUJIAN_ANALYSIS_REPORT_BEGIN+8)	// 强度参数，选择分类，更改指标


//
#define PRICE_POWER_ToolbarUsr			 (QUJIAN_ANALYSIS_REPORT_END+1)  // 除权工具条上用
#define PRICE_POWER						 (QUJIAN_ANALYSIS_REPORT_END+2)  // 价格还权
#define IDR_hexinAxisBackChuQuan			 (QUJIAN_ANALYSIS_REPORT_END+3)  // 向后除权
#define IDR_Advance_PRICE_POWER				 (QUJIAN_ANALYSIS_REPORT_END+4)  // 高级除权
#define IDR_NOT_PRICE_POWER					 (QUJIAN_ANALYSIS_REPORT_END+5)  // 不除权
#define IDR_PRICE_POWER_EX					 (QUJIAN_ANALYSIS_REPORT_END+6)  // 循环除权
#define ID_SET_COMM_WR_HEAD					 (QUJIAN_ANALYSIS_REPORT_END+7)    // 写通信头
#define ID_SET_COMM_WR_DATA					 (QUJIAN_ANALYSIS_REPORT_END+8)    // 写全站数据
#define ID_SET_COMM_NO_WR					 (QUJIAN_ANALYSIS_REPORT_END+9)    // 不写
#define ID_SET_COMM_STATUS_WND				 (QUJIAN_ANALYSIS_REPORT_END+10)   // 弹出窗口
#define ID_CHANGE_MAIN_ICON					 (QUJIAN_ANALYSIS_REPORT_END+11)   // 更改主图标
#define IDM_REPORT_AYALYSE_OPEN_UP			 (QUJIAN_ANALYSIS_REPORT_END+12)	// 打开领涨
#define IDM_REPORT_AYALYSE_OPEN_DOWN		 (QUJIAN_ANALYSIS_REPORT_END+13)	// 打开领跌


#define IDM_MULTREND_BEGIN				(QUJIAN_ANALYSIS_REPORT_END + 14)
#define IDM_MULTREND_END				(IDM_MULTREND_BEGIN + 10)	
#define IDM_MULTREND_2_DAYS			  	(IDM_MULTREND_BEGIN )	
#define IDM_MULTREND_3_DAYS			  	(IDM_MULTREND_BEGIN + 1)
#define IDM_MULTREND_4_DAYS			  	(IDM_MULTREND_BEGIN + 2)
#define IDM_MULTREND_5_DAYS			  	(IDM_MULTREND_BEGIN + 3)
#define IDM_MULTREND_6_DAYS			  	(IDM_MULTREND_BEGIN + 4)
#define IDM_MULTREND_7_DAYS			  	(IDM_MULTREND_BEGIN + 5)
#define IDM_MULTREND_8_DAYS			  	(IDM_MULTREND_BEGIN + 6)
#define IDM_MULTREND_9_DAYS			  	(IDM_MULTREND_BEGIN + 7)
#define IDM_MULTREND_10_DAYS			(IDM_MULTREND_BEGIN + 8)


// 工具条
#define hs_TOOBAR_BEGIN				 (CHART_END+1)
#define hs_TOOBAR_END				 (hs_TOOBAR_BEGIN+130)

#define IDD_SPLITTER_RIGHTHIDE			 (hs_TOOBAR_BEGIN)    //"隐藏/显示右边信息栏\n信息"
#define ID_CHART_MAX					 (hs_TOOBAR_BEGIN+1)	 //"图形放大 Up\n放大"
#define ID_CHART_MIN					 (hs_TOOBAR_BEGIN+2)	 //"图形缩小 Down\n缩小"
#define ID_LEFT_MOVE					 (hs_TOOBAR_BEGIN+3)	 //"图形左移 Ctrl + Left\n左移"
#define ID_RIGHT_MOVE					 (hs_TOOBAR_BEGIN+4)	 //"图形右移 Ctrl + Right\n右移"
#define ID_PRE_PAGE					     (hs_TOOBAR_BEGIN+5)	 //"前一屏 Page Up\n上页"
#define ID_NEXT_PAGE					 (hs_TOOBAR_BEGIN+6)	 //"后一屏 Page Down\n下页"
#define ID_CHART_PLANMOVE				 (hs_TOOBAR_BEGIN+7)  //"图形平移\n平移"

#define IDR_HOR_CUT					     (hs_TOOBAR_BEGIN+8)  // 左分割
#define IDR_VER_CUT					     (hs_TOOBAR_BEGIN+9)  // 上分割
#define IDR_HOR_CUT_Right			     (hs_TOOBAR_BEGIN+10)  // 右分割
#define IDR_VER_CUT_Bottom			     (hs_TOOBAR_BEGIN+11)  // 下分割

#define IDM_DELPANE					     (hs_TOOBAR_BEGIN+12) // 删除分割
#define IDM_UNIONPANE					 (hs_TOOBAR_BEGIN+13) // 合并分割

#define IDM_CHANGE_BROKER_LAG_CHA		 (hs_TOOBAR_BEGIN+14) // 更改经纪队列语言到中文 
#define IDM_CHANGE_BROKER_LAG_ENG		 (hs_TOOBAR_BEGIN+15) // 更改经纪队列语言到英文
#define IDM_CHANGE_BROKER_LAG_CDE		 (hs_TOOBAR_BEGIN+16) // 更改经纪队列语言到代码

#define IDM_POPUP_BROKER_INFO			 (hs_TOOBAR_BEGIN+17) // 经纪详细信息

#define IDM_USERDEF_INDEX				 (hs_TOOBAR_BEGIN+18) // 指数条配置

#define IDD_WIN_FONT_SET				 (hs_TOOBAR_BEGIN+19) // 系统设置-字体

#define ID_POP_WindowStyle				 (hs_TOOBAR_BEGIN+22) // "全屏  Ctrl + F1\n全屏"
#define IDM_USERSTOCK					 (hs_TOOBAR_BEGIN+23) 
#define IDM_BLOCKSTOCK					 (hs_TOOBAR_BEGIN+24) // 加入到板块
#define IDM_DELETESTOCK                  (hs_TOOBAR_BEGIN+25)
#define IDD_WIN_COLOR_SET				 (hs_TOOBAR_BEGIN+26) // 系统设置

#define ID_MENUITEM_SPLITTER_ONE		 (hs_TOOBAR_BEGIN+27)
#define ID_MENUITEM_SPLITTER_HTWO		 (hs_TOOBAR_BEGIN+28)
#define ID_MENUITEM_SPLITTER_VTWO		 (hs_TOOBAR_BEGIN+29)

#define ID_COMM_CONNECT					 (hs_TOOBAR_BEGIN+30) //"连接\n连接"
#define ID_COMM_DISCONNECT				 (hs_TOOBAR_BEGIN+31) //"断开\n断开"

#define IDM_EXPRESSION					 (hs_TOOBAR_BEGIN+32) //"公式管理\n公式"

#define LOAD_PAGE_BY_FILE				 (hs_TOOBAR_BEGIN+33)
#define WM_CONNECT_HQSERVER				 (hs_TOOBAR_BEGIN+34)
#define ID_BACK							 (hs_TOOBAR_BEGIN+35)
#define TIPTEXT_MSG					 (hs_TOOBAR_BEGIN+36) // tip提示消息1
#define TIPTEXT_MSG2				 (hs_TOOBAR_BEGIN+37) // tip提示消息2
#define IDR_CUTPAGE_TECH				 (hs_TOOBAR_BEGIN+38) //MENUITEM "分时走势",
#define IDR_CUTPAGE_FENSHI				 (hs_TOOBAR_BEGIN+39) 
#define ID_FUN_F10						 (hs_TOOBAR_BEGIN+40) // MENUITEM "个股资料(F10)",
#define IDR_MAX							 (hs_TOOBAR_BEGIN+41)
#define ID_MENUITEM33129				 (hs_TOOBAR_BEGIN+42)

#define ID_CURRENT_OPERATOR				 (hs_TOOBAR_BEGIN+43) // 当前操作
#define ID_XIADAN					 (hs_TOOBAR_BEGIN+44) // 下单
#define OPEN_UPDATE_CHUQUAN			 (hs_TOOBAR_BEGIN+45) // 打开/修改除权数据
#define IDS_TIPOFTHEDAY    			 (hs_TOOBAR_BEGIN+46) // 每日提示
#define DOWNLOAD   	        		 (hs_TOOBAR_BEGIN+47) // 数据download
#define HQSERVER_SET        		 (hs_TOOBAR_BEGIN+48) // 通讯设置
#define SET_USERBLOCK				 (hs_TOOBAR_BEGIN+49) // 设置自选/板块
#define	IDM_STAR_FIELD					 (hs_TOOBAR_BEGIN+50) // 星空图

#define HIDESHOW_STATUS				 STATUS // 状态条
#define HIDESHOW_MAINBAR			 MAINBAR // 工具栏 
#define HIDESHOW_ASSISTANTBAR		 ASSISTANTBAR // 工具条 
#define ID_AUTOHIDESHOW_MIANBAR          ID_AUTOHIDEMAINBAR //自动隐藏工具栏
#define DATA_DOWNLOAD				 (hs_TOOBAR_BEGIN+51) // 用户自定义数据下载
#define DATA_MANAGER				 (hs_TOOBAR_BEGIN+52) // 数据管理

//{{xzb
#define IDM_LINE_STOP                (hs_TOOBAR_BEGIN+53) //自动翻屏
#define IDM_LINE_UP                  (hs_TOOBAR_BEGIN+54) 
#define IDM_LINE_DOWN                (hs_TOOBAR_BEGIN+55) 
#define IDM_PAGE_DOWN                (hs_TOOBAR_BEGIN+58)
#define IDM_PAGE_UP					 (hs_TOOBAR_BEGIN+57)
#define ID_AUTOSCROLL_TIMER			 (hs_TOOBAR_BEGIN+56)
#define ID_COLHEADSET				 (hs_TOOBAR_BEGIN+59) //设置字段分组

#define ID_HTTP_COMM_CONNECT			 (hs_TOOBAR_BEGIN+60) // 资讯连接
#define ID_HTTP_COMM_DISCONNECT			 (hs_TOOBAR_BEGIN+61) // 资讯断开

#define ID_QIHUO_DYNMENU				 (hs_TOOBAR_BEGIN+62) // 期货报价动态菜单
#define ID_STOCK_DYNMENU				 (hs_TOOBAR_BEGIN+63) // 股票报价动态菜单

#define TICK_CHART					 (hs_TOOBAR_BEGIN+64) // TICK图
#define GROUP_ADD_TO_BLOCK			 (hs_TOOBAR_BEGIN+65) // 批量加入板块
#define DATA_OUTPUT					 (hs_TOOBAR_BEGIN+66) // 数据输出
#define MAIN_VOLUME					 (hs_TOOBAR_BEGIN+67) // 主力大单
#define OBJ_ZOOM_IN					 (hs_TOOBAR_BEGIN+68) // 图元放大
#define OBJ_ZOOM_OUT				 (hs_TOOBAR_BEGIN+69) // 图元缩小
#define DEL_FROM_BLOCK  			 (hs_TOOBAR_BEGIN+70) // 从板块删除
#define STOCKINBLOCKLIST			 (hs_TOOBAR_BEGIN+71) // 所属板块
#define ID_COMM_AUTOCONNECT				 (hs_TOOBAR_BEGIN+72) // 自动连接
#define SORTSTOCK_DYNMENU			 (hs_TOOBAR_BEGIN+73) // 排名数据项动态菜单
#define SUMSORTSTOCK_DYNMENU		 (hs_TOOBAR_BEGIN+74) // 综合排名数据项动态菜单
#define SUMSORTSTOCK_DYNMENU_HK		 (hs_TOOBAR_BEGIN+75) // 综合排名数据项动态菜单

#define HISTORY_DATA				 (hs_TOOBAR_BEGIN+76) // 历史同步回忆

#define REPORT_ANYSIS				 (hs_TOOBAR_BEGIN+77) // 报表分析
#define CAIWU_ANYSIS				 (hs_TOOBAR_BEGIN+78) // 财务分析
#define ZIXUN_SEARCH					 (hs_TOOBAR_BEGIN+80) // 资讯搜索

#define ID_SERVER_DOWNLOAD				 (hs_TOOBAR_BEGIN+81)  // 系统数据下载更新

#define LLX_ID_ADDALARM					 (hs_TOOBAR_BEGIN+82) //加入到预警

#define ID_change_password				 (hs_TOOBAR_BEGIN+83) //修改密码

#define ID_TaoLi_FenXi					 (hs_TOOBAR_BEGIN+84) //套利分析
#define ID_FudongJiSuanQi				 (hs_TOOBAR_BEGIN+85) //浮动计算器
#define ID_GangGuQiquan					 (hs_TOOBAR_BEGIN+86) //港股期权
#define ID_FanHuiLv						 (hs_TOOBAR_BEGIN+87) //反汇率
//gbq add
#define MULTI_USERDEFINE_STOCK_CHART (hs_TOOBAR_BEGIN+88)
#define ID_DEFAULT_SETTING				 (hs_TOOBAR_BEGIN+89) // 缺省设置		
#define EXPORT_FROM_BLOCK  			 (hs_TOOBAR_BEGIN+90) // 导出板块 20060206
#define ID_AREASTATISTICS				 (hs_TOOBAR_BEGIN+91) // 东航 区间统计 20060512
#define INPORT_TO_BLOCK  			 (hs_TOOBAR_BEGIN+92) // 导入板块 20070225
//
//ADD BY YUHJ 2008.6
#define Reservation                  (hs_TOOBAR_BEGIN+93) // 预约
#define Application                  (hs_TOOBAR_BEGIN+94) // 申请 
#define ClosePanel                   (hs_TOOBAR_BEGIN+95) // 行情
//ADD BY YUHJ 2008.6
#define IDD_HISTORY_REPLAY        (hs_TOOBAR_BEGIN+96)
#define IDD_FUTURE_ANTICIPATE     (hs_TOOBAR_BEGIN+97)
//END
#define IDM_HIDE_TOOLPANEL				(hs_TOOBAR_BEGIN+98) //隐藏工具栏 //ADD BY YJT 20080916
#define IDM_PWD_MODIFY            (hs_TOOBAR_BEGIN+99)
#define LLX_ID_MAINTAIN           (hs_TOOBAR_BEGIN+100)


#define IDM_NO_BIND						 (hs_TOOBAR_BEGIN+101)	//手机号码和帐号绑定
#define IDM_BOOK_SERVICE				 (hs_TOOBAR_BEGIN+102)//定制和点播短信服务
//#define IDM_LINK_ONEWEB                  (hs_TOOBAR_BEGIN+103)  // 链接某网站按钮 Add 20090325 qinhn
#define IDM_STRATEGY_MGR		(hs_TOOBAR_BEGIN+104)//add by lxqi 20090427
#define IDM_STRATEGY_TEST		(hs_TOOBAR_BEGIN+105)//add by lxqi 20090427
#define IDM_BLOCK_MANAGER			(hs_TOOBAR_BEGIN+106)//add by lxqi 20090526
#define IDM_GZ_ZIXUN			(hs_TOOBAR_BEGIN+107)//yulx add 关注资讯
#define HQSERVER_CONNECTINFO  (hs_TOOBAR_BEGIN+108) //yulx add 连接状态
#define ID_AREASORT				(hs_TOOBAR_BEGIN+109) //yulx add 阶段排名 20091126
//----------------------------------------------------------------------
#define IDM_GANN_BEGIN					(hs_TOOBAR_BEGIN+110)
#define IDM_GANN_END					(IDM_GANN_BEGIN+6)
#define IDM_GANN_SQUARE					(IDM_GANN_BEGIN)		//  江恩正方
#define IDM_GANN_HEXAGON				(IDM_GANN_BEGIN+1)		//  江恩六角
#define IDM_GANN_PARAM					(IDM_GANN_END)	        // 参数设置
//}}

#define hs_TOOBAR_END_EX				 (IDM_GANN_END+1)//(hs_TOOBAR_BEGIN+73) // end

#define DRAW_LINE_BEGIN	             (hs_TOOBAR_END_EX)
#define DRAW_LINE_END                (DRAW_LINE_BEGIN+30)

#define DRAW_EMPTY					 (DRAW_LINE_BEGIN)    // "无",
#define DRAW_ZUSELINE				 (DRAW_LINE_BEGIN+1)  // "阻速线",
#define DRAW_GOLDEN_SECTION			 (DRAW_LINE_BEGIN+2)  // "上下甘线",

#define DRAW_LINE1					 (DRAW_LINE_BEGIN+3)  // "线段",      
#define DRAW_LINE2					 (DRAW_LINE_BEGIN+4)  // "直线",    
#define DRAW_SPEEDRESISTANCELINE	 (DRAW_LINE_BEGIN+5)  // "平行线",

#define DRAW_PARALLEL				 (DRAW_LINE_BEGIN+6)  // "黄金分割线", 
#define DRAW_BAODUANLINE			 (DRAW_LINE_BEGIN+7)  // "波段线",
#define DRAW_BEIFENBI				 (DRAW_LINE_BEGIN+8)  // "百分比线",

#define DRAW_HUIGUI  				 (DRAW_LINE_BEGIN+9)  // "线性回归",
#define DRAW_DOUBLEHUIGUI			 (DRAW_LINE_BEGIN+10) // "线性回归带",
#define DRAW_HUIGUILINE				 (DRAW_LINE_BEGIN+11) // "回归通道",

#define DRAW_PERIODLINE				 (DRAW_LINE_BEGIN+12) // "周期线",
#define DRAW_LAQIELINE				 (DRAW_LINE_BEGIN+13) // "费波拉契线",

#define DRAW_RECT					 (DRAW_LINE_BEGIN+14) // "矩形",       
#define DRAW_ARCLINE1				 (DRAW_LINE_BEGIN+15) // "圆弧",
#define DRAW_ARCLINE2				 (DRAW_LINE_BEGIN+16) // "圆弧",

#define DRAW_TEXT					 (DRAW_LINE_BEGIN+17) // "文本",      
#define DRAW_UPARROW				 (DRAW_LINE_BEGIN+18) // "上箭头",        
#define DRAW_DOWNARROW				 (DRAW_LINE_BEGIN+19) // "下箭头",     


#define DRAW_OBJ_BEGIN				 DRAW_LINE_BEGIN
#define DRAW_OBJ_END				 (DRAW_DOWNARROW)	  // draw id end

#define DRAW_LIST					 (DRAW_OBJ_END+1)
#define DRAW_DELETE_ALL				 (DRAW_OBJ_END+2)
#define DRAWOBJ_PROPERTY			 (DRAW_OBJ_END+3)
#define DRAWOBJ_COPY				 (DRAW_OBJ_END+4)
#define DRAWOBJ_DELETE				 (DRAW_OBJ_END+5)

#define DRAW_SAVE					 (DRAW_OBJ_END+6) // 保存
#define DRAW_LOAD					 (DRAW_OBJ_END+7) // 装入


// 字体大小
#define FONT_SET_BEGIN	 (DRAW_LINE_END+1)
#define FONT_SET_END     (FONT_SET_BEGIN+10)

#define FONT_MAX		 FONT_SET_BEGIN
#define FONT_MAXER		 (FONT_SET_BEGIN+1)
#define FONT_MIDDLE		 (FONT_SET_BEGIN+2)
#define FONT_MINER		 (FONT_SET_BEGIN+3)
#define FONT_MIN		 (FONT_SET_BEGIN+4)

// 浏览器
#define BROWSE_ID	 	 (FONT_SET_BEGIN+9)  // 乾隆菜单

//add by lxqi 20090811		支持键盘精灵下单快捷键
#define ID_KEYBOARD_BEGIN		(FONT_SET_END + 20)
#define ID_KEYBOARD_END			(ID_KEYBOARD_BEGIN + 20)

#define ID_KEYBOARD_NOWBUY		(ID_KEYBOARD_BEGIN + 1)			//键盘精灵快捷键21，闪电买入
#define ID_KEYBOARD_NOWSELL		(ID_KEYBOARD_BEGIN + 3)		//键盘精灵快捷键23，闪电卖出
//end by lxqi 20090811

#define IDM_USERBLOCKS_BEGIN  (BROWSE_ID + 50)  // qinhn 20090702 Add
#define IDM_USERBLOCKS_END    (IDM_USERBLOCKS_BEGIN + 50)  //IDM_USERBLOCKS_END 未超过 GLOBAL_END，其减 GLOBAL_BEGIN等于438，小于500

#define IDM_HisInfo_Stat      (IDM_USERBLOCKS_END + 1)    //历史回忆阶段统计

// 光标
#define CURSOR_BEGIN				 (GLOBAL_END+1)     // 28541
#define CURSOR_END					 (CURSOR_BEGIN+50)	

#define IDC_ANCHOR_CURSOR				 28541

//13071889936

#define IDC_DRAGMOVE					 28542
#define IDC_DRAGADD						 28543
#define IDC_DRAGNEW						 28544
#define IDC_DRAGSTOP					 28545

#define IDC_CURSORMOVE					 28546

#define IDC_DRAGNODROP					 28547
#define IDR_DRAGDATA					 28548
#define IDC_DRAGVSIZE					 28549
#define IDC_FLASH						 28550
#define IDC_DRAGSIZE				     28551

#define IDC_TRENDLINE					 28552
#define IDC_SRLINE						 28553
#define IDC_SRFAN						 28554
#define IDC_SRPAL						 28555

#define IDC_RECT						 28556
#define IDD_TEXT						 28557
#define IDD_ZOOMOUT						 28558

#define IDD_ZOOMIN						 28559
#define IDC_CURSORHANDOPEN				 28560
#define IDC_CURSORHANDCLOSE				 28561
#define IDC_MULTI_CHART_CHANGE			 28562
#define IDC_DAYSIZE						 28563

// 图片
#define BITMAP_BEGIN				 (CURSOR_END+1)    // 28592
#define BITMAP_END					 (BITMAP_BEGIN+100)

#define IDB_TECHCHARTDOWNMENU			 28592
#define IDB_BITMAP_PKSEL				 28593
#define IDB_SHOWBTN						 28594
#define IDB_SHOWBTNUP					 28595
#define IDB_SMALL_IMAGELIST				 28596
#define IDB_SORT_IMG				 28597
#define	TOOLBAR						 28598
#define	FAVORATE_TOOLBAR			 28599
#define	IDR_MAINFRAME				     10111//28600

#define	IDB_ALLIMAGE				 28601
#define	IDB_DRAWIMAGE				 28602
#define	IDI_SELECTED				 28603
#define	IDB_CHECKBOXIMG				 28604
#define	DRAWLINE_TOOLBAR			 28605

#define IDB_FOLDER_LEFT                 28606
#define IDB_FOLDER_MID                  28607
#define IDB_FOLDER_RIGHT                28608
#define IDB_FOLDER_RIGHT_PLUS           28609

#define IDB_MAIN_MID				28610
#define IDB_MAIN_RIGHT_MIN				28611

#define IDB_MAIN_RIGHT					28615
#define IDI_ICON_MIN				28616 //Icon

#define IDB_LUTAN_FACE                  28617
#define IDB_FOLDER_RIGHT_NOCLR			28618
#define IDB_FOLDER_RIGHT_PLUS_NOCLR		28619

#define QICQ_ONLINE1                28631
#define QICQ_ONLINE2                28632
#define QICQ_ONLINE3                28633 // 上线
#define QICQ_BEOFF					28634 // 离开
#define QICQ_HIDE	                28635 // 隐身
#define QICQ_OUTLINE                28636 // 离线
#define IDB_CLOSE						28637 // 关闭

// 菜单
#define MENU_BEGIN				     (BITMAP_END+1)   // 28693
#define MENU_END					 (MENU_BEGIN+100)

#define NET_ID_HELP					 (MENU_BEGIN+1)
#define NET_HELP					 (MENU_BEGIN+2) // 网上帮助
#define NET_BBS						 (MENU_BEGIN+3) // 大福星bbs

// 配置
#define CONFIG_BEGIN				 (MENU_END+1)   // 28693
#define CONFIG_END					 (CONFIG_BEGIN+1000)

#define CONFIG_SECTION							 (CONFIG_BEGIN+1)
#define CONFIG_ENTRY_FONT_ADD_SIZE				 (CONFIG_BEGIN+2)   //字体增加大小
#define CONFIG_ENTRY_DEFBIGCODE					 (CONFIG_BEGIN+3)   //大盘缺省代码
#define CONFIG_ENTRY_COLORTEMPL					 (CONFIG_BEGIN+4)	//"ColorTempl.cfg"
#define CONFIG_ENTRY_FONTTEMPL					 (CONFIG_BEGIN+5)	//"FontTempl.cfg"
#define CONFIG_ENTRY_TABLECOLGROUPTEMPL			 (CONFIG_BEGIN+6)	//"TableColGroup.cfg"
#define CONFIG_ENTRY_ANALYSISTEMPLATE			 (CONFIG_BEGIN+7)	//"AnalysisTemplate"
#define CONFIG_ENTRY_TRENDTEMPLATE			   		     (CONFIG_BEGIN+8)   //高级配置
//#define STR_ADVANCE_OPTION			   		     (CONFIG_BEGIN+8)   //高级配置
#define TABLE_CELL_HEIGHT						 (CONFIG_BEGIN+9)	//"表格高度"
#define TABLE_CELL_WIDTH						 (CONFIG_BEGIN+10)	//"表格宽度"
#define TABLE_HEAD_HEIGHT						 (CONFIG_BEGIN+11)  //"表格头高度"
#define CONFIG_XIADAN_PROGRAMSET			     (CONFIG_BEGIN+12)	//快捷程序设置 “快捷程序”
#define CONFIG_ZHANGZHONGBAO_INFO				 (CONFIG_BEGIN+13)  //"InfoTempl.cfg"
#define CONFIG_ZHANGZHONGBAO_INFO_SECTION		 (CONFIG_BEGIN+14)  //"咨询"

//	财务图示菜单，杨朝惠加入，20030925
#define	HS_IDR_FINANCE_BEGIN							(CONFIG_BEGIN + 100)
#define	HS_IDR_FINANCE_END								(CONFIG_BEGIN + 106)
#define	HS_IDR_GRAPHIC_FINANCE							HS_IDR_FINANCE_BEGIN //	财务图示
#define	HS_PAGEID_GRAPHIC_FINANCE_VIEW					(HS_IDR_FINANCE_BEGIN + 1)

// 字符串
#define STRING_BEGIN				 (HS_IDR_FINANCE_END+1)
#define STRING_END					 (STRING_BEGIN+100)

#define STR_TIP_TITLE							STRING_BEGIN
#define STR_OPEN_FORMULA_LIST					(STRING_BEGIN+1)
#define STR_SAY_CLICK_UPDATE_PARAM				(STRING_BEGIN+2)
#define EXPRESS_ALREADY_DELETE					(STRING_BEGIN+3)
#define DISKSECTION								(STRING_BEGIN+4)   //	_T("数据读取路径")
#define DISKENTER_NOW						    (STRING_BEGIN+5)   //_T("盘中数据")
#define DISKENTER_PERIOD_TYPE_DAY				(STRING_BEGIN+6)   //_T("盘后数据")

#define PROGRAM_UPDATE_VERSION_URL              (STRING_BEGIN+7)   // 程序更新版本URL
#define DATA_DOWNLOAD_URL_LIST				    (STRING_BEGIN+8)   // 数据下载URL List
#define DATA_DOWNLOAD_URL_LIST_WEEK			    (STRING_BEGIN+9)   // 数据下载URL 按星期

#define DISKENTER_ZIZUNDATAPATH					(STRING_BEGIN+10)  // 当前缺省咨询数据路径
#define HLW_COMM_BLOCK								(STRING_BEGIN+11)  //"常用板块"
#define IDS_VERSION_NOTE							(STRING_BEGIN+12)  //"版本信息文件说明" 30825


// 右边小图tab弹出菜单
#define hs_RightTabPopup_MENU_BEGIN    40000
#define hs_RightTabPopup_MENU_END	  (hs_RightTabPopup_MENU_BEGIN+40)

#define IDR_hexinRightTabPrice			 (hs_RightTabPopup_MENU_BEGIN)		// "报价", 
#define IDR_hexinRightTabDetail			 (hs_RightTabPopup_MENU_BEGIN+1)		// "明细",
#define IDR_hexinRightTabFenjia			 (hs_RightTabPopup_MENU_BEGIN+2)		// "分价表", 
#define IDR_hexinRightTabFenShi			 (hs_RightTabPopup_MENU_BEGIN+3)		// "分时", 
#define IDR_hexinRightTabBig			 (hs_RightTabPopup_MENU_BEGIN+4)		// "大盘",   
#define IDR_hexinRightTabBuySellTable	 (hs_RightTabPopup_MENU_BEGIN+5)		// "买卖盘", 
#define IDR_hexinRightTabBuySellChart    (hs_RightTabPopup_MENU_BEGIN+6)		// "买卖图", 
#define IDR_hexinRightTabNet		     (hs_RightTabPopup_MENU_BEGIN+7)		// "网页",   
#define IDR_hexinRightTabUser		     (hs_RightTabPopup_MENU_BEGIN+8)		// "自定图", 
//#define IDR_hexinRightTabValue		 (hs_RightTabPopup_MENU_BEGIN+9)		// "值"
#define IDR_hexinRightTabOlnyDetail		 (hs_RightTabPopup_MENU_BEGIN + 10)  // "只要明细"
#define IDR_hexinRightTabSetting		 (hs_RightTabPopup_MENU_BEGIN + 11)  // "设置..."

// 港股页面
#define IDR_HK_R_QUOTE					 (hs_RightTabPopup_MENU_BEGIN + 12)  // "大利市报价"
#define IDR_HK_R_ORDER					 (hs_RightTabPopup_MENU_BEGIN + 13)  // "买卖盘明细"
#define IDR_HK_R_BORKER_BUY				 (hs_RightTabPopup_MENU_BEGIN + 14)  // "经纪龙虎榜-买"
#define IDR_HK_R_BORKER_SELL			 (hs_RightTabPopup_MENU_BEGIN + 15)  // "经纪龙虎榜-卖"
#define IDR_HK_R_BORKER_BUY_NAME		 (hs_RightTabPopup_MENU_BEGIN + 16)  // "经纪龙虎榜-买名"
#define IDR_HK_R_BORKER_SELL_NAME		 (hs_RightTabPopup_MENU_BEGIN + 17)  // "经纪龙虎榜-卖名"
#define IDR_HK_R_LINE					 (hs_RightTabPopup_MENU_BEGIN + 18)  // "相关证券"
#define IDR_HK_R_STATIC					 (hs_RightTabPopup_MENU_BEGIN + 19)  // "静态数据"
#define IDR_HK_R_VALUE					 (hs_RightTabPopup_MENU_BEGIN + 20)  // "港股值"

// 认股证相关
#define IDR_HK_R_QUOTE_WARRANTS			 (hs_RightTabPopup_MENU_BEGIN + 21)  // "认股证大利市报价"
#define IDR_HK_R_STATIC_WARRANTS		 (hs_RightTabPopup_MENU_BEGIN + 22)  // "认股证静态数据"
#define IDR_HK_R_VALUE_WARRANTS			 (hs_RightTabPopup_MENU_BEGIN + 23)  // "认股证值"

// 期货页面
#define IDR_FT_R_VALUE					 (hs_RightTabPopup_MENU_BEGIN + 24)  // "期货右小图值"
#define IDR_FT_R_TICK					 (hs_RightTabPopup_MENU_BEGIN + 25)  // "期货右分笔"
#define IDR_FT_R_QUOTE					 (hs_RightTabPopup_MENU_BEGIN + 26)  // "期货右报价"

// 外汇相关面页
#define IDR_FR_R_QUOTE					 (hs_RightTabPopup_MENU_BEGIN + 27)  // "外汇右报价"

// 期货多空统计表
#define IDR_FT_R_TOTALTABLE				 (hs_RightTabPopup_MENU_BEGIN + 28)  // "期货多空统计表"

// 钱龙风格港股右小图
#define IDR_ML_HKQUOTE					(hs_RightTabPopup_MENU_BEGIN + 29)  // "钱龙风格港股右小图"

//add by fangly: Level2右小图相关宏
#define IDR_BuySell_QUEUE            (hs_RightTabPopup_MENU_BEGIN + 30)  // "level2逐笔成交"
#define IDR_Cancel_QUEUE             (hs_RightTabPopup_MENU_BEGIN + 31)  // "level2买卖队列"
#define IDR_MaxCancel_QUEUE          (hs_RightTabPopup_MENU_BEGIN + 32)  
#define IDR_MaxTotalCancel_QUEUE     (hs_RightTabPopup_MENU_BEGIN + 33)


#define IDR_Usr_def_wnd_Begin		     IDR_MaxTotalCancel_QUEUE	   //	
#define IDR_hexinRightTabFinance		 (IDR_Usr_def_wnd_Begin + 1)   // "财务",
#define IDR_hexinRightTabBuySellTray	 (IDR_Usr_def_wnd_Begin + 2)   // "盘",
#define IDR_hexinRightTabChuQuan		 (IDR_Usr_def_wnd_Begin + 3)   // "除权/配股",
#define IDR_hexinRightTabDaDan  		 (IDR_Usr_def_wnd_Begin + 4)   // "大单",
#define IDR_hexinRightTabValue			 (IDR_Usr_def_wnd_Begin + 5)   // "值",	
#define IDR_Usr_def_wnd_End 		     (IDR_Usr_def_wnd_Begin + 6)

#define IDR_ETF_QUOTE					 (IDR_Usr_def_wnd_Begin + 10)  // ETF小图报价
#define IDR_TITLE						 (IDR_Usr_def_wnd_Begin + 11)  // 标题           IDR_TITLE当前值为40044（20091113）

#define IDR_HistoryInfo_Begin            (IDR_Usr_def_wnd_Begin + 12)
#define IDR_HistoryInfo_perDeal 		 (IDR_Usr_def_wnd_Begin + 13)   // 查看历史回忆 分笔成交
#define IDR_HistoryInfo_DealperMin 		 (IDR_Usr_def_wnd_Begin + 14)   // 查看历史回忆 每分钟成交
#define IDR_HistoryInfo_OtherInfo 		 (IDR_Usr_def_wnd_Begin + 15)   // 查看历史回忆 每分钟成交
#define IDR_HistoryInfo_End              (IDR_Usr_def_wnd_Begin + 16)

// 2003.04.13
#define PAGE_MANAGER_BEGIN					  40500							  // 页面管理开始ID
#define PAGE_MANAGER_END					  (PAGE_MANAGER_BEGIN + 320)  // 页面管理终止ID 629

#define PAGE_BEGINCUT						  (PAGE_MANAGER_BEGIN + 1)    // 开始分割页面
#define PAGE_NEWCUT							  (PAGE_MANAGER_BEGIN + 2)    // 新建分割页面
#define PAGE_DELCUT							  (PAGE_MANAGER_BEGIN + 3)    // 删除分割页面
#define PAGE_SAVECUT						  (PAGE_MANAGER_BEGIN + 4)    // 保存分割页面
#define PAGE_SAVEATCUT						  (PAGE_MANAGER_BEGIN + 5)    // 另保存分割页面
#define PAGE_OPENCUT						  (PAGE_MANAGER_BEGIN + 6)    // 打开分割页面
#define PAGE_CLOSECUT						  (PAGE_MANAGER_BEGIN + 7)    // 关闭分割页面

#define PAGE_DELETE_MODEL					  (PAGE_MANAGER_BEGIN + 8)    // 删除模块
#define PAGE_CREATE_UNION					  (PAGE_MANAGER_BEGIN + 9)    // 建立关联
#define PAGE_DELETE_UNION					  (PAGE_MANAGER_BEGIN + 10)   // 删除关联

// 拆分中使用
#define PAGE_FixedWidth					      (PAGE_MANAGER_BEGIN+11) // 固定宽度
#define PAGE_FixedHeight					  (PAGE_MANAGER_BEGIN+12) // 固定高度
#define PAGE_Group1							  (PAGE_MANAGER_BEGIN+13) // 分组1
#define PAGE_Group2						      (PAGE_MANAGER_BEGIN+14) // 分组2
#define PAGE_Group3						      (PAGE_MANAGER_BEGIN+15) // 分组3
#define PAGE_Group4						      (PAGE_MANAGER_BEGIN+16) // 分组4

//
#define PAGE_TAB_SHOW_TOP					  (PAGE_MANAGER_BEGIN + 20)   // 在顶部显示tab
#define PAGE_TAB_CLOSE_PAGE					  (PAGE_MANAGER_BEGIN + 21)   // 关闭当前tab页
#define PAGE_TAB_PREV_PAGE					  (PAGE_MANAGER_BEGIN + 22)   // tab前一页
#define PAGE_TAB_NEXT_PAGE					  (PAGE_MANAGER_BEGIN + 23)   // tab后一页

// 插入模块--子菜单ID
#define INSERT_MODULE_DYNMENU				  (PAGE_MANAGER_BEGIN + 30)    // 插入模块动态菜单
#define INSERT_MODULE_BEGIN					  (INSERT_MODULE_DYNMENU  + 1) // 分割-插入模块开始ID
#define INSERT_POSITION            (INSERT_MODULE_DYNMENU + 100)//图元菜单
#define INSERT_BOTTOM              (INSERT_MODULE_DYNMENU + 101)//图元菜单
#define INSERT_TOPMOST             (INSERT_MODULE_DYNMENU + 102)//图元菜单
#define INSERT_MOVEUP              (INSERT_MODULE_DYNMENU + 103)//图元菜单
#define INSERT_MOVEDOWN            (INSERT_MODULE_DYNMENU + 104)//图元菜单
#define INSERT_BACKGND             (INSERT_MODULE_DYNMENU + 105)//图元菜单

#define INSERT_OBJECT_MENU             (INSERT_MODULE_DYNMENU + 106)//图元菜单

#define INSERT_MODULE_END					  (INSERT_MODULE_BEGIN + 200)   // END

#define IDD_WIN_STYLE_UI						  (INSERT_MODULE_END + 1)	  // 风格UI消息
#define IDD_WIN_STYLE_BEGIN						  (IDD_WIN_STYLE_UI + 1)		  // 风格开始
#define IDD_WIN_STYLE_END						  (IDD_WIN_STYLE_BEGIN + 20)	  // 风格结束

//#define IDD_WIN_STYLE_STOCK						  (PAGE_MANAGER_BEGIN + 11)   // 股票风格
//#define IDD_WIN_STYLE_FUTURES					  (PAGE_MANAGER_BEGIN + 12)   // 期货风格
//#define IDD_WIN_STYLE_HK						  (PAGE_MANAGER_BEGIN + 13)   // 港股风格


#define IDR_INDEX_BEGIN					(PAGE_MANAGER_END+1)
#define IDR_INDEX_END					(IDR_INDEX_BEGIN+300)

#define IDR_INDEX_HK_UI					(IDR_INDEX_BEGIN+1)	// 港股指数
#define IDR_INDEX_FT_UI					(IDR_INDEX_BEGIN+2)	// 期货指数
#define IDR_INDEX_WP_UI					(IDR_INDEX_BEGIN+3)	// 外盘指数
#define IDR_INDEX_FX_UI					(IDR_INDEX_BEGIN+4)	// 外汇指数

#define IDR_INDEX_HK_BEGIN				(IDR_INDEX_BEGIN+10)	// 港股指数
#define IDR_INDEX_FT_BEGIN				(IDR_INDEX_BEGIN+110)	// 期货指数
#define IDR_INDEX_WP_BEGIN				(IDR_INDEX_BEGIN+160)	// 外盘指数
#define IDR_INDEX_FX_BEGIN				(IDR_INDEX_BEGIN+260)	// 外汇指数

// gbq add, 20030208, 用于特色服务菜单
#define HS_ADD_SERVICE_MENU_BEGIN		(IDR_INDEX_END + 1)
#define HS_ADD_SERVICE_MENU_END			(HS_ADD_SERVICE_MENU_BEGIN + 100)
// gbq add end;

#define hs_END	CONFIG_END	
#endif  // _HEXINBASERES_H_
