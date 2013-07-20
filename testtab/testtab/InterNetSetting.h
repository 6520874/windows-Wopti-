//========================================================================================
//版权所有：版权所有(C) 2013，上海华平信息股份限公司
//系统名称：AVCON6(vchub)
//作　　者：单鹏飞
//创建日期：2013-01-14
//功能说明：九宫格网络设置逻辑dll( CSetInterNet)
//
//---------------------------------------------------------------------------------------
//修改记录：01
//修改人：  XXX
//修改时间：XXXX-XX-XX
//修改内容：XXX
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
		__in    unsigned    iAdapterIndex,      //适配器编号
		__inout string	&	strName,			// 适配器名称
		__inout string	&	strDriverDesc,		// 适配器描述
		__inout string	&	strIP,				// IP地址
		__inout string	&	strNetMask,			// 子网掩码
		__inout string	&	strNetGate,			// 网关
		__inout string	&	strDNS,				// DNS服务器
		__inout bool	&	bEnableDHCP,		//是否启用DHCP
		__inout bool	&	bEnableDNSDHCP		//是否启用DNS DHCP);
		);


	eNetErrorReturnValue SetNetSetting( __in unsigned iAdapterNo,//适配器编号 
		__in bool bEnableDHCP,   //是否启用ip dhcp/
		__in bool bEnableDNSDHCP,//是否启用dns dhcp/
		__in string strIP ="", //IP地址/
		__in string strNetMask = " ",// 子网掩码 
		__in string strNetGate = "", // 网关
		__in string strFirstDNS = "", // 首选DNS,
		__in string strSecondDNs = "");//备用dns;


	void GetNetSettingFromIni(int iIndex, bool &bDHCP,CString &strIP,CString &strMask,CString &strGate,\
		bool &bDNS_DHCP,CString &strDNS1, CString &strDNS2);

	void  WriteNetSettingIni(int iIndex,  CString bIPDhcp, CString bDNSDhcp,CString strip,CString strMask,\
		CString strGate,CString strDns1,CString strDns2);

	//avsh网络设置参数
	bool ApplyNETParam();

	//获取网卡数量
	int  GetLocalNetCardNum();

	//获取网卡序列号
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



