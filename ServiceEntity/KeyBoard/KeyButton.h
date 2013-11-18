#pragma once


// CKeyButton

class CKeyButton : public CButton
{
	DECLARE_DYNAMIC(CKeyButton)

public:
	CKeyButton();
	virtual ~CKeyButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetBkDC(CDC* dc);
	void SetImage(HBITMAP hBmp);

protected:
	CDC m_dcBK;
	CDC m_dcBut;
	HBITMAP	m_hBitmap;
	CSize   m_sizeImage;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnEraseBkgnd(CDC* pDC);
};


