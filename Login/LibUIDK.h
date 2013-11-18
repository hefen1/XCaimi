#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

#include <AFXCMN.H>
#include <vector>
#include <map>
#include <afxole.h>
#include <afxtempl.h>
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#ifdef USEGDIPLUS

#ifndef ULONG_PTR
#if defined(_WIN64)
	typedef unsigned __int64 ULONG_PTR;
#else
	typedef unsigned long ULONG_PTR;
#endif //_WIN64
#endif //ULONG_PTR

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#endif //USEGDIPLUS


#ifdef USEGDIPLUS
typedef Bitmap * BITMAPOBJ;
#else
typedef HBITMAP BITMAPOBJ;
#endif

namespace LibUIDK
{

//////////////////////////////////////////////////////////////////////////
// Message Define:
// Application:			WM_USER+100
// CSkinButton:			WM_USER+200
// CSkinStatic			WM_USER+300
// CSkinEdit			WM_USER+400
// CSkinComboBox		WM_USER+400
// CSkinSliderCtrl(Ex)	WM_USER+500
// CSkinSpinButtonCtrl	WM_USER+600
// CSkinProgressCtrl	WM_USER+700
// CSkinHotKeyCtrl		WM_USER+800
// CSkinIPAddressCtrl	WM_USER+900
// CSkinListCtrl		WM_USER+1000
// CSkinTreeCtrl		WM_USER+1100
// CScrollBarEx			WM_USER+1200
// CThumbnailCtrl		WM_USER+1300
// CTreeListCtrl		WM_USER+1400
// CRichEditCtrl		WM_USER+1500
// CHtmlCtrl			WM_USER+1600
// CClockCtrl			WM_USER+1700
// CSkinHeaderCtrl		WM_USER+1800
// CSkinMenu			WM_USER+1900
// Drag&Drop			WM_USER+2000

// LibUIDK use custom message ID from WM_APPLICATION to WM_LIBUIDK
#define WM_APPLICATION	WM_USER+100
#define WM_BUTTON		WM_USER+200
#define WM_STATIC		WM_USER+300
#define WM_EDIT			WM_USER+400
#define WM_COMBOBOX		WM_USER+400
#define WM_SLIDER		WM_USER+500
#define WM_SPINBUTTON	WM_USER+600
#define WM_PROGRESS		WM_USER+700
#define WM_HOTKEYCTRL	WM_USER+800
#define WM_IPADDRESS	WM_USER+900
#define WM_LISTCTRL		WM_USER+1000
#define WM_TREECTRL		WM_USER+1100
#define WM_SCROLLBAR	WM_USER+1200
#define WM_THUMBNAIL	WM_USER+1300
#define WM_TREELIST		WM_USER+1400
#define WM_RICHEDIT		WM_USER+1500
#define WM_HTMLCTRL		WM_USER+1600
#define WM_CLOCK		WM_USER+1700
#define WM_HEADER		WM_USER+1800
#define WM_MENU			WM_USER+1900
#define WM_DRAGDROP		WM_USER+2000
#define WM_LIBUIDK		WM_USER+3000
#define WM_CUSTOM		WM_USER+4000

#define WM_CHANGESKIN                 WM_APPLICATION+3
#define WM_GETRESIZEPOINT             WM_APPLICATION+5 // LPARAM is the handle of the control.
// The WPARAM of the WM_GETRESIZEPOINT
#define GR_POINTX                     0x00000001 // x of the resizing point
#define GR_POINTY                     0x00000002 // y of the resizing point
#define WM_NEEDRESIZE                 WM_APPLICATION+6
#define WM_ISNEEDRESIZE               WM_APPLICATION+11
#define WM_ISCHILDMOVE                WM_APPLICATION+12 // WPARAM: not used. LPARAM: handle of the child
#define WM_PARENTEVENT                WM_APPLICATION+16
#define WM_DRAWITEMEX                 WM_APPLICATION+17
#define WM_MENUMOUSEMOVE              WM_MENU+1

// Owner draw control types
#define ODT_TREELIST                  10
#define ODT_THUMBNAIL                 11


//////////////////////////////////////////////////////////////////////////
// The error code of return value by CUIWnd::GetLastCreateError()
#define E_LOADBMPB                    1		// failed to load the base image
#define E_LOADBMPN                    2		// failed to load the background image of normal status(end by _N.bmp)
#define E_LOADBMPH                    3		// failed to laod the background image of highlight status(end by _H.bmp)
#define E_LOADBMPD                    4		// failed to load the background image of disable status(end by _D.bmp)
#define E_LOADBMPS                    5		// failed to load the background image of selected status(end by _S.bmp)
#define E_LOADBMPC                    6		// failed to load the background image of checked status(end by _CX.bmp)
#define E_ATTACHBMPB                  50	// failed to attach the HBITMAP of base image to CBitmap object
#define E_ATTACHBMPN                  51	// failed to attach the HBITMAP of normal image to CBitmap object
#define E_ATTACHBMPH                  52	// failed to attach the HBITMAP of highlight image to CBitmap object
#define E_ATTACHBMPD                  53	// failed to attach the HBITMAP of disable image to CBitmap object
#define E_ATTACHBMPS                  54	// failed to attach the HBITMAP of selected image to CBitmap object
#define E_ATTACHBMPC                  55	// failed to attach the HBITMAP of checked image to CBitmap object
#define E_ADJUSTCHILD                 56	// failed to adjust the child controls while change the skin
#define E_CREATECHILD                 110	// failed to create the child window 

//////////////////////////////////////////////////////////////////////////
#define E_SETUIPATH                   200   // failed to set .ui path, it cause by the ui path isn't exists.
#define E_INVALIDLICENSE              201   // invalid license
#define E_NOTUNICODE                  202	// the .ui file not support unicode when you define _UNICODE or UNICODE micro.
#define E_OLDERVERSION                203	// the version of .ui file older than LibUIDK.
#define E_NEWERVERSION                204	// the version of .ui file newer than LibUIDK.

#define DROPM_DRAGENTER				WM_DRAGDROP+1
#define DROPM_DRAGOVER				WM_DRAGDROP+2
#define DROPM_DROP					WM_DRAGDROP+3
#define DROPM_DROPEX				WM_DRAGDROP+4
#define DROPM_DRAGLEAVE				WM_DRAGDROP+5
#define DROPM_DRAGSCROLL            WM_DRAGDROP+6

#define NM_DBLCLKEX                 (NM_FIRST-30)	// Parameter same as CSkinTreeCtrl::NM_DBLCLK
#define BNN_DRAWTEXT				(NM_FIRST-31)
#define TVN_HIGHLIGHTCHANGING		(NM_FIRST-32)	// Parameter same as CSkinTreeCtrl::TVN_SELCHANGING
#define TVN_HIGHLIGHTCHANGED		(NM_FIRST-33)	// Parameter same as CSkinTreeCtrl::TVN_SELCHANGED

//
// type of controls
//
#define ARROW              _T("ARROW")
#define PUSHBUTTON         _T("PUSHBUTTON")
#define CHECK              _T("CHECK")
#define RADIO              _T("RADIO")
#define COMBOBOX           _T("COMBOBOX")
#define STATIC             _T("STATIC")
#define EDIT               _T("EDIT")
#define SLIDER             _T("SLIDER")
#define PROGRESS           _T("PROGRESS")
#define SPIN               _T("SPIN")
#define HOTKEY             _T("HOTKEY")
#define IPADDRESS          _T("IPADDRESS")
#define SLIDEREX           _T("SLIDEREX")
#define LISTCTRL           _T("LISTCTRL")
#define TREECTRL           _T("TREECTRL")
#define SCROLLBAR          _T("SCROLLBAR")
#define SCROLLBAREX        _T("SCROLLBAREX")
#define TREELIST           _T("TREELIST")
#define THUMBNAIL          _T("THUMBNAIL")
#define RICHEDIT           _T("RICHEDIT")
#define HTMLCTRL           _T("HTMLCTRL")

// the LPARAM parameter of BNN_DRAWTEXT notify
struct NMBUTTON
{
	NMHDR hdr;
	HDC hDC;
	RECT rc;
	UINT uFormat;
};

struct CTRLPROPERTIES
{
	CTRLPROPERTIES()
	{
		bVisible = TRUE;
		bDisabled = FALSE;
		nHorLayoutMode = 0;
		nVerLayoutMode = 0;
	}
	CString strType;
	CString strID;
	RECT rect;
	BOOL bVisible;
	BOOL bDisabled;
	int nHorLayoutMode;	// 0: Default; 1: Left; 2: Right; 3: Center
	int nVerLayoutMode;	// 0: Default; 1: Top; 2: Bottom; 3: Center
};

struct BTNBASEPROPERTIES : public CTRLPROPERTIES
{
	enum TEXT_ALIGN_HOR
	{
		TAH_DEFAULT,
		TAH_LEFT,
		TAH_RIGHT,
		TAH_CENTER,
	};
	enum TEXT_ALIGN_VER
	{
		TAV_DEFAULT,
		TAV_TOP,
		TAV_BOTTOM,
		TAV_CENTER,
	};
	enum FOREGROUND_ALIGN_HOR
	{
		FAH_CENTER,
		FAH_LEFT,
		FAH_RIGHT,
	};
	enum FOREGROUND_ALIGN_VER
	{
		FAV_CENTER,
		FAV_TOP,
		FAV_BOTTOM,
	};
	BTNBASEPROPERTIES()
	{
		bUseToolTip = FALSE;
		bR2LReadOrder = FALSE;
		bMultiline = FALSE;
		bTabStop = FALSE;
		bEndEllipsis = FALSE;
		bCanBackgroundStateChange = TRUE;
		bSpecifyBackgroundImages = FALSE;
		bCombineBackgroundImages = FALSE;
		bSpecifyForegroundImages = FALSE;
		bCombineForegroundImages = FALSE;
		bHSameNFont = TRUE;		// the font of Highlight state same to normal
		bSSameNFont = TRUE;
		bDSameNFont = TRUE;
		memset(&lfN, 0, sizeof(LOGFONT));
		memset(&lfH, 0, sizeof(LOGFONT));
		memset(&lfS, 0, sizeof(LOGFONT));
		memset(&lfD, 0, sizeof(LOGFONT));
		bHSameNColor = TRUE;
		bSSameNColor = TRUE;
		bDSameNColor = TRUE;
		crN = RGB(0, 0, 0);
		crH = RGB(0, 0, 0);
		crS = RGB(0, 0, 0);
		crD = RGB(0, 0, 0);
		nTextHorAlignMode = 0;
		nTextVerAlignMode = 0;
		strHorAlign = _T("0");
		strVerAlign = _T("0");
		eForegroundHorAlignMode = FAH_LEFT;
		eForegroundVerAlignMode = FAV_CENTER;
		nForegroundHorAlign = 0;
		nForegroundVerAlign = 0;
	}
	virtual ~BTNBASEPROPERTIES()
	{

	}

	BOOL bUseToolTip;
	BOOL bR2LReadOrder;
	BOOL bMultiline;
	BOOL bTabStop;
	BOOL bEndEllipsis;		// only valid while bMultiline is FALSE
	BOOL bHSameNFont;		// the font of Highlight state same to normal
	BOOL bSSameNFont;
	BOOL bDSameNFont;
	LOGFONT lfN;
	LOGFONT lfH;
	LOGFONT lfS;
	LOGFONT lfD;
	BOOL bHSameNColor;
	BOOL bSSameNColor;
	BOOL bDSameNColor;
	COLORREF crN;
	COLORREF crH;
	COLORREF crS;
	COLORREF crD;
	int nTextHorAlignMode;	// mode: 0-3
	int nTextVerAlignMode;	// mode: 0-3
	CString	strHorAlign;	// value
	CString	strVerAlign;	// value
	CString	strCaption;
	CString strToolTip;
	CString strTag;
	CString strMaskBmp;

	BOOL bCanBackgroundStateChange;	// FALSE: The highlight, selected and disabled background image use normal background image.
		BOOL bSpecifyBackgroundImages;
			BOOL bCombineBackgroundImages;
				CString strBKCombine;
				CString strBKN;
				CString strBKH;
				CString strBKS;
				CString strBKD;

