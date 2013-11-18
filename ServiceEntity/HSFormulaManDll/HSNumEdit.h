
#if !defined(CHSNUMEDIT_H)
#define CHSNUMEDIT_H




class  CHSNumEdit : public CEdit  
{
	
	DECLARE_DYNAMIC(CHSNumEdit)
		
public:
	CHSNumEdit ();
	//	set number of trailing digits after zero. i.e. set the
	//	presision of the values show.
	void SetDigits (int noOfDigitsAfterZero);
	
	//	get Number of digits after zero
	int GetDigits ();
	
	//	get the value of the edit control - assumes its a float,
	//	not just text.
	virtual double GetValue ();
	
	//	set value in edit control, the input is validated first
	virtual void SetValue (double x);
	
protected:
	
	//	responds to user input from keyboard
	virtual void OnChar (UINT nChar, UINT nRepCnt, UINT nFlags);
	
	//	check to see that we only inpu x digits after zero.
	virtual bool CheckInput (UINT nChar);
	virtual int CheckInput (double fData) { return 0; };
	BOOL CheckInput(CString strNum);
	virtual double GetMin()  { return 0; };
	virtual double GetMax()  { return 0; };

    // Data Members for Class Attributes
	
	int m_digits;
	//{{AFX_MSG(CHSNumEdit)
	//}}AFX_MSG

	afx_msg void OnUpdate();

	DECLARE_MESSAGE_MAP()
		
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
#endif
