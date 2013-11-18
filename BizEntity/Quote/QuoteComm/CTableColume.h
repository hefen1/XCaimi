#include "gridctrl\gridcellbase.h"
#include "..\..\..\HelperLib\GeneralHelper.h"
#include "ConfigInterface.h"
#pragma once
// for test
//	查找操作的类型
enum FindType	
{
	none = 0x00, 
	SI_CODE = 0x01,	//	按照股票代码进行查找
	SI_NAME = 0x02,	//	按照股票名称进行查找
	SI_PYJC = 0x04, //	按照拼音简称进行查找
	SI_FOREX= 0x10	//  找不到代码不赋000001股票
};


/*类 用于管理表格列等*/
class CMapWordToPtrEx;
class CTableColume : public GV_ITEM
{
public:
	//列掩码	列风格
	enum { NewColMask = 0x1,NewColFormat = 0x2 };
	CTableColume();
	CTableColume(WORD dID);
	~CTableColume();

	//复制数据
	CTableColume* operator=(CTableColume* pCol);


	BYTE	m_bDelete;					//掩码,标注被清除的内容, 取值:NewColMask, NewColFormat
	WORD	m_dID;						//列ID
	TableColProperty* m_pProperty;		//列属性

	int m_nTempWidth;					// 临时宽度

protected:
	//字段大类哈希表
	static CMapStringToPtr 		 m_mapList;
	static COLORREF*			m_colTest;	//for testing

public:
	//根据掩码, 删除mask, nFormat内存
	void Delete();

	//将取得m_dID列的属性指针, 失败返回NULL
	TableColProperty* GetColProperty();
	//取得列名称, 如果失败, 返回:"无"
	CString				  GetName();
	//取得列掩码以及风格指针
	UINT				  GetRowMask();
	UINT				  GetRowFormat();
	//设置列掩码以及风格	bRow  - 是否为列
	void SetRowMask(UINT nMask,BOOL bRow = TRUE);
	void SetRowFormat(UINT nFormatRef,BOOL bRow = TRUE);

	//取得,设置列宽度	nWidth - 字符个数 * 2
	void SetColWidth(int nWidth);
	int  GetColWidth();

	//新分配一个字段分类哈希表, 返回指针
	static CMapWordToPtrEx*		GetMapColName();
	//在字段大类哈希表中,查找字段分类哈希表, 失败返回NULL, CString strName - KEY
	static CMapWordToPtrEx*		GetMap(CString strName);
	//在字段大类哈希表中,查找字段分类哈希表, 失败返回NULL, WORD key - KEY
	static CMapWordToPtrEx*		GetMap(WORD key);
	//根据字段ID, 取得所属分类哈希表的KEY
	static CString				GetMapName(WORD key);
	//建立哈希表
	static void					InitColName();
	/********************************************************************************
	* 函数功能 : 字段属性读配置文件
	* 函数参数 : CWinApp* pApp - 未使用,
	*			 WORD dNameID - 字段ID,
	*			 CString& strName - 返回值,
	*			 CString strSection - 段名称
	*  返回值  : BOOL
	* 调用依赖 : 
	* 作    者 : 
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	static BOOL					GetColName(CWinApp* pApp,WORD dNameID,CString& strName,CString strSection);

	/********************************************************************************
	* 函数功能 : 字段属性写配置文件
	* 函数参数 : WORD wKey - 字段ID, 
	*			 TableColProperty* pCol - 属性指针 , 
	*			 LPCTSTR lpszSection 段名称,
	*			 LPCTSTR lpszFileName 文件
	*  返回值  : void
	* 调用依赖 : 
	* 作    者 : 
	* 完成日期 : 
	* 修改记录 : 
	*******************************************************************************/
	static void					WriteColName(WORD wKey, TableColProperty* pCol = NULL, LPCTSTR lpszSection = NULL,LPCTSTR lpszFileName = NULL);

	//删除字段分类哈希表, 并写配置文件	strName - 分类KEY
	static void					DelMapColName(CString strName);
	//查找字段是否存在	key - ID
	static BOOL			  	    Find(WORD key);
	//通过字段ID查找该字段属性或者列名, 失败返回NULL,"无"
	static TableColProperty*	GetByKey(WORD key);
	static CString				GetName(WORD key);
	//通过字段ID取得行掩码或者属性指针
	static UINT				GetRowMask(WORD key);
	static UINT				GetRowFormat(WORD key);
	//通过字段ID取得列掩码或者属性指针
	static UINT				GetColMask(WORD key);
	static UINT				GetColFormat(WORD key);

