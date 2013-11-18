#include "StdAfx.h"
#include "BlockManager.h"
#include "..\..\Resource\QuoteLang\Resource.h"
#include "QuoteLangMngInterFace.h"
#include "WinnerApplication.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBlockManager::CBlockManager(void)
{
	m_bInit = FALSE;
}

CBlockManager::~CBlockManager(void)
{
	Release();
}

BOOL CBlockManager::Release(BOOL bSaveToFile /*= TRUE*/)
{
	if (bSaveToFile)
	{
		SaveToFile();
	}
	for (int i=0; i<m_aySysBlockGroup.GetCount(); i++)
	{
		BlockGroup* pGroup = m_aySysBlockGroup.GetAt(i);
		if (pGroup && !IsBadReadPtr(pGroup,1))
		{
			for (int j=0; j<pGroup->ayBlockList.GetCount(); j++)
			{
				BlockInfo* pBlock = pGroup->ayBlockList.GetAt(j);
				if (pBlock && !IsBadReadPtr(pBlock,1))
				{
					//for (int k=0; k<pBlock->ayStockList.GetCount(); k++)
					//{
					//	CodeInfo* pCode = pBlock->ayStockList.GetAt(k);
					//	if (pCode && !IsBadReadPtr(pCode,1))
					//	{
					//		delete pCode;
					//	}
					//}
					pBlock->ayStockList.RemoveAll();
					delete pBlock;
				}
			}
			pGroup->ayBlockList.RemoveAll();
			delete pGroup;
		}
	}
	m_aySysBlockGroup.RemoveAll();

	for (int i=0; i<m_ayUserBlockGroup.GetCount(); i++)
	{
		BlockGroup* pGroup = m_ayUserBlockGroup.GetAt(i);
		if (pGroup && !IsBadReadPtr(pGroup,1))
		{
			for (int j=0; j<pGroup->ayBlockList.GetCount(); j++)
			{
				BlockInfo* pBlock = pGroup->ayBlockList.GetAt(j);
				if (pBlock && !IsBadReadPtr(pBlock,1))
				{
					//for (int k=0; k<pBlock->ayStockList.GetCount(); k++)
					//{
					//	CodeInfo* pCode = pBlock->ayStockList.GetAt(k);
					//	if (pCode && !IsBadReadPtr(pCode,1))
					//	{
					//		delete pCode;
					//	}
					//}
					pBlock->ayStockList.RemoveAll();
					delete pBlock ;
				}
			}
			pGroup->ayBlockList.RemoveAll();
			delete pGroup;
		}
	}
	m_ayUserBlockGroup.RemoveAll();

	POSITION pos = m_mapBlockInfo.GetStartPosition();
	while(pos)
	{
		CString strName;
		BlockInfo* pBlock = NULL;
		m_mapBlockInfo.GetNextAssoc(pos,strName,pBlock);
		if (pBlock && !IsBadReadPtr(pBlock,1))
		{
			for (int i=0; i<pBlock->ayStockList.GetCount(); i++)
			{
				delete pBlock->ayStockList.GetAt(i);
			}
			pBlock->ayStockList.RemoveAll();
			delete pBlock;
		}
	}
	m_mapBlockInfo.RemoveAll();

	return TRUE;
}

BOOL CBlockManager::SaveToFile()
{
	CString strBlockName;
	BlockInfo* pBlockInfo = NULL;
	POSITION pos = m_mapModifiedBlock.GetStartPosition();
	while(pos)
	{
		m_mapModifiedBlock.GetNextAssoc(pos,strBlockName,(void*&)pBlockInfo);
		
		if (m_mapBlockInfo.Lookup(strBlockName,pBlockInfo) && pBlockInfo && !IsBadReadPtr(pBlockInfo,1))
		{
			CString strBlockFilePath;
			strBlockFilePath.Format("%sBlock\\%s.BLK",CGeneralHelper::GetAppPath(),strBlockName.GetString());

			CFile fp;
			if( fp.Open(strBlockFilePath, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary) )
			{
				long lMark = 888; // mark
				fp.Write( &lMark, sizeof(long) );
				for (int i=0; i<pBlockInfo->ayStockList.GetCount(); i++)
				{
					CodeInfo* pCode = pBlockInfo->ayStockList.GetAt(i);
					if (pCode)
					{
						fp.Write(pCode,sizeof(CodeInfo));
					}
				}
				fp.Close();
			}
			pBlockInfo = NULL;
		}
	}
	return TRUE;	
}
BOOL CBlockManager::IsInit()
{
	return m_bInit;
}

