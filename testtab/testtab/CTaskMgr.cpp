// dlg1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "CTaskMgr.h"
#include <Tlhelp32.h>
#include<Windows.h>
#include "FileVerion.h"

// Cdlg1 �Ի���

#define  TIMER_CPUSTART   10000
#define  TIMER_SAVECPU    10001
#define ID_TIMER_REFRESH 1000 
IMPLEMENT_DYNAMIC(CTaskMgrDlg, CDialog)

CTaskMgrDlg::CTaskMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskMgrDlg::IDD, pParent)
{

}

CTaskMgrDlg::~CTaskMgrDlg()
{
}

void CTaskMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//DDX_Control(pDX, IDC_LIST2, m_list2);
}


BEGIN_MESSAGE_MAP(CTaskMgrDlg, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTaskMgrDlg::OnNMRClickList1)
	ON_UPDATE_COMMAND_UI(ID_LocatePro, &CTaskMgrDlg::OnUpdateLocatepro)
	ON_UPDATE_COMMAND_UI(ID_KILLPRO, &CTaskMgrDlg::OnUpdateKillpro)
	ON_BN_CLICKED(IDC_CLEARMEM, &CTaskMgrDlg::OnBnClickedClearmem)
	ON_WM_NCRBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_BAIDUSEARCH, &CTaskMgrDlg::OnUpdateBaidusearch)
	ON_UPDATE_COMMAND_UI(ID_POPUP_KILLALL, &CTaskMgrDlg::OnUpdatePopupKillall)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_DLL, &CTaskMgrDlg::OnBnClickedCheckDll)
END_MESSAGE_MAP()


// Cdlg1 ��Ϣ�������

BOOL CTaskMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ��list�ؼ�
	m_list.InsertColumn(1,L"������",LVCFMT_CENTER,150);
	m_list.InsertColumn(2,L"����ID",LVCFMT_CENTER,80);
	m_list.InsertColumn(3,L"�ڴ��С",LVCFMT_CENTER,80);
	m_list.InsertColumn(4,L"CPUռ��%", LVCFMT_CENTER, 80);
	m_list.InsertColumn(5,L"����·��",LVCFMT_CENTER,500);
	m_list.InsertColumn(6,L"�ļ�����",LVCFMT_CENTER,200);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  //��������ȫ��ѡ��

	m_ImageList.Create(16, 16, ILC_COLOR16, 12, 0);	//����ͼ���б�
	m_ImageList.SetBkColor(RGB(255, 255 ,255));	//���ñ���ɫΪ��ɫ
	m_list.SetImageList(&m_ImageList, LVSIL_SMALL);	//�趨�б���ͼ�ؼ���ͼ���б�

	EnableDebugPriv();
	EmuProcess();
   	//CreateThread(NULL, 0, RefreshDataThread, this, 0, NULL);	//����һ���������ݵ��̣߳�������д��OnTimer�е�
	SetTimer(ID_TIMER_REFRESH,1000,0);
	//�������̵��ڴ�
	HANDLE hProcess = ::GetCurrentProcess();
	SetProcessWorkingSetSize(hProcess, -1, -1);

	EmptyWorkingSet(hProcess);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//����ǹؼ�
