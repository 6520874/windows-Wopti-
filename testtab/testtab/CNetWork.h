#pragma once
#include "afxcmn.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CNetstatDlg dialog
typedef struct{
	DWORD	dwState;		//连接状态
	DWORD	dwLocalAddr;	//本地地址
	DWORD	dwLocalPort;	//本地端口
	DWORD	dwRemoteAddr;	//远程地址
	DWORD	dwRemotePort;	//远程端口
	DWORD	dwProcessID;	//进程ID
} MIB_TCPEXROW, *PMIB_TCPEXROW;


typedef struct{
	DWORD			dwNumEntries;
	MIB_TCPEXROW	table[ANY_SIZE];
}MIB_TCPEXTABLE, *PMIB_TCPEXTABLE;


typedef struct{
	DWORD	dwLocalAddr;	//本地地址
	DWORD	dwLocalPort;	//本地端口
	DWORD	dwProcessID;	//进程ID
} MIB_UDPEXROW, *PMIB_UDPEXROW;


typedef struct{
	DWORD			dwNumEntries;
	MIB_UDPEXROW	table[ANY_SIZE];
}MIB_UDPEXTABLE, *PMIB_UDPEXTABLE_VISTA;


//===========================================
// 此函数只能在win7上使用
//
//===========================================
typedef struct
{
	DWORD dwState;      //连接状态
	DWORD dwLocalAddr;  //本地地址
	DWORD dwLocalPort;  //本地端口
	DWORD dwRemoteAddr; //远程地址
	DWORD dwRemotePort; //远程端口
	DWORD dwProcessID;  //进程标识
	DWORD Unknown;      //待定标识

}MIB_TCPEXROW_VISTA,*PMIB_TCPEXROW_VISTA;

//=====================================================================================//
//Name: DWORD InternalGetTcpTable2()												   //
//                                                                                     //
//Descripion: 该函数在 Windows Vista 以及 Windows 7 下面效								   //
//            				                            						       //
//=====================================================================================//
typedef struct
{
	DWORD dwNumEntries;
	MIB_TCPEXROW_VISTA table[ANY_SIZE];

}MIB_TCPEXTABLE_VISTA,*PMIB_TCPEXTABLE_VISTA;


//================================//
//此函数在xp ，serv2003有效       //
//
//================================//
typedef  DWORD (WINAPI *PFNAllocateAndGetTcpExTableFromStack)(
	PMIB_TCPEXTABLE*	pTcpTable,
	BOOL				bOrder,
	HANDLE				hHeap,
	DWORD				dwZero,
	DWORD				dwFlags
	);


//================================//
//此函数在xp ，serv2003有效       //
//
//================================//
typedef DWORD (WINAPI *PFNAllocateAndGetUdpExTableFromStack)(
	PMIB_UDPEXTABLE_VISTA*	pTcpTable,
	BOOL				bOrder,
	HANDLE				hHeap,
	DWORD				dwZero,
	DWORD				dwFlags
	);


//=====================================================================================//
//Name: DWORD InternalGetTcpTable2()												   //
//                                                                                     //
//Descripion: 该函数在 Windows Vista 以及 Windows 7 下面效								   //
//            				                            						       //
//======================d===============================================================//
typedef DWORD (WINAPI *PFNInternalGetTcpTable2)(
	PMIB_TCPEXTABLE_VISTA *pTcpTable_Vista,
	HANDLE heap,
	DWORD flags
	);


//=====================================================================================//
//Name: DWORD InternalGetUdpTableWithOwnerPid()										   //
//                                                                                     //
//Descripion: 该函数在 Windows Vista 以及 Windows 7 下面效								   //
//            				                            						       //
//=====================================================================================//
typedef DWORD (WINAPI *PFNInternalGetUdpTableWithOwnerPid)(
	PMIB_UDPEXTABLE_VISTA *pUdpTable,
	HANDLE heap,
	DWORD flags
	);


// Cdlg2 对话框

class CNetWorkDlg : public CDialog
{
	DECLARE_DYNAMIC(CNetWorkDlg)

public:
	CNetWorkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetWorkDlg();

// 对话框数据
	enum { IDD = IDD_NETWORK};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void GetUdpTableEx();   
	void GetTcpTableEx();
	void GetProcessPathAndName(DWORD dwProcessID, TCHAR* szPath, TCHAR* szExeFile);
	BOOL LoadTcpAndUdpFunc();
	void DisplayLinkList();
	BOOL EnableDebugPrivileges();
	void OnBtnRefresh() ;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateRefershlist(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLocatepro(CCmdUI *pCmdUI);
	afx_msg void OnUpdateKillpro(CCmdUI *pCmdUI);
	afx_msg void OnBtnTerminateProc();
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);

private:
	CListCtrl   m_listnetwork;
	CImageList  m_ImageList;
	PMIB_UDPEXTABLE_VISTA m_pUdpExTableVista;
	PMIB_TCPEXTABLE_VISTA m_pTcpExTableVista;
	HMODULE m_hModule;
	PFNAllocateAndGetTcpExTableFromStack m_pAllocateAndGetTcpExTableFromStack;
	PFNAllocateAndGetUdpExTableFromStack m_pAllocateAndGetUdpExTableFromStack;
	PFNInternalGetTcpTable2      m_tcptablestack;
	PFNInternalGetUdpTableWithOwnerPid m_udpTablestack;
	CButton m_check;

};
