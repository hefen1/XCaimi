
#pragma once

#define HS_LANGUAGE(x)	            (x)
#define HS_LANGUAGE_CODE(x,y,z)         (y)
#define HS_LANGUAGE_MARKET(x,y)       (y)

#define GVL_FIXFOCUSEDGE		0X00004000  // ��������߿�
#define GVL_FIXEDGE				0X00008000  // �����߿�
#define GVL_ALLOWROWRESIZE		0X00010000  // ����п��Դ�С
#define GVL_ALLOWCOLUMNRESIZE	0X00020000  // ����п��Դ�С
#define GVL_SENDMESSAGETOPARENT	0X00040000
#define GVL_SIZECOLUMNTOFIT		0X00080000  // ʹ�д�С����������ʾ����
#define GVL_LARGEFONT			0X00100000  // ���ֱ���

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

// �ı���ʾ
#define	GVIF_NONO				0x00000100  // ��
#define	GVIF_ARROW				0x00000200  // ��ͷ
#define	GVIF_UNDELINE			0x00000400  // �»���
#define	GVIF_SYMBOL				0x00000800  // ����"+""-"

// ��ɫ��ʾ
#define	GVIF_FgClrSAMEHEAD		0x00001000  // ��ɫ�ͱ��ͷһ��

// ��������
#define GVIF_STRING				0x00010000			//�ַ���
#define GVIF_NUMBER				0x00020000			//��������
#define GVIF_DOUBLE				0x00040000			//������
#define GVIF_DATETIME			0x00080000			//������

#define GVIF_MEMO				0x00100000		//�������ı�ע������
#define	GVIF_FIXED				0x00200000        // ��Ϊ�̶�
#define	GVIF_AUTOSIZE			0x00400000		// �д�СΪ���ƥ��
#define	GVIF_VALUEABS			0x00800000		// ������ʱ����ֵȡ����ֵ
#define	GVIF_USERDEFINE			0x01000000		// �Զ���
#define	GVIF_DRAWGRAPHIC		0x02000000		// ��Ϊ��ͼ�� (����)
#define	GVIF_DRAWGRAPHIC_VER	0x04000000		// ��Ϊ��ͼ�ͣ�����
#define	GVIF_DRAWGRAPHIC_TEXT	0x08000000		// ��Ϊ��ͼ�ͣ���ʾ�ı���
/* ������� DEFINE END */

/* �ֶδ����� DEFINE BEGIN*/
#define MPEXS_RWCONFIG			0x0001			// ֧�ֶ�д�����ļ�
#define MPEXS_COLNAME_NORWCFG	0x0002			// �ֶ�����(����)���������ļ���ȡ

#define MPEXT_HQ				0x01			// ����
#define MPEXT_ORIGINAL_USE		0x02			// �ڲ�ʹ��
#define MPEXT_USER_DEFINE		0x03			// �û��Զ���
#define MPEXT_INFO_EMAIL		0x04			// ��Ѷ/E-Mail
#define MPEXT_BLOCK				0x05			// �����Ϣ		add by lxqi 20090703
/* �ֶδ����� DEFINE END*/

// Ĭ������ֵ
#define ROWMASK_DEFAULT			GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM|GVIF_BKCLR|GVIF_FGCLR
// Ĭ�ϸ�ʽ
#define ROWFORMAT_DEFAULT		DT_RIGHT|DT_VCENTER|DT_SINGLELINE

#define Table_GridTable_Mark						_T("%s-�����ǩ����")
#define Table_GridTable_Report_Tab_Head				_T("����TAB��Ӧ��̧ͷ")
#define Table_GridTable_Report_Tab_Active			_T("����TAB������")

#define Table_GridTable_Report_Tab_Open				_T("����ȱʡ����")

#define Table_GridTable_Default_ColGroup			_T("ȱʡ")
#define Table_GridTable_Default_ColGroup_Stock		_T("��Ʊ")
#define Table_GridTable_Default_ColGroup_QH			_T("�ڻ�")
#define Table_GridTable_Default_ColGroup_CAIWU		_T("����")
#define Table_GridTable_Default_ColGroup_FUND		_T("����")
#define Table_GridTable_Default_ColGroup_BOND		_T("ծȯ")
#define Table_GridTable_Default_ColGroup_FOREIGN	_T("���")
#define Table_GridTable_Default_ColGroup_HK			_T("�۹�")

#define COLORID_BEGIN									1
#define COLORID_END								(COLORID_BEGIN + 2000)

