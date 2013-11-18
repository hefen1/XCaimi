
#pragma once

#define HS_LANGUAGE(x)	            (x)
#define HS_LANGUAGE_CODE(x,y,z)         (y)
#define HS_LANGUAGE_MARKET(x,y)       (y)

#define GVL_FIXFOCUSEDGE		0X00004000  // 调整焦点边框
#define GVL_FIXEDGE				0X00008000  // 调整边框
#define GVL_ALLOWROWRESIZE		0X00010000  // 表格行可以大小
#define GVL_ALLOWCOLUMNRESIZE	0X00020000  // 表格列可以大小
#define GVL_SENDMESSAGETOPARENT	0X00040000
#define GVL_SIZECOLUMNTOFIT		0X00080000  // 使列大小充满整个显示区域
#define GVL_LARGEFONT			0X00100000  // 大字报价

// Messages sent to the grid's parent
#define	GVN_BEGINDRAG           LVN_BEGINDRAG        // LVN_FIRST-9
#define	GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT   // LVN_FIRST-5
#define	GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define	GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     // LVN_FIRST-6


#define GVN_GETDISPINFO         LVN_GETDISPINFO 
#define GVN_ODCACHEHINT         LVN_ODCACHEHINT


#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT

// 文本显示
#define	GVIF_NONO				0x00000100  // 无
#define	GVIF_ARROW				0x00000200  // 箭头
#define	GVIF_UNDELINE			0x00000400  // 下划线
#define	GVIF_SYMBOL				0x00000800  // 加入"+""-"

// 颜色显示
#define	GVIF_FgClrSAMEHEAD		0x00001000  // 颜色和表格头一样

// 数据类型
#define GVIF_STRING				0x00010000			//字符型
#define GVIF_NUMBER				0x00020000			//数字整型
#define GVIF_DOUBLE				0x00040000			//浮点型
#define GVIF_DATETIME			0x00080000			//日期型

#define GVIF_MEMO				0x00100000		//不定长的备注型数据
#define	GVIF_FIXED				0x00200000        // 列为固定
#define	GVIF_AUTOSIZE			0x00400000		// 列大小为最佳匹配
#define	GVIF_VALUEABS			0x00800000		// 列排序时将数值取绝对值
#define	GVIF_USERDEFINE			0x01000000		// 自定列
#define	GVIF_DRAWGRAPHIC		0x02000000		// 行为绘图型 (横向)
#define	GVIF_DRAWGRAPHIC_VER	0x04000000		// 列为绘图型（纵向）
#define	GVIF_DRAWGRAPHIC_TEXT	0x08000000		// 列为绘图型（显示文本）
/* 表格属性 DEFINE END */

/* 字段大类风格 DEFINE BEGIN*/
#define MPEXS_RWCONFIG			0x0001			// 支持读写配置文件
#define MPEXS_COLNAME_NORWCFG	0x0002			// 字段名称(列名)不从配置文件读取

#define MPEXT_HQ				0x01			// 行情
#define MPEXT_ORIGINAL_USE		0x02			// 内部使用
#define MPEXT_USER_DEFINE		0x03			// 用户自定义
#define MPEXT_INFO_EMAIL		0x04			// 资讯/E-Mail
#define MPEXT_BLOCK				0x05			// 板块信息		add by lxqi 20090703
/* 字段大类风格 DEFINE END*/

// 默认掩码值
#define ROWMASK_DEFAULT			GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM|GVIF_BKCLR|GVIF_FGCLR
// 默认格式
#define ROWFORMAT_DEFAULT		DT_RIGHT|DT_VCENTER|DT_SINGLELINE

#define Table_GridTable_Mark						_T("%s-报表标签设置")
#define Table_GridTable_Report_Tab_Head				_T("报表TAB对应的抬头")
#define Table_GridTable_Report_Tab_Active			_T("报表TAB激活项")

#define Table_GridTable_Report_Tab_Open				_T("报表缺省打开项")

#define Table_GridTable_Default_ColGroup			_T("缺省")
#define Table_GridTable_Default_ColGroup_Stock		_T("股票")
#define Table_GridTable_Default_ColGroup_QH			_T("期货")
#define Table_GridTable_Default_ColGroup_CAIWU		_T("财务")
#define Table_GridTable_Default_ColGroup_FUND		_T("基金")
#define Table_GridTable_Default_ColGroup_BOND		_T("债券")
#define Table_GridTable_Default_ColGroup_FOREIGN	_T("外汇")
#define Table_GridTable_Default_ColGroup_HK			_T("港股")

