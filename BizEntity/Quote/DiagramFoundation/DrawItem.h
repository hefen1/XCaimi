/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawItem.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  Item��
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
#include "DiagramFoundationDef.h"
#include "DrawTableStruct.h"
// #include <hash_map>
// #include <list>
// #include <map>

#define ITEM_TEXT_LEFT         0x0001  // �����
#define ITEM_TEXT_CENTER       0x0002  // ����
#define ITEM_TEXT_RIGHT        0x0004  // �Ҷ���
#define ITEM_TEXT_SELECT       0x0008  // �ı����ݿ��Ա�ѡ��

#define ITEM_TEXT_HIGHLIGHT    0x0010  // �ı�������ʾ
#define ITEM_TEXT_NOTRIM       0x0020  // �����ַ������ҿո� Ĭ��������ڻ���ʱ���ȥ���������߿ո�
#define ITEM_RESIZABLE         0x0040  // �����϶���߿�ı��С
#define ITEM_MOVABLE           0x0080  // ����ѡ���϶�λ��

// ��ͷֻ�����ı����ұ߳���
#define ITEM_ARROW_NEED        0x0100  // ��Ҫ��ʾ��ͷ ���ǻ���ȷ����ʾ�ĸ�
#define ITEM_ARROW_UP          0x0200  // ��ʾ���ϼ�ͷ
#define ITEM_ARROW_DOWN        0x0400  // ��ʾ���¼�ͷ
#define ITEM_ARROW_LEFT        0x0800  // ��ͷ��ʾ�����(Ĭ����ʾ���ұ�)

#define ITEM_ARROW_WIDTH           16  // ��ͷ���
#define ITEM_MIN_WIDTH             30  // Item��С���
#define ITEM_LEFT_MARGIN            5  // ���Ԥ���հ�
#define ITEM_RIGHT_MARGIN           5  // �ұ�Ԥ���հ�
//////////////////////////////////////////////////////////////////////////
// CDrawCellArray
class CDrawItem;
class CDrawItemArray : public CPtrArray
{
public:
	CDrawItem*& operator[](int nIndex) {return (CDrawItem*&)ElementAt(nIndex); }
	//free all elements
	void DeleteAll();
	~CDrawItemArray() { DeleteAll(); }
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ����CDrawItem�ڴ�
//typedef stdext::hash_map<int, CDrawItem*>  mapItemMem;
// class CDrawItemMemControl
// { 
// public:
// 	// �����ڴ�
// 	CDrawItemMemControl();
// 	~CDrawItemMemControl();
// 	CDrawItem* AllocMem(ItemConstructionStruct* pConstructionStruct);   
// 	CDrawItem* AllocMem(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);
// 	void FreeMem(CDrawItem* &pItem);    // �ͷ��ڴ�
// 
// private:
// 	CMapWordToPtr         m_mapAlloc;   // �Ѿ�������ڴ�
// //	mapItemMem            m_mapAlloc;
// 	std::list<CDrawItem*> m_lstIdle;    // �ͷų��Ŀ����ڴ�
// };
//////////////////////////////////////////////////////////////////////////
class CDrawItem
{
public:
	CDrawItem(ItemConstructionStruct* pConstructionStruct = NULL);
	CDrawItem(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);
	~CDrawItem(void);

	void  ReCreate(ItemConstructionStruct* pConstructionStruct);
	void  ReCreate(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);

	void  SetText(CString str, WORD wClr = 0, WORD wFont = 0, DWORD dwMask = 0, DWORD dwDelMask = 0 );
	void  SetText(const char* strText, WORD wClr = 0, WORD wFont = 0, DWORD wMask = 0, DWORD dwDelMask = 0);
	BOOL  DrawItem(CDC* pDC, CRect rect);

	void  Copy(CDrawItem* pItem); // �����ı������Լ�����  
	void  SetEmpty(); // ����ԪС���������

	//////////////////////////////////////////////////////////////////////////
	// 
// 	static CDrawItem*  Create(ItemConstructionStruct* pConstructionStruct = NULL);
// 	static CDrawItem*  Create(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent);
// 	static void  Free(CDrawItem* pItem);
	//////////////////////////////////////////////////////////////////////////
//private:
	char        m_str[64];  // �ı�����
	WORD	    m_wColor;   // ǰ��ɫ
	WORD	    m_wFont;    // ����
	DWORD	    m_dwStyle;  // ����ֵ(��: ���롢��ʾ�����Ρ�ѡ�С������϶��ı��С���ƶ���)
	CRect	    m_rectDraw; // ʵ�ʻ��Ƶ�������DrawItem����ִ��ʱ�������
	double     	m_dWidth;   // ��ռ�ٷֱ�
	DWORD       m_dwExtent; // ������չ�� 
// private:
// 	static CDrawItemMemControl m_memControl;  // �ڴ���ƶ���
};