	BOOL bSpecifyForegroundImages;
		BOOL bCombineForegroundImages;
			CString strFKCombine;
			CString strFKN;
			CString strFKH;
			CString strFKS;
			CString strFKD;
		FOREGROUND_ALIGN_HOR eForegroundHorAlignMode;
		FOREGROUND_ALIGN_VER eForegroundVerAlignMode;
		int	nForegroundHorAlign;	// value
		int	nForegroundVerAlign;	// value
};

struct BTNPROPERTIES : public BTNBASEPROPERTIES
{
	enum BTN_TYPE
	{
		BT_NORMAL,
		BT_MINIMIZEBOX,
		BT_CLOSEBOX,
	};
	BTNPROPERTIES()
	{
		strType = PUSHBUTTON;
		eBtnType = BT_NORMAL;
		bDefPushButton = FALSE;
	}
	virtual ~BTNPROPERTIES()
	{

	}
	BTN_TYPE eBtnType;
	BOOL bDefPushButton;
};

struct TOGGLEBTNPROPERTIES : public BTNBASEPROPERTIES
{
	TOGGLEBTNPROPERTIES()
	{

	}
	virtual ~TOGGLEBTNPROPERTIES()
	{

	}
	CString strBKCN;
	CString strBKCH;
	CString strBKCS;
	CString strBKCD;
	CString strFKCN;
	CString strFKCH;
	CString strFKCS;
	CString strFKCD;
};

struct CHKPROPERTIES : public TOGGLEBTNPROPERTIES
{
	enum CHK_TYPE
	{
		CT_NORMAL,
		CT_MAXIMIZEBOX,
	};
	CHKPROPERTIES()
	{
		strType = CHECK;
		eChkType = CT_NORMAL;
	}
	virtual ~CHKPROPERTIES()
	{

	}
	CHK_TYPE eChkType;
};

struct RADPROPERTIES : public TOGGLEBTNPROPERTIES
{
	RADPROPERTIES()
	{
		strType = RADIO;
		bGroup = FALSE;
	}
	BOOL bGroup;
};

struct TXTPROPERTIES : public CTRLPROPERTIES
{
	TXTPROPERTIES()
	{
		strType = STATIC;
		bUseToolTip = FALSE;
		bR2LReadOrder = FALSE;
		bNoWrap = FALSE;
		bPathEllipsis = FALSE;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
		// font
		memset(&lfN, 0, sizeof(LOGFONT));
		memset(&lfD, 0, sizeof(LOGFONT));
		bDSameNFont = TRUE;
		// color
		crN = RGB(0, 0, 0);
		crD = RGB(0, 0, 0);
		bDSameNColor = TRUE;
		// alignment
		nTextHorAlignMode = 0;
		nTextVerAlignMode = 0;
		strHorAlign = _T("0");
		strVerAlign = _T("0");
	}
	BOOL bUseToolTip;
	BOOL bR2LReadOrder;
	BOOL bNoWrap;
		BOOL bPathEllipsis; // only valid while bNoWrap is TRUE.
	// font
	LOGFONT lfN;
	LOGFONT lfD;
	BOOL bDSameNFont;
	// color
	COLORREF crN;
	COLORREF crD;
	BOOL bDSameNColor;
	// alignment
	int nTextHorAlignMode;
	int nTextVerAlignMode;
	CString strCaption;
	CString strToolTip;
	CString	strHorAlign;
	CString	strVerAlign;