BOOL CBlockManager::Init()
{
	m_bInit = TRUE;
	CString strSysBlockFileName = CGeneralHelper::GetAppPath() + _T("Block\\block.def");
	CString strUserBlockFileName = CGeneralHelper::GetAppPath() + _T("Block\\userblock.def");

	CStringArray ayGroupName;
	if (!strSysBlockFileName.IsEmpty() && CGeneralHelper::ReadConfigSectionEntry(NULL,ayGroupName,strSysBlockFileName))
	{
		for (int i=0; i<ayGroupName.GetCount(); i++)
		{
			BlockGroup* pGroup = new BlockGroup;
			pGroup->strGroupName.Format("%s",ayGroupName.GetAt(i));
			CStringArray ayBlock;
			if (!pGroup->strGroupName.IsEmpty() 
				&& CGeneralHelper::ReadConfigSectionEntry(pGroup->strGroupName,ayBlock,strSysBlockFileName))
			{
				for (int j=0; j<ayBlock.GetCount(); j++)
				{
					BlockInfo* pBlockInfo = new BlockInfo;
					pBlockInfo->strBlockName.Format("%s",ayBlock.GetAt(j));
					pGroup->ayBlockList.Add(pBlockInfo);
					m_mapBlockInfo.SetAt(pBlockInfo->strBlockName,NULL);
				}
				m_aySysBlockGroup.Add(pGroup);
			}
			else
			{
				delete pGroup;
			}
		}
	}

	if (!strUserBlockFileName.IsEmpty() && CGeneralHelper::ReadConfigSectionEntry(NULL,ayGroupName,strUserBlockFileName))
	{
		for (int i=0; i<ayGroupName.GetCount(); i++)
		{
			BlockGroup* pGroup = new BlockGroup;
			pGroup->strGroupName.Format("%s",ayGroupName.GetAt(i));
			CStringArray ayBlock;
			if (!pGroup->strGroupName.IsEmpty() 
				&& CGeneralHelper::ReadConfigSectionEntry(pGroup->strGroupName,ayBlock,strUserBlockFileName))
			{
				for (int j=0; j<ayBlock.GetCount(); j++)
				{
					BlockInfo* pBlockInfo = new BlockInfo;
					pBlockInfo->strBlockName.Format("%s",ayBlock.GetAt(j));
					pGroup->ayBlockList.Add(pBlockInfo);
					m_mapBlockInfo.SetAt(pBlockInfo->strBlockName,NULL);
				}
				m_ayUserBlockGroup.Add(pGroup);
			}
			else
			{
				delete pGroup;
			}
		}
	}

	// 自选股特殊处理
	BlockInfo* pBlockInfo = NULL;
	CString strSelfBlock = ((IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG))->LoadStr(IDS_SELF_BLOCK);
	if (!m_mapBlockInfo.Lookup(strSelfBlock,pBlockInfo))
	{
		pBlockInfo = new BlockInfo;
		pBlockInfo->strBlockName = strSelfBlock;
		m_mapBlockInfo.SetAt(strSelfBlock,pBlockInfo);
		CString strTempPath;
		strTempPath.Format("%sBlock\\%s.BLK",CGeneralHelper::GetAppPath(),strSelfBlock);
		if (_access(strTempPath.GetString(),0) != 0)
		{
			m_mapModifiedBlock.SetAt(strSelfBlock,NULL);
		}
	}
	return TRUE;
}

