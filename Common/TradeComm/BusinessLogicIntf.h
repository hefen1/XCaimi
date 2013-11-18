/*******************************************************
  Դ��������:BusinesslogicIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����ҵ���߼�
  			1��ί�нӿڶ���
  			2����ѯ�ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "QueryStockAccountIntf.h"
#include "QueryFundInfoIntf.h"
#include "QueryBankIntf.h"
#include "QueryUserInfoIntf.h"
#include "QueryStockFundInfoIntf.h"
#include "QueryFundStockAccountIntf.h"
#include "QueryFundCompanyIntf.h"

#define PASSWORD_TYPE_FUND		"1"//�ʽ�����
#define PASSWORD_TYPE_TRADE		"2"//��������
#define PASSWORD_TYPE_COMM		"3"//ͨѶ����

struct IBusinessLogic
{
	//�ͷ�
	virtual void Release() = 0;

	virtual void SetAccount(IAccount* lpAccount) = 0;
  
	/*��ȡ�ɶ��˺�
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : �ɶ��˺��б�
	*/
	virtual IStockAccountList * GetStockAccount(CString& sMsg) = 0;

	/*֤ȯί��
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   eEntrustBs:��������
	*@param   dEntrustPrice:ί�м۸�
	*@param   iEntrustAmount:ί������
	*@param   sEntrustProp:ί������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL StockEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, CString& sMsg) = 0;

	/*֤ȯ����
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sEntrustNo:ί�б��
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL StockWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, 
		CString& sMsg) = 0;	

	/*����ͬ��
	*@param   dDataType:��Ҫͬ��������
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	//virtual BOOL SyncCacheData(const DWORD dDataType, CString& sMsg) = 0;

//////////////////////////////////////////////////////////////////////////
	
	/*��ȡ������Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ������Ϣ�б�
	*/
	virtual IFundInfoList * GetFundInfo(CString& sMsg) = 0;	

	/*��ȡ�����˻�
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : �����˻��б�
	*/
	virtual IFundStockAccountList * GetFundStockAccount(CString& sMsg, BOOL bQueryFlag = FALSE) = 0;	

	/*��ȡ����˾
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ����˾�б�
	*/
	virtual IFundCompanyList * GetFundCompany(CString& sMsg) = 0;	
	
	/*�����Ϲ�
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:�Ϲ����
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundSubcribe(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg) = 0;

	/*�����깺
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:�Ϲ����
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg) = 0;

	/*�������
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dAmount:�������
	*@param   sAllotDate:�깺����
	*@param   sAllotNo:�깺���
	*@param   sExceedFlag:�޶���ر�־
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, CString& sMsg) = 0;

	/*���𳷵�
	*@param   sfundcompany:����˾
	*@param   sfundcode:�������
	*@param   sentrustno:ί�б��
	*@param   sallotdate:�깺����
	*@param   sallotno:�깺���
	*@param   smsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundWithDraw(const CString sFundCompany, const CString sFundCode, const CString sEntrustNo, 
		const CString sAllotDate, const CString sAllotNo,CString& sMsg) = 0;	

	/*�ֺ�����
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   sDividendMethod:�ֺ��־
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundSetDividend(const CString sFundCompany, const CString sFundCode, const CString sDividendMethod, const CString sChargeType, CString& sMsg) = 0;

	/*����ת��
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   sTransCode:ת������
	*@param   dTransAmount:ת������
	*@param   sAllotDate:�깺����
	*@param   sAllotNo:�깺���
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundTrans(const CString sFundCompany, const CString sFundCode,const CString sTransCode, const double dTransAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sChargeType, CString& sMsg) = 0;

	/*���÷��յȼ�
	*@param   sRiskLevel:���յȼ�
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundSetRiskLevel(const CString sRiskLevel, CString& sMsg) = 0;

	/*����ԤԼ�깺
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:�Ϲ����
	*@param   sChargeType:�շѷ�ʽ
	*@param   sHopeDate:ԤԼ����
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundHopeAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sHopeDate, CString& sMsg) = 0;

	/*����ԤԼ���
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dAmount:�������
	*@param   sAllotDate:�깺����
	*@param   sAllotNo:�깺���
	*@param   sExceedFlag:�޶���ر�־
	*@param   sChargeType:�շѷ�ʽ
	*@param   sHopeDate:ԤԼ����
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL FundHopeRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, const CString sHopeDate, CString& sMsg) = 0;

	/*����Ͷ
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:��Ͷ���
	*@param   sChargeType:�շѷ�ʽ
	*@param   sBeginDate:��ʼ����
	*@param   sEndDate:��ֹ����
	*@param   sFundDate:�¿ۿ���
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL Fund7432(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sBeginDate, const CString sEndDate, const CString sFundDate, CString& sMsg) = 0;


	/*��ȡ������Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ������Ϣ���б�
	*/
	virtual IBankList * GetBankInfo(CString& sMsg) = 0;	

	/*��֤ת��
	*@param   mtValue:����
	*@param   sBankNo:���д���
	*@param   sBankPsw:��������
	*@param   sFundPsw:�ʽ�����
	*@param   sDirection:ת�ʷ���
	*@param   dBalance:�������
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL BankStockTransfer(const EMoneyType mtValue, const CString sBankNo, const CString sBankPsw, 
		const CString sFundPsw, const CString sDirection, const double dBalance, CString& sMsg) = 0;
	
	/*�޸�����
	*@param   sPasswordType:��������
	*@param   sOldPassword:ԭ����
	*@param   sNewPassword:������
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL ChangePassword(const CString sPasswordType, const CString sOldPassword, const CString sNewPassword, CString& sMsg) = 0;

	/*���óɱ���
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   dCostPrice:�ɱ���
	*@param   sSeat:ϯλ
	*@param   dEntrustBalance:���
	*@param   sRemark:��ע
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL SetStockCostPrice(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const double dCostPrice, const CString sSeat, const double dEntrustBalance, const CString sRemark, CString& sMsg) = 0;

	/*��ȡ�û���Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : �û���Ϣ
	*/
	virtual IUserInfoItem * GetUserInfo(CString& sMsg, BOOL bQueryFlag = FALSE) = 0;	

	/*����ί��
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   eEntrustBs:��������
	*@param   dEntrustPrice:ί�м۸�
	*@param   iEntrustAmount:ί������
	*@param   sEntrustProp:ί������
	*@param   sSeatNo:�Է�ϯλ
	*@param   sConferNo:Э����
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL StockSBEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, 
		const CString sSeatNo, const CString sConferNo, CString& sMsg) = 0;
	/*֤ȯ���峷��
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sEntrustNo:ί�б��
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL StockSBWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, 
		CString& sMsg) = 0;	

	/*�����̺�ҵ��ί��
	*@param   iAction:���ܺ�
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   iEntrustAmount:ί������
	*@param   sTransCode:ת������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL StockFundEntrust(const int iAction, const CString sExchangeType, const CString sStockAccount, 
		const CString sStockCode, const int iEntrustAmount, const CString sTransCode, CString& sMsg) = 0;

	/*�����̺�ҵ�񳷵�
	*@param   sExchangeType:�г�
	*@param   sStockCode:֤ȯ����
	*@param   sEntrustNo:ί�б��
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL StockFundWithDraw(const CString sExchangeType, const CString sStockCode, const CString sEntrustNo, 
		CString& sMsg) = 0;	


	/*�����̺�ҵ�������Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ������Ϣ�б�
	*/
	virtual IStockFundInfoList * GetStockFundInfo(CString& sMsg) = 0;

	/*������ȯί��
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   eEntrustBs:��������
	*@param   dEntrustPrice:ί�м۸�
	*@param   iEntrustAmount:ί������
	*@param   sEntrustProp:ί������
	*@param   sEntrustType:ί�����
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL RZRQEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, 
		const CString sEntrustType, CString& sMsg) = 0;

	/*������ȯֱ�ӻ���
	*@param   sMoneyType:����
	*@param   dEntrustBalance:���
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL RZRQZJHK(const CString sMoneyType, const double dEntrustBalance, CString& sMsg) = 0;

	/*������ȯֱ�ӻ�ȯ
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   iEntrustAmount:ί������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL RZRQZJHQ(const CString sExchangeType, const CString sStockAccount, const CString sStockCode,
		const int iEntrustAmount, CString& sMsg) = 0;

	/*ETF�����ֽ��Ϲ�
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   iEntrustAmount:ί������
	*@param   sEntrustProp:ί������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL ETF9835(const CString sExchangeType, const CString sStockAccount, 
		const CString sStockCode, const int iEntrustAmount,const CString sEntrustProp, CString& sMsg) = 0;
	
	/*ETF���¹ɷݻ���
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sComponentCode:�ɷݹɴ���
	*@param   eEntrustBs:��������
	*@param   iEntrustAmount:ί������
	*@param   sETFCode:ETF����
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	virtual BOOL ETF9837(const CString sExchangeType, const CString sStockAccount, 
		const CString sComponentCode, const int iEntrustAmount, const CString sETFCode, const CString sEntrustProp, CString& sMsg) = 0;


};