	BOOL bSpecifyImages;	// FALSE: The disabled background image same as normal image.
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct EDTPROPERTIES : public CTRLPROPERTIES
{
	EDTPROPERTIES()
	{
		strType = EDIT;
		bTabStop = TRUE;
		bHScroll = FALSE;
		bVScroll = FALSE;
		bWantReturn = FALSE;
		bR2LReadOrder = FALSE;
		bAutoHScroll = TRUE;
		bAutoVScroll = FALSE;
		bLowerCase = FALSE;
		bNumber = FALSE;
		bPassword = FALSE;
		bReadOnly = FALSE;
		bUpperCase = FALSE;
		bMultiline = FALSE;
		bDisableNoScroll = FALSE; // For Rich Edit
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
		// font
		memset(&lfN, 0, sizeof(LOGFONT));

		strHorAlign = _T("0");
		strVerAlign = _T("0");
		nTextHorAlignMode = 0; // can be ES_LEFT, ES_CENTER or ES_RIGHT
	}
	BOOL bTabStop;
	BOOL bHScroll;
	BOOL bVScroll;
	BOOL bWantReturn;
	BOOL bR2LReadOrder;
	BOOL bAutoHScroll;
	BOOL bAutoVScroll;
	BOOL bLowerCase;
	BOOL bNumber;
	BOOL bPassword;
	BOOL bReadOnly;
	BOOL bUpperCase;
	BOOL bMultiline;
	BOOL bDisableNoScroll; // For Rich Edit
	// font
	LOGFONT lfN;

	CString strHorAlign;
	CString strVerAlign;
	CString strTag;
	int nTextHorAlignMode; // can be ES_LEFT, ES_CENTER or ES_RIGHT

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKH;
			CString strBKS;
			CString strBKD;
};

struct CMBPROPERTIES : public CTRLPROPERTIES
{
	CMBPROPERTIES()
	{
		strType = COMBOBOX;
		bTabStop = TRUE;
		bVScroll = TRUE;
		bAutoHScroll = TRUE;
		bDisableNoScroll = FALSE;
		bSort = TRUE;
		bUppercase = FALSE;
		bLowercase = FALSE;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
		nComboStyle = 1;
		memset(&lfN, 0, sizeof(LOGFONT));
		strHorAlign = _T("0");
		strVerAlign = _T("0");
		strDropListHeight = _T("80");
		strBtnWidth = _T("16");
	}
	BOOL bTabStop;
	BOOL bVScroll;
	BOOL bAutoHScroll;
	BOOL bDisableNoScroll;
	BOOL bSort;
	BOOL bUppercase;
	BOOL bLowercase;
	int nComboStyle;
	LOGFONT lfN;
	CString strHorAlign;
	CString strVerAlign;
	CString strDropListHeight;
	CString strBtnWidth;
	CString strTag;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKH;
			CString strBKS;
			CString strBKD;
};

struct SPNPROPERTIES : public CTRLPROPERTIES
{
	SPNPROPERTIES()
	{
		strType = SPIN;
		nAlignMode = 0;
		bVertical = FALSE;
		bTabStop = FALSE;
		bAutoBuddy = FALSE;
		bSetBuddyInt = FALSE;
		bNoThousands = FALSE;
		bWrap = FALSE;
		bArrowKeys = TRUE;
		bHotTrack = FALSE;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
	}
	int nAlignMode;
	BOOL bVertical;
	BOOL bTabStop;
	BOOL bAutoBuddy;
	BOOL bSetBuddyInt;
	BOOL bNoThousands;
	BOOL bWrap;
	BOOL bArrowKeys;
	BOOL bHotTrack;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKH;
			CString strBKS;
			CString strBKD;
};

struct SLDPROPERTIES : public CTRLPROPERTIES
{
	SLDPROPERTIES()
	{
		strType = SLIDER;
		bVertical = FALSE;
		bUseMark = TRUE;
		bUseToolTip = FALSE;
		nExtendedStyle = 0;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
	}
	BOOL bVertical;
	BOOL bUseMark;
	BOOL bUseToolTip;
	int nExtendedStyle;
	CString strThumbN;
	CString strThumbH;
	CString strThumbS;
	CString strThumbD;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKMarkN;
			CString strBKMarkD;
			CString strBKD;
};

struct PRGPROPERTIES : public CTRLPROPERTIES
{
	PRGPROPERTIES()
	{
		strType = PROGRESS;
		bVertical = FALSE;
		bShowText = TRUE;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
		memset(&lfN, 0, sizeof(LOGFONT));
		strHorAlignN = _T("0");
		strVerAlignN = _T("0");
	}
	BOOL bVertical;
	BOOL bShowText;
	LOGFONT lfN;
	CString strThumbN;
	CString strHorAlignN;
	CString strVerAlignN;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct HOTPROPERTIES : public CTRLPROPERTIES
{
	HOTPROPERTIES()
	{
		strType = HOTKEY;
		memset(&lfN, 0, sizeof(LOGFONT));
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
	}
	LOGFONT lfN;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct IPAPROPERTIES : public CTRLPROPERTIES
{
	IPAPROPERTIES()
	{
		strType = IPADDRESS;
		memset(&lfN, 0, sizeof(LOGFONT));
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
	}
	LOGFONT lfN;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct LSTCTRLPROPERTIES : public CTRLPROPERTIES
{
	LSTCTRLPROPERTIES()
	{
		strType = LISTCTRL;
		bTabStop = FALSE;
		bAutoArrange = FALSE;
		bEditLabels = FALSE;
		bNoHeader = FALSE;
		bNoLabelWrap = FALSE;
		bNoScroll = FALSE;
		bNoSortHeader = FALSE;
		bShowSelAlways = FALSE;
		bSingleSel = FALSE;
		nListStyle = 0;
		nListAlign = 0;
		nListSort = 0;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
	}
	BOOL bTabStop;
	BOOL bAutoArrange;
	BOOL bEditLabels;
	BOOL bNoHeader;
	BOOL bNoLabelWrap;
	BOOL bNoScroll;
	BOOL bNoSortHeader;
	BOOL bShowSelAlways;
	BOOL bSingleSel;
	int nListStyle;
	int nListAlign;
	int nListSort;
	CString strHeaderBkN;
	CString strHeaderBkH;
	CString strHeaderBkS;
	CString strHeaderBkD;
	CString strSeparatorBkN;
	CString strTag;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct TREECTRLPROPERTIES : public CTRLPROPERTIES
{
	TREECTRLPROPERTIES()
	{
		strType = TREECTRL;
		bTabStop = FALSE;
		bCheckBoxes = FALSE;
		bDisDragDrop = FALSE;
		bEditLabels = FALSE;
		bFullRowSel = FALSE;
		bHasButtons = FALSE;
		bHasLines = FALSE;
		bInfoTip = FALSE;
		bLinesAtRoot = FALSE;
		bNonEvenHeight = FALSE;
		bScroll = TRUE;
		bShowSelAlways = FALSE;
		bSingleExpand = FALSE;
		bToolTips = TRUE;
		bTrackSel = FALSE;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
		memset(&lfN, 0, sizeof(LOGFONT));
		memset(&lfD, 0, sizeof(LOGFONT));
		bDSameNFont = TRUE;
	}
	BOOL bTabStop;
	BOOL bCheckBoxes;
	BOOL bDisDragDrop;
	BOOL bEditLabels;
	BOOL bFullRowSel;
	BOOL bHasButtons;
	BOOL bHasLines;
	BOOL bInfoTip;
	BOOL bLinesAtRoot;
	BOOL bNonEvenHeight;
	BOOL bScroll;
	BOOL bShowSelAlways;
	BOOL bSingleExpand;
	BOOL bToolTips;
	BOOL bTrackSel;
	LOGFONT lfN;
	LOGFONT lfD;
	BOOL bDSameNFont;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct SBPROPERTIES : public CTRLPROPERTIES
{
	SBPROPERTIES()
	{
		strType = SCROLLBAR;
		bVertical = FALSE;
		bScrollBarEx = TRUE;
		strLTUPx = _T("0");
		strLTUPy = _T("0");
		strRTDNx = _T("0");
		strRTDNy = _T("0");
	}
	BOOL bVertical;
	BOOL bScrollBarEx;
	CString strThumbN;
	CString strThumbH;
	CString strThumbS;
	CString strThumbD;
	CString strBkN;
	CString strBkH;
	CString strBkS;
	CString strBkD;
	CString strLTUPx;
	CString strLTUPy;
	CString strRTDNx;
	CString strRTDNy;
};

struct THMPROPERTIES : public CTRLPROPERTIES
{
	THMPROPERTIES()
	{
		strType = THUMBNAIL;
		bListStyle = FALSE;
		bTabStop = FALSE;

		strTotalItemWidth = _T("140");
		strTotalItemHeight = _T("140");
		
		strThumbnailLeftN = _T("4");
		strThumbnailTopN = _T("4");
		strThumbnailRightN = _T("96");
		strThumbnailBottomN = _T("83");

		strThumbnailLeftH = _T("4");
		strThumbnailTopH = _T("4");
		strThumbnailRightH = _T("96");
		strThumbnailBottomH = _T("83");

		strLabelLeftN = _T("4");
		strLabelTopN = _T("90");
		strLabelRightN = _T("96");
		strLabelBottomN = _T("116");

		strLabelLeftH = _T("4");
		strLabelTopH = _T("90");
		strLabelRightH = _T("96");
		strLabelBottomH = _T("116");
	}
	BOOL bListStyle;
	BOOL bTabStop;

	CString strTotalItemWidth;
	CString strTotalItemHeight;
	
	CString strThmBkN;
	CString strThmBkH;
	CString strThmBkS;
	CString strThmBkD;

	CString strThumbnailLeftN;
	CString strThumbnailTopN;
	CString strThumbnailRightN;
	CString strThumbnailBottomN;

	CString strThumbnailLeftH;
	CString strThumbnailTopH;
	CString strThumbnailRightH;
	CString strThumbnailBottomH;

	CString strLabelLeftN;
	CString strLabelTopN;
	CString strLabelRightN;
	CString strLabelBottomN;

	CString strLabelLeftH;
	CString strLabelTopH;
	CString strLabelRightH;
	CString strLabelBottomH;
};

struct TREELISTPROPERTIES : public CTRLPROPERTIES
{
	TREELISTPROPERTIES()
	{
		strType = TREELIST;
		bTabStop = FALSE;
		bVariableItemHeight = FALSE;
		bSpecifyImages = FALSE;
		bCombineImages = FALSE;
	}
	BOOL bTabStop;
	BOOL bVariableItemHeight;

	BOOL bSpecifyImages;
		BOOL bCombineImages;
			CString strBKCombine;
			CString strBKN;
			CString strBKD;
};

struct HTMLCTRLPROPERTIES : public CTRLPROPERTIES
{
	HTMLCTRLPROPERTIES()
	{
		strType = HTMLCTRL;
		bHideBorder = FALSE;
		bHideScrollBar = FALSE;
		bHideContextMenu = FALSE;
	}
	BOOL bHideBorder;
	BOOL bHideScrollBar;
	BOOL bHideContextMenu;
};

struct CONTROL
{
	CString strControlType;
	CTRLPROPERTIES *pControlStruct;
};

struct UIWNDPROPERTIES
{
	UIWNDPROPERTIES ()
	{
		bUseMaskColor = FALSE;
		crMask = RGB(255, 0, 255);
		bResize = FALSE;
		ptResize.x = ptResize.y = 0;
		nTitleBarHeight = 0;
	}
	CString strID;
	CString strBkN;
	CString strBkH;
	CString strBkS;
	CString strBkD;
	CString strBkNo;
	BOOL bUseMaskColor;
	COLORREF crMask;
	BOOL bResize;
	POINT ptResize;
	int nTitleBarHeight;	// -1: All area of window can be drag to move; 0: window can't be moved; >0: the height of title bar.
	std::vector<CONTROL> vControls;
};

// Globals
int BackgroundMask(CWnd *pWnd, CString szMaskBitmap, COLORREF crMask);
int BackgroundMask(CWnd *pWnd, BITMAPOBJ hBmpMask, COLORREF crMask);
void TransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent);
BOOL TileBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc);
int SubBitmap(BITMAPOBJ hSrcBmp, BITMAPOBJ &hDstBmp, int nX, int nY, int nWidth, int nHeight);
int StretchBitmap(HBITMAP hSrcBmp, HBITMAP *phDstBmp, int nResizingX, int nResizingY, int nDstWidth, int nDstHeight, int nTilePixelX=1, int nTilePixelY=1);
int StretchBitmap(HBITMAP hSrcBmp, HBITMAP *phDstBmp, int nDstWidth, int nDstHeight);
int AutoStretchControlBk(HDC hdcDest, int nDstWidth, int nDstHeight, HBITMAP hSrcBmp, int nResizingX, int nResizingY, int nTilePixelX=1, int nTilePixelY=1);
int StretchBltEx(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, HBITMAP hbmpSrc, int nResizingXSrc, int nResizingYSrc, int nTilePixelX=1, int nTilePixelY=1);
int BitBltEx(HDC hdcDest, int nXDest, int nYDest, int nDstWidth, int nDstHeight, HBITMAP hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop);
// for DPI
int GetDPIBitmap(HBITMAP hSrcBmp, HBITMAP *phDstBmp);
int GetDPIRect(LPRECT lpRect);
int GetDPIPointX(long &nX);
int GetDPIPointY(long &nY);
int GetDPILogFont(LPLOGFONT lpLogFont);
#ifdef USEGDIPLUS
BOOL TileDrawImage(Graphics *pGraphicsDst, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, Bitmap *pSrcImg, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc);
int StretchBitmap(Bitmap *pSrcBmp, Bitmap **phDstBmp, int nResizingX, int nResizingY, int nDstWidth, int nDstHeight, int nTilePixelX=1, int nTilePixelY=1);
int StretchBitmap(Bitmap *pSrcBmp, Bitmap **phDstBmp, int nDstWidth, int nDstHeight);
int AutoStretchControlBk(HDC hdcDest, int nDstWidth, int nDstHeight, Bitmap *hSrcBmp, int nResizingX, int nResizingY, int nTilePixelX=1, int nTilePixelY=1);
int StretchBltEx(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, Bitmap *hbmpSrc, int nResizingXSrc, int nResizingYSrc, int nTilePixelX=1, int nTilePixelY=1);
int BitBltEx(HDC hdcDest, int nXDest, int nYDest, int nDstWidth, int nDstHeight, Bitmap *hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop);
// for DPI
int GetDPIBitmap(Bitmap *pSrcBmp, Bitmap **phDstBmp);
#endif
BOOL IsWinXPSp2ORLater();

void ASSERT_BITMAPOBJ(BITMAPOBJ obj);
void SafeDeleteBITMAPOBJ(BITMAPOBJ &obj);
void SafeDeleteHGDIOBJ(HGDIOBJ &obj);
template <class T>
void SafeDelete(T *&p)
{
	if(p != NULL)
	{
		delete p;
		p = NULL;
	}
}

template <class T>
void SafeArrayDelete(T *&p)
{
	if(p != NULL)
	{
		delete []p;
		p = NULL;
	}
}
BITMAPOBJ LoadBITMAPOBJ(LPCTSTR lpszFile);
BITMAPOBJ LoadBITMAPOBJ(UINT uID);
HBITMAP LoadHBITMAP(LPCTSTR lpszFile);
HICON LoadHICON(LPCTSTR lpszFile);
int GetBITMAPOBJSize(BITMAPOBJ obj, LPSIZE size);
int GetHBITMAPSize(HBITMAP obj, LPSIZE size);

#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
#define TSTRCPY _tcscpy
#else
#define TSTRCPY _tcscpy_s
#endif // (_MSC_VER <= 1310)

void TSPLITPATH(LPCTSTR path, LPTSTR drive, size_t driveSizeInCharacters, LPTSTR dir, size_t dirSizeInCharacters, LPTSTR fname, size_t nameSizeInCharacters, LPTSTR ext, size_t extSizeInBytes);
void TSTRCAT(LPTSTR strDestination, size_t sizeInBytes, LPCTSTR strSource);
CString StringFromInt(int nValue);
FILE *TFOPEN(LPCTSTR filename, LPCTSTR mode);

class CSkinButton : public CButton
{
// Construction
public:
	CSkinButton();
	virtual ~CSkinButton();

public:
	void SetUIWndName(LPCTSTR lpszUIWndName);
	int SetToolTips(LPCTSTR lpszToolTips);
	CToolTipCtrl *GetToolTipCtrl();
	
	void SetTag(LPCTSTR lpszTag);
	void GetTag(CString &strTag);

	// color of button's text
	;
	void SetTextColor(COLORREF crN, COLORREF crH, COLORREF crS, COLORREF crD);
	void GetTextColor(COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD);
	void SetCheckedTextColor(COLORREF crCN, COLORREF crCH, COLORREF crCS, COLORREF crCD);
	void GetCheckedTextColor(COLORREF *pcrCN, COLORREF *pcrCH, COLORREF *pcrCS, COLORREF *pcrCD);

	// font of button's text
	;
	void SetTextFont(CFont *pFontN, CFont *pFontH, CFont *pFontS, CFont *pFontD);
	void GetTextFont(CFont **pFontN, CFont **pFontH, CFont **pFontS, CFont **pFontD);
	void SetCheckedTextFont(CFont *pFontCN, CFont *pFontCH, CFont *pFontCS, CFont *pFontCD);
	void GetCheckedTextFont(CFont **pFontCN, CFont **pFontCH, CFont **pFontCS, CFont **pFontCD);

	// offset of button's text
	;
	void SetTextOffsetHor(int nHOffsetN, int nHOffsetH, int nHOffsetS, int nHOffsetD);
	void GetTextOffsetHor(int *pnHOffsetN, int *pnHOffsetH, int *pnHOffsetS, int *pnHOffsetD);
	void SetTextOffsetVer(int nVOffsetN, int nVOffsetH, int nVOffsetS, int nVOffsetD);
	void GetTextOffsetVer(int *pnVOffsetN, int *pnVOffsetH, int *pnVOffsetS, int *pnVOffsetD);
	void SetCheckedTextOffsetHor(int nHOffsetCN, int nHOffsetCH, int nHOffsetCS, int nHOffsetCD);
	void GetCheckedTextOffsetHor(int *pnHOffsetCN, int *pnHOffsetCH, int *pnHOffsetCS, int *pnHOffsetCD);
	void SetCheckedTextOffsetVer(int nVOffsetCN, int nVOffsetCH, int nVOffsetCS, int nVOffsetCD);
	void GetCheckedTextOffsetVer(int *pnVOffsetCN, int *pnVOffsetCH, int *pnVOffsetCS, int *pnVOffsetCD);

	// align mode of the button
	;
	void SetTextAlignHor(int nHorAlignModeN, int nHorAlignModeH, int nHorAlignModeS, int nHorAlignModeD);
	void GetTextAlignHor(int *pnHorAlignModeN, int *pnHorAlignModeH, int *pnHorAlignModeS, int *pnHorAlignModeD);
	void SetTextAlignVer(int nVerAlignModeN, int nVerAlignModeH, int nVerAlignModeS, int nVerAlignModeD);
	void GetTextAlignVer(int *pnVerAlignModeN, int *pnVerAlignModeH, int *pnVerAlignModeS, int *pnVerAlignModeD);
	void SetCheckedTextAlignHor(int nHorAlignModeCN, int nHorAlignModeCH, int nHorAlignModeCS, int nHorAlignModeCD);
	void GetCheckedTextAlignHor(int *pnHorAlignModeCN, int *pnHorAlignModeCH, int *pnHorAlignModeCS, int *pnHorAlignModeCD);
	void SetCheckedTextAlignVer(int nVerAlignModeCN, int nVerAlignModeCH, int nVerAlignModeCS, int nVerAlignModeCD);
	void GetCheckedTextAlignVer(int *pnVerAlignModeCN, int *pnVerAlignModeCH, int *pnVerAlignModeCS, int *pnVerAlignModeCD);

	// bitmap and checked bitmap
	;
	void SetBitmap(BITMAPOBJ hbmpN, BITMAPOBJ hbmpH, BITMAPOBJ hbmpS, BITMAPOBJ hbmpD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	void GetBitmap(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpH, BITMAPOBJ *phbmpS, BITMAPOBJ *phbmpD);
	void SetForegroundBitmap(BITMAPOBJ hBmpForegroundN, BITMAPOBJ hBmpForegroundH, BITMAPOBJ hBmpForegroundS, BITMAPOBJ hBmpForegroundD,
		int nDstX = 0, int nDstY = 0, int nAlignModeHor = 0, int nAlignModeVer = 0, COLORREF crMask = RGB(255, 0, 255), BOOL bReDraw = TRUE);
	void GetForegroundBitmap(BITMAPOBJ *phBmpForegroundN, BITMAPOBJ *phBmpForegroundH, BITMAPOBJ *phBmpForegroundS, BITMAPOBJ *phBmpForegroundD,
		int *pnDstX, int *pnDstY, int *pnAlignModeHor, int *pnAlignModeVer, COLORREF *pcrMask);
	void SetForegroundIcon(HICON hIcon);
	HICON GetForegroundIcon() const;

	int  SetCheckedBitmap(BITMAPOBJ hbmpCN, BITMAPOBJ hbmpCH, BITMAPOBJ hbmpCS, BITMAPOBJ hbmpCD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	int  SetCheckedBitmap(UINT uIDCN, UINT uIDCH, UINT uIDCS, UINT uIDCD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	void GetCheckedBitmap(BITMAPOBJ *phbmpCN, BITMAPOBJ *phbmpCH, BITMAPOBJ *phbmpCS, BITMAPOBJ *phbmpCD);
	void SetCheckedForegroundBitmap(BITMAPOBJ hBmpForegroundCN, BITMAPOBJ hBmpForegroundCH, BITMAPOBJ hBmpForegroundCS, BITMAPOBJ hBmpForegroundCD,
		int nDstXC = 0, int nDstYC = 0, int nAlignModeHorC = 0, int nAlignModeVerC = 0, COLORREF rcMaskC = RGB(255, 0, 255), BOOL bReDraw = TRUE);
	void GetCheckedForegroundBitmap(BITMAPOBJ *phBmpForegroundCN, BITMAPOBJ *phBmpForegroundCH, BITMAPOBJ *phBmpForegroundCS, BITMAPOBJ *phBmpForegroundCD,
		int *pnDstXC, int *pnDstYC, int *pnAlignModeHorC, int *pnAlignModeVerC, COLORREF *prcMaskC);
	void SetCheckedForegroundIcon(HICON hIcon);
	HICON GetCheckedForegroundIcon() const;

	// mask bitmap for checked and unchecked status
	;
	int SetMaskBitmap(LPCTSTR lpszMaskN, LPCTSTR lpszMaskH, LPCTSTR lpszMaskS, LPCTSTR lpszMaskD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	int SetMaskBitmap(BITMAPOBJ hbmpMaskN, BITMAPOBJ hbmpMaskH, BITMAPOBJ hbmpMaskS, BITMAPOBJ hbmpMaskD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	int GetMaskBitmap(BITMAPOBJ *phbmpMaskN, BITMAPOBJ *phbmpMaskH, BITMAPOBJ *phbmpMaskS, BITMAPOBJ *phbmpMaskD);
	int SetCheckedMaskBitmap(LPCTSTR lpszMaskCN, LPCTSTR lpszMaskCH, LPCTSTR lpszMaskCS, LPCTSTR lpszMaskCD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	int SetCheckedMaskBitmap(BITMAPOBJ hbmpMaskCN, BITMAPOBJ hbmpMaskCH, BITMAPOBJ hbmpMaskCS, BITMAPOBJ hbmpMaskCD, BOOL bDeleteOld=TRUE, BOOL bRedraw=TRUE);
	int GetCheckedMaskBitmap(BITMAPOBJ *phbmpMaskCN, BITMAPOBJ *phbmpMaskCH, BITMAPOBJ *phbmpMaskCS, BITMAPOBJ *phbmpMaskCD);

	// set, get or remove resize point for button, if not set, the button will get it from parent
	;
	int SetResizePoint(LPPOINT pt);
	int GetResizePoint(LPPOINT pt);
	int RemoveResizePoint();

	// animate text
	;
	enum Direction
	{
		UNKNOWN,
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,
	};
	int AnimateText(int nDirection, int nDelay, int nPixel = 1);
	int StopAnimateText();

	int SetEndEllipsis(BOOL bEndEllipsis);
	BOOL IsEndEllipsis();

	int SetActiveArea(const CRect &rect);
	int GetActiveArea(LPRECT lpRect);
	BOOL IsHasActiveArea() const;
	int RemoveActiveArea();

	virtual void OnDraw(CDC *pDC);
	virtual int DrawText(CDC *pDC, const CString &strText, LPRECT lpRect, UINT uFormat);
	// hide the same name interface in base class
private:
	// DON'T call some interfaces in base class
	HBITMAP SetBitmap(HBITMAP hBitmap);
	HBITMAP GetBitmap() const;
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinButton)
	;
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinButton)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void *m_pMember;
};

class CSkinEdit : public CEdit
{
// Construction
public:
	CSkinEdit();
	virtual ~CSkinEdit();

public:
	BOOL SetBkColor(COLORREF clrBkgnd);
	COLORREF GetBkColor();
	
	BOOL SetTextColor(COLORREF clrText);
	COLORREF GetTextColor();

	BOOL SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpH, BITMAPOBJ hbmpS, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	BOOL GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpH, BITMAPOBJ *phbmpS, BITMAPOBJ *phbmpD);
	
	void SetDeflateRight(int nDeflate);
	
	int SetTextFont(CFont *pFont);
	int GetTextFont(CFont **pFont);
	
	int SetTextAlign(int nAlignHor, int nAlignVer);
	int GetTextAlign(int *pnAlignHor, int *pnAlignVer);

	void SetTag(LPCTSTR lpszTag);
	void GetTag(CString &strTag);

	int SetResizePoint(LPPOINT pt);
	int GetResizePoint(LPPOINT pt);
	int RemoveResizePoint();

	// hide the same name interface in base class
private:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

protected:
	int UpdateBackgroundBrush();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinEdit)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void *m_pMember;
};

class CSkinComboBox : public CComboBox
{
// Construction
public:
	CSkinComboBox();
	virtual ~CSkinComboBox();

public:
	int GetBitmap(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpH, BITMAPOBJ *phbmpS, BITMAPOBJ *phbmpD);
	int SetBitmap(BITMAPOBJ hbmpN, BITMAPOBJ hbmpH, BITMAPOBJ hbmpS, BITMAPOBJ hbmpD, BOOL bDeleteOld=TRUE);

	void SetTextFont(CFont *pFontN);
	void GetTextFont(CFont **pFontN);

	BOOL SetTextColor(COLORREF clrText);
	COLORREF GetTextColor() const;
	BOOL SetDropdownListTextColor(COLORREF crText);
	COLORREF GetDropdownListTextColor() const;

	void SetTextOffsetN(int nHOffsetN, int nVOffsetN);
	void GetTextOffsetN(int *pnHOffsetN, int *pnVOffsetN);

	void SetButtonWidth(int nWidth);
	void GetButtonWidth(int *pnWidth);

	void SetTag(LPCTSTR lpszTag);
	void GetTag(CString &strTag);

	void DrawFocusRectangle(BOOL bDraw);
	BOOL IsDrawFocusRectangle();

	// hide the same name interface in base class
private:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinComboBox)
	afx_msg void OnCloseup();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
	CSkinEdit m_ctrlEdit;
};

class CSkinSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CSkinSliderCtrl();
	virtual ~CSkinSliderCtrl();

// Attributes
public:
	void SetUIWndName(LPCTSTR lpszUIWndName);
	int SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpD, BOOL bDeleteOld=TRUE);
	int GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpD);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinSliderCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	int InitBitmaps();

	// Generated message map functions
protected:
	BOOL IsMouseOverThumb();
	//{{AFX_MSG(CSkinSliderCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnGetThumbRect(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
private:
	void *m_pMember;
};

class CSkinStatic : public CStatic
{
// Construction
public:
	CSkinStatic();
	virtual ~CSkinStatic();

public:
	int SetBitmap(BITMAPOBJ hbmpN, BITMAPOBJ hbmpD, BOOL bDeleteOld=TRUE);
	int GetBitmap(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpD);
	void SetTextNoWrap(BOOL bNoWrap);
	BOOL GetTextNoWrap();
	int SetPathEllipsis(BOOL bPathEllipsis);
	BOOL IsPathEllipsis() const;
	int SetEndEllipsis(BOOL bEndEllipsis);
	BOOL IsEndEllipsis() const;
	void SetTextAlignHor(int nHorAlignModeN, int nHorAlignModeD);
	void GetTextAlignHor(int *pnHorAlignModeN, int *pnHorAlignModeD);
	void SetTextAlignVer(int nVerAlignModeN, int nVerAlignModeD);
	void GetTextAlignVer(int *pnVerAlignModeN, int *pnVerAlignModeD);
	void SetTextColor(COLORREF crN, COLORREF crD);
	void GetTextColor(COLORREF *pcrN, COLORREF *pcrD);
	void SetTextFont(CFont *pFontN, CFont *pFontD);
	void GetTextFont(CFont **ppFontN, CFont **ppFontD);
	void SetTextOffsetN(int nHOffsetN, int nVOffsetN); 
	void GetTextOffsetN(int *pnHOffsetN, int *pnVOffsetN);
	void SetTextOffsetD(int nHOffsetD, int nVOffsetD); 
	void GetTextOffsetD(int *pnHOffsetD, int *pnVOffsetD);