#define COLORID_BEGIN									1
#define COLORID_END								(COLORID_BEGIN + 2000)

#define COLOR_REPORT_BEGIN						(COLORID_BEGIN +1)					//2

#define COLOR_Background						(COLOR_REPORT_BEGIN +1)				//背景色   
#define COLOR_ReportCode						(COLOR_REPORT_BEGIN +2)				//报价证券代码
#define COLOR_RepottCodeName					(COLOR_REPORT_BEGIN +3)				//报价证券名称
#define COLOR_SelfSelectCode                    (COLOR_REPORT_BEGIN +4)				//自选股票名称
#define COLOR_PriceUp							(COLOR_REPORT_BEGIN +5)				//上涨
#define COLOR_PriceDown							(COLOR_REPORT_BEGIN +6)				//下跌
#define COLOR_PriceEqual						(COLOR_REPORT_BEGIN +7)				//平盘
#define COLOR_PreClose							(COLOR_REPORT_BEGIN +8)				//昨收
#define COLOR_TotalHand							(COLOR_REPORT_BEGIN +9)				//总手
#define COLOR_CurrentHand						(COLOR_REPORT_BEGIN +10)			//现手
#define COLOR_TradeMoney						(COLOR_REPORT_BEGIN +11)			//成交额
#define COLOR_OrderBuyVolume                    (COLOR_REPORT_BEGIN +12)			//委买量
#define COLOR_OrderSellVolume                   (COLOR_REPORT_BEGIN +13)			//委卖量
#define COLOR_ExchangeRatio                     (COLOR_REPORT_BEGIN +14)			//换手率
#define COLOR_LiangBi							(COLOR_REPORT_BEGIN +15)			//量比
#define COLOR_ShiYingLv							(COLOR_REPORT_BEGIN +16)			//市盈率
#define COLOR_HightBackground                   (COLOR_REPORT_BEGIN +17)			//报价加亮背景色
#define COLOR_HightForegroud                    (COLOR_REPORT_BEGIN +18)			//报价加亮前景色
#define COLOR_PriceChangeBk						(COLOR_REPORT_BEGIN +19)			//行情更新.高亮文本背景
#define COLOR_SortUp							(COLOR_REPORT_BEGIN +20)			//报价升序加亮
#define COLOR_SortDown							(COLOR_REPORT_BEGIN +21)			//报价降序加亮
#define COLOR_GridHead							(COLOR_REPORT_BEGIN +22)			//报价抬头文本
#define COLOR_GridFixed							(COLOR_REPORT_BEGIN +23)			//报价固定文本
#define COLOR_FixTabText						(COLOR_REPORT_BEGIN +24)			//固定Tab文本
#define COLOR_TabText							(COLOR_REPORT_BEGIN +25)			//活动Tab文本
#define COLOR_ActiveTabBK						(COLOR_REPORT_BEGIN +26)			//活动Tab击活背景
#define COLOR_ActiveTabText						(COLOR_REPORT_BEGIN +27)			//活动Tab击活文本
#define COLOR_TabBK								(COLOR_REPORT_BEGIN +28)			//Tab背景
#define COLOR_TabGrid							(COLOR_REPORT_BEGIN +29)			//Tab边框
#define COLOR_GeneralSortVerLine				(COLOR_REPORT_BEGIN +30)			//综合排名横线
#define COLOR_GeneralSortSplitterLine			(COLOR_REPORT_BEGIN +31)			//综合排名.拆分边框色
#define COLOR_GuideTab_Frame					(COLOR_REPORT_BEGIN +32)			// 固定标签页边框线
#define COLOR_GuideTab_BK						(COLOR_REPORT_BEGIN +33)			// 固定标签页背景
#define COLOR_FixedRowColBK						(COLOR_REPORT_BEGIN +34)			// 报价固定行固定列背景色
#define COLOR_FixedRowBK						(COLOR_REPORT_BEGIN +35)			// 报价固定行背景色
#define COLOR_FixedColBK						(COLOR_REPORT_BEGIN +36)			// 报价固定列背景色
#define COLOR_TabButtonBK						(COLOR_REPORT_BEGIN +37)			// 标签页按钮背景

