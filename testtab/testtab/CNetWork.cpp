// dlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "CNetWork.h"


// Cdlg2 对话框

IMPLEMENT_DYNAMIC(CNetWorkDlg, CDialog)

CNetWorkDlg::CNetWorkDlg(CWnd* pParent /*=NULL*/)
: CDialog(CNetWorkDlg::IDD, pParent)
{

}

CNetWorkDlg::~CNetWorkDlg()
{
}

void CNetWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listnetwork);
	DDX_Control(pDX, IDC_CHECK1, m_check);
}


BEGIN_MESSAGE_MAP(CNetWorkDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CNetWorkDlg::OnBnClickedCheck1)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_REFERSHLIST, &CNetWorkDlg::OnUpdateRefershlist)
	ON_UPDATE_COMMAND_UI(ID_LocatePro, &CNetWorkDlg::OnUpdateLocatepro)
	ON_UPDATE_COMMAND_UI(ID_KILLPRO, &CNetWorkDlg::OnUpdateKillpro)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CNetWorkDlg::OnNMRClickList1)
END_MESSAGE_MAP()


// Cdlg2 消息处理程序

BOOL CNetWorkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_listnetwork.InsertColumn(0, L"进程名", LVCFMT_CENTER, 120);
	m_listnetwork.InsertColumn(1, L"协议", LVCFMT_CENTER, 40);
	m_listnetwork.InsertColumn(2, L"本地IP", LVCFMT_LEFT, 100);
	m_listnetwork.InsertColumn(3, L"本地端口", LVCFMT_CENTER, 70);
	m_listnetwork.InsertColumn(4, L"目标IP", LVCFMT_LEFT, 100);
	m_listnetwork.InsertColumn(5, L"目标端口", LVCFMT_CENTER, 70);
	m_listnetwork.InsertColumn(6, L"状态", LVCFMT_CENTER, 60);
	m_listnetwork.InsertColumn(7, L"程序路径", LVCFMT_LEFT, 360);

	m_ImageList.Create(24, 24, ILC_COLOR24, 0, 40);
	m_listnetwork.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_listnetwork.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listnetwork.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	EnableDebugPrivileges();
	DisplayLinkList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CNetWorkDlg::EnableDebugPrivileges()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = {0};

	if (!OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		MessageBox(L"打开进程令牌失败！");
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid))
	{
		MessageBox(L"查看系统特权失败！");
		CloseHandle(hToken);
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		MessageBox(L"提权失败！");
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

VOID CNetWorkDlg::DisplayLinkList()
{
	EnableDebugPrivileges();  //进行提权
	LoadTcpAndUdpFunc();
	GetUdpTableEx();
	GetTcpTableEx();
}

BOOL CNetWorkDlg::LoadTcpAndUdpFunc()
{
	m_hModule = ::LoadLibrary(L"iphlpapi.dll");
	if (NULL == m_hModule)
	{
		MessageBox(L"加载iphlpapi.dll失败！");
		return FALSE;
	}

	m_pAllocateAndGetTcpExTableFromStack = (PFNAllocateAndGetTcpExTableFromStack)
		::GetProcAddress(m_hModule, "AllocateAndGetTcpExTableFromStack");

	m_pAllocateAndGetUdpExTableFromStack = (PFNAllocateAndGetUdpExTableFromStack)
		::GetProcAddress(m_hModule, "AllocateAndGetUdpExTableFromStack");

	if (m_pAllocateAndGetTcpExTableFromStack == NULL ||
		m_pAllocateAndGetUdpExTableFromStack == NULL)
	{

		m_pTcpExTableVista  =NULL;
		m_tcptablestack = (PFNInternalGetTcpTable2)GetProcAddress
			(m_hModule, "InternalGetTcpTable2");

		if (!m_tcptablestack)       //如果没有加载成功
		{
			if (m_pTcpExTableVista)
			{
				HeapFree(GetProcessHeap(), 0, m_pTcpExTableVista);
			}

			FreeLibrary(m_hModule);
			m_hModule = NULL;

		}
		m_udpTablestack = (PFNInternalGetUdpTableWithOwnerPid)GetProcAddress(m_hModule,"InternalGetUdpTableWithOwnerPid");


		m_pUdpExTableVista = NULL;

		if (m_udpTablestack!= NULL)
		{
			if (m_udpTablestack(&m_pUdpExTableVista, GetProcessHeap(), 1))
			{
				if (m_pUdpExTableVista)
				{
					HeapFree(GetProcessHeap(), 0, m_pUdpExTableVista);
				}

				FreeLibrary(m_hModule);
				m_hModule = NULL;

			}
		}
	}
	return TRUE;
}


void CNetWorkDlg::GetProcessPathAndName(DWORD dwProcessID, TCHAR *szPath,  TCHAR*szExeFile)
{
	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
	if (!hProcess)
	{
		//MessageBox(L"打开进程失败！");
		return;
	}
	
	DWORD cbNeeded = 0;
	HMODULE hModule;

	if(!EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
	{
		//MessageBox(L"枚举进程模块失败！"); bug有些进程无法枚举
		return;
	}

	GetModuleFileNameEx(hProcess, NULL, szPath, MAX_PATH);
	GetModuleBaseName(hProcess, hModule, szExeFile, MAX_PATH);

	CloseHandle(hProcess);
	//CloseHandle(hModule);
	return;
}


void CNetWorkDlg::GetTcpTableEx()
{
	PMIB_TCPEXTABLE pTcpExTable;
	
	char szLocalAddr[128] = {0};
	char szRemoteAddr[128] = {0};
	in_addr inadLocal, inadRemote;
	DWORD dwRemotePort = 0;
	TCHAR szExeFile[50] = {0};
	TCHAR szLocalPort[10] = {0};
	TCHAR szRemotePort[10] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	TCHAR    szState[128] = {0};
	SHFILEINFO  info = {0};

	if(!m_pAllocateAndGetTcpExTableFromStack) 
	{
		//win7 
		//AfxMessageBox(L"win7 tcp遍历开始");

		//modif by spf   去掉了！
		if (m_tcptablestack(&m_pTcpExTableVista, GetProcessHeap(), 1))
		{
			if (m_pTcpExTableVista)
			{
				HeapFree(GetProcessHeap(), 0, m_pTcpExTableVista);
			}

			FreeLibrary(m_hModule);
			m_hModule = NULL;
			return ;
		}

		for (UINT i=0;i<m_pTcpExTableVista->dwNumEntries;i++)
		{

			switch(m_pTcpExTableVista->table[i].dwState)
			{
			case MIB_TCP_STATE_CLOSED:    wcscpy_s(szState,128,L"CLOSED");break;
			case MIB_TCP_STATE_TIME_WAIT: wcscpy_s(szState,128, L"TIME_WAIT");break;
			case MIB_TCP_STATE_LAST_ACK:  wcscpy_s(szState,128, L"LAST_ACK");break;
			case MIB_TCP_STATE_CLOSING:   wcscpy_s(szState, 128,L"CLOSING");break;
			case MIB_TCP_STATE_CLOSE_WAIT:wcscpy_s(szState,128, L"CLOSE_WAIT");break;
			case MIB_TCP_STATE_FIN_WAIT1: wcscpy_s(szState,128,L"FIN_WAIT1");break;
			case MIB_TCP_STATE_ESTAB:     wcscpy_s(szState,128, L"ESTAB");break;
			case MIB_TCP_STATE_SYN_RCVD:  wcscpy_s(szState,128, L"SYN_RCVD");break;
			case MIB_TCP_STATE_SYN_SENT:  wcscpy_s(szState,128, L"SYN_SENT");break;
			case MIB_TCP_STATE_LISTEN:    wcscpy_s(szState,128,L"LISTEN");break;
			case MIB_TCP_STATE_DELETE_TCB:wcscpy_s(szState,128, L"DELETE");break;
			default:break;
			}

			inadLocal.s_addr = m_pTcpExTableVista->table[i].dwLocalAddr;
			inadRemote.s_addr = m_pTcpExTableVista->table[i].dwRemoteAddr;

			strcpy_s(szLocalAddr,128, inet_ntoa(inadLocal));
			strcpy_s(szRemoteAddr,128, inet_ntoa(inadRemote));

			if (wcscpy_s(szState,128, L"LISTEN") == 0)
			{
				dwRemotePort = 0;
			}
			else
			{
				dwRemotePort = m_pTcpExTableVista->table[i].dwRemotePort;
			}

			wsprintf(szLocalPort, L"%u" , ntohs((u_short)m_pTcpExTableVista->table[i].dwLocalPort));
			wsprintf(szRemotePort, L"%u", ntohs((u_short)m_pTcpExTableVista->table[i].dwRemotePort));
			GetProcessPathAndName(m_pTcpExTableVista->table[i].dwProcessID, szPath, szExeFile);
			SHGetFileInfo(szPath, 0, &info, sizeof(info), SHGFI_ICON | SHGFI_SYSICONINDEX);

			int index = m_listnetwork.InsertItem(0, szExeFile, m_ImageList.Add(info.hIcon));
			TCHAR Local[128] = {0};
			TCHAR Remote[128] = {0};
			int n = 128;
			MultiByteToUnicode(szLocalAddr,128,Local,n);
			n  = 128;
			MultiByteToUnicode(szRemoteAddr,128,Remote,n);
			m_listnetwork.SetItemText(index, 1, L"TCP");
			m_listnetwork.SetItemText(index, 2, Local);
			m_listnetwork.SetItemText(index, 3, szLocalPort);
			m_listnetwork.SetItemText(index, 4, Remote);
			m_listnetwork.SetItemText(index, 5, szRemotePort);
			m_listnetwork.SetItemText(index, 6, szState);
			m_listnetwork.SetItemText(index, 7, szPath);
			m_listnetwork.SetItemData(index, m_pTcpExTableVista->table[i].dwProcessID);
		}
	
	}

	else
	{
		//为xp，server2003
		if (ERROR_SUCCESS != m_pAllocateAndGetTcpExTableFromStack(&pTcpExTable, TRUE, GetProcessHeap(), 2, 2))
		{
			MessageBox(L"获取TCP列表失败！");
		}

		for (UINT i = 0; i < pTcpExTable->dwNumEntries; ++i)
		{
			switch (pTcpExTable->table[i].dwState)
			{
			case MIB_TCP_STATE_CLOSED:
				wcscpy_s(szState, 128,L"CLOSED");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				wcscpy_s(szState,128, L"TIME_WAIT");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				wcscpy_s(szState,128, L"LAST_ACK");
				break;
			case MIB_TCP_STATE_CLOSING:
				wcscpy_s(szState, 128,L"CLOSING");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				wcscpy_s(szState,128, L"CLOSE_WAIT");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				wcscpy_s(szState,128,L"FIN_WAIT1");
				break;
			case MIB_TCP_STATE_ESTAB:
				wcscpy_s(szState,128, L"ESTAB");
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				wcscpy_s(szState,128, L"SYN_RCVD");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				wcscpy_s(szState,128, L"SYN_SENT");
				break;
			case MIB_TCP_STATE_LISTEN:
				wcscpy_s(szState,128,L"LISTEN");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				wcscpy_s(szState,128, L"DELETE");
				break;
			default:
				break;
			}
			
			inadLocal.s_addr = pTcpExTable->table[i].dwLocalAddr;
			inadRemote.s_addr = pTcpExTable->table[i].dwRemoteAddr;

			strcpy_s(szLocalAddr,128, inet_ntoa(inadLocal));
			strcpy_s(szRemoteAddr,128, inet_ntoa(inadRemote));

			if (wcscpy_s(szState,128, L"LISTEN") == 0)
			{
				dwRemotePort = 0;
			}
			else
			{
				dwRemotePort = pTcpExTable->table[i].dwRemotePort;
			}

			wsprintf(szLocalPort, L"%u" , ntohs((u_short)pTcpExTable->table[i].dwLocalPort));
			wsprintf(szRemotePort, L"%u", ntohs((u_short)pTcpExTable->table[i].dwRemotePort));
			GetProcessPathAndName(pTcpExTable->table[i].dwProcessID, szPath, szExeFile);
			SHGetFileInfo(szPath, 0, &info, sizeof(info), SHGFI_ICON | SHGFI_SYSICONINDEX);

			int index = m_listnetwork.InsertItem(0, szExeFile, m_ImageList.Add(info.hIcon));
			TCHAR Local[128] = {0};
			TCHAR Remote[128] = {0};
			int n = 128;
			MultiByteToUnicode(szLocalAddr,128,Local,n);
			n  = 128;
			MultiByteToUnicode(szRemoteAddr,128,Remote,n);
			m_listnetwork.SetItemText(index, 1, L"TCP");
			m_listnetwork.SetItemText(index, 2, Local);
			m_listnetwork.SetItemText(index, 3, szLocalPort);
			m_listnetwork.SetItemText(index, 4, Remote);
			m_listnetwork.SetItemText(index, 5, szRemotePort);
			m_listnetwork.SetItemText(index, 6, szState);
			m_listnetwork.SetItemText(index, 7, szPath);
			m_listnetwork.SetItemData(index, pTcpExTable->table[i].dwProcessID);
		}
	}
}

void CNetWorkDlg::GetUdpTableEx()
{
	PMIB_UDPEXTABLE_VISTA pUdpExTable;	

	CHAR szLocalAddr[128] = {0};
	in_addr inadLocal;
	TCHAR szExeFile[50] = {0};
	TCHAR szLocalPort[10] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	SHFILEINFO  info = {0};

	if(!m_pAllocateAndGetUdpExTableFromStack)
	{
		//win7  serv2008
		for (UINT i = 0; i < m_pUdpExTableVista->dwNumEntries; ++i)
		{
			inadLocal.s_addr = m_pUdpExTableVista->table[i].dwLocalAddr;
			strcpy_s(szLocalAddr,128, inet_ntoa(inadLocal));
			wsprintf(szLocalPort, L"%u", ntohs((u_short)m_pUdpExTableVista->table[i].dwLocalPort));
			GetProcessPathAndName(m_pUdpExTableVista->table[i].dwProcessID, szPath, szExeFile);
			SHGetFileInfo(szPath, 0, &info, sizeof(info), SHGFI_ICON | SHGFI_SYSICONINDEX);
			int index = m_listnetwork.InsertItem(0, szExeFile, m_ImageList.Add(info.hIcon));
			m_listnetwork.SetItemText(index, 1, L"UDP");
			TCHAR stzlocal[128] ={0};
			int n  =128;
			MultiByteToUnicode(szLocalAddr,128,stzlocal,n);
			m_listnetwork.SetItemText(index, 2, stzlocal);
			m_listnetwork.SetItemText(index, 3, szLocalPort);
			m_listnetwork.SetItemText(index, 4, L"*.*.*.*");
			m_listnetwork.SetItemText(index, 5, L"0");
			//m_listnetwork.SetItemText(index,6,)
			m_listnetwork.SetItemText(index, 7, szPath);
			m_listnetwork.SetItemData(index, m_pUdpExTableVista->table[i].dwProcessID);
		}

	}
	else
	{
		if (ERROR_SUCCESS != m_pAllocateAndGetUdpExTableFromStack(&pUdpExTable, TRUE, GetProcessHeap(), 2, 2))
		{
			MessageBox(L"获取UDP列表失败！");
			return;
		}

		//如果系统为xp windows2003
		for (UINT i = 0; i < pUdpExTable->dwNumEntries; ++i)
		{
			inadLocal.s_addr = pUdpExTable->table[i].dwLocalAddr;
			strcpy_s(szLocalAddr,128, inet_ntoa(inadLocal));
			wsprintf(szLocalPort, L"%u", ntohs((u_short)pUdpExTable->table[i].dwLocalPort));
			GetProcessPathAndName(pUdpExTable->table[i].dwProcessID, szPath, szExeFile);
			SHGetFileInfo(szPath, 0, &info, sizeof(info), SHGFI_ICON | SHGFI_SYSICONINDEX);
			int index = m_listnetwork.InsertItem(0, szExeFile, m_ImageList.Add(info.hIcon));
			m_listnetwork.SetItemText(index, 1, L"UDP");
			TCHAR stzlocal[128] ={0};
			int n  =128;
			MultiByteToUnicode(szLocalAddr,128,stzlocal,n);
			m_listnetwork.SetItemText(index, 2, stzlocal);
			m_listnetwork.SetItemText(index, 3, szLocalPort);
			m_listnetwork.SetItemText(index, 4, L"*.*.*.*");
			m_listnetwork.SetItemText(index, 5, L"0");
			m_listnetwork.SetItemText(index, 7, szPath);
			m_listnetwork.SetItemData(index, pUdpExTable->table[i].dwProcessID);
		}

	}
}

void CNetWorkDlg::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
	m_listnetwork.SetRedraw(FALSE);
	m_listnetwork.DeleteAllItems();
	GetUdpTableEx();
	GetTcpTableEx();
	m_listnetwork.SetRedraw(TRUE);
	m_listnetwork.Invalidate();
	m_listnetwork.UpdateWindow();

}

void CNetWorkDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnBtnRefresh();
	CDialog::OnTimer(nIDEvent);
}

