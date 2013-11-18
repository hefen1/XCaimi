/*******************************************************************************
 * Copyright (c)2003, ���ݺ�����Ϣ�������޹�˾
 * All rights reserved.
 *
 * �ļ����ƣ�Memdc.h
 * �ļ���ʶ���ڴ�DC�ඨ�弰��ʵ��
 * ժ    Ҫ���ڴ�DC�ඨ�弰��ʵ��
 *
 * ��ǰ�汾��
 * ��    �ߣ�
 * ������ڣ�
 *******************************************************************************/
#if !defined(ST_USE_MEMDC)
#define ST_USE_MEMDC

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMemDC : public CDC
{
public:
    // constructor sets up the memory DC
    CMemDC(CDC* pDC);
    
    // Destructor copies the contents of the mem DC to the original DC
    ~CMemDC();

    // Allow usage as a pointer
    CMemDC* operator->();
        
    // Allow usage as a pointer
    operator CMemDC*();

private:
	// Offscreen bitmap
    CBitmap  m_bitmap;      
	// bitmap originally found in CMemDC
    CBitmap* m_pOldBitmap;  
	// Saves CDC passed in constructor
    CDC*     m_pDC;         
	// Rectangle of drawing area.
    CRect    m_rect;        
	// TRUE if CDC really is a Memory DC.
    BOOL     m_bMemDC;      
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(ST_USE_MEMDC)
