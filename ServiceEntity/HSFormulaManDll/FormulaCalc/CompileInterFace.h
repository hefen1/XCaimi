/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	HSCompileInterFace.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	һЩ�ӿڲ����ṹ��˵��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
***************************************************************/

#if !defined(HSCOMPILEINTERFACE_H)
#define HSCOMPILEINTERFACE_H

#include "FormulaManPublic.h"

class CExpression;
class CHSDataSource;

//******************************������litao����*****************************************
///����ִ��ʱ��ʶ���кŽṹ
struct  tagCompRecord
{
	CString  strName;
	int  SerialNo;
};
typedef struct tagCompRecord  OBJNO;
typedef struct tagCompRecord* LPOBJNO;

//******************************������litao����*****************************************

/*************************************************************
*	�� �� �ƣ�	nodElement
*	�� �� ����	һ��������Ԫ(tree Ҷ)
***************************************************************/
struct nodElement
{
	struct nodElement* left;        // ���������
	struct nodElement* right;       // �������ұ�
	unsigned char	   operatie;    // ������
	unsigned char	   cDataType;   // ��cDataType == DataTypeOperStrʱ��valoarestr ��Ч��	
	void*			   pData;       // �ⲿ����ָ��,��ָ�벻��ɾ�������ⲿɾ��;
									// �� operatie == Operator_Function ʱ���Ǻ�����Ϣָ��;
	union
	{
		double   valoare;
		CString* valoarestr;		// ֵ(��ָ������)
	};
	
	// ��ʼ��
	nodElement();
	// ���ռ����Դ
	~nodElement();

	// ����COPY
	void		 Copy(const nodElement* pClone);
	virtual void Serialize(CExpression* pExpress,CArchive& ar);	
};
typedef struct nodElement  NODELEMENT;
typedef struct nodElement* LPNODELEMENT;

/*************************************************************
*	�� �� �ƣ�	FormulaGeneralParam
*	�� �� ����	ͨ�ò���
***************************************************************/
struct FormulaGeneralParam
{
	//
	CArray<LPNODELEMENT,LPNODELEMENT>* m_ayVars; // ��������ָ��

	LPNODELEMENT GetParam(int nPos);
	int			 GetSize();
	
	void		 Add(LPNODELEMENT pParam);
	void		 InsertAt(int nIndex,LPNODELEMENT pParam);
	
	CArray<LPNODELEMENT,LPNODELEMENT>* GetArray();
	
	FormulaGeneralParam();
	~FormulaGeneralParam();
};
//����ʱ�����ⲿ����
//struct CCalcExternParam
//{
//	long		m_nBeginDate; // ��ʼ����,��19901020
//	long	    m_nEndDate;   
//	CodeInfo    m_sCode;	  // ����
//	char 		m_szBlock[BLOCK_NAME_LENGTH]; // �������
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
*	�� �� �ƣ�	HSCurDataInfoStatus
*	�� �� ����	�����м�����ṹ
***************************************************************/
struct CCalcExternParam;
struct HSCurDataInfoStatus
{
	int				m_nDataPos;  // ����λ��
	HSDouble		m_lPreData;  // ǰһ��������
	DWORD			m_dwStatus;  // ����״̬
	
	HSDouble*		m_pData;	// ����Ľ��
	long			m_lSize;	// ����ĳ���

	CHSDataSource*	m_pDataSource; // ���ݰ�װ

	CCalcExternParam* m_pCalcParam; // �ⲿ���������಻��ɾ��

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

	// �õ����Ӻ���ַ���
	CString GetCatStr();
};


/*************************************************************
*	�� �� �ƣ�	CExpressionBase
*	�� �� ����	��ʽ����,ûʵ��ʲô����
***************************************************************/
class CExpressionBase : public CObject
{
	DECLARE_DYNCREATE(CExpressionBase)

public:
	CExpressionBase() {}
	~CExpressionBase() {}
public:
	virtual double vexp ( LPNODELEMENT a,HSCurDataInfoStatus* pCurDataStatus) { return hx_DefMinValues; }       // ����
	virtual CHSDataSource* GetData() { return NULL; }
};

// ϵͳ��������ָ�����Ͷ���
typedef HSDouble ( *COMPILEFUNCTION)( CExpressionBase* pCompile, FormulaGeneralParam* ayVars,
									  HSCurDataInfoStatus* pCurDataStatus, int& nError );

#endif // HSCOMPILEINTERFACE_H