#define COLOR_REPORT_BEGIN						(COLORID_BEGIN +1)					//2

#define COLOR_Background						(COLOR_REPORT_BEGIN +1)				//����ɫ   
#define COLOR_ReportCode						(COLOR_REPORT_BEGIN +2)				//����֤ȯ����
#define COLOR_RepottCodeName					(COLOR_REPORT_BEGIN +3)				//����֤ȯ����
#define COLOR_SelfSelectCode                    (COLOR_REPORT_BEGIN +4)				//��ѡ��Ʊ����
#define COLOR_PriceUp							(COLOR_REPORT_BEGIN +5)				//����
#define COLOR_PriceDown							(COLOR_REPORT_BEGIN +6)				//�µ�
#define COLOR_PriceEqual						(COLOR_REPORT_BEGIN +7)				//ƽ��
#define COLOR_PreClose							(COLOR_REPORT_BEGIN +8)				//����
#define COLOR_TotalHand							(COLOR_REPORT_BEGIN +9)				//����
#define COLOR_CurrentHand						(COLOR_REPORT_BEGIN +10)			//����
#define COLOR_TradeMoney						(COLOR_REPORT_BEGIN +11)			//�ɽ���
#define COLOR_OrderBuyVolume                    (COLOR_REPORT_BEGIN +12)			//ί����
#define COLOR_OrderSellVolume                   (COLOR_REPORT_BEGIN +13)			//ί����
#define COLOR_ExchangeRatio                     (COLOR_REPORT_BEGIN +14)			//������
#define COLOR_LiangBi							(COLOR_REPORT_BEGIN +15)			//����
#define COLOR_ShiYingLv							(COLOR_REPORT_BEGIN +16)			//��ӯ��
#define COLOR_HightBackground                   (COLOR_REPORT_BEGIN +17)			//���ۼ�������ɫ
#define COLOR_HightForegroud                    (COLOR_REPORT_BEGIN +18)			//���ۼ���ǰ��ɫ
#define COLOR_PriceChangeBk						(COLOR_REPORT_BEGIN +19)			//�������.�����ı�����
#define COLOR_SortUp							(COLOR_REPORT_BEGIN +20)			//�����������
#define COLOR_SortDown							(COLOR_REPORT_BEGIN +21)			//���۽������
#define COLOR_GridHead							(COLOR_REPORT_BEGIN +22)			//����̧ͷ�ı�
#define COLOR_GridFixed							(COLOR_REPORT_BEGIN +23)			//���۹̶��ı�
#define COLOR_FixTabText						(COLOR_REPORT_BEGIN +24)			//�̶�Tab�ı�
#define COLOR_TabText							(COLOR_REPORT_BEGIN +25)			//�Tab�ı�
#define COLOR_ActiveTabBK						(COLOR_REPORT_BEGIN +26)			//�Tab�����
#define COLOR_ActiveTabText						(COLOR_REPORT_BEGIN +27)			//�Tab�����ı�
#define COLOR_TabBK								(COLOR_REPORT_BEGIN +28)			//Tab����
#define COLOR_TabGrid							(COLOR_REPORT_BEGIN +29)			//Tab�߿�
#define COLOR_GeneralSortVerLine				(COLOR_REPORT_BEGIN +30)			//�ۺ���������
#define COLOR_GeneralSortSplitterLine			(COLOR_REPORT_BEGIN +31)			//�ۺ�����.��ֱ߿�ɫ
#define COLOR_GuideTab_Frame					(COLOR_REPORT_BEGIN +32)			// �̶���ǩҳ�߿���
#define COLOR_GuideTab_BK						(COLOR_REPORT_BEGIN +33)			// �̶���ǩҳ����
#define COLOR_FixedRowColBK						(COLOR_REPORT_BEGIN +34)			// ���۹̶��й̶��б���ɫ
#define COLOR_FixedRowBK						(COLOR_REPORT_BEGIN +35)			// ���۹̶��б���ɫ
#define COLOR_FixedColBK						(COLOR_REPORT_BEGIN +36)			// ���۹̶��б���ɫ
#define COLOR_TabButtonBK						(COLOR_REPORT_BEGIN +37)			// ��ǩҳ��ť����

#define COLOR_REPORT_END					(COLOR_REPORT_BEGIN +200)				//202