#define COLOR_REPORT_END					(COLOR_REPORT_BEGIN +200)				//202


#define	COLOR_TREND_BEGIN						(COLOR_REPORT_END )						//202
#define COLOR_TrendAverage						(COLOR_TREND_BEGIN +1)					//分时图.均线
#define COLOR_TrendPrice						(COLOR_TREND_BEGIN +2)					//分时图.价格线
#define COLOR_Volume_Up							(COLOR_TREND_BEGIN +3)					//分时图.成交量跌线
#define COLOR_Volume_Down						(COLOR_TREND_BEGIN +4)					//分时图.成交量涨线
#define COLOR_VerAmountTxt						(COLOR_TREND_BEGIN +5)					//成交量坐标文本
#define COLOR_TrendGridLine						(COLOR_TREND_BEGIN +6)					//分时图.网格横线
#define COLOR_TrendMidHorLine					(COLOR_TREND_BEGIN +7)					//分时图.中间横线
#define COLOR_TrendMidVerLine					(COLOR_TREND_BEGIN +8)					//分时图.中间竖线
#define COLOR_TrendHourLine						(COLOR_TREND_BEGIN +9)					//分时图.小时线
#define COLOR_TrendHalfHourLine					(COLOR_TREND_BEGIN +10)					//分时图.半小时线
#define COLOR_TrendAxisLine						(COLOR_TREND_BEGIN +11)					//分时,日线图坐标轴线
#define COLOR_TrendScaleLine					(COLOR_TREND_BEGIN +12)					//分时,日线坐标刻度线
#define COLOR_TrendLiangBiTxt					(COLOR_TREND_BEGIN +13)					//量比指标坐标文本
#define COLOR_TrendADLTXT						(COLOR_TREND_BEGIN +14)					//ADL指标坐标文本
#define COLOR_TrendLiangBiLine					(COLOR_TREND_BEGIN +15)					//量比指标线
#define COLOR_TrendADLline						(COLOR_TREND_BEGIN +16)					//ADL指标线
#define COLOR_DuoKongAxixTxt					(COLOR_TREND_BEGIN +17)					//多空指标坐标文本
#define COLOR_BuySellTxt						(COLOR_TREND_BEGIN +18)					//买卖力道坐标文本
#define COLOR_DuoKongLine						(COLOR_TREND_BEGIN +19)					//多空指标线
#define COLOR_DiffValue							(COLOR_TREND_BEGIN +20)					//买卖差值
#define COLOR_TrendBuyLine						(COLOR_TREND_BEGIN +21)					//买卖力道之买量线
#define COLOR_TrendSellLine						(COLOR_TREND_BEGIN +22)					//买卖力道之卖量线
#define COLOR_TrendIOPV							(COLOR_TREND_BEGIN +23)					//IOPV线
#define COLOR_DrawBoxline						(COLOR_TREND_BEGIN +24)					//浮动框边框(须重启)
#define COLOR_DrawBoxTxt						(COLOR_TREND_BEGIN +25)					//浮动框字段文本

#define COLOR_TREND_END							(COLOR_TREND_BEGIN + 200)				//402