void CTaskMgrDlg::RefreshData()
{ 
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		m_list.SetItemText(i,6,_T("FALSE"));  //���flase
	}
   
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcessSnap);
		return;
	}

	PROCESSENTRY32 pe32 = {0};
	pe32.dwSize = sizeof(pe32);
	SHFILEINFO  info = {0};
    DWORD dwMemoryUse = 0;
	PROCITEM_S  * pProcItem ;
    BOOL    bIsExist = TRUE;
	CString strProcessID;
	CString strCntThread;
	CString strMemUse;
	CString strExePath;

	BOOL bProcess = Process32First(hProcessSnap, &pe32);
	if (bProcess == FALSE)
	{
		CloseHandle(hProcessSnap);
		return;
	}

	while(bProcess)
	{
       dwMemoryUse =  GetProcessMemoyUse(pe32.th32ProcessID);

	    for(int i=0; i<m_list.GetItemCount() ;i++)
		{ 
          	pProcItem = (PROCITEM_S *)m_list.GetItemData(i);

		   if(pProcItem->dwProID == pe32.th32ProcessID && 
			wcscmp(pProcItem->szExeFile,pe32.szExeFile) ==0)
		   {
              //ԭ�ؼ�ˢ����
			   bIsExist = TRUE;
			   m_list.SetItemText(i, 6, _T("TRUE"));
			   if (pProcItem->cntThreads != pe32.cntThreads)
			   {
				   strCntThread.Format(_T("%d"), pe32.cntThreads);
				   m_list.SetItemText(i, 2, strCntThread);
			   }

			   if (pProcItem->dwMemoryUse != dwMemoryUse)
			   {
				   strMemUse.Format(_T("%dK"), dwMemoryUse);
				   m_list.SetItemText(i, 3, strMemUse);
			   }
		   }
		   else
		   {
			     
			   bIsExist = FALSE;
		   }
		}
	}

	if(bIsExist == FALSE)
	{
		PROCITEM_S * pPi = new PROCITEM_S;
		pPi->dwProID = pe32.th32ProcessID;
	    wcscpy(pPi->szExeFile, pe32.szExeFile);
		pPi->cntThreads = pe32.cntThreads;
		pPi->dwMemoryUse = GetProcessMemoyUse(pe32.th32ProcessID);

		SHGetFileInfo(GetProcessPath(pe32.th32ProcessID), 0, &info, sizeof(info), 
			SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
  
		int idx = m_list.GetItemCount();
		strProcessID.Format(_T("%d"), pe32.th32ProcessID);
		strCntThread.Format(_T("%d"), pe32.cntThreads);
		strMemUse.Format(_T("%d K"), dwMemoryUse);
		strExePath = GetProcessPath(pe32.th32ProcessID);

		m_list.InsertItem(idx, pe32.szExeFile, m_ImageList.Add(info.hIcon));
		m_list.SetItemText(idx, 1, strProcessID);
		m_list.SetItemText(idx, 2, strMemUse);
		m_list.SetItemText(idx, 3, strExePath);
		//m_List.SetItemText(idx, 4, "TRUE");
		m_list.SetItemData(idx, (DWORD)pPi);
	}
	
    
}

DWORD WINAPI RefreshDataThread(LPVOID lpParameter)
{
	CTaskMgrDlg* pThis = (CTaskMgrDlg*)lpParameter;

	while(TRUE)
	{

		pThis->RefreshData();
		Sleep(1000);
	}
}


void CTaskMgrDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CPoint point;
	GetCursorPos( & point );
	CMenu m_menu;
	VERIFY( m_menu.LoadMenu( IDR_MENUTASKMGR) );
	CMenu* popup = m_menu.GetSubMenu(0);
	ASSERT( popup != NULL );
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	*pResult = 0;
}