	void SetForegroundBitmap(BITMAPOBJ hBmpForeground, int nXSrc, int nYSrc, COLORREF rcMask=RGB(255, 0, 255), BOOL bReDraw=TRUE, BOOL bDeleteOld=TRUE);
	void GetForegroundBitmap(BITMAPOBJ *phBmpForeground);

	void SetToolTips(LPCTSTR lpszToolTips);
	CToolTipCtrl *GetToolTipCtrl();

	// hide the same name interface in base class
private:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinStatic)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void *m_pMember;
};

class CSkinProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CSkinProgressCtrl();
	virtual ~CSkinProgressCtrl();

public:
	void ShowText(BOOL bIsShow);
	BOOL IsShowText();

	int SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	int GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpD);

	int SetThumbImages(BITMAPOBJ hbmpThumbN, BITMAPOBJ hbmpThumbD, BOOL bDeleteOld = TRUE);
	int GetThumbImages(BITMAPOBJ *phbmpThumbN, BITMAPOBJ *phbmpThumbD);

	int SetTextFont(CFont *pFontN, CFont *pFontD, BOOL bDeleteOld=TRUE);
	int GetTextFont(CFont **ppFontN, CFont **ppFontD);

	int SetThumbResizePoint(const LPPOINT lpPt);
	int GetThumbResizePoint(LPPOINT lpPt);

	// hide the same name interface in base class
private:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

protected:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinProgressCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinProgressCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	void *m_pMember;
};

class CSkinSpinButtonCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CSkinSpinButtonCtrl();
	virtual ~CSkinSpinButtonCtrl();

public:
	int SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpH, BITMAPOBJ hbmpS, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	int GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpH, BITMAPOBJ *phbmpS, BITMAPOBJ *phbmpD);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinSpinButtonCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinSpinButtonCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	void *m_pMember;
};

class CSkinIPAddressCtrl : public CIPAddressCtrl
{
// Construction
public:
	CSkinIPAddressCtrl();
	virtual ~CSkinIPAddressCtrl();

public:
	int SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	int GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpD);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinIPAddressCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinIPAddressCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

class CSkinHotKeyCtrl : public CHotKeyCtrl
{
// Construction
public:
	CSkinHotKeyCtrl();
	virtual ~CSkinHotKeyCtrl();

public:
	int SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	int GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpD);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinHotKeyCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinHotKeyCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx window

#ifdef _WIN32
#define WC_TRACKBAREXA            "msctls_trackbar32ex"
#define WC_TRACKBAREXW            L"msctls_trackbar32ex"

#ifdef UNICODE
#define  WC_TRACKBAREX            WC_TRACKBAREXW
#else
#define  WC_TRACKBAREX            WC_TRACKBAREXA
#endif

#else
#define WC_TRACKBAREX             "msctls_trackbarex"
#endif

#define WM_GETTOOLTIPS            WM_SLIDER+1	// wParam: point to a CString object; lParam, handle of the sliderex

#define TBS_EX_NORMAL             0x00000000
#define TBS_EX_PROGRESS           0x00001000
#define TBS_EX_SPLITPROGRESS      0x00002000
class CSliderCtrlEx : public CWnd
{
// Construction
public:
	CSliderCtrlEx();
	virtual ~CSliderCtrlEx();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	DWORD GetExtendedStyle();
	DWORD SetExtendedStyle(DWORD dwNewStyle);

public:
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
	void SetThumbSize(SIZE sizeThumb);
	int SetDeflate(UINT nLeftTop, UINT nRightBottom);
	int GetDeflate(UINT *pnLeftTop, UINT *pnRightBottom);
	void GetThumbRect(LPRECT lprc) const;