#define	COLOR_TREND_BEGIN						(COLOR_REPORT_END )						//202
#define COLOR_TrendAverage						(COLOR_TREND_BEGIN +1)					//��ʱͼ.����
#define COLOR_TrendPrice						(COLOR_TREND_BEGIN +2)					//��ʱͼ.�۸���
#define COLOR_Volume_Up							(COLOR_TREND_BEGIN +3)					//��ʱͼ.�ɽ�������
#define COLOR_Volume_Down						(COLOR_TREND_BEGIN +4)					//��ʱͼ.�ɽ�������
#define COLOR_VerAmountTxt						(COLOR_TREND_BEGIN +5)					//�ɽ��������ı�
#define COLOR_TrendGridLine						(COLOR_TREND_BEGIN +6)					//��ʱͼ.�������
#define COLOR_TrendMidHorLine					(COLOR_TREND_BEGIN +7)					//��ʱͼ.�м����
#define COLOR_TrendMidVerLine					(COLOR_TREND_BEGIN +8)					//��ʱͼ.�м�����
#define COLOR_TrendHourLine						(COLOR_TREND_BEGIN +9)					//��ʱͼ.Сʱ��
#define COLOR_TrendHalfHourLine					(COLOR_TREND_BEGIN +10)					//��ʱͼ.��Сʱ��
#define COLOR_TrendAxisLine						(COLOR_TREND_BEGIN +11)					//��ʱ,����ͼ��������
#define COLOR_TrendScaleLine					(COLOR_TREND_BEGIN +12)					//��ʱ,��������̶���
#define COLOR_TrendLiangBiTxt					(COLOR_TREND_BEGIN +13)					//����ָ�������ı�
#define COLOR_TrendADLTXT						(COLOR_TREND_BEGIN +14)					//ADLָ�������ı�
#define COLOR_TrendLiangBiLine					(COLOR_TREND_BEGIN +15)					//����ָ����
#define COLOR_TrendADLline						(COLOR_TREND_BEGIN +16)					//ADLָ����
#define COLOR_DuoKongAxixTxt					(COLOR_TREND_BEGIN +17)					//���ָ�������ı�
#define COLOR_BuySellTxt						(COLOR_TREND_BEGIN +18)					//�������������ı�
#define COLOR_DuoKongLine						(COLOR_TREND_BEGIN +19)					//���ָ����
#define COLOR_DiffValue							(COLOR_TREND_BEGIN +20)					//������ֵ
#define COLOR_TrendBuyLine						(COLOR_TREND_BEGIN +21)					//��������֮������
#define COLOR_TrendSellLine						(COLOR_TREND_BEGIN +22)					//��������֮������
#define COLOR_TrendIOPV							(COLOR_TREND_BEGIN +23)					//IOPV��
#define COLOR_DrawBoxline						(COLOR_TREND_BEGIN +24)					//������߿�(������)
#define COLOR_DrawBoxTxt						(COLOR_TREND_BEGIN +25)					//�������ֶ��ı�

#define COLOR_TREND_END							(COLOR_TREND_BEGIN + 200)				//402


#define COLOR_KLINE_BEGIN					(COLOR_TREND_END )					    //402
#define COLOR_KLINE_HorAxisTxt					(COLOR_KLINE_BEGIN +1)				//���ߺ������ı�
#define COLOR_KLINE_VerAxisTxt					(COLOR_KLINE_BEGIN +2)				//�����������ı�
#define COLOR_KLINE_UpLine						(COLOR_KLINE_BEGIN +3)				//����ͼ.������(����)
#define COLOR_KLINE_DownLine					(COLOR_KLINE_BEGIN +4)				//����ͼ.�µ���(����)
#define COLOR_KLINE_UpFill						(COLOR_KLINE_BEGIN +5)				//����ͼ.�������ɫ
#define COLOR_KLINE_DownFile					(COLOR_KLINE_BEGIN +6)				//����ͼ.�������ɫ
#define COLOR_KLINE_MaxMinBK					(COLOR_KLINE_BEGIN +7)				//�����Сֵ��������
#define COLOR_KLINE_GridHorLine					(COLOR_KLINE_BEGIN +8)				//����ͼ.�������
#define COLOR_KLINE_GridVerLine					(COLOR_KLINE_BEGIN +9)				//����ͼ.��������
#define COLOR_KLINE_TopHorline					(COLOR_KLINE_BEGIN +10)				//����ͼ.��������
#define COLOR_KLINE_IndexName					(COLOR_KLINE_BEGIN +11)				//ָ���������ı�
#define COLOR_KLINE_ChartTxt					(COLOR_KLINE_BEGIN +12)				//ͼԪ���ı�
#define COLOR_KLINE_TipTitleBk					(COLOR_KLINE_BEGIN +13)				//tip��ʾ���ⱳ��
#define COLOR_KLINE_TipTitleFg					(COLOR_KLINE_BEGIN +14)				//tip��ʾ����ǰ��
#define COLOR_KLINE_TipTextBK					(COLOR_KLINE_BEGIN +15)				//tip��ʾ�ı�����
#define COLOR_KLINE_TipTextFg					(COLOR_KLINE_BEGIN +16)				//tip��ʾ�ı�ǰ��
#define COLOR_KLINE_MoveLine					(COLOR_KLINE_BEGIN +17)				//�ƶ������
#define COLOR_KLINE_END						(COLOR_KLINE_BEGIN + 200)				//602