//��Ȩ��������DEBUG
BOOL CTaskMgrDlg::EnableDebugPriv()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp = {0};
	if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		MessageBox(L"�򿪽�������ʧ�ܣ�");
		return FALSE;
	}
		//ZwQueryProcessInform

	if (!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
	{
		MessageBox(L"�鿴ϵͳȨ�޵���Ȩֵʧ�ܣ�");
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;	

	if (!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		::CloseHandle(hToken);
		MessageBox(L"��Ȩʧ�ܣ�");
		return FALSE;
	}
	return TRUE;
}




void FreeMem() 
{
	/*typedef LONG (WINAPI *ZWQUERYSYSTEMINFORMATION)(UINT, PVOID, ULONG, PULONG);
	ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;
	HINSTANCE hDll = NULL;
	PVOID pBuffer = NULL;
	ULONG cbBuffer = 0x8000;
	HANDLE hHeap;
	LONG Status;
	PSYSTEM_PROCESSES pProcesses;
	HANDLE hProcess;

	if ((hDll = LoadLibrary(TEXT("ntdll.dll"))) == NULL) {
		return;
	}

	if ((ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(hDll,
		"ZwQuerySystemInformation")) == NULL) {
			FreeLibrary(hDll);
			return;
	}

	// ��ȡϵͳ������Ϣ
	hHeap = GetProcessHeap();     //����ѿռ�

	do {
		pBuffer = HeapAlloc(hHeap, 0, cbBuffer);
		if (!pBuffer) {
			FreeLibrary(hDll);
			return;
		}

		Status = ZwQuerySystemInformation(5, pBuffer, cbBuffer, NULL);
		if (Status == STATUS_INFO_LENGTH_MISMATCH) 
		{
			HeapFree(hHeap, 0, pBuffer);
			pBuffer = NULL;
			cbBuffer *= 2;
		} 
		else 
			if(!NT_SUCCESS(Status)) 
		{
			HeapFree(hHeap, 0, pBuffer);
			FreeLibrary(hDll);
			return;
		}
	} while (Status == STATUS_INFO_LENGTH_MISMATCH);

	// ѭ�����ÿ�����̵Ĺ�����
	pProcesses = (PSYSTEM_PROCESSES)pBuffer;
	for (;;) {

		// �ж��Ƿ�����һ����ͬ��ʵ��
		if  ((lstrcmp(pProcesses->ProcessName.Buffer, L"tasttab.exe") == 0)
			&& (pProcesses->ProcessId != GetCurrentProcessId())) 
		{
			if ((hProcess = OpenProcess(PROCESS_TERMINATE, FALSE,
				(DWORD)(pProcesses->ProcessId))) != NULL) 
			{
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		} 

		else {
			if ((hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE,
				(DWORD)(pProcesses->ProcessId))) != NULL)
			{
				SetProcessWorkingSetSize(hProcess, (DWORD)-1, (DWORD)-1);
				CloseHandle(hProcess);
			}
		}

		if (pProcesses->NextEntryDelta == 0)
			break;
		pProcesses = (PSYSTEM_PROCESSES)((LPBYTE)pProcesses + pProcesses->NextEntryDelta);
	}

	HeapFree(hHeap, 0, pBuffer);
	FreeLibrary(hDll);
	return;
*/
}



BOOL  CTaskMgrDlg::EmuProcess(void )
{
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (INVALID_HANDLE_VALUE == hProcessSnap)
	{ 
		MessageBox(L"�������̿���ʧ�ܣ�");
		return 0;
	}

	PROCESSENTRY32 pinfo;
	HANDLE  hModule =NULL;
	CString  shortpath;
	CString pzxfile;
	CString strMemUse = L"";
	SHFILEINFO  info = {0};
	int i = 1;
	int index = 0;
	CString  processid = L"1234";
	PROCITEM_S *  myProcess;
	pinfo.dwSize= sizeof(pinfo);  //�������Ҫ

	BOOL report = Process32First(hProcessSnap,&pinfo);
	if(!report)
	{
		CloseHandle(hProcessSnap);
		ASSERT(-1);
		return 0;
	}
	
	while(report)   
	{   
		myProcess = new  PROCITEM_S;
		myProcess->dwProID = pinfo.th32ProcessID;
		wcscpy_s(myProcess->szExeFile,100, pinfo.szExeFile);
		myProcess->cntThreads = pinfo.cntThreads;
		myProcess->dwMemoryUse = GetProcessMemoyUse(myProcess->dwProID);

		SHGetFileInfo(GetProcessPath(myProcess->dwProID), 0, &info, sizeof(info), 
			SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

		index = m_list.InsertItem(i,pinfo.szExeFile,m_ImageList.Add(info.hIcon));  //��������
		i++;
		processid.Format(L"%d",myProcess->dwProID);
		m_list.SetItemText(index,1, processid);
		strMemUse.Format(L"%d", GetProcessMemoyUse(myProcess->dwProID));
		strMemUse += L" K";
		m_list.SetItemText(index,2, strMemUse);
	
		SetTimer(TIMER_CPUSTART,1000,0);
		SetTimer(TIMER_SAVECPU,2000,0);
		
        
		if(!wcscmp(myProcess->szExeFile,L"smss.exe"))
		{
			m_list.SetItemText(index,4,L"C:\\WINDOWS\\system32\\smss.exe"); 
		}

		else 
		{
			if(!wcscmp(myProcess->szExeFile,L"system")||!wcscmp(myProcess->szExeFile,L"[System Process]")||!wcscmp(myProcess->szExeFile,L"audiodg.exe"))
			{
				m_list.SetItemText(index,0,L"Idle");
				m_list.SetItemText(index,4,L"NT OS Kernel");
				m_list.SetItemText(index,5,L"�Ͳ�������");
			}
			else
			{
				if(!wcscmp(myProcess->szExeFile,L"system")||!wcscmp(myProcess->szExeFile,L"System"))
				{
					m_list.SetItemText(index,4,L"NT OS Kernel");
						m_list.SetItemText(index,5,L"�Ͳ�������");
				}
				else
				{
					m_list.SetItemText(index,4,GetProcessPath(myProcess->dwProID));
					CFileVersion   cversion;
					cversion.Open(GetProcessPath(myProcess->dwProID).GetBuffer());
                    m_list.SetItemText(index,5, cversion.GetCompanyName());
				}
			}
		}

		//��ȡcpu����Ϣ
		report = Process32Next(hProcessSnap, &pinfo);     
		if(myProcess)
		{
			delete  myProcess;
		}
	} 

	CloseHandle(hProcessSnap);
	return 0 ;
}

//��ȡdwProcessID��ʶ�Ľ���·��
CString CTaskMgrDlg::GetProcessPath(DWORD dwProcessID)
{
	CString strShortPath;
	char szLongPath[MAX_PATH] = {0};///cstring  �Ļ������,csring������
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
	if (hProcess == NULL)
	{	
		return ((LPCTSTR)(LPCSTR)szLongPath);
	}

	DWORD dwNeeded;
	HMODULE hModule = NULL;
	if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwNeeded))
	{
		DWORD dwRet = GetModuleFileNameEx(hProcess, NULL, strShortPath.GetBuffer(MAX_PATH), MAX_PATH);
		strShortPath.ReleaseBuffer();
		int n = strShortPath.Find(L":\\");
		strShortPath = strShortPath.Mid(n-1);
		GetLongPathName(strShortPath, (LPWSTR)szLongPath, MAX_PATH);
	}
	CloseHandle(hProcess);
	return ((LPCTSTR)(LPCSTR)szLongPath);
}