	int GetPos() const;	
	void SetPos(int nPos);

	int GetProgressPos() const;
	void SetProgressPos(int nPos);

	int SetImages(BITMAPOBJ hbmpN, BITMAPOBJ hbmpMarkN, BITMAPOBJ hbmpMarkD, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	int GetImages(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpMarkN, BITMAPOBJ *phbmpMarkD, BITMAPOBJ *phbmpD);

	int SetThumbImages(BITMAPOBJ hbmpThumbN, BITMAPOBJ hbmpThumbH, BITMAPOBJ hbmpThumbS, BITMAPOBJ hbmpThumbD, BOOL bDeleteOld = TRUE);
	int GetThumbImages(BITMAPOBJ *phbmpThumbN, BITMAPOBJ *phbmpThumbH, BITMAPOBJ *phbmpThumbS, BITMAPOBJ *phbmpThumbD);

	int UseMarkBitmap(BOOL bUseMark);
	BOOL IsUseMarkBitmap();

	int SetSplitCount(int nCount);
	int GetSplitCount();
	int SetSplitList(const int *pnFirst, const int *pnLast, BOOL bRedraw=TRUE);
	int AddColorImage(int nValue, HBITMAP hbmpColor);
	int RemoveColorImage(int nValue);

	int EnableToolTipsEx(BOOL bEnable, BOOL bMouseMove = TRUE);
	int SetToolTips(LPCTSTR lpszToolTips);
	CToolTipCtrl *GetToolTipCtrl();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderCtrlEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	;
	virtual void DrawHorzSlider(CDC *pMemDC);
	virtual void DrawVertSlider(CDC *pMemDC);

	// Generated message map functions
protected:
	//{{AFX_MSG(CSliderCtrlEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

class CSkinHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CSkinHeaderCtrl();
	virtual ~CSkinHeaderCtrl();

public:
	void SetTreeCtrlEx(CWnd *pWnd);
	CWnd *GetTreeCtrlEx();

	int SetImages(BITMAPOBJ hbmpBkN, BITMAPOBJ hbmpBkH, BITMAPOBJ hbmpBkS, BITMAPOBJ hbmpBkD, BITMAPOBJ hbmpBkSeparator, BOOL bDeleteOld=TRUE);
	int GetImages(BITMAPOBJ *phbmpBkN, BITMAPOBJ *phbmpBkH, BITMAPOBJ *phbmpBkS, BITMAPOBJ *phbmpBkD, BITMAPOBJ *phbmpSeparatorBkN);

	void SetTextColor(COLORREF cr);
	void GetTextColor(COLORREF *pcr);

	void SetTextFont(CFont *pFontN, CFont *pFontD = NULL);
	void GetTextFont(CFont **ppFontN, CFont **ppFontD = NULL);

	void SetHeaderHeight(int nHeight);
	int GetHeaderHeight();

	int HitTest(CPoint pt);

	// hide the same name interface in base class
private:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinHeaderCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	;
	virtual void DrawItem(CDC *pDC, int nItem);

protected:
	//{{AFX_MSG(CSkinHeaderCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnNMEndTrack(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx window

//   _________________________________________
//  | 0 |    3     |////// 2 /////|   4   | 1 |
//  |___|__________|______________|_______|___|
//
//  0: Left or Up arrow
//  1: Right or Down arrow
//  2: Thumb
//  3: Space1
//  4: Space2

#define GIR_LTUP             0x00000001
#define GIR_RTDN             0x00000010
#define GIR_THUMB            0x00000100
#define GIR_ALL              GIR_LTUP|GIR_RTDN|GIR_THUMB

#ifdef _WIN32
#define WC_SCROLLBAREXA            "ScrollBarEx"
#define WC_SCROLLBAREXW            L"ScrollBarEx"

#ifdef UNICODE
#define  WC_SCROLLBAREX            WC_SCROLLBAREXW
#else
#define  WC_SCROLLBAREX            WC_SCROLLBAREXA
#endif

#else
#define WC_SCROLLBAREX             "ScrollBarEx"
#endif

class CScrollBarEx : public CWnd
{
// Construction
public:
	CScrollBarEx();
	virtual ~CScrollBarEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollBarEx)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
	;
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetLTUPOffset(int nX, int nY);
	void SetRTDNOffset(int nX, int nY);
	BOOL SetBkBitmaps(BITMAPOBJ hbmpBkN, BITMAPOBJ hbmpBkH, BITMAPOBJ hbmpBkS, BITMAPOBJ hbmpBkD, BOOL bDeleteOld=TRUE);
	int GetBkBitmaps(BITMAPOBJ *phbmpBkN, BITMAPOBJ *phbmpBkH, BITMAPOBJ *phbmpBkS, BITMAPOBJ *phbmpBkD);
	BOOL SetThumbBitmaps(BITMAPOBJ hbmpThumbN, BITMAPOBJ hbmpThumbH, BITMAPOBJ hbmpThumbS, BITMAPOBJ hbmpThumbD, BOOL bDeleteOld=TRUE);
	int GetThumbBitmaps(BITMAPOBJ *phbmpThumbN, BITMAPOBJ *phbmpThumbH, BITMAPOBJ *phbmpThumbS, BITMAPOBJ *phbmpThumbD);
	void SetLTUPButton(int nValue);
	int GetLTUPButton();
	void SetRTDNButton(int nValue);
	int GetRTDNButton();

	void SetNotifyWnd(CWnd *pWnd);
	CWnd *GetNotifyWnd();

	virtual void DrawSizeBox(CDC *pDC, CDC *pMemDC);
	virtual void DrawHorzItem(CDC *pDC, CDC *pMemDC);
	virtual void DrawVertItem(CDC *pDC, CDC *pMemDC);

	void GetItemRect(LPRECT lprcLTUP, LPRECT lprcRTDN, LPRECT lprcThumb, UINT nMask = GIR_ALL);
	void ShowScrollBar(BOOL bShow = TRUE);
	BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);
	
	void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
	void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
	
	BOOL SetScrollPos(int nPos, BOOL bRedraw = TRUE);
	int GetScrollPos() const;
	
	BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
	BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
	
	int GetScrollLimit();

#if(WINVER >= 0x0500)
	BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const;
#endif //(WINVER >= 0x0500)

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollBarEx)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	UINT m_nLTUP; // Width of the left arrow of a horizontal scroll bar or Height of the up arrow of a vertical scroll bar
	UINT m_nRTDN; // Width of the right arrow of a horizontal scroll bar or Height of the down arrow of a vertical scroll bar
	SCROLLINFO m_si;
	DWORD m_dwStyle;

	BITMAPOBJ m_hbmpBk[4];
	BITMAPOBJ m_hbmpThumb[4];
	
	int m_nCapture;				// -1: mouse not be captrue; 0: captured by LTUP arrow; 1: captured by RTDN arrow;  2: captured by thumb; 3: captured by space1; 4: captured by space2;
								// 2: captured by thumb, 3: captured by space1; 4: captured by space2
	int m_nArrowFlags;			// disable or enable the arrow of the scroll
	CPoint m_ptLTUPOffset;		// Offset of the left arrow of a horizontal scroll bar or the up arrow of a vertical scroll bar from the background image
	CPoint m_ptRTDNOffset;		// Offset of the right or bottom scroll bar from the background image
	CPoint m_ptDragThumb;		// record the point when drag the thumb
	SCROLLINFO m_siDrag;		// record the scroll information when drag the thumb
	int m_nDragOffset;			// record the offset from first position when drag the thumb
	CWnd *m_pNotifyWnd;
	BOOL m_bCaptureThumb;
	BOOL m_bIsMouseOverThumb;
	BOOL m_bCaptureLTUPButton;
	BOOL m_bIsMouseOverLTUPButton;
	BOOL m_bCaptureRTDNButton;
	BOOL m_bIsMouseOverRTDNButton;
};

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl window

#define WM_DRAWLIST                       WM_LISTCTRL+1		// WPARAM(CPaintDC *pDC), LPARAM((CSkinListCtrl *)this)

class CSkinListCtrl : public CListCtrl
{
// Construction
public:
	CSkinListCtrl();
	virtual ~CSkinListCtrl();

public:
	int SetHeaderImages(BITMAPOBJ hbmpHeaderN, BITMAPOBJ hbmpHeaderH, BITMAPOBJ hbmpHeaderS, BITMAPOBJ hbmpHeaderD, BITMAPOBJ hbmpHeaderSeparator);
	int GetHeaderImages(BITMAPOBJ *phbmpHeaderN, BITMAPOBJ *phbmpHeaderH, BITMAPOBJ *phbmpHeaderS, BITMAPOBJ *phbmpHeaderD, BITMAPOBJ *phbmpHeaderSeparator);

	// for icon in other columns
	int SetImageListEx(CImageList* pImageList);
	CImageList* GetImageListEx() const;
	CRect GetSubItemIconRect(int nItem, int nSubItem);

	BOOL SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText, int nImageEx);
	int GetItemImage(int nIndex, int nSubItem);

	// for background image
	int EnableBkImage(BOOL bEnable);
	BOOL IsEnableBkImage() const;

	int SetImages(BITMAPOBJ hbmpBkN, BITMAPOBJ hbmpBkD, BOOL bDeleteOld=TRUE);
	int GetImages(BITMAPOBJ *phbmpBkN, BITMAPOBJ *phBmpBkD);

	int EnableOwnerDraw(BOOL bEnable);
	BOOL IsEnableOwnerDraw() const;

	// for selected item
	// ;
	// draw selected background fit to the width of client, not to the right of last header item.	int EnableSelectedFitClient(BOOL bEnable);
	int EnableSelectedFitClient(BOOL bEnable);
	BOOL IsEnableSelectedFitClient() const;

	int SetSelectedItemBkColor(COLORREF cr);
	COLORREF GetSelectedItemBkColor() const;

	int SetSelectedItemBkImages(BITMAPOBJ hbmpBkFocus, BITMAPOBJ hbmpBkUnfocus, BOOL bDeleteOld = TRUE);
	int GetSelectedItemBkImages(BITMAPOBJ *phbmpBkFocus, BITMAPOBJ *phbmpBkUnfocus);

	// for highlight item
	int EnableHighlightItem(BOOL bEnable);
	BOOL IsEnableHighlightItem() const;

	int SetHighlightItemBkColor(COLORREF cr);
	COLORREF GetHighlightItemBkColor() const;

	int SetHighlightItemBkImages(BITMAPOBJ hbmpBkFocus, BITMAPOBJ hbmpBkUnfocus, BOOL bDeleteOld = TRUE);
	int GetHighlightItemBkImages(BITMAPOBJ *phbmpBkFocus, BITMAPOBJ *phbmpBkUnfocus);

	int GetHighlightItem() const;

	void SetTextFont(CFont *pFontN, CFont *pFontD);
	void GetTextFont(CFont **ppFontN, CFont **ppFontD);

	void SetTag(LPCTSTR lpszTag);
	void GetTag(CString &strTag);

	int SetResizePoint(LPPOINT pt);
	int GetResizePoint(LPPOINT pt);
	int RemoveResizePoint();

	// for custom scroll bar
	int UseCustomScrollBar(BOOL bUseCustomScrollBar);
	BOOL IsUseCustomScrollBar() const;

	CScrollBarEx *GetHorzScrollBarEx() const;
	CScrollBarEx *GetVertScrollBarEx() const;

	int UpdateCustomScrollBar();

	// hide the same name interface in base class
private:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont* GetFont() const;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinListCtrl)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
#if(_MSC_VER <= 1200) // VC6.0
	virtual INT OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#else
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#endif

protected:
	virtual void DrawItemEx(CDC *pDC, int nItem);
	virtual void DrawReportItem(CDC *pDC, int nItem, const CRect &rcItem);
	virtual void DrawIconItem(CDC *pDC, int nItem, const CRect &rcItem);

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinListCtrl)
	afx_msg BOOL OnSetTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

class CSkinTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CSkinTreeCtrl();
	virtual ~CSkinTreeCtrl();

public:
	int SetImages(BITMAPOBJ hbmpBkN, BITMAPOBJ hbmpBkD, BOOL bDeleteOld=TRUE);
	int GetImages(BITMAPOBJ *phbmpBkN, BITMAPOBJ *phBmpBkD);
	
	// for selected item
	// ;
	int SetSelectedBkColor(COLORREF cr);
	COLORREF GetSelectedBkColor() const;

	int SetSelectedBkImages(BITMAPOBJ hbmpBkN, BITMAPOBJ hbmpBkD, BOOL bDeleteOld = TRUE);
	int GetSelectedBkImages(BITMAPOBJ *phbmpBkN, BITMAPOBJ *phbmpBkD);

	int SetSelectedTextColor(COLORREF cr);
	COLORREF GetSelectedTextColor() const;

	// for highlight item
	// ;
	int EnableHighlightItem(BOOL bEnable);
	BOOL IsEnableHighlightItem() const;
	HTREEITEM GetHighlightItem() const;

	int SetHighlightItemBkColor(COLORREF cr);
	COLORREF GetHighlightItemBkColor() const;

	int SetHighlightItemBkImages(BITMAPOBJ hbmpBkN, BITMAPOBJ hbmpBkD, BOOL bDeleteOld = TRUE);
	int GetHighlightItemBkImages(BITMAPOBJ *phbmpBkN, BITMAPOBJ *phbmpBkD);

	// for disable text color
	// ;
	int SetDisabledTextColor(COLORREF cr);
	COLORREF GetDisabledTextColor() const;

	// for mouse hover
	// ;
	void SetHoverTime(DWORD dwHoverTime, BOOL bRepeat=FALSE);
	void GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat);

	// for custom scroll bar
	// ;
	CScrollBarEx *GetHorzScrollBarEx() const;
	CScrollBarEx *GetVertScrollBarEx() const;
	int UseCustomScrollBar(BOOL bUseCustomScrollBar);
	BOOL IsUseCustomScrollBar();
	int UpdateCustomScrollBar();

	int SetTextFont(CFont *pFontN, CFont *pFontD = NULL);
	int GetTextFont(CFont **ppFontN, CFont **ppFontD = NULL);

	int SetItemHeightEx(HTREEITEM hItem, UINT cyItemHeight);
	UINT GetItemHeightEx(HTREEITEM hItem);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinTreeCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinTreeCtrl)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItem(CDC *pDC, HTREEITEM hItem);

private:
	void *m_pMember;
};

class COleDropInfo : public CObject
{
public:
	COleDropInfo();
	COleDropInfo(COleDataObject* pDataObject,DWORD dwKeyState,
		DROPEFFECT dropEffect,DROPEFFECT dropEffectList,CPoint point );
	virtual ~COleDropInfo();

public:
	//COleDropTargetEx member functions params
	COleDataObject* m_pDataObject;
	DWORD m_dwKeyState;
	DROPEFFECT m_dropEffect;
	DROPEFFECT m_dropEffectList;
	CPoint m_point;
};

class COleDropTargetEx : public COleDropTarget
{
public:
	COleDropTargetEx();
	virtual ~COleDropTargetEx();

protected:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragScroll( CWnd* pWnd, DWORD dwKeyState, CPoint point );
		
};

/////////////////////////////////////////////////////////////////////////////
// CThumbnailCtrl window

#ifdef _WIN32
#define WC_THUMBNAILVIEWA            "SysThumbnailView32"
#define WC_THUMBNAILVIEWW            L"SysThumbnailView32"

#ifdef UNICODE
#define  WC_THUMBNAILVIEW            WC_THUMBNAILVIEWW
#else
#define  WC_THUMBNAILVIEW            WC_THUMBNAILVIEWA
#endif

#else
#define WC_THUMBNAILVIEW             "SysThumbnailView"
#endif

#define LVN_ENDDRAG                  1

class CThumbnailCtrl : public CWnd
{
// Construction
public:
	CThumbnailCtrl();
	virtual ~CThumbnailCtrl();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	
	// must be call after the object is created and before the window is created
	BOOL SetItemBitmap(LPCTSTR lpszNormal, LPCTSTR lpszHighlight, LPCTSTR lpszSelected, LPCTSTR lpszDisabled = NULL);
	BOOL SetItemBitmap(HBITMAP hItemBkN, HBITMAP hItemBkH, HBITMAP hItemBkS, HBITMAP hItemBkD = NULL);

	//
	int GetItemBitmap(HBITMAP *phItemBKN, HBITMAP *phItemBKH, HBITMAP *phItemBKS, HBITMAP *phItemBKD = NULL) const;

	// must be call after the object is created and before the window is created.
	// the total size is one image take up, include all space, from one item's left side to another item's left side.
	// this interface only support non-list style
	BOOL SetItemSize(CSize sizeTotal);
	
	// must be call after the object is created.
	BOOL SetItemPosition(LPCRECT lprcThumbnail, LPCRECT lprcHLThumbnail, LPCRECT lprcLabel, LPCRECT lprcHLLabel);
	BOOL SetLinesPerItem(int nLines);

	// 
	int GetItemPosition(LPRECT lprcThumbnail, LPRECT lprcHLThumbnail, LPRECT lprcLabel, LPRECT lprcHLLabel) const;
	
	// if necessary, call befor the CThumbnailCtrlEx object is created
	void SetScrollBarWidth(int nWidth); 

	int SetItemHeight(int nItemHeight);
	int GetItemHeight() const;
// Attributes
public:
	void ClientToWindow(LPRECT lpRect) const;
	void WindowToClient(LPRECT lpRect) const;

	int GetFocusItem();
	int SetFocusItem(int nItem);

	void GetScrollBarRect(CRect &rcScrollBar) const;
	void EnableDragDrop(BOOL bEnable = TRUE);// Enable Drag & Drop item in the view, muse call 	AfxOleInit() when app is init.
	void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);
	
	POSITION GetFirstSelectedItemPosition() const; // 
	int GetNextSelectedItem(POSITION& pos);// 
	int GetFirstVisibleItem(BOOL bPartialOK = TRUE);
	int GetLastVisibleItem(BOOL bPartialOK = TRUE);
	int GetItemCount() const;
	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
	int GetNextItem(int nItem, int nFlags);
	int GetHotItem() const;
	int GetTopIndex(); // only valid with list style
	UINT GetItemState(int nItem, UINT nMask) const;
	UINT GetSelectedCount();
	BOOL GetItem(LVITEM *pItem) const;
	BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const; // relative to the pos return by GetViewRect
	HBITMAP GetItemThumbnail(int nItem);
	BOOL GetViewRect(LPRECT lpRect) const;
	CString GetItemText(int nItem, int nSubItem);
#if(_MSC_VER <= 1200) // VC6.0
	DWORD GetItemData(int nItem);
#else
	DWORD_PTR GetItemData(int nItem);
#endif
	COLORREF GetBkColor() const;
	COLORREF GetTextColor() const;
	int GetCountPerPage( ) const;

	BOOL SetBkColor(COLORREF cr);

	int GetBkImage(BITMAPOBJ *phbm);
	BOOL SetBkImage(BITMAPOBJ hbm, BOOL fTile=TRUE, BOOL bDeleteOld=TRUE);

	BOOL SetItem(const LVITEM *pItem);
	BOOL SetItemState(int nItem, LVITEM *pItem);
	BOOL SetItemState(int nItem, UINT nState, UINT nMask);
	BOOL SetItemData(int nItem, DWORD dwData);
	BOOL SetItemText(int nItem, int nSubItem, LPTSTR lpszText);
	BOOL SetTextColor(COLORREF cr);
	BOOL SetItemThumbnail(int nItem, HBITMAP hbmpThumbnail, BOOL bDeleteOld = TRUE);
	void SetShiftStart(int nShiftStart);

	// for CScrollBarEx's background
	void SetScrollBarExBk(BITMAPOBJ hbmpScrollbarExN, BITMAPOBJ hbmpScrollbarExH, BITMAPOBJ hbmpScrollbarExS, BITMAPOBJ hbmpScrollbarExD);
	void GetScrollBarExBk(BITMAPOBJ *phbmpScrollbarExN, BITMAPOBJ *phbmpScrollbarExH, BITMAPOBJ *phbmpScrollbarExS, BITMAPOBJ *phbmpScrollbarExD);

	void SetScrollBarExThumb(BITMAPOBJ hbmpScrollbarExThumbN, BITMAPOBJ hbmpScrollbarExThumbH, BITMAPOBJ hbmpScrollbarExThumbS, BITMAPOBJ hbmpScrollbarExThumbD);
	void GetScrollBarExThumb(BITMAPOBJ *phbmpScrollbarExThumbN, BITMAPOBJ *phbmpScrollbarExThumbH, BITMAPOBJ *phbmpScrollbarExThumbS, BITMAPOBJ *phbmpScrollbarExThumbD);

	void SetScrollBarExLTUPOffset(int nX, int nY);
	void SetScrollBarExRTDNOffset(int nX, int nY);

	void SetScrollBarExLTUPButton(int nValue);
	int GetScrollBarExLTUPButton();

	void SetScrollBarExRTDNButton(int nValue);
	int GetScrollBarExRTDNButton();

// Operations
public:
	void MoveItem(int nDest, int nSrc);
	BOOL DeleteSelectedItem();
	int  InsertItem(const LVITEM *pItem);
	int  InsertItem(int nItem, LPCTSTR lpszItem);
	int  InsertItem(int nItem, LPCTSTR lpszItem, HBITMAP hThumbnail);
	int  InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	BOOL DeleteAllItems();
	BOOL DeleteItem(int nItem);
	BOOL EnsureVisible(int nItem, BOOL bPartialOK = TRUE);
	BOOL SelectAll(BOOL bSelected = TRUE);
	
	int HitTestDrop(CPoint point);
	int HitTest(CPoint pt, UINT *pFlags=NULL) const;
	void RefreshDisplay(bool bUpdateImmediately = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThumbnailCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


//Operations
protected:
	int GetSpace() const;
	void UpdateScrollingDisplay();
	BOOL IsValidIndex(int nIndex) const;
	void SelectByRect(LPRECT rcSel, BOOL bClear = TRUE);
	BOOL DoLButtonDown(UINT nFlags, CPoint point, BOOL bIsLButtonDown=TRUE);
	void DoMultipleSelection(BOOL bShift, BOOL bCtrl, int nPos, BOOL bIsLButtonDown=TRUE);
	CSize CalcThumbnailGrid() const;
	CDC *BeginRepaint();
	void EndRepaint(CDC* pDC);
	void BeginDrag(UINT nFlags, CPoint point);
	void EndDragSelect(POINT pt);
	BOOL OnMovementKey(UINT nChar);
	
	// Generated message map functions
protected:
	virtual void OnDraw(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//{{AFX_MSG(CThumbnailCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl window

#ifdef _WIN32
#define WC_TREELISTVIEWA            "SysTreeListView32"
#define WC_TREELISTVIEWW            L"SysTreeListView32"

#ifdef UNICODE
#define  WC_TREELISTVIEW            WC_TREELISTVIEWW
#else
#define  WC_TREELISTVIEW            WC_TREELISTVIEWA
#endif

#else
#define WC_TREELISTVIEW             "SysTreeListView"
#endif


// Tree control's extended style
#define TVS_EX_GRIDLINES            0x00000001
#define TVS_EX_BKLINE               0x00000002
#define TVS_EX_OWNERDRAWFIXED       0x00000004

// Height of Header control
#define IDW_TREELIST                1
#define IDW_TREELISTHEADER          0

class CTreeList : public CTreeCtrl
{
// Construction
public:
	CTreeList();
	virtual ~CTreeList();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeList)
	protected:
	;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
;
public:
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);

	BOOL SetItemTextEx(HTREEITEM hTI, int nCol, LPCTSTR lpszText);
	CString GetItemTextEx(HTREEITEM hTI, int nCol);

	void SetHeaderPtr(CSkinHeaderCtrl *pHeader);
	CSkinHeaderCtrl *GetHeaderCtrl();

	BOOL SetBkImage(BITMAPOBJ hbmN, BITMAPOBJ hbmD, BOOL bDeleteOld=TRUE);
	int GetBkImage(BITMAPOBJ *phbmN, BITMAPOBJ *phbmD);

	void SetTextFont(CFont *pFontN, CFont *pFontD = NULL);
	void GetTextFont(CFont **ppFontN, CFont **ppFontD = NULL);

	void SetHoverTime(DWORD dwHoverTime, BOOL bRepeat=FALSE);
	void GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat);

	int SetItemHeightEx(HTREEITEM hItem, UINT cyItemHeight);
	UINT GetItemHeightEx(HTREEITEM hItem);

	int ListIndexByHTREEITEM(HTREEITEM hTI);

	CListCtrl * GetListCtrl();

	// hide the same name interface in base class
