//========================================================================================
//��Ȩ���У���Ȩ����(C) 2013���Ϻ���ƽ��Ϣ�ɷ��޹�˾
//ϵͳ���ƣ�AVCON6(vchub)
//�������ߣ�������
//�������ڣ�2013-01-14
//����˵�����Ź������������߼�dll( CSetInterNet)
//
//---------------------------------------------------------------------------------------
//�޸ļ�¼��01
//�޸��ˣ�  XXX
//�޸�ʱ�䣺XXXX-XX-XX
//�޸����ݣ�XXX
//---------------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include <atlbase.h>
#include <atlstr.h>
#include <vector>
#include <string>
#include <list>
#include <comdef.h>
#include <WinSock2.h>
#include <Wbemcli.h>
#pragma comment(lib, "wbemuuid.lib")
using namespace  std;

#ifndef NETSET
#define NETSET_DLL __declspec(dllexport)
#else
#define NETSET_DLL __declspec(dllimport)
#endif

class  NETSET_DLL  CSetInterNet
{
public:
	CSetInterNet(void);
	~CSetInterNet(void);

	bool GetNetSetting(
		__in    unsigned    iAdapterIndex,      //���������
		__inout string	&	strName,			// ����������
		__inout string	&	strDriverDesc,		// ����������
		__inout string	&	strIP,				// IP��ַ
		__inout string	&	strNetMask,			// ��������
		__inout string	&	strNetGate,			// ����
		__inout string	&	strDNS,				// DNS������
		__inout bool	&	bEnableDHCP,		//�Ƿ�����DHCP
		__inout bool	&	bEnableDNSDHCP		//�Ƿ�����DNS DHCP);
		);


	eNetErrorReturnValue SetNetSetting( __in unsigned iAdapterNo,//��������� 
		__in bool bEnableDHCP,   //�Ƿ�����ip dhcp/
		__in bool bEnableDNSDHCP,//�Ƿ�����dns dhcp/
		__in string strIP ="", //IP��ַ/
		__in string strNetMask = " ",// �������� 
		__in string strNetGate = "", // ����
		__in string strFirstDNS = "", // ��ѡDNS,
		__in string strSecondDNs = "");//����dns;


	void GetNetSettingFromIni(int iIndex, bool &bDHCP,CString &strIP,CString &strMask,CString &strGate,\
		bool &bDNS_DHCP,CString &strDNS1, CString &strDNS2);

	void  WriteNetSettingIni(int iIndex,  CString bIPDhcp, CString bDNSDhcp,CString strip,CString strMask,\
		CString strGate,CString strDns1,CString strDns2);

	//avsh�������ò���
	bool ApplyNETParam();

	//��ȡ��������
	int  GetLocalNetCardNum();

	//��ȡ�������к�
	void GetNetNoSequence(__inout list<string> &list_net);

private:
	CString			m_csCurIP;
	CString			m_csCurMark;
	CString			m_csGateway;
	CString			m_csDNS1;
	CString			m_csDNS2;
	IWbemLocator	* m_pIWbemLocator;
	IWbemServices	* m_pIWbemServices;
	BSTR m_bstrClassPath;
	VARIANT m_varPathName;
	vector<ADAPTER_INFOpp*> m_AdapterInfoVector;

	void Init();
	void OnInitInterNet();
	CString GetCurNetCardIndex();
	BOOL GetAdapterInfo();
	void ReleaseVector();
	void ClearDefaultGateway(BSTR IPAddress,BSTR SubnetMask);
	int  CheckReticleState(CString strName);
	BOOL IsFullyIp(const char* szIP);
	BOOL RegGetIP(ADAPTER_INFOpp *pAI,  LPCTSTR lpszAdapterName);
	BOOL InitWmiEnvironment();
	BOOL GetPathName(string strName);
	BOOL EnableDHCP();
	BOOL EnableDNS();
	BOOL SetStaticIP(BSTR IPAddress,BSTR SubnetMask);
	BOOL SetGateway(BSTR Gateway,UINT metric);
	BOOL SetDNS(BSTR bstrDNS,BSTR bstrDNS2);

	eNetErrorReturnValue CheckHostSubNumber( string strIP, string strNetMask );
	eNetErrorReturnValue CheckDHCPIPAddress(string strIPAddress);
	eNetErrorReturnValue CheckReticleState(int iAdapter);
	eNetErrorReturnValue CheckNoDhcpIpAdress(string strIP, string strNetGate,string strNetMask,string strDNS);

};



