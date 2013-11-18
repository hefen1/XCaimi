
#if !defined(CHSNUMSPINEDIT_H)
#define CHSNUMSPINEDIT_H



class CHSNumRangeEdit;
class CHSSpin;
class  CHSNumSpinEdit : public CWnd  
{
	DECLARE_DYNAMIC(CHSNumSpinEdit )		
public:
	//	constructor
	CHSNumSpinEdit ();
	
	virtual ~CHSNumSpinEdit ();
	
	//	get access to the spinner in the control
	CHSSpin* GetSpinner ();
	
	//	get dircet access to the edit
	CHSNumRangeEdit* GetEdit ();
	
	//	check for page up and down and increase values
	virtual BOOL PreTranslateMessage (MSG* pMsg);
	
	//	set digits after zero
	void SetDigits (int no);
	
	//	set limits in range, min, max and indicate if the values
	//	should wrap around limits
	void SetRange (double min, double max, double def,bool wrap);
	
	//	set the size of each step.
	void SetStepSize (double dx);
	
	
	//	set the font of the control
	void SetFont (CFont* pFont, BOOL bRedraw = TRUE);
	
	//SetTextColor
	void SetTextColor (int r, int b, int g);
	double GetValue ();
	void SetValue (double val);

	void MoveWindow(int x, int y, int nWidth, int nHeight,
				BOOL bRepaint = TRUE);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

protected:	
	int  OnCreate (LPCREATESTRUCT lpCreateStruct);
	void OnSize (UINT nType, int cx, int cy);
	void OnKeyDown (UINT nChar, UINT nRepCnt, UINT flags);
	void OnSetFocus (CWnd* pOldWnd);
	HBRUSH OnCtlColor (CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		
	CHSSpin *m_spin;
	CHSNumRangeEdit *m_edit;
	//{{AFX_MSG(CHSNumSpinEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()	

private:
	COLORREF m_textColor;
public:
	virtual long OnEditUpdate(WPARAM wParam, LPARAM lParam);
};
#endif