private:
	// use SetTextFont
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	// use GetTextFont
	CFont* GetFont() const;
	// use SetItemTextEx
	BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem);
	// use GetItemTextEx
	CString GetItemText(HTREEITEM hItem) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeList)
	;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(CDC *pDC, HTREEITEM hTI);

private:
	CListCtrl m_List; //temp to restor data
	CSkinHeaderCtrl *m_pHeader;
	COLORREF m_crSelectedBK;
	BITMAPOBJ m_hBackgroundBitmapN;
	BITMAPOBJ m_hBackgroundBitmapD;
	DWORD m_dwHoverTime;
	BOOL m_bHoverRepeat;
	CFont *m_pFontN;
	CFont *m_pFontD;
	BOOL m_bRedrawFlag; // only change this value by "USER", the CTreeList must not change it.
};

class CTreeListCtrl : public CWnd
{
	friend class CTreeList;
public:
	CTreeListCtrl();
	virtual ~CTreeListCtrl();
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, 
		int nHeaderHeight=24, BOOL bUseCustomScroll=FALSE, int nScrollWidth=16);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeListCtrl)
	protected:
	;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
;
public:
	CTreeList *GetTreeListWnd();
	CSkinHeaderCtrl *GetHeaderCtrl();
	CScrollBarEx *GetScrollBarEx();
	BOOL IsUseScrollBarEx();

	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	BOOL SetBkImage(BITMAPOBJ hbmpN, BITMAPOBJ hbmpD, BOOL bDeleteOld = TRUE);
	int GetBkImage(BITMAPOBJ *phbmpN, BITMAPOBJ *phbmpD);

	void SetScrollBarExBk(BITMAPOBJ hbmpScrollbarExN, BITMAPOBJ hbmpScrollbarExH, BITMAPOBJ hbmpScrollbarExS, BITMAPOBJ hbmpScrollbarExD);
	void GetScrollBarExBk(BITMAPOBJ *phbmpScrollbarExN, BITMAPOBJ *phbmpScrollbarExH, BITMAPOBJ *phbmpScrollbarExS, BITMAPOBJ *phbmpScrollbarExD);
	void SetScrollBarExThumb(BITMAPOBJ hbmpScrollbarExThumbN, BITMAPOBJ hbmpScrollbarExThumbH, BITMAPOBJ hbmpScrollbarExThumbS, BITMAPOBJ hbmpScrollbarExThumbD);
	void GetScrollBarExThumb(BITMAPOBJ *phbmpScrollbarExThumbN, BITMAPOBJ *phbmpScrollbarExThumbH, BITMAPOBJ *phbmpScrollbarExThumbS, BITMAPOBJ *phbmpScrollbarExThumbD);
	void SetScrollBarExLTUPOffset(int nX, int nY);
	void SetScrollBarExRTDNOffset(int nX, int nY);

	SHORT SetItemHeight(SHORT cyHeight);
	SHORT GetItemHeight() const;

	void UpdateScrollingDisplay();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeListCtrl)
	;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void *m_pMember;
};

class CSkinRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CSkinRichEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinRichEditCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinRichEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinRichEditCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CHtmlCtrl : public CHtmlView
{
public:
	CHtmlCtrl();
	virtual ~CHtmlCtrl();

// html Data
public:
	//{{AFX_DATA(CHtmlCtrl)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
;
public:
	void ShowBorder(BOOL bShow, BOOL bRedraw=TRUE);
	void ShowIEScrollBar(BOOL bShow, BOOL bRedraw=TRUE);
	void ShowContextMenu(BOOL bShow);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlCtrl)
	;
	public:
#if(_MSC_VER <= 1200)
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
#else
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO *pInfo);
	virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual void PostNcDestroy(){}

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

#ifdef USEGDIPLUS

class CClockCtrl : public CWnd
{
// Construction
public:
	CClockCtrl();
	virtual ~CClockCtrl();

// Operations
public:
	int SetBitmap(BITMAPOBJ hbmpBk);
	BITMAPOBJ GetBitmap();
	int SetPointer(BITMAPOBJ hbmpPointer);
	BITMAPOBJ GetPointer();

	int SetRange(int nMix, int nMax);
	int GetRange(int *pnMix, int *pnMax);
	int SetPos(int nPos);
	int GetPos();

	int SetBkCenter(LPPOINT pt);
	int GetBkCenter(LPPOINT pt);
	int SetPtCenter(LPPOINT pt);
	int GetPtCenter(LPPOINT pt);

protected:
	virtual void OnDraw(CDC *pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CClockCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void *m_pMember;
};
#endif


class CSkinMenuItem
{
public:
	DWORD m_dwMagicNum;	//A magic number to distingush our data
	DWORD m_dwID;		//Menu ID
	bool m_bSeparator;	//Separator
	bool m_bSideBar;	//A gradient sidebar
	bool m_bButtonOnly;	//Button only style item
	CString m_strText;	//Menu item text
	HICON m_hIcon;		//Menu icon
	int m_nSize;		//Height of the item(Width of the sidebar if m_bSideBar is true)

public:
	CSkinMenuItem() 
	{
		m_dwMagicNum = 0x0505a0a0;
		m_dwID = 0;
		m_bSeparator = false;
		m_bSideBar = false;
		m_bButtonOnly = false;
		m_hIcon = NULL;
		m_nSize = 16;
	};

	virtual ~CSkinMenuItem()
	{
		if (m_hIcon)
			::DestroyIcon(m_hIcon);
	}

	BOOL IsMyData(void) { return m_dwMagicNum == 0x0505a0a0; };
};

class CSkinMenuText : public CSkinMenuItem	//Normal item with text and an optional icon
{
public:
	CSkinMenuText(DWORD dwID, LPCTSTR strText, HICON hIcon = NULL) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_strText = strText;
		m_hIcon = hIcon;
	}
};

class CSkinMenuSeparator : public CSkinMenuItem //A separator item
{
public:
	CSkinMenuSeparator() : CSkinMenuItem()
	{
		m_bSeparator = true;
	}
};

class CSkinMenuSideBar : public CSkinMenuItem //A gradient sidebar
{
public:
	CSkinMenuSideBar(int nSize = 32, LPCTSTR strText = NULL, HICON hIcon=NULL, DWORD dwID=0) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_bSideBar = true;
		m_strText = strText;
		m_hIcon = hIcon;
		m_nSize = nSize;
		m_dwID = dwID;
	}
};

class CSkinMenuButton : public CSkinMenuItem //A button only item
{
public:
	CSkinMenuButton(DWORD dwID, HICON hIcon = NULL) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_bButtonOnly = true;
		m_hIcon = hIcon;
	}
};

class CSkinMenuIcon : public CSkinMenuItem //A button only item
{
public:
	CSkinMenuIcon(DWORD dwID, HICON hIcon = NULL) : CSkinMenuItem()
	{
		m_dwID = dwID;
		m_hIcon = hIcon;
	}
};

class CSkinMenu : public CMenu  
{
	DECLARE_DYNAMIC(CSkinMenu)

public:
	CSkinMenu();
	virtual ~CSkinMenu();

	//Menu style(Default: STYLE_OFFICE)
	typedef enum 
	{
		STYLE_OFFICE,		//Draw a float button around the icon
		STYLE_STARTMENU,	//show selected bar below the icon
		STYLE_XP			//use different color for the icon area
	} MENUSTYLE;

	//Below is the functions to build the menu
	BOOL AddSideBar(CSkinMenuSideBar *pItem);
	BOOL AppendSkinMenu(UINT nFlags, CSkinMenuItem *pItem, ACCEL *pAccel=0);
	BOOL AppendSeparator(void);
	BOOL AppendSkinPopup(UINT nFlags, CSkinMenu *pPopup, CSkinMenuItem *pItem);
	BOOL InsertSkinMenu(UINT nFlags, UINT nPos, CSkinMenuItem *pItem);
	void Break(void); //change a column(the next item added will be in a new column)
	void BreakBar(void); //change a column with a break line(same as Break, except that a break line is drawn between two columns)
	CSkinMenu* LoadSkinMenu(UINT uID, MENUSTYLE sty, COLORREF bg, COLORREF selbar, COLORREF iconbg, LOGFONT* pLF = NULL); //add items to the menu based CSkinMenu class
	CSkinMenu* LoadSubMenu(CMenu* pSubMenu, CMenu* pSubSubMenu, int j, CSkinMenu* pPopup, MENUSTYLE sty, COLORREF bg, COLORREF selbar, COLORREF iconbg);
	BOOL SetIcon(UINT nID, HICON *hIcon, BOOL bByPos=FALSE); //add an icon to the given item

protected:
	CFont m_fontMenu;
	COLORREF m_clrBackGround;		//Background color
	COLORREF m_clrSelectedBar;	//selected bar color
	COLORREF m_clrText;			//Text color
	COLORREF m_clrSelectedText;	//selected text color
	COLORREF m_clrDisabledText;	//disabled text color

	COLORREF m_clrSideBarStart;	//Start color of the gradient sidebar
	COLORREF m_clrSideBarEnd;	//end color of the gradient sidebar
	COLORREF m_clrIconArea;		//Background color of the button(icon) area

	BOOL m_bBreak;		//if true, next item inserted into the menu will be added with the sytle MF_MENUBREAK
	BOOL m_bBreakBar;	//if true, next item inserted into the menu will be added with the sytle MF_MENUBARBREAK

	MENUSTYLE m_Style;	//menu style(currently support office or startmenu style)

public: //User these functions to change the default attribute of the menu
	void SetBackColor(COLORREF clr) { m_clrBackGround = clr; }
	void SetSelectedBarColor(COLORREF clr) { m_clrSelectedBar = clr; }
	void SetTextColor(COLORREF clr) { m_clrText = clr; }
	void SetSelectedTextColor(COLORREF clr) { m_clrSelectedText = clr; }
	void SetDisabledTextColor(COLORREF clr) { m_clrDisabledText = clr; }
	void SetSideBarStartColor(COLORREF clr) { m_clrSideBarStart = clr; }
	void SetSideBarEndColor(COLORREF clr) { m_clrSideBarEnd = clr; }
	void SetIconAreaColor(COLORREF clr) { m_clrIconArea = clr; }
	void SetMenuStyle(MENUSTYLE style) { m_Style = style; }
	BOOL SetMenuFont(LOGFONT lgfont);

	//Find the popupmenu from a menuitem id, you may not need it
	CSkinMenu *FindSubMenuFromID(DWORD dwID);

public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	static LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);

protected:
	virtual void DrawBackGround(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled);
	virtual void DrawButton(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked);
	virtual void DrawIcon(CDC *pDC, CRect rect, HICON hIcon, BOOL bSelected, BOOL bDisabled);
	virtual void DrawSideBar(CDC *pDC, CRect rect, HICON hIcon, CString strText);
	virtual void DrawText(CDC *pDC, CRect rect, CString strText, BOOL bSelected, BOOL bDisabled, BOOL bBold);
	virtual void DrawCheckMark(CDC *pDC, CRect rect, BOOL bSelected);
	virtual void DrawMenuText(CDC& dc, CRect rc, CString text, COLORREF color);
	virtual void DrawIconArea(CDC *pDC, CRect rect, BOOL bSelected, BOOL bDisabled, BOOL bChecked);

	void Clear(void); //Clean all memory and handles

	//helpers 
	HBITMAP CreateGradientBMP(HDC hDC,COLORREF cl1,COLORREF cl2,int nWidth,int nHeight,int nDir,int nNumColors);
	void DrawEmbossed(CDC *pDC, HICON hIcon, CRect rect, BOOL bColor = FALSE);
	void FillRect(CDC *pDC, const CRect& rc, COLORREF color);
};

