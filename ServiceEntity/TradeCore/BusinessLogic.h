/*******************************************************
  Դ��������:Businesslogic.h
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
#include "BusinessLogicIntf.h"

class CBusinessLogic : public IBusinessLogic
{
public:
	CBusinessLogic();
	~CBusinessLogic();
	//�ͷ�
	void Release();

	void SetAccount(IAccount* lpAccount);

	/*��ȡ�ɶ��˺�
	*@param   lpExchangeType:�г�
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : �ɶ��˺��б�
	*/
	IStockAccountList * GetStockAccount(CString& sMsg);

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
	BOOL StockEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, CString& sMsg);

	/*֤ȯ����
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sEntrustNo:ί�б��
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL StockWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, 
		CString& sMsg);	

	/*����ͬ��
	*@param   dDataType:��Ҫͬ��������
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	//BOOL SyncCacheData(const DWORD dDataType, CString& sMsg);

//////////////////////////////////////////////////////////////////////////
	/*��ȡ������Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ������Ϣ���б�
	 */
	IFundInfoList * GetFundInfo(CString& sMsg);	

	/*��ȡ�����˻�
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : �����˻��б�
	*/
	IFundStockAccountList * GetFundStockAccount(CString& sMsg, BOOL bQueryFlag /* = FALSE */);

	/*��ȡ����˾
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ����˾�б�
	*/
	IFundCompanyList * GetFundCompany(CString& sMsg);

	/*�����Ϲ�
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:�Ϲ����
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundSubcribe(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg);

	/*�����깺
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:�Ϲ����
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, CString& sMsg);

	/*�������
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dAmount:�������
	*@param   sAllotDate:�깺����
	*@param   AllotNo:�깺���
	*@param   sExceedFlag:�޶���ر�־
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, CString& sMsg);

	/*���𳷵�
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   sEntrustNo:ί�б��
	*@param   sAllotDate:�깺����
	*@param   sAllotNo:�깺���
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundWithDraw(const CString sFundCompany, const CString sFundCode, const CString sEntrustNo, 
		const CString sAllotDate, const CString sAllotNo,CString& sMsg);

	/*�ֺ�����
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   sDividendMethod:�ֺ��־
	*@param   sChargeType:�շѷ�ʽ
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundSetDividend(const CString sFundCompany, const CString sFundCode, const CString sDividendMethod, const CString sChargeType, CString& sMsg);

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
	BOOL FundTrans(const CString sFundCompany, const CString sFundCode,const CString sTransCode, const double dTransAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sChargeType, CString& sMsg);

	/*���÷��յȼ�
	*@param   sRiskLevel:���յȼ�
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundSetRiskLevel(const CString sRiskLevel, CString& sMsg);

	/*����ԤԼ�깺
	*@param   sFundCompany:����˾
	*@param   sFundCode:�������
	*@param   dBalance:�Ϲ����
	*@param   sChargeType:�շѷ�ʽ
	*@param   sHopeDate:ԤԼ����
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL FundHopeAllot(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sHopeDate, CString& sMsg);

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
	BOOL FundHopeRedeem(const CString sFundCompany, const CString sFundCode, const double dAmount, 
		const CString sAllotDate, const CString sAllotNo, const CString sExceedFlag, const CString sChargeType, const CString sHopeDate, CString& sMsg);

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
	BOOL Fund7432(const CString sFundCompany, const CString sFundCode, const double dBalance, const CString sChargeType, const CString sBeginDate, const CString sEndDate, const CString sFundDate, CString& sMsg);


	/*��ȡ������Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ������Ϣ���б�
	*/
	IBankList * GetBankInfo(CString& sMsg);	

	/*��֤ת��
	*@param   mtValue:����
	*@param   sBankNo:���д���
	*@param   sDirection:ת�ʷ���
	*@param   dBalance:�������
	*@return : �Ƿ�ɹ�
	*/
	BOOL BankStockTransfer(const EMoneyType mtValue, const CString sBankNo, const CString sBankPsw, 
		const CString sFundPsw, const CString sDirection, const double dBalance, CString& sMsg);

	/*�޸�����
	*@param   sPasswordType:��������
	*@param   sOldPassword:ԭ����
	*@param   sNewPassword:������
	*@return : �Ƿ�ɹ�
	*/
	BOOL ChangePassword(const CString sPasswordType, const CString sOldPassword, const CString sNewPassword, CString& sMsg);

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
	BOOL SetStockCostPrice(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const double dCostPrice, const CString sSeat, const double dEntrustBalance, const CString sRemark, CString& sMsg);

	/*��ȡ�û���Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : �û���Ϣ
	*/
	IUserInfoItem * GetUserInfo(CString& sMsg, BOOL bQueryFlag /* = FALSE */);

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
	BOOL StockSBEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, 
		const CString sSeatNo, const CString sConferNo, CString& sMsg);

	/*֤ȯ���峷��
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sEntrustNo:ί�б��
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL StockSBWithDraw(const CString sExchangeType, const CString sStockAccount, const CString sEntrustNo, 
		CString& sMsg);	

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
	BOOL StockFundEntrust(const int iAction, const CString sExchangeType, const CString sStockAccount, 
		const CString sStockCode, const int iEntrustAmount, const CString sTransCode, CString& sMsg);

	/*�����̺�ҵ�񳷵�
	*@param   sExchangeType:�г�
	*@param   sStockCode:֤ȯ����
	*@param   sEntrustNo:ί�б��
	*@param   sMsg: [out]������Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL StockFundWithDraw(const CString sExchangeType, const CString sStockCode, const CString sEntrustNo, 
		CString& sMsg);	


	/*�����̺�ҵ�������Ϣ
	*@param   sMsg: [out]��ѯ��Ϣ
	*@return : ������Ϣ�б�
	*/
	IStockFundInfoList * GetStockFundInfo(CString& sMsg);

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
	BOOL RZRQEntrust(const CString sExchangeType, const CString sStockAccount, const CString sStockCode, 
		const EBSDirection eEntrustBs, const double dEntrustPrice, const int iEntrustAmount, const CString sEntrustProp, 
		const CString sEntrustType, CString& sMsg);

	/*������ȯֱ�ӻ���
	*@param   sMoneyType:����
	*@param   dEntrustBalance:���
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL RZRQZJHK(const CString sMoneyType, const double dEntrustBalance, CString& sMsg);

	/*������ȯֱ�ӻ�ȯ
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   iEntrustAmount:ί������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL RZRQZJHQ(const CString sExchangeType, const CString sStockAccount, const CString sStockCode,
		const int iEntrustAmount, CString& sMsg);

	/*ETF�����ֽ��Ϲ�
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sStockCode:֤ȯ����
	*@param   iEntrustAmount:ί������
	*@param   sEntrustProp:ί������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL ETF9835(const CString sExchangeType, const CString sStockAccount, 
		const CString sStockCode, const int iEntrustAmount,const CString sEntrustProp, CString& sMsg);
	
	/*ETF���¹ɷݻ���
	*@param   sExchangeType:�г�
	*@param   sStockAccount:�ɶ��˺�
	*@param   sComponentCode:�ɷݹɴ���
	*@param   iEntrustAmount:ί������
	*@param   sETFCode:ETF����
	*@param   sEntrustProp:ί������
	*@param   sMsg: [out]ί����Ϣ
	*@return : �Ƿ�ɹ�
	*/
	BOOL ETF9837(const CString sExchangeType, const CString sStockAccount, 
		const CString sComponentCode, const int iEntrustAmount, const CString sETFCode, const CString sEntrustProp, CString& sMsg);



private:
	IStockAccountList* m_StockAccountList;
	IAccount* m_Account;
	IFundInfoList* m_FundInfoList;	
	IBankList* m_BankList;
	IUserInfoList* m_UserInfoList;
	IStockFundInfoList* m_StockFundInfoList;
	IFundStockAccountList* m_FundStockAccountList;
	IFundCompanyList* m_FundCompanyList;
};