#define COLOR_KLINE_BEGIN					(COLOR_TREND_END )					    //402
#define COLOR_KLINE_HorAxisTxt					(COLOR_KLINE_BEGIN +1)				//日线横坐标文本
#define COLOR_KLINE_VerAxisTxt					(COLOR_KLINE_BEGIN +2)				//日线纵坐标文本
#define COLOR_KLINE_UpLine						(COLOR_KLINE_BEGIN +3)				//日线图.上涨线(阳线)
#define COLOR_KLINE_DownLine					(COLOR_KLINE_BEGIN +4)				//日线图.下跌线(阴线)
#define COLOR_KLINE_UpFill						(COLOR_KLINE_BEGIN +5)				//日线图.阳线填充色
#define COLOR_KLINE_DownFile					(COLOR_KLINE_BEGIN +6)				//日线图.阴线填充色
#define COLOR_KLINE_MaxMinBK					(COLOR_KLINE_BEGIN +7)				//最大最小值加亮背景
#define COLOR_KLINE_GridHorLine					(COLOR_KLINE_BEGIN +8)				//日线图.网格横线
#define COLOR_KLINE_GridVerLine					(COLOR_KLINE_BEGIN +9)				//日线图.网格竖线
#define COLOR_KLINE_TopHorline					(COLOR_KLINE_BEGIN +10)				//日线图.顶部横线
#define COLOR_KLINE_IndexName					(COLOR_KLINE_BEGIN +11)				//指标线名称文本
#define COLOR_KLINE_ChartTxt					(COLOR_KLINE_BEGIN +12)				//图元内文本
#define COLOR_KLINE_TipTitleBk					(COLOR_KLINE_BEGIN +13)				//tip提示标题背景
#define COLOR_KLINE_TipTitleFg					(COLOR_KLINE_BEGIN +14)				//tip提示标题前景
#define COLOR_KLINE_TipTextBK					(COLOR_KLINE_BEGIN +15)				//tip提示文本背景
#define COLOR_KLINE_TipTextFg					(COLOR_KLINE_BEGIN +16)				//tip提示文本前景
#define COLOR_KLINE_MoveLine					(COLOR_KLINE_BEGIN +17)				//移动光标线
#define COLOR_KLINE_END						(COLOR_KLINE_BEGIN + 200)				//602


#define COLOR_RIGHTVIEW_BEGIN				(COLOR_KLINE_END )						//602
#define COLOR_RIGHTVIEW_CodeName				(COLOR_RIGHTVIEW_BEGIN +1 )				//右图.分价.明细证券名称
#define COLOR_RIGHTVIEW_Code				    (COLOR_RIGHTVIEW_BEGIN +2 )				//右图.分价.明细证券代码
#define COLOR_RIGHTVIEW_Time					(COLOR_RIGHTVIEW_BEGIN +3 )				//右图.时间日期
#define COLOR_RIGHTVIEW_Head					(COLOR_RIGHTVIEW_BEGIN +4 )				//右图字段名称
#define COLOR_RIGHTVIEW_HorLine					(COLOR_RIGHTVIEW_BEGIN +5 )				//右图横线
#define COLOR_RIGHTVIEW_VerLine					(COLOR_RIGHTVIEW_BEGIN +6 )				//右图竖线
#define COLOR_RIGHTVIEW_CodeGrade				(COLOR_RIGHTVIEW_BEGIN +7 )				//右图附加星级色
#define COLOR_RIGHTVIEW_CodeGradeFg				(COLOR_RIGHTVIEW_BEGIN +8 )				//右图附加类型前景色
#define COLOR_RIGHTVIEW_CodeGradeBK				(COLOR_RIGHTVIEW_BEGIN +9 )				//右图附加类型背景色
#define COLOR_RIGHTVIEW_MainIndexName			(COLOR_RIGHTVIEW_BEGIN +10)				//大盘右图名称列
#define COLOR_RIGHTVIEW_TableTitle              (COLOR_RIGHTVIEW_BEGIN +11)             // 表格标题图元字体颜色
#define COLOR_RIGHTVIEW_TableTitleBK            (COLOR_RIGHTVIEW_BEGIN +12)             // 表格标题图元背景色
#define COLOR_RIGHTVIEW_TRADEFIXEDNAME          (COLOR_RIGHTVIEW_BEGIN +13)             // 交易图元名称列

#define COLOR_RIGHTVIEW_END					(COLOR_RIGHTVIEW_BEGIN + 200)			//802


#define COLOR_INFORMATION_BEGIN				(COLOR_RIGHTVIEW_END )					//802
#define COLOR_INFORMATION_Bk					(COLOR_INFORMATION_BEGIN +1)			//资讯背景颜色
#define COLOR_INFORMATION_Fg					(COLOR_INFORMATION_BEGIN +2)			//资讯前景颜色
#define COLOR_INFORMATION_END				(COLOR_INFORMATION_BEGIN + 200)			//1002


