#pragma once
#include "afxcmn.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CNetstatDlg dialog
typedef struct{
	DWORD	dwState;		//����״̬
	DWORD	dwLocalAddr;	//���ص�ַ
	DWORD	dwLocalPort;	//���ض˿�
	DWORD	dwRemoteAddr;	//Զ�̵�ַ
	DWORD	dwRemotePort;	//Զ�̶˿�
	DWORD	dwProcessID;	//����ID
} MIB_TCPEXROW, *PMIB_TCPEXROW;


typedef struct{
	DWORD			dwNumEntries;
	MIB_TCPEXROW	table[ANY_SIZE];
}MIB_TCPEXTABLE, *PMIB_TCPEXTABLE;


typedef struct{
	DWORD	dwLocalAddr;	//���ص�ַ
	DWORD	dwLocalPort;	//���ض˿�
	DWORD	dwProcessID;	//����ID
} MIB_UDPEXROW, *PMIB_UDPEXROW;


typedef struct{
	DWORD			dwNumEntries;
	MIB_UDPEXROW	table[ANY_SIZE];
}MIB_UDPEXTABLE, *PMIB_UDPEXTABLE_VISTA;


//===========================================
// �˺���ֻ����win7��ʹ��
//
//===========================================
typedef struct
{
	DWORD dwState;      //����״̬
	DWORD dwLocalAddr;  //���ص�ַ
	DWORD dwLocalPort;  //���ض˿�
	DWORD dwRemoteAddr; //Զ�̵�ַ
	DWORD dwRemotePort; //Զ�̶˿�
	DWORD dwProcessID;  //���̱�ʶ
	DWORD Unknown;      //������ʶ

}MIB_TCPEXROW_VISTA,*PMIB_TCPEXROW_VISTA;

//=====================================================================================//
//Name: DWORD InternalGetTcpTable2()												   //
//                                                                                     //
//Descripion: �ú����� Windows Vista �Լ� Windows 7 ����Ч								   //
//            				                            						       //
//=====================================================================================//
typedef struct
{
	DWORD dwNumEntries;
	MIB_TCPEXROW_VISTA table[ANY_SIZE];

}MIB_TCPEXTABLE_VISTA,*PMIB_TCPEXTABLE_VISTA;


//================================//
//�˺�����xp ��serv2003��Ч       //
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
//�˺�����xp ��serv2003��Ч       //
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
//Descripion: �ú����� Windows Vista �Լ� Windows 7 ����Ч								   //
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
//Descripion: �ú����� Windows Vista �Լ� Windows 7 ����Ч								   //
//            				                            						       //
//=====================================================================================//
typedef DWORD (WINAPI *PFNInternalGetUdpTableWithOwnerPid)(
	PMIB_UDPEXTABLE_VISTA *pUdpTable,
	HANDLE heap,
	DWORD flags
	);


// Cdlg2 �Ի���

class CNetWorkDlg : public CDialog
{
	DECLARE_DYNAMIC(CNetWorkDlg)

public:
	CNetWorkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetWorkDlg();

// �Ի�������
	enum { IDD = IDD_NETWORK};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
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
