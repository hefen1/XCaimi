#pragma once
#pragma warning(disable:4996)
#include <afxmt.h>

// font style
#define FONT_NORMAL				0x01  // ����
#define FONT_BOLD				0x02  // ����
#define FONT_ITALIC				0x04  // б��
#define FONT_UNDERLINE			0x08  // �»���

// ����ת����ʽ
#define LTOA_MASK_DIVISION_NOROUND		0x0010			// ����Ҫ��������
#define LTOA_MASK_DIVISION_CARRY		0x0020			// С����λ
#define LTOA_MASK_DIVISION_ROUND		0x0040			// ��������
#define LTOA_MASK_DIVISION_TRUNCATION	0x0080			// С����ȥ

#define LTOA_MASK_SHOW_PLUS			0x0100			// ��ʾ'+'��
#define LTOA_MASK_SHOW_SUB			0x0200			// ��ʾ'-'��
#define LTOA_MASK_SHOW_PERSENT		0x0400			// ��ʾ�ٷֺţ����Ϊ��Ч���֣�
#define LTOA_MASK_SHOWEX_UNITAUTO   0x0001          // ��ʾ��λ(�Զ�) ��:�ڡ���
#define LTOA_MASK_SHOWEX_UNITYI     0x0002          // ��ʾ��λ(ֻ��ʾ��)
#define LTOA_MASK_SHOWEX_UNITWAN    0x0004          // ��ʾ��λ(ֻ��ʾ��)

#define LTOA_MASK_NOTHING_IFZERO	0x1000			// ������Ϊ 0 ʱ��ת��Ϊһ���ո���ַ���
#define LTOA_MASK_HORLINE_IFZERO	0x2000			// ������Ϊ 0 ʱ��ת��Ϊһ�� " - "���ַ���
#define	LTOA_MASK_FIXEDLEN_IFZERO	0x4000			// ������Ϊ 0 ʱ��ת��Ϊһ���̶����ȵ�0����С����Ϊ00000.000...��

#define LTOA_MASK_DEFAULT	LTOA_MASK_DIVISION_ROUND


#define OneHundredMillion  100000000
#define TenThousand        10000
class CMutexHelper
{
private:
	CMutex* m_pMutex;
public:
	CMutexHelper(CMutex *pMutex)
	{
		ASSERT(pMutex != NULL);
		m_pMutex = pMutex;
		m_pMutex->Lock();
	}

	~CMutexHelper()
	{
		m_pMutex->Unlock();
	}
};

class CGeneralHelper
{
public:
	enum PathFlag
	{
		SYSCONFIG_PATH,
		FACE_PATH,
		SETTING_PATH,
		BLOCK_PATH,
		DATA_PATH,
		ZIXUN_PATH
	};
public:
	CGeneralHelper(void);
	~CGeneralHelper(void);

	static CString GetAppPath();
	static CString GetSpecifyPath(PathFlag flag);
	static void GetEnvironmentString(CStringArray &envArr);
	static CString GetEnvironmentString();

	static long long GetDivision(long long x, long y, int nRoundType = 0);

	static void LongToString(double lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask = 0);
	static void LongToString(unsigned long lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask = 0);
	static void LongToString(double long lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask = 0);
	static void LongToString(long lData, long lClose, long lUnit, short nDecimal, char* strText,short lMask = 0);
	
	static CTime	CompToAbs(long lDisc, long lRefDate = 0);	// ��Ե�ʱ�䣨�죩���������ԵĲ�������ȥ�м�������죩
	static CTime    LongToDate(long lDate, long lRefDate = 0); //--2010.11.12 litao����2012ʱ������

	static  CString DoubleToString( double lData, int nDecimal = 0 );
	static  long    StringToLong( char* strText, long lClose, long lUnit,short nDecimal );

	static long StringToLongSimple(const char *pBuffer); //��pBufferָ��������ַ���ת��Ϊlong��



