/*******************************************************************************
 * Copyright (c)2003, 杭州恒生信息技术有限公司
 * All rights reserved.
 *
 * 文件名称：Memdc.h
 * 文件标识：内存DC类定义及其实现
 * 摘    要：内存DC类定义及其实现
 *
 * 当前版本：2.0
 * 作    者：俞礼石
 * 完成日期：
 *
 * 取代版本：
 * 原 作 者：俞礼石
 * 完成日期：
 * 备	 注：
 *******************************************************************************/
#if !defined(ST_USE_MEMDC)
#define ST_USE_MEMDC

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMemdc : public CDC
{
public:
    // constructor sets up the memory DC
    CMemdc(CDC* pDC) : CDC()
    {				
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
        // Create a Memory DC      
        if (m_bMemDC)    
        {
            pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else        // Make a copy of the relevent parts of the current DC for printing
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    
    // Destructor copies the contents of the mem DC to the original DC
    ~CMemdc()
    {
        if (m_bMemDC) 
        {    
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);

            //Swap back the original bitmap.
            SelectObject(m_pOldBitmap);
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.
            m_hDC = m_hAttribDC = NULL;
        }
    }

    // Allow usage as a pointer
    CMemdc* operator->() {return this;}
        
    // Allow usage as a pointer
    operator CMemdc*() {return this;}

private:
	// Offscreen bitmap
    CBitmap  m_bitmap;      
	// bitmap originally found in CMemdc
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
