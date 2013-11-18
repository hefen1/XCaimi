/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	HSCompileInterFace.h
*	文件标识：	
*	摘	  要：	一些接口参数结构的说明
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
***************************************************************/

#if !defined(HSCOMPILEINTERFACE_H)
#define HSCOMPILEINTERFACE_H

#include "FormulaManPublic.h"

class CExpression;
class CHSDataSource;

//******************************以下是litao新增*****************************************
///编译执行时标识序列号结构
struct  tagCompRecord
{
	CString  strName;
	int  SerialNo;
};
typedef struct tagCompRecord  OBJNO;
typedef struct tagCompRecord* LPOBJNO;

//******************************以上是litao新增*****************************************

/*************************************************************
*	类 名 称：	nodElement
*	类 描 述：	一个操作单元(tree 叶)
***************************************************************/
struct nodElement
{
	struct nodElement* left;        // 操作符左边
	struct nodElement* right;       // 操作符右边
	unsigned char	   operatie;    // 操作符
	unsigned char	   cDataType;   // 当cDataType == DataTypeOperStr时，valoarestr 有效。	
	void*			   pData;       // 外部数据指针,此指针不能删除，由外部删除;
									// 当 operatie == Operator_Function 时，是函数信息指针;
	union
	{
		double   valoare;
		CString* valoarestr;		// 值(如指标名等)
	};
	
	// 初始化
	nodElement();
	// 清空占用资源
	~nodElement();

	// 数据COPY
	void		 Copy(const nodElement* pClone);
	virtual void Serialize(CExpression* pExpress,CArchive& ar);	
};
typedef struct nodElement  NODELEMENT;
typedef struct nodElement* LPNODELEMENT;

/*************************************************************
*	类 名 称：	FormulaGeneralParam
*	类 描 述：	通用参数
***************************************************************/
struct FormulaGeneralParam
{
	//
	CArray<LPNODELEMENT,LPNODELEMENT>* m_ayVars; // 参数数组指针

	LPNODELEMENT GetParam(int nPos);
	int			 GetSize();
	
	void		 Add(LPNODELEMENT pParam);
	void		 InsertAt(int nIndex,LPNODELEMENT pParam);
	
	CArray<LPNODELEMENT,LPNODELEMENT>* GetArray();
	
	FormulaGeneralParam();
	~FormulaGeneralParam();
};
//计算时传入外部参数
//struct CCalcExternParam
//{
//	long		m_nBeginDate; // 开始日期,如19901020
//	long	    m_nEndDate;   
//	CodeInfo    m_sCode;	  // 代码
//	char 		m_szBlock[BLOCK_NAME_LENGTH]; // 板块名称
//
//	CCalcExternParam()
//	{
//		EmptyBlock();
//	}
//
//	void    EmptyBlock() { memset(m_szBlock,0,sizeof(m_szBlock)); }
//	CString GetBlockName() { return CString(m_szBlock).Left(BLOCK_NAME_LENGTH); }
//	void    SetBlockName(CString pData)
//	{
//		EmptyBlock();
//		if( pData.IsEmpty() )
//			return;
//
//		strncpy(m_szBlock,pData,sizeof(m_szBlock));
//	}
//};
/*************************************************************
*	类 名 称：	HSCurDataInfoStatus
*	类 描 述：	计算中间变量结构
***************************************************************/
struct CCalcExternParam;
struct HSCurDataInfoStatus
{
	int				m_nDataPos;  // 数据位置
	HSDouble		m_lPreData;  // 前一计算数据
	DWORD			m_dwStatus;  // 计算状态
	
	HSDouble*		m_pData;	// 计算的结果
	long			m_lSize;	// 计算的长度

	CHSDataSource*	m_pDataSource; // 数据包装

	CCalcExternParam* m_pCalcParam; // 外部参数，本类不作删除

	HSCurDataInfoStatus(HSCurDataInfoStatus* pCurDataInfoStatus);
	HSCurDataInfoStatus(DWORD dwStatus = 0);
	~HSCurDataInfoStatus();
	void Empty();

	void SetStatus(DWORD dwStatus);
	BOOL IsStatus(DWORD dwStatus);
	void AddStatus(DWORD dwStatus);
	void RemoveStatus(DWORD dwStatus);

	void Alloc( int nCurSize,HSDouble defValues = hx_DefMinValues );
	void Free();

	// 得到连接后的字符串
	CString GetCatStr();
};


/*************************************************************
*	类 名 称：	CExpressionBase
*	类 描 述：	公式基类,没实现什么功能
***************************************************************/
class CExpressionBase : public CObject
{
	DECLARE_DYNCREATE(CExpressionBase)

public:
	CExpressionBase() {}
	~CExpressionBase() {}
public:
	virtual double vexp ( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus) { return hx_DefMinValues; }       // 计算
	virtual CHSDataSource* GetData() { return NULL; }
};

// 系统函数函数指针类型定义
typedef HSDouble ( *COMPILEFUNCTION)( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
									  HSCurDataInfoStatus* pCurDataStatus, int& nError );

#endif // HSCOMPILEINTERFACE_H