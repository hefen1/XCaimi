
#if !defined(CHSSPIN_H)
#define CHSSPIN_H


class  CHSSpin : public CSpinButtonCtrl  
{
	DECLARE_DYNAMIC(CHSSpin)
public:
	// constructor
	CHSSpin ();
	// modifies the value in the control with steps*delta
	void Change (int steps);
	// the amount to change up and down.
	void SetDelta (double x);
	// responds to event.
	void OnDeltaPos (NMHDR* pNMHDR, LRESULT* pResult);
		
protected:
	//{{AFX_MSG(CHSSpin)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double m_delta;
};

#endif