DWORD CTaskMgrDlg::GetProcessMemoyUse(DWORD dwProcessID)
{
	PROCESS_MEMORY_COUNTERS pmc = {0};
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
	if (hProcess == NULL)
	{
		return 0;
	}
	BOOL bGetMem = GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));

	if (!bGetMem)
	{
		AfxMessageBox(L"�ڴ����");
		return 0;
	}

	return (pmc.WorkingSetSize/1024);
}

__int64 CTaskMgrDlg::FileTimeToInt64(const FILETIME& time)
{
	ULARGE_INTEGER tt;
	tt.LowPart = time.dwLowDateTime;
	tt.HighPart = time.dwHighDateTime;
	return(tt.QuadPart);
}


double CTaskMgrDlg::GetCpuUsage( int iProcessID )
{

	static int s_processor_count = 0;	//cpu����
	static __int64 s_last_time = 0;  	//��һ�ε�ʱ��
	static __int64 s_last_system_time = 0;
	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	__int64 system_time;
	__int64 time;

	double dCpu = -1;
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	if(s_processor_count == 0)
	{
		s_processor_count =  info.dwNumberOfProcessors;
	}

	GetSystemTimeAsFileTime(&now);

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, iProcessID);
	if (!hProcess)
	{
		CloseHandle(hProcess);
		return -1;
	}

	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		CloseHandle(hProcess);
		return -1;
	}

	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / s_processor_count;  //CPUʹ��ʱ��
	time = FileTimeToInt64(now);		//���ڵ�ʱ��

	s_last_system_time = system_time;
	s_last_time = time;
	CloseHandle( hProcess );
  

	//Sleep();  

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, iProcessID);
	if (!hProcess)
	{
		CloseHandle(hProcess);
		return -1;
	}
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		CloseHandle(hProcess);
		return -1;
	}
	GetSystemTimeAsFileTime(&now);
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time))/s_processor_count;  //CPUʹ��ʱ��
	time = FileTimeToInt64(now);		//���ڵ�ʱ��
	CloseHandle( hProcess );
	dCpu = ((double)(system_time - s_last_system_time) / (double)(time - s_last_time)) * 100;
	return dCpu;

}

void CTaskMgrDlg::OnUpdateLocatepro(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

	TCHAR  strPath[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	int index = m_list.GetSelectionMark();
	m_list.GetItemText(index, 4, szPath, MAX_PATH);

   wsprintfW(strPath,L"Explorer /select, %s", szPath);
  
   char szExePath[MAX_PATH] = {0};
   
   int exepath_len = 128;
   UnicodeToMultiByte(strPath,128,szExePath,exepath_len);
   WinExec(szExePath, SW_SHOWNORMAL);
}

void CTaskMgrDlg::OnUpdateKillpro(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

	DWORD dwProcess;
	TCHAR  szBuf[MAX_PATH] ={0};

	int index = m_list.GetSelectionMark();
	m_list.GetItemText(index, 1, szBuf, MAX_PATH);
	dwProcess = _wtol(szBuf); //��ȡ����ID

	HANDLE hProcess =  OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcess);
	if (!hProcess)
	{
		MessageBox(L"���ǽ��̾��ʧ�ܣ�");
		return ;
	}

	if (TerminateProcess(hProcess, 0))
	{
		PROCITEM_S * pProcItem;
		pProcItem = (PROCITEM_S*)m_list.GetItemData(index);
		delete pProcItem;
		m_list.DeleteItem(index);
	}
	else
	{
		MessageBox(L"��������ʧ�ܣ�");
	}
}

