#ifndef CHSSTATICH
#define CHSSTATICH


class CStaticTitle : public CStatic
{
public:
	CStaticTitle();
	DECLARE_MESSAGE_MAP()
protected:
	CString m_strBKImage;		// ±³¾°Í¼Æ¬
	CString m_strFrontImage;		// Ç°¾°Í¼Æ¬
	COLORREF m_TitleColor;

public:
	BOOL SetBkImage(CString strBKImage);
	BOOL SetFrontImage(CString strFrontImage);
	BOOL SetTitleColor(COLORREF col);

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

class CQuoteButton: public CMFCButton
{
public:
	CQuoteButton();
	~CQuoteButton();
protected:
	CString m_strBKImage;
public:
	BOOL SetBkImage(CString strBKImage);
	DECLARE_MESSAGE_MAP()
public:
// 	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
// 	afx_msg void OnPaint();
// 	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

#endif