#pragma once
#include "ConfigInterface.h"


class CBlockManager :
	public IBlockManager
{
public:
	BOOL IsInit();
	BOOL Init();
	long LoadBlock(IN CString strBlockName);
	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

	CBlockManager(void);
	~CBlockManager(void);

	// 板块操作
	long	GetAllBlockGroupName(OUT CStringArray& ayBlockGroupName);	// 获取所有板块分类名称
	long	GetAllSysBlockGroupName(OUT CStringArray& aySysBlockGroupName);
	long	GetAllUserBlockGroupName(OUT CStringArray& ayUserBlockGroupName);
	long	GetBlockName(IN CString& strBlockGroupName, OUT CStringArray& ayBlockName);	// 获取指定板块分类的所有板块名称
	long	GetAllBlockName(OUT CBlockGroupArray& ayBlockName);			// 获取所有板块名称

	// 获取成分股信息  
	long	GetStockList(IN CString strBlockName, OUT CArray<CodeInfo>& ayStockList);	// 获取指定板块的成分股列表
	long    GetStockList(IN CString strBlockName, OUT CMapStringToPtr& mapStockList);   // 获取指定板块的成分股列表 key - code, value - null
	long	GetStockListCount(IN CString strBlockName);		// 获取指定板块成分股个数
	long	GetStockAdscription(IN CodeInfo* pCode,OUT CStringArray& ayBlockName);		// 查找所属板块

	// 操作成分股列表
	BOOL	AddStockToBlock(IN CString strBlockName, IN CodeInfo* code);		// 添加一个成分股到板块末尾
	BOOL	InsertStockToBlock(IN CString strBlockName, IN CodeInfo* code, IN long nIndex);	// 插入一个成分股到指定板块的指定位置
	long	DelStockFromBlock(IN CString strBlockName, IN CodeInfo* code);	// 从指定板块删除指定成分股

protected:
	BOOL m_bInit;
	CBlockGroupArray	m_aySysBlockGroup;		// 系统板块
	CBlockGroupArray	m_ayUserBlockGroup;		// 用户定义板块
	CBlockInfoMap		m_mapBlockInfo;			// 板块索引
	CMapStringToPtr		m_mapModifiedBlock;		// 已更改板块名
};
