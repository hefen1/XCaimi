#pragma once
#pragma warning(disable:4996)
#include <afxmt.h>

// font style
#define FONT_NORMAL				0x01  // 正常
#define FONT_BOLD				0x02  // 粗体
#define FONT_ITALIC				0x04  // 斜体
#define FONT_UNDERLINE			0x08  // 下划线

// 文字转换方式
#define LTOA_MASK_DIVISION_NOROUND		0x0010			// 不需要四舍五入
#define LTOA_MASK_DIVISION_CARRY		0x0020			// 小数进位
#define LTOA_MASK_DIVISION_ROUND		0x0040			// 四舍五入
#define LTOA_MASK_DIVISION_TRUNCATION	0x0080			// 小数舍去

#define LTOA_MASK_SHOW_PLUS			0x0100			// 显示'+'号
#define LTOA_MASK_SHOW_SUB			0x0200			// 显示'-'号
#define LTOA_MASK_SHOW_PERSENT		0x0400			// 显示百分号（如果为有效数字）
#define LTOA_MASK_SHOWEX_UNITAUTO   0x0001          // 显示单位(自动) 如:亿、万
#define LTOA_MASK_SHOWEX_UNITYI     0x0002          // 显示单位(只显示亿)
#define LTOA_MASK_SHOWEX_UNITWAN    0x0004          // 显示单位(只显示万)

#define LTOA_MASK_NOTHING_IFZERO	0x1000			// 当数字为 0 时，转换为一个空格的字符串
#define LTOA_MASK_HORLINE_IFZERO	0x2000			// 当数字为 0 时，转换为一个 " - "的字符串
#define	LTOA_MASK_FIXEDLEN_IFZERO	0x4000			// 当数字为 0 时，转换为一串固定长度的0（有小数则为00000.000...）

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
	
	static CTime	CompToAbs(long lDisc, long lRefDate = 0);	// 相对的时间（天）差数到绝对的差数（除去中间的星期天）
	static CTime    LongToDate(long lDate, long lRefDate = 0); //--2010.11.12 litao修正2012时间问题

	static  CString DoubleToString( double lData, int nDecimal = 0 );
	static  long    StringToLong( char* strText, long lClose, long lUnit,short nDecimal );

	static long StringToLongSimple(const char *pBuffer); //将pBuffer指向的数字字符串转换为long型



	/********************************************************************************
	* 函数功能 : 根据百分比求得两个区间内的颜色值
	* 函数参数 : COLORREF colSrc - 源颜色值
	*			 COLORREF colDes - 目标颜色值 , 
	*			 int nPersent : 源颜色值向目标颜色值的偏移百分比
	*  返回值  : COLORREF 得到的颜色值
	* 调用依赖 : 
	* 作    者 : 
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	static COLORREF DelutionColor(COLORREF colSrc, COLORREF colDes, int nPersent);

	//将字符串strResource以字符strDelimit分隔成多个CString串，并保存在ayRet中
	static int  AnalyseStr(CString strResource,const char* strDelimit,CStringArray& ayRet);

		/********************************************************************************
	 * 函数功能 : 绘制三角形, 以一圆为基准,绘制三角形
	 * 函数参数 : HDC hDC - 绘图设备句柄 非NULL 
	 *			  CPoint ptOrg - 圆心 
	 *			  int iRadius - 半径 
	 *			  BOOL bUp - 箭头上还是下
	 *  返回值  : 成功返回TRUE, 失败返回FALSE
	 * 调用依赖 : 
	 * 作    者 :
	 * 完成日期 :
	 * 修改记录 : 
	 *******************************************************************************/
	static int DrawTriangle(HDC hDC, CPoint ptOrg, int iRadius, BOOL bUp);


	//	根据指定的字体信息创建字体
	static CFont*   CreateFont(char* cFontName,int nFontSize, int FontStyle);
	static CFont*   CreateFont(char * cFontName, int nFontHeight,  int nFontWeight,int nFontItalic);

	//  根据指定的字体信息创建字体 
	static CFont* CreateFontEx(char * cFontName, int nFontHeight,  int nFontWeight,int nFontItalic, 
								BYTE nOutPre, BYTE nPitch,BYTE nCharset);


		/********************************************************************************
	 * 函数功能 : 绘制箭头或者方块等
	 * 函数参数 : CHSBaseDC *pDC - 绘图设备指针 非NULL
	 *			  CRect rect - 区域,
	 *			  COLORREF lColor - 颜色,
	 *			  BOOL bUp - 低16位:非0表示向下,0表示向上; 高16位:形状,0:三角 4:方块等
	 *  返回值  : void
	 * 调用依赖 : 
	 * 作    者 :
	 * 完成日期 :
	 * 修改记录 : 
	 *******************************************************************************/
	static void DrawArrow(CDC * pDC,CRect rect,COLORREF lColor,BOOL bUp);

	static void GetListImage(CImageList& ImageList, CBitmap& bitmap, int nImageIndex);
	static void ScaleBitmap(CBitmap *pBitmap, CBitmap &BitmapNew, int nWidth, int nHeight);

	static BOOL ReadConfigSectionEntry(const char* strSection, CStringArray& strArray,const char* strFileName = NULL);

	/******************************************************************
	*	函数功能：	读取指定段内指定名称的字符串键值
	*	函数参数：	const char* strSection	：指定的段名
	*				const char* strEntry	：待读取的键的名称
	*				CString& strItem		：保存所读取键的值的缓冲
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL 					：操作成功返回TRUE，失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	static BOOL ReadConfigString(const char* strSection,const char* strEntry,CString& strItem,const char* strFileName = NULL);

	/******************************************************************
	*	函数功能：	读取指定段内指定名称的整型键值
	*	函数参数：	const char* strSection	：指定的段名
	*				const char* strEntry	：待读取的键的名称
	*				int& nValue				：保存所读取键的值
	*				const char* strFileName ：待读写的文件名称
	*	返 回 值：	BOOL 					：操作成功返回TRUE，失败时返回FALSE。
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003-4-27
	*	修改说明：	
	*****************************************************************/
	static BOOL ReadConfigInt(const char* strSection,const char* strEntry,int& nValue,const char* strFileName = NULL);
	//日期转换成中文星期
	static CString ConvertChinaOfWeek(int lDate);
	static CString GetWeekDayString(unsigned long lDate);//2010.11.09 litao增加判断日期为星期几的判断

};