	// 返回字段大类哈希表指针
	static CMapStringToPtr*		GetList() { return &m_mapList; }
	//添加字段分类哈希表		CString strName - KEY ,CMapWordToPtrEx* pValue - 分类哈希表指针
	static void					AddToList(CString strName,CMapWordToPtrEx* pValue);
	//删除字段大类哈希表 字段配置写配置文件
	static void					DelList();
	//删除一字段 写配置文件 未使用
	static void					DelColumn(WORD key);

	//通过字段ID取相关数据	
	static CString				GetNameByColID(WORD dID);		//列名
	static COLORREF				GetBkClrByColID(WORD dID);		//背景色
	static COLORREF				GetFgClrByColID(WORD dID);		//前景色
	static UINT					GetStyleByColID(WORD dID);		//风格
	static UINT					GetDataTypeByColID(WORD dID);	//数据类型
};

class CMapWordToPtrEx : public CMapWordToPtr
{
protected:
	/*类型风格	高16位,基本风格	定义:MPEXS系列 按位与或	*
	*	8 - 15位,类型,	定义:MPEXT系列 比较值
	*	0 - 7位,保留*/
	DWORD m_dwStyle;
public:
	CMapWordToPtrEx()
	{
		m_dwStyle = MAKELONG( 0, MPEXS_RWCONFIG);		
	}
public:
	//设置哈希表
	void SetAt(WORD key, void* newValue,CWinApp* pApp,CString& strName,CString& strSection)
	{
		TableColProperty* pCol = (TableColProperty*)newValue;

		if(pCol != NULL)
		{
			if( IsStyle(MPEXS_RWCONFIG) && 
				CTableColume::GetColName(pApp,key,strName,strSection) )
			{
				CStringArray ayRet;
				if(CGeneralHelper::AnalyseStr(strName,";",ayRet) >= 7)
				{
					if( !IsStyle(MPEXS_COLNAME_NORWCFG) )
					{
						pCol->m_strName.Format("%s",ayRet[0]);
					}
					//pCol->m_nRowMask   = atol(ayRet[1]);
					//pCol->m_nRowFormat = atol(ayRet[2]);
					pCol->m_nColMask   = atol(ayRet[3]);
					pCol->m_nColFormat = atol(ayRet[4]);
					pCol->m_TextColor  = atol(ayRet[5]);
					pCol->m_cWidth     = (char)atol(ayRet[6]);
					pCol->m_cConfigWidth = pCol->m_cWidth;
				}
				else
				{
					pCol->m_strName.LoadString(atoi(pCol->m_strName));
				}
			}
			else
			{
				pCol->m_strName.LoadString(atoi(pCol->m_strName));
			}
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
	void SetAt(WORD key, void* newValue)
	{
		void* pValue;
		if( CMapWordToPtr::Lookup(key,pValue) ) // 已经存在
		{
			delete (CTableColume*)pValue;
			RemoveKey(key);
			return;
		}
		CMapWordToPtr::SetAt(key,newValue);
	}
public:
	//判断风格
	BOOL IsStyle(WORD wStyle){return wStyle & HIWORD(m_dwStyle);}
	//设置风格
	void ModifyStyle(WORD wAddStyle, WORD wRemoveStyle)
	{
		m_dwStyle |= MAKELONG(0, wAddStyle);
		m_dwStyle &= ~ MAKELONG(0, wRemoveStyle);
	}
public:
	//取得或者设置类型
	BYTE GetType(){ return HIBYTE( LOWORD(m_dwStyle) );}
	void SetType(BYTE tType)
	{
		m_dwStyle &= 0xFFFF00FF;
		m_dwStyle |= MAKELONG(MAKEWORD(0, tType), 0);
	} 
public:
	//保留
	BYTE GetReserve(){ return LOBYTE( LOWORD(m_dwStyle) );}
	void SetReserve(BYTE tType)
	{
		m_dwStyle &= 0xFFFFFF00;
		m_dwStyle |= MAKELONG(MAKEWORD(tType, 0), 0);
	} 

};