#define COLOR_RIGHTVIEW_BEGIN				(COLOR_KLINE_END )						//602
#define COLOR_RIGHTVIEW_CodeName				(COLOR_RIGHTVIEW_BEGIN +1 )				//��ͼ.�ּ�.��ϸ֤ȯ����
#define COLOR_RIGHTVIEW_Code				    (COLOR_RIGHTVIEW_BEGIN +2 )				//��ͼ.�ּ�.��ϸ֤ȯ����
#define COLOR_RIGHTVIEW_Time					(COLOR_RIGHTVIEW_BEGIN +3 )				//��ͼ.ʱ������
#define COLOR_RIGHTVIEW_Head					(COLOR_RIGHTVIEW_BEGIN +4 )				//��ͼ�ֶ�����
#define COLOR_RIGHTVIEW_HorLine					(COLOR_RIGHTVIEW_BEGIN +5 )				//��ͼ����
#define COLOR_RIGHTVIEW_VerLine					(COLOR_RIGHTVIEW_BEGIN +6 )				//��ͼ����
#define COLOR_RIGHTVIEW_CodeGrade				(COLOR_RIGHTVIEW_BEGIN +7 )				//��ͼ�����Ǽ�ɫ
#define COLOR_RIGHTVIEW_CodeGradeFg				(COLOR_RIGHTVIEW_BEGIN +8 )				//��ͼ��������ǰ��ɫ
#define COLOR_RIGHTVIEW_CodeGradeBK				(COLOR_RIGHTVIEW_BEGIN +9 )				//��ͼ�������ͱ���ɫ
#define COLOR_RIGHTVIEW_MainIndexName			(COLOR_RIGHTVIEW_BEGIN +10)				//������ͼ������
#define COLOR_RIGHTVIEW_TableTitle              (COLOR_RIGHTVIEW_BEGIN +11)             // ������ͼԪ������ɫ
#define COLOR_RIGHTVIEW_TableTitleBK            (COLOR_RIGHTVIEW_BEGIN +12)             // ������ͼԪ����ɫ
#define COLOR_RIGHTVIEW_TRADEFIXEDNAME          (COLOR_RIGHTVIEW_BEGIN +13)             // ����ͼԪ������

#define COLOR_RIGHTVIEW_END					(COLOR_RIGHTVIEW_BEGIN + 200)			//802


#define COLOR_INFORMATION_BEGIN				(COLOR_RIGHTVIEW_END )					//802
#define COLOR_INFORMATION_Bk					(COLOR_INFORMATION_BEGIN +1)			//��Ѷ������ɫ
#define COLOR_INFORMATION_Fg					(COLOR_INFORMATION_BEGIN +2)			//��Ѷǰ����ɫ
#define COLOR_INFORMATION_END				(COLOR_INFORMATION_BEGIN + 200)			//1002


