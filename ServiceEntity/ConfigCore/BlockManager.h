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

	// ������
	long	GetAllBlockGroupName(OUT CStringArray& ayBlockGroupName);	// ��ȡ���а���������
	long	GetAllSysBlockGroupName(OUT CStringArray& aySysBlockGroupName);
	long	GetAllUserBlockGroupName(OUT CStringArray& ayUserBlockGroupName);
	long	GetBlockName(IN CString& strBlockGroupName, OUT CStringArray& ayBlockName);	// ��ȡָ������������а������
	long	GetAllBlockName(OUT CBlockGroupArray& ayBlockName);			// ��ȡ���а������

	// ��ȡ�ɷֹ���Ϣ  
	long	GetStockList(IN CString strBlockName, OUT CArray<CodeInfo>& ayStockList);	// ��ȡָ�����ĳɷֹ��б�
	long    GetStockList(IN CString strBlockName, OUT CMapStringToPtr& mapStockList);   // ��ȡָ�����ĳɷֹ��б� key - code, value - null
	long	GetStockListCount(IN CString strBlockName);		// ��ȡָ�����ɷֹɸ���
	long	GetStockAdscription(IN CodeInfo* pCode,OUT CStringArray& ayBlockName);		// �����������

	// �����ɷֹ��б�
	BOOL	AddStockToBlock(IN CString strBlockName, IN CodeInfo* code);		// ���һ���ɷֹɵ����ĩβ
	BOOL	InsertStockToBlock(IN CString strBlockName, IN CodeInfo* code, IN long nIndex);	// ����һ���ɷֹɵ�ָ������ָ��λ��
	long	DelStockFromBlock(IN CString strBlockName, IN CodeInfo* code);	// ��ָ�����ɾ��ָ���ɷֹ�

protected:
	BOOL m_bInit;
	CBlockGroupArray	m_aySysBlockGroup;		// ϵͳ���
	CBlockGroupArray	m_ayUserBlockGroup;		// �û�������
	CBlockInfoMap		m_mapBlockInfo;			// �������
	CMapStringToPtr		m_mapModifiedBlock;		// �Ѹ��İ����
};