long CBlockManager::LoadBlock(IN CString strBlockName)
{
	BlockInfo* pBlockInfo = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlockInfo) 
		&& pBlockInfo && !IsBadReadPtr(pBlockInfo,1)
		&& pBlockInfo->ayStockList.GetCount() > 0)
	{
	}
	else
	{
		CString strBlockFilePath;
		strBlockFilePath.Format("%sBlock\\%s.BLK",CGeneralHelper::GetAppPath(),strBlockName.GetString());
		CFile fp;
		if( fp.Open(strBlockFilePath, CFile::shareDenyNone|CFile::modeRead|CFile::typeBinary) )
		{
			int nLen = (int)fp.GetLength();
			if(nLen > 0)
			{
				char* pData = new char[nLen+1];
				fp.Read( pData,  sizeof(char)*nLen );
				fp.Close();

				char* pCurData = pData;
				long* sMark = (long*)pCurData;
				if (*sMark == 888)
				{
					pCurData += sizeof(long);

					if (!pBlockInfo || IsBadReadPtr(pBlockInfo,1))
					{
						pBlockInfo = new BlockInfo;
						pBlockInfo->strBlockName = strBlockName;
					}
					CodeInfo* pInfo = (CodeInfo*)pCurData;
					nLen = (nLen - sizeof(long))/sizeof(CodeInfo);

					for( int i = 0; i < nLen; pInfo++, i++)
					{
						CodeInfo* pCode = new CodeInfo(pInfo->m_cCodeType,pInfo->GetCode());
						pBlockInfo->ayStockList.Add(pCode);
					}
					m_mapBlockInfo.SetAt(strBlockName,pBlockInfo);
				}
				else if (*sMark == 911)	// 跳过参数
				{
					pCurData += sizeof(long);

					if (!pBlockInfo || IsBadReadPtr(pBlockInfo,1))
					{
						pBlockInfo = new BlockInfo;
						pBlockInfo->strBlockName = strBlockName;
					}
					CodeInfo* pInfo = (CodeInfo*)pCurData;
					nLen = (nLen - sizeof(long))/(sizeof(CodeInfo)+4);

					for( int i = 0; i < nLen; i++)
					{
						CodeInfo* pCode = new CodeInfo(pInfo->m_cCodeType,pInfo->GetCode());
						pBlockInfo->ayStockList.Add(pCode);
						pInfo++;
						pInfo = (CodeInfo*)(((char*)pInfo)+4);
					}
					m_mapBlockInfo.SetAt(strBlockName,pBlockInfo);
				}

				delete pData;
			}
			return nLen;
		}
	}
	return 0;
}

// 板块操作
long CBlockManager::GetAllBlockGroupName(OUT CStringArray& ayBlockGroupName)	// 获取所有板块分类名称
{
	GetAllSysBlockGroupName(ayBlockGroupName);

	GetAllUserBlockGroupName(ayBlockGroupName);

	return ayBlockGroupName.GetCount();
}
long CBlockManager::GetAllSysBlockGroupName(OUT CStringArray& aySysBlockGroupName)
{
	for (int i=0; i<m_aySysBlockGroup.GetCount(); i++)
	{
		BlockGroup* pGroup = m_aySysBlockGroup.GetAt(i);
		aySysBlockGroupName.Add(pGroup->strGroupName);
	}
	return aySysBlockGroupName.GetCount();
}
long CBlockManager::GetAllUserBlockGroupName(OUT CStringArray& ayUserBlockGroupName)
{
	for (int i=0; i<m_ayUserBlockGroup.GetCount(); i++)
	{
		BlockGroup* pGroup = m_ayUserBlockGroup.GetAt(i);
		ayUserBlockGroupName.Add(pGroup->strGroupName);
	}
	return ayUserBlockGroupName.GetCount();
}

long CBlockManager::GetBlockName(IN CString& strBlockGroupName, OUT CStringArray& ayBlockName)	// 获取指定板块分类的所有板块名称
{
	CArray<BlockInfo*,BlockInfo*>* pAyBlockName = NULL;
	for (int i=0; i<m_aySysBlockGroup.GetCount(); i++)
	{
		if (!strBlockGroupName.CompareNoCase(m_aySysBlockGroup.GetAt(i)->strGroupName))
		{
			pAyBlockName = &m_aySysBlockGroup.GetAt(i)->ayBlockList;
			for (int j=0; j<pAyBlockName->GetCount(); j++)
			{
				ayBlockName.Add(pAyBlockName->GetAt(j)->strBlockName);
			}
			return ayBlockName.GetCount();
		} 
	}
	for (int i=0; i<m_ayUserBlockGroup.GetCount(); i++)
	{
		if (!strBlockGroupName.CompareNoCase(m_ayUserBlockGroup.GetAt(i)->strGroupName))
		{
			pAyBlockName = &m_ayUserBlockGroup.GetAt(i)->ayBlockList;
			for (int j=0; j<pAyBlockName->GetCount(); j++)
			{
				ayBlockName.Add(pAyBlockName->GetAt(j)->strBlockName);
			}
			return ayBlockName.GetCount();
		} 
	}
	return 0;
}

// 不建议使用
long CBlockManager::GetAllBlockName(OUT CBlockGroupArray& ayBlockName)			// 获取所有板块名称
{
	ayBlockName.Copy(m_aySysBlockGroup);
	ayBlockName.Append(m_ayUserBlockGroup);
	return ayBlockName.GetCount();
}