#define COLOR_MISCELLANY_BEGIN				(COLOR_INFORMATION_END )				//1002
#define COLOR_MISCELLANY_StatusTxt				(COLOR_MISCELLANY_BEGIN +1)				//״̬���ı�
#define COLOR_MISCELLANY_HuoYanShan				(COLOR_MISCELLANY_BEGIN +2)				//����ɽ
#define COLOR_MISCELLANY_HuoYaoDu				(COLOR_MISCELLANY_BEGIN +3)				//��Ծ��
#define COLOR_MISCELLANY_DefCos					(COLOR_MISCELLANY_BEGIN +4)				//�ɱ��ֲ�
#define COLOR_MISCELLANY_PopInfoBk				(COLOR_MISCELLANY_BEGIN +5)				//��Ϣ��ʾ����ɫ
#define COLOR_MISCELLANY_PopInfoFg				(COLOR_MISCELLANY_BEGIN +6)				//��Ϣ��ʾǰ��ɫ
#define COLOR_MISCELLANY_ScrollTxt              (COLOR_MISCELLANY_BEGIN +7)             //������ı���ɫ
#define COLOR_MISCELLANY_ScrollBK               (COLOR_MISCELLANY_BEGIN +8)             //����Ʊ�����ɫ
#define COLOR_MISCELLANY_DiagramLayoutBKColor            (COLOR_MISCELLANY_BEGIN +9)             //ͼ���ǩҳ����	
#define COLOR_MISCELLANY_DiagramLayoutActiveFill         (COLOR_MISCELLANY_BEGIN +10)             //ͼ���ǩҳ����
#define COLOR_MISCELLANY_DiagramLayoutFill               (COLOR_MISCELLANY_BEGIN +11)             //ͼ���ǩҳ����
#define COLOR_MISCELLANY_DiagramLayoutFore               (COLOR_MISCELLANY_BEGIN +12)             //ͼ���ǩҳǰ��
#define COLOR_MISCELLANY_DiagramLayoutBorder             (COLOR_MISCELLANY_BEGIN +13)             //ͼ���ǩҳ�߿�
#define COLOR_MISCELLANY_END				(COLOR_MISCELLANY_BEGIN + 200)			//1202


#define COLOR_DEFAULTINDEX_BEGIN			(COLOR_MISCELLANY_END)				//1202
#define COLOR_DefTechColor1						(COLOR_DEFAULTINDEX_BEGIN +1)				//ȱʡָ����1
#define COLOR_DefTechColor2						(COLOR_DEFAULTINDEX_BEGIN +2)				//ȱʡָ����2
#define COLOR_DefTechColor3						(COLOR_DEFAULTINDEX_BEGIN +3)				//ȱʡָ����3
#define COLOR_DefTechColor4						(COLOR_DEFAULTINDEX_BEGIN +4)				//ȱʡָ����4
#define COLOR_DefTechColor5						(COLOR_DEFAULTINDEX_BEGIN +5)				//ȱʡָ����5
#define COLOR_DefTechColor6						(COLOR_DEFAULTINDEX_BEGIN +6)				//ȱʡָ����6
#define COLOR_DefTechColor7						(COLOR_DEFAULTINDEX_BEGIN +7)				//ȱʡָ����7
#define COLOR_DefTechColor8						(COLOR_DEFAULTINDEX_BEGIN +8)				//ȱʡָ����8
#define COLOR_DefTechColor9						(COLOR_DEFAULTINDEX_BEGIN +9)				//ȱʡָ����9
#define COLOR_DefTechColor10					(COLOR_DEFAULTINDEX_BEGIN +10)				//ȱʡָ����10
#define COLOR_DefTechColor11					(COLOR_DEFAULTINDEX_BEGIN +11)				//ȱʡָ����11
#define COLOR_DefTechColor12					(COLOR_DEFAULTINDEX_BEGIN +12)				//ȱʡָ����12
#define COLOR_DefTechColor13					(COLOR_DEFAULTINDEX_BEGIN +13)				//ȱʡָ����13
#define COLOR_DefTechColor14					(COLOR_DEFAULTINDEX_BEGIN +14)				//ȱʡָ����14
#define COLOR_DefTechColor15					(COLOR_DEFAULTINDEX_BEGIN +15)				//ȱʡָ����15
#define COLOR_DefTechColor16					(COLOR_DEFAULTINDEX_BEGIN +16)				//ȱʡָ����16
#define COLOR_DefTechColor17					(COLOR_DEFAULTINDEX_BEGIN +17)				//ȱʡָ����17
#define COLOR_DefTechColor18					(COLOR_DEFAULTINDEX_BEGIN +18)				//ȱʡָ����18
#define COLOR_DefTechColor19					(COLOR_DEFAULTINDEX_BEGIN +19)				//ȱʡָ����19
#define COLOR_DefTechColor20					(COLOR_DEFAULTINDEX_BEGIN +20)				//ȱʡָ����20
#define COLOR_DefTechColor21					(COLOR_DEFAULTINDEX_BEGIN +21)				//ȱʡָ����21
#define COLOR_DefTechColor22					(COLOR_DEFAULTINDEX_BEGIN +22)				//ȱʡָ����22
#define COLOR_DefTechColor23					(COLOR_DEFAULTINDEX_BEGIN +23)				//ȱʡָ����23
#define COLOR_DEFAULTINDEX_END				(COLOR_DEFAULTINDEX_BEGIN + 200)	//1402
	

