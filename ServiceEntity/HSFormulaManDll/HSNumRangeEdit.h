
#if !defined(CHSNUMRANGEEDIT_H)
#define CHSNUMRANGEEDIT_H



#include "HSNumEdit.h"

class  CHSNumRangeEdit : public CHSNumEdit  
{	
	DECLARE_DYNAMIC(CHSNumRangeEdit )				
public:
	//	constructor
	CHSNumRangeEdit ();
	
    //	get maximum limit
	virtual double GetMax ();
	
	//	get minimum limit
	virtual double GetMin ();
	//virtual float GetMin();
	
	void SetRange (double min, double max);
	
	//	set value in edit control, the input is validated before
	//	inserted
	virtual void SetValue (double x);
	
	//	sets wrap to true or false. mostly used with spinner.
	void SetWrap (bool wrap);
	
protected:	
	//	check to see that we only inpu x digits after zero.
	virtual bool CheckInput (UINT nChar);
	virtual int CheckInput (double fData);

	// HS更改，设置值的边界问题
	//virtual void OnUpdate();
	
    // Data Members for Class Attributes
		
	double m_max;
	double m_min;
 	  //{{AFX_MSG(CHSNumRangeEdit)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
    // Data Members for Class Attributes	
	bool m_wrap;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif
