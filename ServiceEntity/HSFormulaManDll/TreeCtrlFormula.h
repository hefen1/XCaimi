/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	TreeCtrlFormula.h
*	�ļ���ʶ��	��ʽ���ؼ��༰������ؽṹ����
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_HXTREECTRL_H__D334AF88_CFC7_11D4_903D_5254AB285D5A__INCLUDED_)
#define AFX_HXTREECTRL_H__D334AF88_CFC7_11D4_903D_5254AB285D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlFormula.h : header file
//
#include "TreeDropTarget.h"
#include "Express.h"
#include "ExpPublic.h"
#include "ExpValue.h"
/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlFormula window

/*************************************************************
*	�� �� �ƣ�	FunctionNameTable
*	�� �� ����	����������
***************************************************************/
struct FunctionNameTable
{
	CString		 m_strName; // ������
	FunctionNameTable(CString strName)
	{
		m_strName.Format("%s",strName);
	}
};


#ifdef Support_HWJBKCJ
#define Express_Header "!@#$%^&*()QWAZXSDDCFVGH-1970"
#else
#define Express_Header "!@#$%^&*()QWAZXSDDCFVGH-1971"
#endif

struct SaveTransZipData
{
	char			m_szHeader[256];
	unsigned short	m_nType;		// ��������,��ΪRT_ZIPDATA

	short 			m_nAlignment;	// Ϊ��4�ֽڶ������ӵ��ֶ�

	long			m_lZipLen;		// ѹ����ĳ���
	long			m_lOrigLen;		// ѹ��ǰ�ĳ���
	char*		    m_cData;		// ѹ���������

	SaveTransZipData()
	{
		//memset(this,0,sizeof(SaveTransZipData));
		strncpy(m_szHeader,Express_Header,sizeof(m_szHeader));
	}

	void Free()
	{
		if( m_cData )
		{
			delete[] m_cData;
			m_cData = NULL;
		}
	}
};

extern int   IsZipExpressData(const char* szData);
extern char* unzipData(SaveTransZipData* pZipData);
extern SaveTransZipData* zipData(void *lpszBuffer, long cbBuffer);


// CTreeGroup::
#define hx_RootGroup 0x01	// ������
#define hx_AddGroup  0x02	// ����(��������������)
#define hx_GroupItem 0x04	// Ҷ����Ŀ
#define hx_RecycleBinGroup 0x08 // ����վ 
#define hx_OtherData	   0x10 // ����,��ʱCTreeGroup::m_pData��Ч

struct CTreeGroup
{	
	char		m_nStatus;	// ״̬
	HTREEITEM   m_hTree;    // ����Ŀ�����Ϣ
	union
	{		
		void*		m_pData;  // ָ��CExpValue����(!(m_nStatus&hx_OtherData))
		long		m_dwID;	  // ��������ID
	}; // ID��������ָ��(����m_nStatus�ж�)

	CTreeGroup()
	{
		Empty();
	}
	CTreeGroup(char nStatus,long dwID)
	{
		m_nStatus = nStatus;
		m_dwID    = dwID;
		m_hTree   = NULL;
	}

	CTreeGroup(void* pData,char nStatus,HTREEITEM hTree)
	{
		m_pData   = pData;
		m_nStatus = nStatus;
		m_hTree	  = hTree;
	}

	// ״̬����
	BOOL IsStatus(char nStatus) { return (m_nStatus == nStatus); }
	void SetStatus(char nStatus) { m_nStatus = nStatus; }
	// ���
	void Empty()
	{
		memset(this,0,sizeof(CTreeGroup));
	}
	
	// �õ������Ĺ�ʽ
	CExpression* GetData()
	{
		if( !(m_nStatus & hx_OtherData) && 
			(m_pData != NULL) )
		{
			return ((CExpValue*)m_pData)->GetExp();
		}
		return NULL;
	}

	// �Ƿ�ָ��ʽ(CExpValue*)pValue
	BOOL IsExpValue(long pValue) 
	{
		return ( !(m_nStatus & hx_OtherData) && ((long)m_pData == pValue) );
	}

	// �Ƚ����������ǲ�����ͬ
	BOOL Compare(CTreeGroup* pTreeGroup)
	{
		if( GetData() != NULL && 
			pTreeGroup != NULL && pTreeGroup->GetData() != NULL )
		{
			return (GetData() == pTreeGroup->GetData());
		}
		return FALSE;
	}
	// �õ���ʽ��
	CString GetName()
	{
		CExpression* pExp = GetData();
		if( pExp == NULL )
			return "";
		
		CString strName;
		strName.Format("%s",pExp->GetName());
		return strName;
	}
};

class CDlgFormulaMan;

