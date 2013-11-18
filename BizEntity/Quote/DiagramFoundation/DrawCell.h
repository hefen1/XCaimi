/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawCell.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Ԫ����
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-01-13
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DrawTableStruct.h"
#include "DrawItem.h"
#include "DiagramFoundationDef.h"

// ��Ԫ������
#define CELL_BORDER_ENY        0x000F  // ȫ�߿�
#define CELL_BORDER_LEFT       0x0001  // ��߿�
#define CELL_BORDER_RIGHT      0x0002  // �ұ߿�
#define CELL_BORDER_TOP        0x0004  // �ϱ߿�
#define CELL_BORDER_BOTTOM     0x0008  // �±߿�

#define CELL_RESIZABLEWE       0x0010  // �����϶���߿�ı䵥Ԫ���С
#define CELL_MOVABLE           0x0020  // ����ѡ���϶�λ��
#define CELL_BK_HIGHLIGHT      0x0040  // ��������
#define CELL_TIP               0x0080  // ��ʾTip

#define CELL_MIN_WIDTH             40  // ��Ԫ����С���
//////////////////////////////////////////////////////////////////////////
// CDrawCellArray
class CDrawCell;
class CDrawCellArray : public CPtrArray
{
public:
	CDrawCell*& operator[](int nIndex) {return (CDrawCell*&)ElementAt(nIndex); }
	//free all elements
	void DeleteAll();
	~CDrawCellArray() { DeleteAll(); }
};
//////////////////////////////////////////////////////////////////////////

class CDrawTableBase; // ������ǰ������ ��Ϊ��������໥����ͷ�ļ������
class CDrawCell
{
public:
	CDrawCell(CArray<ItemConstructionStruct>* ayItemStruct, DWORD dwCellMask = 0);
	CDrawCell(CString str, WORD lClr, WORD lFont, DWORD nMask = 0, DWORD dwCellMask = 0, DWORD dwExtent = 0); // ����Ԫ��ֻ��һ�е����
	~CDrawCell(void);
	void  ReadCfg();  // �������ļ�
	void  ReCreate(CArray<ItemConstructionStruct>* pAyItemStruct, DWORD dwCellMask = 0);  // ���������ع���Ԫ��
	void  ReCreate(CString str, WORD lClr, WORD lFont, DWORD nMask = 0, DWORD dwCellMask = 0, DWORD dwExtent = 0);

	BOOL  DrawCell(CDC* pDC, CRect rect, CPoint pos, CDrawTableBase* pTableBase);   // ����Ԫ��
	BOOL  DrawBoard(CDC* pDC, CRect rect);  // ���߿�
	void  SetItemText(int nItemID, const char* strText, WORD lClr = 0, WORD lFont = 0, DWORD lMark = 0, DWORD lDelMark = 0); // ���ø�ʽ���ı�
	int   GetItemSignal(CPoint point, int nCellWidth, int nLeft);  // ����ƶ� ��Ҫ�ǲ�����Ԫ���ڵ�Item 
	int   GetItemID(CPoint point, int nCellWidth,  int nLeft, int &nDiff);  // �жϵ�ǰ��������ĸ�Item, ���ҷ��ؾ�����߿�ĳ��� 
	CRect GetItemRect(CRect rectCell, int nItemID);   // ����Ԫ��ľ����Լ�ItemID �õ�Item�ľ���

	BOOL  GetItemStyle(int nItemID, ItemConstructionStruct& sConstructionStruct); // ����ԪС��С�� �õ���ԪС������
	BOOL  GetItemStyle(CArray<ItemConstructionStruct>& ayItemStruct); // ��ȡ���е�ԪС������
	
	// ��������λ��
	void    ReSize(int nItemLeft, int nItemRight, int nCellWidth, int nDiff);
	// ��ԭItem��״̬(��:ȥ������)
	void    ResetItem();
	// ���д���ʼλ���ƶ���Ŀ��λ��
	void    Move(int nBegin, int nEnd);
	// ����Ҫ�ƶ�ʱ ����϶��ľ���
	CRect   GetDrawRect(CPoint pos, int nCellWidth, int nCellHeight);
	// ����Item���ı�����, ���-1��ʾ����ȫ����
	CString GetItemText(int nItemID = -1);
	// ͨ����ʾ���ݷ���Item�±� 
	int     GetItemID(CString str);
	// ���Ƶ�Ԫ���ı�����
	void    Copy(CDrawCell* pCell);
	// ����Ԫ���������
	void    SetEmpty(); 

	CDrawItemArray m_ayItem;     // ����Item����
	WORD		m_wSignal;       // ��굱ǰλ�ñ�־
	int			m_nItemID;       // ��굱ǰ���ڵ�Item
	
	DWORD		m_dwStyle;		 // ��Ԫ������
	HsLOGPEN	m_logPen;		 // ���ʽṹ
};