void CNetWorkDlg::OnBtnTerminateProc() 
{
	// TODO: Add your control notification handler code here

}


void CNetWorkDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point;
	::GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_MENUTASKMGR);
	CMenu* pPopup = menu.GetSubMenu(0);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
	*pResult = 0;
}


void CNetWorkDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = m_check.GetCheck();
	if (i == 1)
	{ 
		SetTimer(0, 5000, NULL);
	}

	else
	{
		KillTimer(0);
	}
}


void CNetWorkDlg::OnUpdateRefershlist(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnBtnRefresh();
}

void CNetWorkDlg::OnUpdateLocatepro(CCmdUI *pCmdUI)
{
	TCHAR  strPath[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	int index = m_listnetwork.GetSelectionMark();
	m_listnetwork.GetItemText(index, 7, szPath, MAX_PATH);

	wsprintfW(strPath,L"Explorer /select, %s", szPath);

	char szExePath[MAX_PATH] = {0};

	int exepath_len = 128;
	UnicodeToMultiByte(strPath,128,szExePath,exepath_len);
	WinExec(szExePath, SW_SHOWNORMAL);

}

void CNetWorkDlg::OnUpdateKillpro(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

	int index = m_listnetwork.GetSelectionMark();

	if (index < 0 || index > m_listnetwork.GetItemCount())
	{
		MessageBox(L"请选择相应的连接！");
		return;
	}
	DWORD dwProcessID = m_listnetwork.GetItemData(index);

	HANDLE hProcess =  OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess)
	{
		MessageBox(L"打开那进程失败！");
		return;
	}

	if (TerminateProcess(hProcess, 0))
	{
		MessageBox(L"结束进程成功！");
		OnBtnRefresh();
	}
	else
	{
		MessageBox(L"结束进程失败！");
	}
}