/*************************************************************
*	�� �� �ƣ�	CTreeCtrlFormula
*	�� �� ����	��ʽ���ؼ�
***************************************************************/
class CTreeCtrlFormula : public CTreeCtrl
{
// Construction
public:
	// strName��ʽ������,һ���ǹ�ʽ������
	CTreeCtrlFormula(CString strName = ""); 
	virtual ~CTreeCtrlFormula();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlFormula)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlFormula)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	// ������Ŀ�¼�����( �������� ) 
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	// ˫����Ŀ�¼����� ����AddFunToEdit()
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	// ��Ŀ������Ϣ����( �������� ) 
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	// ѡ��ı䴦��
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	// �Ҽ����´���
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// ������´���
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// �Ҽ�������Ŀ�¼�����
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	// ���˫������
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// �޸���Ŀ����"��ʼ����"
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	// �޸���Ŀ����"��������"
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	// �ַ�������Ϣ����
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// ������Ϣ����
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) ;
	//}}AFX_MSG
	// ������ʱ���ϷŴ���
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnNewGroup();
	afx_msg void OnExpandGroup();
	afx_msg void OnReduceGroup();

	afx_msg void OnNewExpress();	
	afx_msg void OnEditExpress();

	afx_msg void OnDelExpress();
	afx_msg void OnDelGroup();

	afx_msg void OnDefaultPress();

	afx_msg void OnDelUnion();

	afx_msg void OnKillFocus( CWnd* pNewWnd );

	afx_msg LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	// ��ʽ�༭�ؼ�ָ��
	static class CCompileEditView*  m_pCompileEditView;
	// ��ʽ��������ָ��
	static class CCompileEditView*  m_pHelpView;
	

	/******************************************************************
	*	�������ܣ�	��������ΪnType�����к������ڵ���Ϊg_strFunName[nType].m_strName�Ľڵ���
	*	����������	short nType : [in] ��������(ҲΪ������)
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void		InsertFunByType(short nType);

	/******************************************************************
	*	�������ܣ�	��������ΪnType�����й�ʽ���ڵ���ΪstrName�Ľڵ���
	*	����������	CString strName : [in] Ҫ����ĸ��ڵ���
	*				short nType		: [in] ����Ĺ�ʽ����
	*				short nImage	: [in] ����ĸ�����ͼ������
	*	�� �� ֵ��	HTREEITEM  : ���صĸ������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM   InsertExpressByType(CString strName,short nType,short nImage);

	/******************************************************************
	*	�������ܣ�	����ע����Ϣ�������Ĵ�������
	*	����������	HTREEITEM hItem : [in] Ҫ���ð�������Ŀ���
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void		SetFunNotes(HTREEITEM hItem);

	/******************************************************************
	*	�������ܣ�	�����������������ͬ�Ĵ���
						Export : ���뵼�����ڷ��͹�ʽ������Ϣ
					EditFunRef : ��ʽ�༭�����ڷ��ͺ������ִ�
					EditExpRef : ��ʽ�༭�����ڷ��͹�ʽ����(CValue*)
					  Function : ��ʽ�༭�ؼ����ͺ�����Ϣ����HSAddStrCompileEdit*
					   Express : �򿪹�ʽ�༭������
	*	����������	���ޣ�
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void		AddFunToEdit();

public:
	CMapVariabile*  m_pInExpression;     // �ⲿ���ʽ��

public:
	// ���ù�ʽ�༭�ؼ��������ؼ�
	void  Atatch(class CCompileEditView* pCompileEditView,class CCompileEditView* pHelpView);

public:

	/******************************************************************
	*	�������ܣ�	���������͵Ĺ�ʽ������䵽�ؼ���
	*	����������	CMapVariabile* pExternFunction : [in] ����
	*	�� �� ֵ��	void 
	*	����������	InsertFunByType()
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void FillFunction(CMapVariabile* pExternFunction);

	/******************************************************************
	*	�������ܣ�	�����й�ʽ��������䵽�ؼ���
	*	����������	CMapVariabile* pExternExpression : [in] ����
	*	�� �� ֵ��	void 
	*	����������	InsertExpressByType()
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void FillExpress(CMapVariabile* pExternExpression);

	/******************************************************************
	*	�������ܣ�	����ʽ������ΪstrName�����й�ʽ��䵽�����ΪstrName�ĸ������
	*	����������	CMapVariabile* pExternExpression : [in] ����
	*				CString strName : [in] ������
	*				char cType		: [in] ��ʽ����
	*				short nImage	: [in] �б��ͼ������
	*	�� �� ֵ��	void 
	*	����������	InsertExpressByType()
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void FillExpress(CMapVariabile* pExternExpression,CString strName,char cType,short nImage);

public:
	/******************************************************************
	*	�������ܣ�	�Ƿ���Ϸ�
	*	����������	HTREEITEM hSelectedItem : [in] Դ��Ŀ���
	*				HTREEITEM hTDropItem    : [in] Ŀ����Ŀ���
	*	�� �� ֵ��	BOOL : ���Ϸ�(TRUE), �����Ϸ�(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsDrag(HTREEITEM hSelectedItem,HTREEITEM hTDropItem);

	// ����������
	enum TreeType{ None/*����*/,Function/*����*/,Express/*��ʽ*/,AlignWin/*����*/,EditFunRef/*����(����ʱ��)*/,EditExpRef/*��ʽ����(����ʱ��)*/,Export/*���뵼��*/,HSBrowser/*���*/ };
	enum CopyType { CopyNone     = 0x00010000, // ��������
					CopyDelete   = 0x00020000, // ��ɾ���ٿ���
					CopyLoad     = 0x00040000, // ��������
					CopyNotGroup = 0x00080000, // ��Ҫ��
					CopyNoRoot	 = 0x00100000, // ��Ҫ���ڵ�
					CopyMustHasChildren = 0x00200000, // ����Ҫ�к��ӽڵ����
					CopyMask     = 0xFFFF0000  // ��������MASK
	              };

	// ������
	TreeType   m_treeType;
	// �����Ͳ���
	BOOL	   IsType(TreeType nType)  { return m_treeType == nType;}
	void	   SetType(TreeType nType) { m_treeType = nType; }


	/******************************************************************
	*	�������ܣ�	չ��������ָ�����µ�������
	*	����������	HTREEITEM hCurItem : [in] ָ������,ΪNULLʱȡROOT
	*				UINT nCode : [in] ������ʽ, �����¼���:
										 TVE_COLLAPSE  -- Collapses the list.
									TVE_COLLAPSERESET  -- Collapses the list and removes the child items.
										   TVE_EXPAND  -- Expands the list.
										   TVE_TOGGLE  -- Collapses the list if it is currently expanded or expands it if it is currently collapsed. 
	*	�� �� ֵ��	void	  
	*	����������	Expand()
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void	   ExpandGroup(HTREEITEM hCurItem,UINT nCode);

	/******************************************************************
	*	�������ܣ�	��������strName�Ƿ��Ѿ�����
	*	����������	CString strName		: [in] Ҫ���ҵ�����
	*				BOOL bPrompt = TRUE : [in] ������ҵ���������,�Ƿ���ʾ��Ϣ
	*	�� �� ֵ��	HTREEITEM  : �Ҳ���(NULL), ����Ϊ�ҵ���Ŀ�ľ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM  FindGroupName(CString strName,BOOL bPrompt = TRUE,HTREEITEM hRootItem = NULL);

	/******************************************************************
	*	�������ܣ�	��������strName��hParentItem��Ů��(���������� ??? )�Ƿ��Ѿ�����
					���hParentItemΪҶ�ӽ���������ֵ���Ѱ��
	*	����������	CString strName			: [in] Ҫ���ҵ�����
	*				HTREEITEM hParentItem	: [in] Ҫ���ҵ�����
	*	�� �� ֵ��	HTREEITEM : �Ҳ���(NULL), ����Ϊ�ҵ���Ŀ�ľ��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM  FindNameInGroup(CString strName,HTREEITEM hParentItem);

	/******************************************************************
	*	�������ܣ�	ɾ����ĿhItem������������������Ŀ
	*	����������	HTREEITEM hItem  : [in] Ҫɾ������Ŀ
	*				BOOL bRecycleBin : [in] �Ƿ�������վ(������ɾ��)
	*	�� �� ֵ��	void	  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void	   DelAll(HTREEITEM hItem,BOOL bRecycleBin);
	/* ɾ������û�к��ӵ���֦ */
	void	   DelAllNoChildren(HTREEITEM hItem = NULL);

	/******************************************************************
	*	�������ܣ�	ɾ����Ŀ����������Ĺ�ʽ��Ϣ(��ʹ��Ҳ������IsUsed()�Ͳ���ɾ��,�Ҿ��������е����� ??? )
	*	����������	HTREEITEM hItem			: [in] Ҫɾ������Ŀ���
	*				BOOL bDelSource = TRUE  : [in] �Ƿ�ɾ����ʽ��(ȫ��)
	*	�� �� ֵ��	void	  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void	   DelFromArray(HTREEITEM hItem,BOOL bDelSource = TRUE);

	/******************************************************************
	*	�������ܣ�	����һ����Ŀ
	*	����������	HTREEITEM hParent  : [in] ������Ŀ�ĸ���Ŀ
	*				CString csItemName : [in] ��Ŀ��
	*				HTREEITEM hInsAfter : [in] ��hInsAfter�������Ŀ
	*				int iSelImage      : [in] ѡ��ʱ��ʾ��ͼ��
	*				int iNonSelImage   : [in] ûѡ��ʱ��ʾ��ͼ��
	*				long lParam	: [in] ����������
	*	�� �� ֵ��	HTREEITEM : �������Ŀ���, ����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM  AddItem( HTREEITEM hParent, CString csItemName, HTREEITEM hInsAfter, int iSelImage , int iNonSelImage, long lParam);

	/******************************************************************
	*	�������ܣ�	����һ����Ŀ
	*	����������	CString strName : [in] ��Ŀ��(ͬʱҲ�ǹ�ʽ��pExternExpression�е�һ����ʽ��)
	*				CMapVariabile* pExternExpression : [in] �ⲿ��ʽ��
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	BOOL	   AddItem(CString strName,CMapVariabile* pExternExpression);

	/******************************************************************
	*	�������ܣ�	����һ����Ŀ
	*	����������	CString strName : [in] ��Ŀ��
	*				CExpValue* pValue : [in] ��ʽ����
	*				CMapVariabile* pExternExpression : [in] ����
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	BOOL	   AddItem(CString strName,CExpValue* pValue,CMapVariabile* pExternExpression);

	/******************************************************************
	*	�������ܣ�	��hItem(��Ҷ��)��hItem(��Ҷ��)�ĸ��ڵ��²���һ����Ŀ,ͬʱ����ʽ��������ΪcType
	*	����������	CString strName : [in] ��Ŀ��(ͬʱҲ�ǹ�ʽ��pExternExpression�е�һ����ʽ��)
	*				char cType		: [in] �����õĹ�ʽ���� 
	*				HTREEITEM hItem : [in] Ҫ����ڵ�ĸ��ڵ�����ֵܽ��
	*				CMapVariabile* pExternExpression : [in] �ⲿ��ʽ��
	*	�� �� ֵ��	HTREEITEM  : �������Ŀ���, ����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM  AddItem(CString strName,char cType,HTREEITEM hItem,CMapVariabile* pExternExpression);

	/******************************************************************
	*	�������ܣ�	��Դ��ĿhSrcTItem����������Ŀ(����hSrcDestTItem��)������hDestTItem��
	*	����������	HTREEITEM hSrcTItem	    : [in] Դ��Ŀ
	*				HTREEITEM hDestTItem    : [in] Ŀ�길��Ŀ
	*				HTREEITEM hSrcDestTItem : [in] ����������Ŀ 
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem,HTREEITEM hSrcDestTItem);

	/******************************************************************
	*	�������ܣ�	һ�������ڹ�ʽ����,������hItem�����hItem���ֵ��н��и�����Ӧ�����Ŀ������
	*	����������	CString strName : [in] ������Ĺ�ʽ��
	*				HTREEITEM hItem : [in] Ҫ��������Ŀ���ڵ���������ֵ�
	*				CMapVariabile* pExternExpression : [in] �ⲿ��ʽ��
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	BOOL	   UpDateItem(CString strName,HTREEITEM hItem,CMapVariabile* pExternExpression);

	/******************************************************************
	*	�������ܣ�	��pNewVal�Ĺ�ʽ��������ΪpOldVal�Ĺ�ʽ����,����,����m_ayTreeGroup��,ԭ��ָ��pOldVal������ָ��pNewVal
	*	����������	CValue* pOldVal : [in] �ɹ�ʽ
	*				CValue* pNewVal : [in] �¹�ʽ
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)     
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	BOOL	   UpdateTreeItemData(CValue* pOldVal,CValue* pNewVal);

	/******************************************************************
	*	�������ܣ�	������Ŀ��ͼ������
	*	����������	HTREEITEM hSrcTItem : [in] Ҫ��������Ŀ���
	*				int &nSelItemImage  : [in] ѡ��ʱ��ͼ������
	*				int &nNonSelItemImage : [in] ûѡ��ʱ��ͼ������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void GetItemImages(HTREEITEM hSrcTItem, int &nSelItemImage, int &nNonSelItemImage);

	// �õ���Ŀ����(��ǰ�ؼ�����)
	CArray<CTreeGroup*,CTreeGroup*>* GetTreeGroup() { return &m_ayTreeGroup; }

protected:
	// �Ϸ�����  -- û��ʹ�ã��Ż����ɾ��  ???
	CTreeDropTarget m_CTreeDropTarget;

protected:
	// ��Ŀ���� 
	CArray<CTreeGroup*,CTreeGroup*> m_ayTreeGroup;

public:
	// �����Ŀ����
	void Delete();
	// �����Ŀ����ͬʱ�����ʾ����Ŀ
	void DeleteTree();

public:
	// ��ʽ����ṹ
	struct LoadItemData
	{
		CExpValue* pExpValue; // ��ʽ 

		CString	   strName;   // ��ʽ��
		char	   nStatus;   // ���״̬��CTreeGroup::m_nStatus
		int		   nSelItemImage; // ѡ��ʱ��ͼ������
		int		   nNonSelItemImage; // ûѡ��ʱ��ͼ������

		char       nExpressType; // ��ʽ����

		UINT stateMask,state; // tree state

		LoadItemData()
		{
			pExpValue = NULL;
			stateMask = TVIS_BOLD;
			state = 0;
		}

		// ���л�
		void Serialize(CArchive& ar)
		{
			char cStatus = 0;
			if( ar.IsStoring() )
			{
				if( pExpValue != NULL )
				{
					// cStatus = 1;
					// ar << cStatus;
					pExpValue->Serialize(ar);
				}
				else
				{
					ar << cStatus;
				}
				ar << nExpressType;
				ar << strName;
				ar << nStatus;
				ar << nSelItemImage;
				ar << nNonSelItemImage;
			}
			else
			{
				ar >> cStatus;
				if(cStatus)
				{
					if( pExpValue != NULL )
					{
						delete pExpValue;
					}
					if( cStatus == 1 )
					{
						pExpValue = new CExpValue;
					}
					else if( cStatus == 2 )
					{
						pExpValue = new CExpValue;
						pExpValue->SetStyle(HX_UNION_EXPRESS);
					}
					pExpValue->Serialize(ar);
				}
				ar >> nExpressType;
				ar >> strName;
				ar >> nStatus;
				ar >> nSelItemImage;
				ar >> nNonSelItemImage;
			}
		}
	};
protected:
	/******************************************************************
	*	�������ܣ�	������hRootItem�µ����н������
	*	����������	CArchive& ar		: [in] ���л��ļ�
	*				HTREEITEM hRootItem : [in] Ҫ�������������
	*				LoadItemData* status: [in] ����ʱʹ�õ��м�����ṹ 
	*				int& nCountRef : [in/out] ����Ľ����
	*	�� �� ֵ��	void	  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void	     Save(CArchive& ar,HTREEITEM hRootItem,LoadItemData* status,int& nCountRef);

	/******************************************************************
	*	�������ܣ�	����m_ayTreeGroup��������hParentItem�� ( ??? ָ��ʹ��ǰû���ж� )
					����status.m_strName���ⲿ��ʽ���еõ�����,����Ҳ����򵱹�ʽ�鴦��
	*	����������	HTREEITEM hParentItem : [in] �����
	*				LoadItemData& status  : [in] ���������
	*				int nCopy = CopyNone  : [in] ��ʽ����
	*	�� �� ֵ��	HTREEITEM : �������е���Ŀ���,����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM	 Load(HTREEITEM hParentItem,LoadItemData& status,int nCopy = CopyNone,DWORD dwStyle = 0);

	/******************************************************************
	*	�������ܣ�	��status��������m_ayTreeGroup��������hParentItem��
					����status.m_pData�õ���ʽ����,�滻�ⲿ��ʽ����ǵ��뵼����ʽ���еĹ�ʽ����)
	*	����������	HTREEITEM hParentItem : [in] �����
	*				LoadItemData& status  : [in] ���������
	*				int nCopy = CopyNone  : [in] ����
	*	�� �� ֵ��	HTREEITEM : �������е���Ŀ���,����ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM	 Read(HTREEITEM hParentItem,LoadItemData& status,int nCopy = CopyNone,DWORD dwStyle = 0);

	/******************************************************************
	*	�������ܣ�	�����ؼ�pTree�µ���hRootItem����������ǰ�ؼ��µ�hDesRootItem����
	*	����������	CTreeCtrlFormula* pTree	: [in] Դ���ؼ�
	*				HTREEITEM hRootItem : [in] Դ�������
	*				LoadItemData* status: [in] ����������м���� 
	*				HTREEITEM hDesRootItem : [in] Ŀ���������
	*				int nCopy : [in] ������ʽ enum CopyType
	*	�� �� ֵ��	void		 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void		 Copy(CTreeCtrlFormula* pTree,HTREEITEM hRootItem,LoadItemData* status,HTREEITEM hDesRootItem,int nCopy,DWORD dwStyle = 0);

	/******************************************************************
	*	�������ܣ�	���л�,����ֻʵ�ֵı���(�ڵ���ʱʹ��)
	*	����������	CArchive& ar   : [in] ���л�����
	*				int& nCountRef : [in] ����Ľ�����ݸ���
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	virtual void Serialize(CArchive& ar,int& nCountRef);

	// �����ļ���(��ȥ��׺��·��)--һ���Ƿ�����
	CString		 m_strFileName; 

public:
	/******************************************************************
	*	�������ܣ�	�������빫ʽ����
	*	����������	UINT nMode : [in] ����(CArchive::store)���Ƕ���(CArchive::load)
	*				CString strFileName = "" : [in] �ļ���,Ϊ��ʱ,��ָ CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) +m_strFileName+".exp";
	*				CDlgFormulaMan* pFormulaMan = NULL : [in] ��ʽ������
	*				DWORD dStyle = 0 : [in] ��������, HS_EXP_TEMPIN��ʱ����
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��13��
	*	�޸�˵����	
	*****************************************************************/
	BOOL SaveLoad(UINT nMode,CString strFileName = "",CDlgFormulaMan* pFormulaMan = NULL,DWORD dStyle = 0);	

public:
	/******************************************************************
	*	�������ܣ�	pCurGroup�����Ƿ�ʹ��
					��hRootItemΪҶ��ʱ,�����ֵ���Ѱ��,�ڵ��������Ƿ����pCurGroup,��������ͷ���TRUE,����FALSE
					��hRootItem��������,����������Ѱ��,�ڵ��������Ƿ����pCurGroup,��������ͷ���TRUE,����FALSE
	*	����������	HTREEITEM hRootItem  : [in] Ŀ���������
	*				CTreeGroup* pCurGroup: [in] Ŀ������
	*	�� �� ֵ��	BOOL : �ҵ�Ҳ������ʹ��(TRUE), ����(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsUsed(HTREEITEM hRootItem,CTreeGroup* pCurGroup);
	// pCurGroup�Ƿ��ڳ���"����վ"�������ʹ��
	BOOL IsUsed(CTreeGroup* pCurGroup);

	// ����strName(������)�õ���ʽ����
	char GetType(CString strName);
	// �õ�hCurItem�ڵ�Ĺ�ʽ��������ڵ�Ĺ�ʽ���Ͳ���������丸�ڵ����,ֱ�ҵ����������ҵ�����Ϊֹ
	char GetType(HTREEITEM& hCurItem);
	// ����m_strName(������)�õ���ʽ����
	char GetType();

public:
	/******************************************************************
	*	�������ܣ�	Ҫ���и�����в����Ƿ���ڽ����ΪstrName�Ľ��
	*	����������	CString strName : [in] Ҫ���ҵĽ����,Ϊ��ʱ,��m_strName
	*	�� �� ֵ��	HTREEITEM  : �Ҳ�����ΪNULL, ����Ϊ�ҵ��ĸ������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM   GetRootTree(CString strName);

	// �˺�������ʹ��,���ܲ��Ǻ�����
	HTREEITEM	GetCurSavePos(HTREEITEM hCurItem);

	/******************************************************************
	*	�������ܣ�	����ʽ���ؼ�pTree�����н�㿽������ǰ����
	*	����������	CTreeCtrlFormula* pTree  : [in] Դ���ؼ�
	*				int nCopy = CopyNone : [in] ������ʽ
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void		CopyTree(CTreeCtrlFormula* pTree,int nCopy = CopyNone,DWORD dwStyle = 0);

	/******************************************************************
	*	�������ܣ�	��ĿhCurItem�Ƿ�����ĿhDesItem��
	*	����������	HTREEITEM hCurItem : [in] Ҫ�ҵ���Ŀ
	*				HTREEITEM hDesItem : [in] �ڴ���Ŀ��Ѱ�� 
	*	�� �� ֵ��	BOOL : ��ĿhCurItem����ĿhDesItem�·���TRUE, ���򷵻�FALSE
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL		OneItemInOtherItem(HTREEITEM hCurItem,HTREEITEM hDesItem);
	
	/******************************************************************
	*	�������ܣ�	����ǰ�ؼ���,����ǰ������㶼����˵���
	*	����������	UINT nID	: [in] �˵�ID��ʼ
	*				CMenu& popup: [in] Ŀ��˵�
	*	�� �� ֵ��	void	  
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void	  AddTreeGroupToMenu(UINT nID,CMenu& popup);

	/******************************************************************
	*	�������ܣ�	�õ������pRootTree�µ��ӽڵ��Ƿ���������ΪstrRefName�Ľ��
	*	����������	CString strRefName  : [in] Ҫ���ҵĽ����
	*				HTREEITEM pRootTree : [in] �����, ΪNULLʱ��������
	*	�� �� ֵ��	HTREEITEM  : NULL�Ҳ���,����Ϊ�ҵ����ӽ����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM GetTreeItem(CString strRefName,HTREEITEM pRootTree);

	// �õ�hItem��Ŀ��TVITEM::lParam����
	DWORD	  GetItemData(HTREEITEM hItem);

	static CString g_strTech;			// ����ָ��
	static CString g_strCondition;		// ����ѡ��
	static CString g_strExchange;		// ����ϵͳ
	static CString g_strColorKLine;		// ���K��
	static CString g_strStrategy;		// ���Խ��� 20090302 YJT ����
	static CString g_strRecycleBin;		// ����վ

	static CString g_strUnionCondition;	// ��Ϲ�ʽ 
	static CString g_strJiben;			// ������

	static CString g_strFirstTechLine;  // ��1��ָ����
	
	static CString g_strDefault;		// Ĭ��ָ�� == "����"

	static FunctionNameTable g_strFunName[]; // ����������

	static CString m_strExpGroup;	// "����";
	static CString m_strExpAll  ;	// "����";
	static CString m_strExpComm ;	// "����";
	static CString m_strExpUser ;	// "�Ա�";

public:
	class CDlgFormulaMan* m_pFormulaMan;	// ��ʽ����������

public:		
	/******************************************************************
	*	�������ܣ�	�������hRootItem�µ�����������Ĺ�ʽ����ȫ��ΪnExpressType
	*	����������	HTREEITEM hRootItem : [in] Ҫ��������
	*				int nExpressType    : [in] ��ʽ����
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void Save(HTREEITEM hRootItem,int nExpressType);

	/******************************************************************
	*	�������ܣ�	�������й�ʽ
	*	����������	int nExpressType : [in] ������
	*				void* pData      : [in] ������
	*	�� �� ֵ��	void 
	*	����������	SaveLoad()
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void Save(int nExpressType,void* pData);

protected:
	// ������
	CString m_strName; 
public:
	CString GetTreeName() { return m_strName; }

	// ��ǰ�����Ƿ�ΪstrName
	BOOL	  IsTree(CString strName);

	/******************************************************************
	*	�������ܣ�	����һ����Ŀ������Ŀ����m_ayTreeGroup�м���
	*	����������	CString strRootName : [in] ���������
	*				int nType			: [in] ��ʽ����	
	*				CString strSub		: [in] ����������
	*				CExpValue* pValue	: [in] ����Ĺ�ʽ
	*				HTREEITEM hItem = NULL : [in] �������
	*				DWORD dStyle = 0 : [in] �Ƿ����TVIS_BOLD(�Ӵ�)���
	*	�� �� ֵ��	HTREEITEM : �����Ľ����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM InputItem(CString strRootName,int nType,CString strSub,CExpValue* pValue,HTREEITEM hItem = NULL,DWORD dStyle = 0);

	/******************************************************************
	*	�������ܣ�	�ڸ��ڵ�hItem(��ΪstrRootName)�µ�strSub(��Ϊ��)��ڵ��²���һ�������ڵ�
	*	����������	CString strRootName : [in] ������
	*				int nImg			: [in] ����
	*				CString strSub		: [in] ����ڵ���, Ϊ����û��ڵ�
	*				CFunExpresion* pValue : [in] 
	*				HTREEITEM hItem = NULL: [in] ���ڵ���
	*	�� �� ֵ��	HTREEITEM : ����Ľڵ���, ���ɹ���ΪNULL
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM InputFunItem(CString strRootName,int nImg,CString strSub,CFunExpresion* pValue,HTREEITEM hItem = NULL);

public:
	/******************************************************************
	*	�������ܣ�	���빫ʽ����
	*	����������	CString strName : [in] ��ʽ�ļ���
	*	�� �� ֵ��	void 
	*	����������	SaveLoad();
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void LoadFromFile(CString strName);

	// �������,ȫ�ǵ��û���,�ڴ˲���˵��
public:
	HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct);
	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
						int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
						HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT,
						 HTREEITEM hInsertAfter = TVI_SORT);
	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
						 HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_SORT);
	

	// check
public:
	/******************************************************************
	*	�������ܣ�	��hCurItem�����������е�CHECK״̬��ΪnCode
	*	����������	HTREEITEM hCurItem : [in] Ҫ���������ڵ�
	*				UINT nCode         : [in] CHECK״̬
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void Check(HTREEITEM hCurItem, UINT nCode);

	/******************************************************************
	*	�������ܣ�	��hTSelItem����������ĳ���hTSelItemһ����״̬��ͬʱ,�ı��丸�ڵ�״̬ ??? 
	*	����������	HTREEITEM hTSelItem : [in] Ҫ���������ڵ�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void Check(HTREEITEM hTSelItem);
	

	/******************************************************************
	*	�������ܣ�	hCurItem�����ӽڵ����Ƿ���CHECKED�Ľڵ����
	*	����������	HTREEITEM hCurItem : [in] Ҫ���������ڵ�
	*	�� �� ֵ��	BOOL : ��������ӽڵ��д���CHECKED�ڵ�,�򷵻�TRUE, ���򷵻�FALSE
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsSubCheck( HTREEITEM hCurItem );

	/******************************************************************
	*	�������ܣ�	�ӽڵ�ı�״̬��,ͬʱ�ı��丸�ڵ�״̬
	*	����������	HTREEITEM hCurItem : [in] �ӽڵ�
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void CheckParent( HTREEITEM hCurItem );

	/******************************************************************
	*	�������ܣ�	�鿴���ֵܽڵ��CHECK״̬
	*	����������	HTREEITEM hCurItem : [in] Ҫ�鿴�Ľڵ�
	*				int& nCount		   : [in] �ܽڵ���
	*				int& nCheckCount   : [in] CHECKED�Ľڵ���
	*				int& nWaitCheck	   : [in] ����CHECKED�Ľڵ���
	*	�� �� ֵ��	BOOL : (int)���ǽڵ������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL IsSiblingItemCheck(HTREEITEM hCurItem,int& nCount,int& nCheckCount,int& nWaitCheck);


	/******************************************************************
	*	�������ܣ�	����hItem��Check״̬
	*	����������	HTREEITEM hItem : [in] Ҫ�����ľ��
	*				BOOL fCheck     : [in] Ҫ���õ�״̬
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL TreeViewSetCheckState(HTREEITEM hItem, BOOL fCheck);
	/******************************************************************
	*	�������ܣ�	�õ�hItem��Check״̬
	*	����������	HTREEITEM hItem : [in] Ҫ�����ľ��
	*	�� �� ֵ��	BOOL : checked( TRUE ), ����(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL TreeViewGetCheckState( HTREEITEM hItem);

// export / import
public:
	/******************************************************************
	*	�������ܣ�	����ǰ��ʽ���ؼ�������н������Ӧ�Ĺ�ʽ����ȫ�ֹ�ʽ����
	*	����������	CDlgFormulaMan* pFormulaMan : [in] ��ʽ����������
	*				WORD wStyle				 : [in] ��ʽ���뷽ʽ,EXTERN_EXPRESS_OVR�򸲸����й�ʽ
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void LoadTree(CDlgFormulaMan* pFormulaMan,WORD wStyle);

	/******************************************************************
	*	�������ܣ�	���ڵ�hRootItem�����нڵ�����Ӧ�Ĺ�ʽ���뵽ȫ�ֹ�ʽ����(ע��,�ڵ㱾��������)
	*	����������	CDlgFormulaMan* pFormulaMan : [in] ��ʽ����������
	*				WORD wStyle				 : [in] ��ʽ���뷽ʽ,EXTERN_EXPRESS_OVR�򸲸����й�ʽ
	*				HTREEITEM hRootItem		 : [in] Ҫ����ĸ����
	*				LoadItemData* status	 : [in] �м����,�����õ��ǹ�ʽ��������
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void Load(CDlgFormulaMan* pFormulaMan,WORD wStyle,HTREEITEM hRootItem,LoadItemData* status);

public:
	/******************************************************************
	*	�������ܣ�	չ����ѡ���ڽڵ�hChildItem�°����Ĺ�ʽ������CExpression::m_pExternExpression���е���Ŀ
	*	����������	DWORD dOperator	: [in] ����ΪEXTERN_EXPRESS_SAME_NAME,���򲻲���
	*				HTREEITEM hChildItem = NULL : [in] Ҫ����ĸ����, ΪNULLʱָ����ڵ�
	*	�� �� ֵ��	long : �ɹ��򷵻�EXTERN_EXPRESS_SAME_NAME, ���򷵻�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	long OperatorTree(DWORD dOperator,HTREEITEM hChildItem = NULL);

	/******************************************************************
	*	�������ܣ�	��hitemDrag����������Ŀ�ƶ���hitemDrop��
					ע��, �ƶ���,Դ�ڵ�����ڽ���ȥ���е������ɾ��
					��:
						TransferItem(hItem, hItemTO);
						DeleteItem(hItem);
					��������һ���������ƶ�����
	*	����������	HTREEITEM hitemDrag : [in] Դ���ڵ�
	*				HTREEITEM hitemDrop : [in] Ŀ����ڵ�
	*	�� �� ֵ��	BOOL : �ƶ��ɹ�TRUE, ����ΪFALSE 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL		 TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop);

	/******************************************************************
	*	�������ܣ�	��Ҷ�ӽڵ�hitemDrag�ƶ���
					��ڵ�hitemDrop(Ϊ��ʱ,��ʾ�ƶ����ڵ���ΪstrInsertName����ڵ���)��
	*	����������	HTREEITEM hitemDrag : [in] Ҫ�ƶ���Ҷ��
	*				HTREEITEM hitemDrop : [in] Ŀ����ڵ�
	*				CString strInsertName = "" : [in] Ŀ����ڵ���
	*				DWORD dwMask = TVE_EXPAND  : [in] ���ΪTVE_EXPAND���ƶ���ɺ�չ���ƶ���Ľڵ�
	*	�� �� ֵ��	BOOL : �ƶ��ɹ�TRUE,����ΪFALSE
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL	     ChangItemToGroup(HTREEITEM hitemDrag, HTREEITEM hitemDrop,
								  CString strInsertName = "",
								  DWORD dwMask = TVE_EXPAND);
// drag/drop
protected:

	/******************************************************************
	*	�������ܣ�	���������º���,��Ҫ���ϷŴ���
	*	����������	UINT nFlags	 : [in] ���±�־
	*				CPoint point : [in] �����¼�����ʱ���ָ������λ��
	*	�� �� ֵ��	BOOL : ����ʧ��(FALSE), �ɹ�(TRUE)		 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL		 MouseDown(UINT nFlags, CPoint point);

	/******************************************************************
	*	�������ܣ�	hitemSuspectedParent�Ƿ�ΪhitemChild������
	*	����������	HTREEITEM hitemChild	: [in] �ӽڵ�
	*				HTREEITEM hitemSuspectedParent : [in] ��ڵ�
	*	�� �� ֵ��	BOOL : hitemSuspectedParent��hitemChild�������򷵻�TRUE, ����FALSE
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	BOOL		 IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);

// find
public:

	/******************************************************************
	*	�������ܣ�	ѡ�в���չ����ʽpExpression���ڵ���ڵ�
	*	����������	CExpression* pExpression : [in] Ҫչ���Ĺ�ʽ
	*				DWORD dMask : [in] TVE_EXPAND��չ��,����չ��
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void		Show(CExpression* pExpression,DWORD dMask); // show express

	/******************************************************************
	*	�������ܣ�	ѡ�в���չ����ʽpExpression���ڵ���ڵ�, ������������
	*	����������	CExpression* pExpression : [in] Ҫչ���Ĺ�ʽ
	*				DWORD dMask = 0 : [in] TVE_EXPAND��չ��, ����չ��
	*	�� �� ֵ��	CTreeGroup* : ��Ŀ����(������ʽ��һЩ������������)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	CTreeGroup*	Find(CExpression* pExpression,DWORD dMask = 0); // find express

	/******************************************************************
	*	�������ܣ�	�õ���ڵ���ΪstrGroupName�Ľڵ���,����ýڵ㲻����,�����
	*	����������	CString strGroupName : [in] ��ڵ���
	*				bool bPrompt = TRUE	 : [in] ����
	*	�� �� ֵ��	HTREEITEM : NULL�ü���ʧ��, ����Ϊ������ҵ��Ľڵ���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	HTREEITEM   NewGroup(CString strGroupName, bool bPrompt = TRUE);

public:
	// �õ���ǰ��ʽ���������Ĺ�ʽ����(��16λ),���Ƿ��л����湫ʽ(��16λ)
//	long GetExpressType();
	long GetHasExpressType(long* pJiben = NULL,long* pUnion = NULL);

	/******************************************************************
	*	�������ܣ�	��ʼ������ʽ(�ӱ��ͷ�����еõ�����ʽ����ID)
	*	����������	CString strName : [in] ����ʽ��
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��8��
	*	�޸�˵����	
	*****************************************************************/
	void InitCWData(CString strName);

	BOOL IsEditExpress(HTREEITEM hHitItem);

	// gbq add, ��鹫ʽ�Ƿ���Ҫ����
	BOOL ExPassword(HTREEITEM hItem);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HXTREECTRL_H__D334AF88_CFC7_11D4_903D_5254AB285D5A__INCLUDED_)
