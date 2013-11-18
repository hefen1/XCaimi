#include "stdafx.h"
#include "KBEdit.h"
#include "Express.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CKBEdit, CEdit)

CKBEdit::CKBEdit()
{
}

CKBEdit::~CKBEdit()
{
}


BEGIN_MESSAGE_MAP(CKBEdit, CEdit)
	//{{AFX_MSG_MAP(CHSEdit)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CKBEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}

HBRUSH CKBEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

HBRUSH CKBEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CKBEdit::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	CEdit::PreSubclassWindow();
}

BOOL CKBEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CEdit::PreCreateWindow(cs);
}

BOOL CKBEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CKBEdit::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(this->GetParent()->GetSafeHwnd(),HX_USER_DRAGDATA,wParam,lParam);
}