	/********************************************************************************
	* �������� : ���ݰٷֱ�������������ڵ���ɫֵ
	* �������� : COLORREF colSrc - Դ��ɫֵ
	*			 COLORREF colDes - Ŀ����ɫֵ , 
	*			 int nPersent : Դ��ɫֵ��Ŀ����ɫֵ��ƫ�ưٷֱ�
	*  ����ֵ  : COLORREF �õ�����ɫֵ
	* �������� : 
	* ��    �� : 
	* ������� : 
	* �޸ļ�¼ : 
	*******************************************************************************/
	static COLORREF DelutionColor(COLORREF colSrc, COLORREF colDes, int nPersent);

	//���ַ���strResource���ַ�strDelimit�ָ��ɶ��CString������������ayRet��
	static int  AnalyseStr(CString strResource,const char* strDelimit,CStringArray& ayRet);

		/********************************************************************************
	 * �������� : ����������, ��һԲΪ��׼,����������
	 * �������� : HDC hDC - ��ͼ�豸��� ��NULL 
	 *			  CPoint ptOrg - Բ�� 
	 *			  int iRadius - �뾶 
	 *			  BOOL bUp - ��ͷ�ϻ�����
	 *  ����ֵ  : �ɹ�����TRUE, ʧ�ܷ���FALSE
	 * �������� : 
	 * ��    �� :
	 * ������� :
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	static int DrawTriangle(HDC hDC, CPoint ptOrg, int iRadius, BOOL bUp);


	//	����ָ����������Ϣ��������
	static CFont*   CreateFont(char* cFontName,int nFontSize, int FontStyle);
	static CFont*   CreateFont(char * cFontName, int nFontHeight,  int nFontWeight,int nFontItalic);

	//  ����ָ����������Ϣ�������� 
	static CFont* CreateFontEx(char * cFontName, int nFontHeight,  int nFontWeight,int nFontItalic, 
								BYTE nOutPre, BYTE nPitch,BYTE nCharset);


		/********************************************************************************
	 * �������� : ���Ƽ�ͷ���߷����
	 * �������� : CHSBaseDC *pDC - ��ͼ�豸ָ�� ��NULL
	 *			  CRect rect - ����,
	 *			  COLORREF lColor - ��ɫ,
	 *			  BOOL bUp - ��16λ:��0��ʾ����,0��ʾ����; ��16λ:��״,0:���� 4:�����
	 *  ����ֵ  : void
	 * �������� : 
	 * ��    �� :
	 * ������� :
	 * �޸ļ�¼ : 
	 *******************************************************************************/
	static void DrawArrow(CDC * pDC,CRect rect,COLORREF lColor,BOOL bUp);

	static void GetListImage(CImageList& ImageList, CBitmap& bitmap, int nImageIndex);
	static void ScaleBitmap(CBitmap *pBitmap, CBitmap &BitmapNew, int nWidth, int nHeight);

	static BOOL ReadConfigSectionEntry(const char* strSection, CStringArray& strArray,const char* strFileName = NULL);

	/******************************************************************
	*	�������ܣ�	��ȡָ������ָ�����Ƶ��ַ�����ֵ
	*	����������	const char* strSection	��ָ���Ķ���
	*				const char* strEntry	������ȡ�ļ�������
	*				CString& strItem		����������ȡ����ֵ�Ļ���
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL 					�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	static BOOL ReadConfigString(const char* strSection,const char* strEntry,CString& strItem,const char* strFileName = NULL);

	/******************************************************************
	*	�������ܣ�	��ȡָ������ָ�����Ƶ����ͼ�ֵ
	*	����������	const char* strSection	��ָ���Ķ���
	*				const char* strEntry	������ȡ�ļ�������
	*				int& nValue				����������ȡ����ֵ
	*				const char* strFileName ������д���ļ�����
	*	�� �� ֵ��	BOOL 					�������ɹ�����TRUE��ʧ��ʱ����FALSE��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003-4-27
	*	�޸�˵����	
	*****************************************************************/
	static BOOL ReadConfigInt(const char* strSection,const char* strEntry,int& nValue,const char* strFileName = NULL);
	//����ת������������
	static CString ConvertChinaOfWeek(int lDate);
	static CString GetWeekDayString(unsigned long lDate);//2010.11.09 litao�����ж�����Ϊ���ڼ����ж�

};