// 获取成分股信息
long CBlockManager::GetStockList(IN CString strBlockName, OUT CArray<CodeInfo>& ayStockList)	// 获取指定板块的成分股列表
{
	LoadBlock(strBlockName);
	BlockInfo* pBlock = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlock) && pBlock)
	{
		for (int i=0; i<pBlock->ayStockList.GetCount(); i++)
		{
			CodeInfo* pCode = pBlock->ayStockList.GetAt(i);
			if (pCode)
			{
				CodeInfo codeinfo(pCode->m_cCodeType,pCode->GetCode());
				ayStockList.Add(codeinfo);
			}
		}
		return ayStockList.GetCount();
	}
	return 0;
}

long CBlockManager::GetStockList(IN CString strBlockName, OUT CMapStringToPtr& mapStockList)
{
	LoadBlock(strBlockName);
	BlockInfo* pBlock = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlock) && pBlock)
	{
		for (int i=0; i<pBlock->ayStockList.GetCount(); i++)
		{
			CodeInfo* pCode = pBlock->ayStockList.GetAt(i);
			mapStockList.SetAt(pCode->GetCode(), NULL);
		}
		return mapStockList.GetCount();
	}
	return 0;
}

long CBlockManager::GetStockListCount(IN CString strBlockName)		// 获取指定板块成分股个数
{
	LoadBlock(strBlockName);
	BlockInfo* pBlock = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlock) && pBlock)
	{
		return pBlock->ayStockList.GetCount();
	}
	return 0;
}
long CBlockManager::GetStockAdscription(IN CodeInfo* pCode,OUT CStringArray& ayBlockName)		// 查找所属板块
{
	POSITION pos = m_mapBlockInfo.GetStartPosition();
	while(pos)
	{
		CString strBlockName;
		BlockInfo* pBlock = NULL;
		m_mapBlockInfo.GetNextAssoc(pos,strBlockName,pBlock);
		LoadBlock(strBlockName);
		if (m_mapBlockInfo.Lookup(strBlockName,pBlock) 
			&& pBlock && !IsBadReadPtr(pBlock,1) 
			&& pBlock->ayStockList.GetCount()>0)
		{
			for (int i=0; i<pBlock->ayStockList.GetCount(); i++)
			{
				CodeInfo* pTempCode = pBlock->ayStockList.GetAt(i);
				if (MakeMainMarket(pTempCode->m_cCodeType)==MakeMainMarket(pCode->m_cCodeType)
					&& pTempCode->GetCode()==pCode->GetCode())
				{
					ayBlockName.InsertAt(0,strBlockName);
					break;
				}
			}
		}
	}
	return ayBlockName.GetCount();
}

// 操作成分股列表
BOOL CBlockManager::AddStockToBlock(IN CString strBlockName, IN CodeInfo* code)		// 添加一个成分股到板块末尾
{
	DelStockFromBlock(strBlockName,code);

	BlockInfo* pBlock = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlock) && pBlock)
	{
		CodeInfo* pCode = new CodeInfo(code->m_cCodeType,code->GetCode());
		pBlock->ayStockList.Add(pCode);
		m_mapModifiedBlock.SetAt(strBlockName,NULL);
		return TRUE;
	}
	return FALSE;
}
BOOL CBlockManager::InsertStockToBlock(IN CString strBlockName, IN CodeInfo* code, IN long nIndex)	// 插入一个成分股到指定板块的指定位置
{
	if (nIndex < 0)
	{
		return FALSE;
	}
	long nOldIndex = DelStockFromBlock(strBlockName,code);
	if (nOldIndex > 0 && nOldIndex <= nIndex)
	{
		nIndex--;
	}

	BlockInfo* pBlock = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlock) && pBlock)
	{
		CodeInfo* pCode = new CodeInfo(code->m_cCodeType,code->GetCode());
		pBlock->ayStockList.InsertAt(nIndex,pCode);
		m_mapModifiedBlock.SetAt(strBlockName,NULL);
		return TRUE;
	}
	return FALSE;
}
long CBlockManager::DelStockFromBlock(IN CString strBlockName, IN CodeInfo* code)	// 从指定板块删除指定成分股
{
	LoadBlock(strBlockName);
	BlockInfo* pBlock = NULL;
	if (m_mapBlockInfo.Lookup(strBlockName,pBlock) && pBlock)
	{
		for (int i=0; i<pBlock->ayStockList.GetCount(); i++)
		{
			if (code->CompareCode(pBlock->ayStockList.GetAt(i)))
			{
				delete pBlock->ayStockList.GetAt(i);
				pBlock->ayStockList.RemoveAt(i);
				m_mapModifiedBlock.SetAt(strBlockName,NULL);
				return i;
			}
		}
	}
	return -1;
}