#define COLOR_MISCELLANY_BEGIN				(COLOR_INFORMATION_END )				//1002
#define COLOR_MISCELLANY_StatusTxt				(COLOR_MISCELLANY_BEGIN +1)				//状态条文本
#define COLOR_MISCELLANY_HuoYanShan				(COLOR_MISCELLANY_BEGIN +2)				//火焰山
#define COLOR_MISCELLANY_HuoYaoDu				(COLOR_MISCELLANY_BEGIN +3)				//活跃度
#define COLOR_MISCELLANY_DefCos					(COLOR_MISCELLANY_BEGIN +4)				//成本分布
#define COLOR_MISCELLANY_PopInfoBk				(COLOR_MISCELLANY_BEGIN +5)				//信息提示背景色
#define COLOR_MISCELLANY_PopInfoFg				(COLOR_MISCELLANY_BEGIN +6)				//信息提示前景色
#define COLOR_MISCELLANY_ScrollTxt              (COLOR_MISCELLANY_BEGIN +7)             //跑马灯文本颜色
#define COLOR_MISCELLANY_ScrollBK               (COLOR_MISCELLANY_BEGIN +8)             //跑马灯背景颜色
#define COLOR_MISCELLANY_DiagramLayoutBKColor            (COLOR_MISCELLANY_BEGIN +9)             //图层标签页背景	
#define COLOR_MISCELLANY_DiagramLayoutActiveFill         (COLOR_MISCELLANY_BEGIN +10)             //图层标签页激活
#define COLOR_MISCELLANY_DiagramLayoutFill               (COLOR_MISCELLANY_BEGIN +11)             //图层标签页常规
#define COLOR_MISCELLANY_DiagramLayoutFore               (COLOR_MISCELLANY_BEGIN +12)             //图层标签页前景
#define COLOR_MISCELLANY_DiagramLayoutBorder             (COLOR_MISCELLANY_BEGIN +13)             //图层标签页边框
#define COLOR_MISCELLANY_END				(COLOR_MISCELLANY_BEGIN + 200)			//1202


#define COLOR_DEFAULTINDEX_BEGIN			(COLOR_MISCELLANY_END)				//1202
#define COLOR_DefTechColor1						(COLOR_DEFAULTINDEX_BEGIN +1)				//缺省指标线1
#define COLOR_DefTechColor2						(COLOR_DEFAULTINDEX_BEGIN +2)				//缺省指标线2
#define COLOR_DefTechColor3						(COLOR_DEFAULTINDEX_BEGIN +3)				//缺省指标线3
#define COLOR_DefTechColor4						(COLOR_DEFAULTINDEX_BEGIN +4)				//缺省指标线4
#define COLOR_DefTechColor5						(COLOR_DEFAULTINDEX_BEGIN +5)				//缺省指标线5
#define COLOR_DefTechColor6						(COLOR_DEFAULTINDEX_BEGIN +6)				//缺省指标线6
#define COLOR_DefTechColor7						(COLOR_DEFAULTINDEX_BEGIN +7)				//缺省指标线7
#define COLOR_DefTechColor8						(COLOR_DEFAULTINDEX_BEGIN +8)				//缺省指标线8
#define COLOR_DefTechColor9						(COLOR_DEFAULTINDEX_BEGIN +9)				//缺省指标线9
#define COLOR_DefTechColor10					(COLOR_DEFAULTINDEX_BEGIN +10)				//缺省指标线10
#define COLOR_DefTechColor11					(COLOR_DEFAULTINDEX_BEGIN +11)				//缺省指标线11
#define COLOR_DefTechColor12					(COLOR_DEFAULTINDEX_BEGIN +12)				//缺省指标线12
#define COLOR_DefTechColor13					(COLOR_DEFAULTINDEX_BEGIN +13)				//缺省指标线13
#define COLOR_DefTechColor14					(COLOR_DEFAULTINDEX_BEGIN +14)				//缺省指标线14
#define COLOR_DefTechColor15					(COLOR_DEFAULTINDEX_BEGIN +15)				//缺省指标线15
#define COLOR_DefTechColor16					(COLOR_DEFAULTINDEX_BEGIN +16)				//缺省指标线16
#define COLOR_DefTechColor17					(COLOR_DEFAULTINDEX_BEGIN +17)				//缺省指标线17
#define COLOR_DefTechColor18					(COLOR_DEFAULTINDEX_BEGIN +18)				//缺省指标线18
#define COLOR_DefTechColor19					(COLOR_DEFAULTINDEX_BEGIN +19)				//缺省指标线19
#define COLOR_DefTechColor20					(COLOR_DEFAULTINDEX_BEGIN +20)				//缺省指标线20
#define COLOR_DefTechColor21					(COLOR_DEFAULTINDEX_BEGIN +21)				//缺省指标线21
#define COLOR_DefTechColor22					(COLOR_DEFAULTINDEX_BEGIN +22)				//缺省指标线22
#define COLOR_DefTechColor23					(COLOR_DEFAULTINDEX_BEGIN +23)				//缺省指标线23
#define COLOR_DEFAULTINDEX_END				(COLOR_DEFAULTINDEX_BEGIN + 200)	//1402
	