//////////////////////////////////////////////////////////////////////////
// CMemStdioFile

class CMemStdioFile : public CMemFile  
{
public:
	CMemStdioFile();
	virtual ~CMemStdioFile();
	virtual BOOL OpenResource(LPCTSTR lpszID, LPCTSTR lpszType);

public:
	virtual BOOL ReadString(CString& rString);
	virtual BOOL ReadStringA(CString& rString);
	virtual BOOL ReadStringW(CString& rString);
	virtual void WriteString(LPCTSTR lpsz);
};

class CUIWnd : public CWnd
{
// Construction
public:
	CUIWnd(CWnd* pParentWnd = NULL);	// pParentWnd only for modal window
	BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, BOOL bLoadBkImageFromFile=TRUE, BOOL bUseLayeredWindow=TRUE);
	DWORD GetLastCreateError();

	int DoModal(DWORD dwStyle = 0);
	void EndDialog(int nResult);

public:
	;
	int SetScrollSize(CSize size);
	int GetScrollSize(CSize *pSize);
	int GetCXBorder();
	int SetCXBorder(int cx);
	int GetCYBorder();
	int SetCYBorder(int cy);

	int SetDragResizeFlag(BOOL bDragResize);
	BOOL GetDragResizeFlag();

	int SetBkImage(UINT uID, UINT uIDN, UINT uIDH, UINT uIDS, UINT uIDD, BOOL bDeleteOld=TRUE, BOOL bResetSize=TRUE);
	int SetBkImage(BITMAPOBJ hbmp, BITMAPOBJ hbmpN, BITMAPOBJ hbmpH, BITMAPOBJ hbmpS, BITMAPOBJ hbmpD, BOOL bDeleteOld=TRUE, BOOL bResetSize=TRUE);
	int SetBkImage(BITMAPOBJ hbmp, BOOL bDeleteOld=TRUE);
	BITMAPOBJ GetBkImage();

	int SetUIWndProperties(const UIWNDPROPERTIES *pProperties);
	const UIWNDPROPERTIES *GetUIWndProperties();

	int SetLoadBkImageFromFile(BOOL bFromFile);
	BOOL IsLoadBkImageFromFile() const;

	int UseLayeredWindow(BOOL bUsed);
	BOOL IsUseLayeredWindow();

protected:
	virtual void WindowID() = 0; 
	HWND PreModal();
	void PostModal();

protected:
	virtual void OnDraw(CDC *pMemDC);
#ifdef USEGDIPLUS
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;
	int SetLayeredWindow(BOOL bSet);
	virtual int UpdateMainWindow(int nTransparent=255);
#endif // USEGDIPLUS
	// provide your own images for background, by default, LibUIDK get theme from the path set in .ui file.
	virtual int InitGdiObject(BITMAPOBJ *pbmpBase, BITMAPOBJ *pbmpN, BITMAPOBJ *pbmpH, BITMAPOBJ *pbmpS, BITMAPOBJ *pbmpD, BOOL *pbDeleteObjAfterUsed);

	// When resizing the window, reset images for child. if bForceSet is TRUE, always set the image, otherwise, only set that needed.
	int SetChildImage(BITMAPOBJ bmpN, BITMAPOBJ bmpH, BITMAPOBJ bmpS, BITMAPOBJ bmpD);

	// Hit Test the point for resizing the window, if bSetCursor is TRUE, set the cursor for the window
	int HitTest(CPoint pt, BOOL bSetCursor);

	BOOL IsChildNeedResize(LPCRECT rect);

	void ReleaseGdiObject();
	void InitPath();
	int Init();

private:
	BOOL AdjustChildControls();
	BOOL CreateChildControls(BITMAPOBJ bmpN, BITMAPOBJ bmpH, BITMAPOBJ bmpS, BITMAPOBJ bmpD);
	BOOL DestroyChildControls();

protected:
	UINT IDD;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUIWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUIWnd)
	afx_msg void OnPaint();
	afx_msg LRESULT OnChildMove(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	void *m_pMember;
	CWnd* m_pParentWnd;             // parent/owner window
};

/////////////////////////////////////////////////////////////////////////////
// CMenuBar window

class CMenuBar : public CWnd
{
// Construction
public:
	CMenuBar();
	virtual ~CMenuBar();

	BOOL Create(DWORD dwStyle, CPoint ptLeftTop, CWnd* pParentWnd, UINT nWinID, UINT nMenuID, BITMAPOBJ hCombineImage);
	BOOL Create(DWORD dwStyle, CPoint ptLeftTop, CWnd* pParentWnd, UINT nWinID, UINT nMenuID, BITMAPOBJ hBmpN, BITMAPOBJ hBmpH, BITMAPOBJ hBmpS, BITMAPOBJ hBmpD);

public:
	int SetMenuID(UINT uMenuID);
	UINT GetMenuID() const;

	int SetMenuButtonImages(BITMAPOBJ hBmpN, BITMAPOBJ hBmpH, BITMAPOBJ hBmpS, BITMAPOBJ hBmpD, BOOL bDeleteOld = TRUE);
	int SetMenuButtonImages(BITMAPOBJ hCombineImage, BOOL bDeleteOld = TRUE);
	int GetMenuButtonImages(BITMAPOBJ *phBmpN, BITMAPOBJ *phBmpH, BITMAPOBJ *phBmpS, BITMAPOBJ *phBmpD);

	int PopupMenu(UINT uButtonID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnterMenuLoop(BOOL bIsTrackPopupMenu);
	afx_msg void OnExitMenuLoop(BOOL bIsTrackPopupMenu);
	afx_msg LRESULT OnMenuMouseMove(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void *m_pMember;
};

//////////////////////////////////////////////////////////////////////////
// CUIMgr

class CUIMgr
{
public:
	CUIMgr();
	virtual ~CUIMgr();

	static int SetUIPath(LPCTSTR lpszUIPath, BOOL bCheckUnicode = TRUE, BOOL bCheckVersion = FALSE);
	static int SetUIPath(LPCTSTR lpszUIID, LPCTSTR lpszResouceID, LPCTSTR lpszResourceType);

	static CString GetUIPath();
	static CString GetUIResourcePath();
	static CString GetUIPathWithoutTitle();

	static int GetCurrentMajorVersion();
	static int GetCurrentMinorVersion();
	static int GetCurrentBuildVersion();
	static int GetHighestMajorVersion();
	static int GetHighestMinorVersion();
	static int GetHighestBuildVersion();

	static int GetLogPixelsX();
	static int GetLogPixelsY();
	static int EnableDPI(BOOL bEnable);
	static BOOL IsEnableDPI();

	static RECT MAKERECT(LPCTSTR lpszRect);

	static int StringIDToInt(LPCTSTR lpszIDs);

	static CString StringIDFromInt(int nID);

	static int StringToStruct(LPCTSTR lpszStruct, void *pStruct, UINT uSizeStruct);
	static CString StructToString(void *pStruct, UINT uSizeStruct);

	static CUIWnd *GetUIWnd(LPCTSTR lpszWindowID);

	static std::vector<UIWNDPROPERTIES> *GetUIWndPropertiesList() { return &m_vWindows; }
	static UIWNDPROPERTIES *GetUIWNDPROPERTIES(LPCTSTR lpszWindowID);
	static CONTROL *GetCONTROL(LPCTSTR lpszWindowID, LPCTSTR lpszCtrlID);
	static int ReleaseSkin();
	static int LoadSkin();
	static int LoadIDDefined();
protected:
	static int LoadSetting(int *pnLogPixelsX, int *pnLogPixelsY);
	static int LoadUIWnd(UIWNDPROPERTIES *pWndProperties);
	static int LoadPushButton(BTNPROPERTIES *pBtnProperties);
	static int LoadCheckBox(CHKPROPERTIES *pChkProperties);
	static int LoadRadioButton(RADPROPERTIES *pRadProperties);
	static int LoadStatic(TXTPROPERTIES *pStaProperties);
	static int LoadEdit(EDTPROPERTIES *pEdtProperties);
	static int LoadComboBox(CMBPROPERTIES *pCmbProperties);
	static int LoadSpinButton(SPNPROPERTIES *pSpnProperties);
	static int LoadProgress(PRGPROPERTIES *pPrgProperties);
	static int LoadSlider(SLDPROPERTIES *pSldProperties);
	static int LoadHotKey(HOTPROPERTIES *pHotProperties);
	static int LoadIPAddress(IPAPROPERTIES *pIPAProperties);
	static int LoadListCtrl(LSTCTRLPROPERTIES *pLstCtrlProperties);
	static int LoadTreeCtrl(TREECTRLPROPERTIES *pTreeCtrlProperties);
	static int LoadScrollBarEx(SBPROPERTIES *pSBProperties);
	static int LoadThumbnail(THMPROPERTIES *pThmProperties);
	static int LoadTreeList(TREELISTPROPERTIES *pTreeListProperties);
	static int LoadHtmlCtrl(HTMLCTRLPROPERTIES *pHtmlCtrlProperties);
	static int AddControlToWndList(LPCTSTR lpszSection, const CONTROL *pCtrl);

private:
	static void LicenseWarning()
	{
		//AfxMessageBox(_T("This libarary hasn't been licensed! register it from http://www.iuishop.com"));
		//ASSERT(FALSE);
	}

private:
	static BOOL m_bLoadFromFile;

	// for hard disk skin
	static CString m_strUIPath;
	static CString m_strUIPathWithoutTitle;
	static CString m_strUIResourcePath;

	// for resource skin
	static CString m_strUIID;
	static CString m_strUIResourcehID;
	static CString m_strUIResourceType;

	// memory file
	static CMemStdioFile m_fileTheUI;
	static CMemStdioFile m_fileResourceH;
	static std::vector<UIWNDPROPERTIES> m_vWindows;
	static std::vector<std::pair<CString, int> > m_vIDs;

	// for version
	static int m_nCurrentMajorVersion;
	static int m_nCurrentMinorVersion;
	static int m_nCurrentBuildVersion;
	static int m_nHighestMajorVersion;
	static int m_nHighestMinorVersion;
	static int m_nHighestBuildVersion;

	// for DPI
	static BOOL m_bEnableDPI;
	static int m_nLogPixelsX;
	static int m_nLogPixelsY;
};

//////////////////////////////////////////////////////////////////////////
// CTaskWndInfo

#define WS_TASKSTYLE WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS

class CTaskWndInfo
{
public:
	CTaskWndInfo();
	~CTaskWndInfo();

public:
	CWnd *m_pTaskWnd;
	UINT m_nTaskWndID;
	DWORD m_dwTaskWndCreateStyle;
	CString m_strWindowName;
};


class CTaskWndMgr
{
public:
	CTaskWndMgr(BOOL bVirtualTaskRadio);	// bVirtualTaskRadio is TRUE: not use radio button to switch task window.
	virtual ~CTaskWndMgr();

public:
	static LRESULT ParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT PosWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	int SetParent(CWnd *pParent);
	int SetPosWindow(CWnd *pPosWnd);
	int AddTaskWindow(UINT nTaskRadioButton, CWnd *pTaskPanel, UINT nTaskWndID, LPCTSTR lpszWindowName = NULL, DWORD dwStyle = WS_TASKSTYLE);
	int RemoveTaskWindow(UINT nTaskRadioButton, BOOL bDestroyTaskWindow);
	int RemoveAllTaskWindows(BOOL bDestroyTaskWindow);
	int DestroyTaskWindow(UINT nTaskRadioButton);
	int SwitchToTaskWindow(UINT nID);
	int SwitchToTaskWindow(CSkinButton *pRadTask);
	UINT GetCurrentTaskWindow() const;
	int AdjustTaskWindowPos(LPCRECT lpRect);

private:
	void *m_pMember;
	std::map<UINT, CTaskWndInfo> m_mapTaskWnd;
};

}// name space LibUIDK


#endif //_CONTROLS_H_