#define FONTID_BEGIN						(COLORID_END )					//2001
#define FONT_ReportColumnName					(FONTID_BEGIN +1)					//����̧ͷ�ֶ���
#define FONT_ReportCodeName						(FONTID_BEGIN +2)					//����֤ȯ����
#define FONT_ReportCode							(FONTID_BEGIN +3)					//����֤ȯ����
#define FONT_ReportRefreshTxt					(FONTID_BEGIN +4)					//�������.�����ı�
#define FONT_ReportDynamicTxt					(FONTID_BEGIN +5)					//���۶�̬�ı�(���ж�̬�У���۸�������)
#define FONT_ReportfixedTxt						(FONTID_BEGIN +6)					//���۹̶��ı��������ǰ��ġ��򡱺�
#define FONT_StatusTxt							(FONTID_BEGIN +7)					//״̬��ָ�������ı�
#define FONT_TabTxt								(FONTID_BEGIN +8)					//Tab��ǩ�ı�
#define FONT_F10Info							(FONTID_BEGIN +9)					//F10��Ѷ��������
#define FONT_GraphHoriAxis						(FONTID_BEGIN +10)					//��ʱ,k��ͼ�������ı�
#define FONT_GraphVerAxis						(FONTID_BEGIN +11)					//��ʱ,k��ͼ�������ı�
#define FONT_DrawBoxTitle						(FONTID_BEGIN +12)					//�������ֶ��ı�
#define FONT_DrawBoxData						(FONTID_BEGIN +13)					//������̬�ı�
#define FONT_ChartTitle							(FONTID_BEGIN +14)					//ͼԪ�������⣨��ʱ��K�߶���ָ����Ϣ��
#define FONT_ChartData							(FONTID_BEGIN +15)					//ͼԪ���ı�����ʱ��K��ͼ�����ݣ�
#define FONT_KPeriodType						(FONTID_BEGIN +16)					//�������ڣ�K��ͼ.ָ���������͵��ı���
#define FONT_TipTitle							(FONTID_BEGIN +17)					//TIP��ʾ����
#define FONT_TipTxt								(FONTID_BEGIN +18)					//TIP��ʾ�ı�
#define FONT_RightViewFixed						(FONTID_BEGIN +19)					//��ͼ�ֶ�����
#define FONT_RightViewCode						(FONTID_BEGIN +20)					//��ͼ.�ּ�.��ϸ֤ȯ����
#define FONT_RightViewCodeName					(FONTID_BEGIN +21)					//��ͼ.�ּ�.��ϸ֤ȯ����
#define FONT_RightViewFinace					(FONTID_BEGIN +22)					//��ͼ���ֵҳ�ı�
#define FONT_RightViewDynamic					(FONTID_BEGIN +23)					//��ͼ��̬�ı�(���Ʊ��۶�̬�ı�)
#define FONT_RightViewMainIndexName				(FONTID_BEGIN +24)					//������ͼ������
#define FONT_RightViewUints						(FONTID_BEGIN +25)					//��ͼ��λ����
#define FONT_RightViewFutureTradeType			(FONTID_BEGIN +26)					//��ͼ�ڻ��ɽ�����
#define FONT_BuySellSequenceTitle				(FONTID_BEGIN +27)					//�������б���
#define FONT_MoveCost							(FONTID_BEGIN +28)					//�ɱ��ֲ����ı�
#define FONT_DetailTradeTime					(FONTID_BEGIN +29)					//��ϸ��ʱ����
#define FONT_FenJiaTradePrice					(FONTID_BEGIN +30)					//�ּ۱�ɽ�����
#define FONT_PrintHeadTitle						(FONTID_BEGIN +31)					//��ӡ�����ı�
#define FONT_PrintFootTitle						(FONTID_BEGIN +32)					//��ӡҳ���ı�
#define FONT_ScrollTxt							(FONTID_BEGIN +33)                  //�������������
#define FONT_DiagramLayoutTab   				(FONTID_BEGIN +34)                  //ͼ���ǩҳ����
#define FONT_DiagramTableTitle                  (FONTID_BEGIN +35)                  // ��ȡ������ͼԪ�ı�����
#define FONT_DiagramTrade                       (FONTID_BEGIN +36)                  // ��ȡ����ͼԪ�ı�
#define FONTID_END							(FONTID_BEGIN + 2000)		//4001	