#define FONTID_BEGIN						(COLORID_END )					//2001
#define FONT_ReportColumnName					(FONTID_BEGIN +1)					//报价抬头字段列
#define FONT_ReportCodeName						(FONTID_BEGIN +2)					//报价证券名称
#define FONT_ReportCode							(FONTID_BEGIN +3)					//报价证券代码
#define FONT_ReportRefreshTxt					(FONTID_BEGIN +4)					//行情更新.高亮文本
#define FONT_ReportDynamicTxt					(FONTID_BEGIN +5)					//报价动态文本(所有动态列，如价格，数量等)
#define FONT_ReportfixedTxt						(FONTID_BEGIN +6)					//报价固定文本，如代码前面的“序”号
#define FONT_StatusTxt							(FONTID_BEGIN +7)					//状态栏指数数据文本
#define FONT_TabTxt								(FONTID_BEGIN +8)					//Tab标签文本
#define FONT_F10Info							(FONTID_BEGIN +9)					//F10资讯内容字体
#define FONT_GraphHoriAxis						(FONTID_BEGIN +10)					//分时,k线图横坐标文本
#define FONT_GraphVerAxis						(FONTID_BEGIN +11)					//分时,k线图纵坐标文本
#define FONT_DrawBoxTitle						(FONTID_BEGIN +12)					//浮动框字段文本
#define FONT_DrawBoxData						(FONTID_BEGIN +13)					//浮动框动态文本
#define FONT_ChartTitle							(FONTID_BEGIN +14)					//图元顶部标题（分时，K线顶部指标信息）
#define FONT_ChartData							(FONTID_BEGIN +15)					//图元内文本（分时，K线图内数据）
#define FONT_KPeriodType						(FONTID_BEGIN +16)					//日线周期（K线图.指数周期类型的文本）
#define FONT_TipTitle							(FONTID_BEGIN +17)					//TIP提示标题
#define FONT_TipTxt								(FONTID_BEGIN +18)					//TIP提示文本
#define FONT_RightViewFixed						(FONTID_BEGIN +19)					//右图字段名称
#define FONT_RightViewCode						(FONTID_BEGIN +20)					//右图.分价.明细证券名称
#define FONT_RightViewCodeName					(FONTID_BEGIN +21)					//右图.分价.明细证券代码
#define FONT_RightViewFinace					(FONTID_BEGIN +22)					//右图配财值页文本
#define FONT_RightViewDynamic					(FONTID_BEGIN +23)					//右图动态文本(类似报价动态文本)
#define FONT_RightViewMainIndexName				(FONTID_BEGIN +24)					//大盘右图名称列
#define FONT_RightViewUints						(FONTID_BEGIN +25)					//右图单位名称
#define FONT_RightViewFutureTradeType			(FONTID_BEGIN +26)					//右图期货成交类型
#define FONT_BuySellSequenceTitle				(FONTID_BEGIN +27)					//买卖队列标题
#define FONT_MoveCost							(FONTID_BEGIN +28)					//成本分布云文本
#define FONT_DetailTradeTime					(FONTID_BEGIN +29)					//明细表时间列
#define FONT_FenJiaTradePrice					(FONTID_BEGIN +30)					//分价表成交价列
#define FONT_PrintHeadTitle						(FONTID_BEGIN +31)					//打印标题文本
#define FONT_PrintFootTitle						(FONTID_BEGIN +32)					//打印页脚文本
#define FONT_ScrollTxt							(FONTID_BEGIN +33)                  //跑马灯数据字体
#define FONT_DiagramLayoutTab   				(FONTID_BEGIN +34)                  //图层标签页字体
#define FONT_DiagramTableTitle                  (FONTID_BEGIN +35)                  // 获取表格标题图元文本字体
#define FONT_DiagramTrade                       (FONTID_BEGIN +36)                  // 获取交易图元文本
#define FONTID_END							(FONTID_BEGIN + 2000)		//4001	