void CTaskMgrDlg::OnBnClickedClearmem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ���DebugȨ��
	//EnablePrivilege();

/*	EnableDebugPriv();
	// ÿ��5�����Ż�һ��
		FreeMem();
		Sleep(300000);

	 AfxMessageBox(L"�ڴ��������");
	 */
}


void CTaskMgrDlg::OnUpdateBaidusearch(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

	DWORD dwProcess;
	TCHAR  szBuf[MAX_PATH] ={0};

	int index = m_list.GetSelectionMark();
	m_list.GetItemText(index, 0, szBuf, MAX_PATH);

	TCHAR sz[MAX_PATH] = {0}; 

	wsprintf(sz,L"http://www.baidu.com/s?wd=%s&rsv_spt=1&issp=1&rsv_bp=0&ie=utf-8&tn=baiduhome_pg&rsv_sug3=3&rsv_sug=0&rsv_sug1=3&rsv_sug4=266&inputT=5488",szBuf);
	ShellExecute(m_hWnd,L"open",sz,NULL,NULL,SW_NORMAL);	

} 
void CTaskMgrDlg::OnUpdatePopupKillall(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������a
	int index = m_list.GetSelectionMark();
		TCHAR  szBuf[MAX_PATH] ={0};
	m_list.GetItemText(index, 0, szBuf, MAX_PATH);
 
	CString strcmd;
	strcmd.Format(L"taskkill -f -im %s",szBuf);
	USES_CONVERSION;
	system(CW2A(strcmd));
}

void CTaskMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
     if(nIDEvent == ID_TIMER_REFRESH)
	 {
		 CString strMemUse;
		 MEMORYSTATUS ms = {0};
		 GlobalMemoryStatus(&ms);
		 ms.dwLength = sizeof(MEMORYSTATUS);
		 strMemUse.Format(L"%d", ms.dwMemoryLoad);
		 strMemUse = L"�ڴ�ʹ�ã�" + strMemUse + L"%";
		 SetDlgItemText(IDC_MEM_USE, strMemUse);

		 CString strCntProcess;
		 strCntProcess.Format(L"��������%d", m_list.GetItemCount());
		 SetDlgItemText(IDC_NUM_PROC,strCntProcess);
	 }

	 else if(nIDEvent== TIMER_CPUSTART)
	 {
        m_process.Start();

	 }
	 else if(nIDEvent == TIMER_SAVECPU)
	 {

		 MAP_CPUMEM_INFO mapInfo;
		 m_process.GetInfoMap(mapInfo);
		 for (MAP_CPUMEM_INFO::iterator it=mapInfo.begin(); it!=mapInfo.end(); ++it)
		 {
			 int			iProcessId		= it->second.iProcessId;
			 CString		strProcessName	= CA2T(it->second.strProcessName.c_str());
			 double		dbCpuUsage		= it->second.dbCpuUsage;
			 double		dbMemUseage		= it->second.dbMemUsage;

			 int iCpuPercent = (int)m_process.CpuToPercent(dbCpuUsage);
			 int iMemPercent = (int)m_process.MemToPercent(dbMemUseage);
		      
			for(int i =0 ;i<m_list.GetItemCount();i++)
			{
				CString cs = m_list.GetItemText(i,0); 
				USES_CONVERSION;
				CString csProcessname  = A2CW(it->second.strProcessName.c_str());
				if(!cs.Compare(csProcessname))
				{
					cs.Format(L"%d",iCpuPercent);
					m_list.SetItemText(i,3,cs);
				}
			}

			 //m_list.SetDlgItemText()

		 }

	 }
	 
	CDialog::OnTimer(nIDEvent);
}



void CTaskMgrDlg::OnBnClickedCheckDll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int isCheck = ((CButton*)GetDlgItem(IDC_CHECK_DLL))->GetCheck();
	CRect rect1, rect2;
	GetDlgItem(IDC_LIST1)->GetWindowRect(&rect1);
	GetDlgItem(IDC_LIST2)->GetWindowRect(&rect2);
	int height = rect2.Height();

	if (isCheck == 1)
	{
		GetDlgItem(IDC_LIST1)->SetWindowPos(NULL,rect1.left, rect1.top, 
			rect1.Width(), rect1.Height() - height, SWP_NOZORDER | SWP_NOMOVE );
		GetDlgItem(IDC_LIST2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_LIST2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST1)->SetWindowPos(NULL,rect1.left, rect1.top,
			rect1.Width(), rect1.Height() + height, SWP_NOZORDER | SWP_NOMOVE );
	}
}

