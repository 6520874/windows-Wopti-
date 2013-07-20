// FileExplorer.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "FileExplorer.h"
#include "HSelectDlg.h"
#include "HBrowseFolder.h"

// CFileExplorer 对话框

IMPLEMENT_DYNAMIC(CFileExplorer, CDialog)


CFileExplorer::CFileExplorer(CWnd* pParent /*=NULL*/)
: CDialog(CFileExplorer::IDD, pParent)
{
	m_hWatchThread = NULL;	
	m_hDirectory = INVALID_HANDLE_VALUE;

	m_vecCI.clear();

}

CFileExplorer::~CFileExplorer()
{
	pCheckInfo PI;
	for (size_t i = 0; i < m_vecCI.size(); i++)
	{
		PI = NULL;
		PI = m_vecCI[i];
		if (NULL != PI)
			delete PI;
	}
	m_vecCI.clear();

	if (m_pszListIndex != NULL)
	{
		delete m_pszListIndex;
		m_pszListIndex = NULL;
	}

	if(m_hWatchThread !=NULL)
	{
		::TerminateThread(m_hWatchThread, 0 );
		m_hWatchThread = NULL;
	}

	if(m_hDirectory !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDirectory);
		m_hDirectory = INVALID_HANDLE_VALUE;
	}
}

void CFileExplorer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTEN, m_listen);
}


BEGIN_MESSAGE_MAP(CFileExplorer, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileExplorer::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_STARTMONITOR, &CFileExplorer::OnBnClickedStartmonitor)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CFileExplorer::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFileExplorer::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CHOICEPATH, &CFileExplorer::OnBnClickedButtonChoicepath)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLEARALL, &CFileExplorer::OnBnClickedButtonClearall)
	ON_NOTIFY(NM_CLICK, IDC_LISTEN, &CFileExplorer::OnNMClickListen)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CFileExplorer::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_CHECK1, &CFileExplorer::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CFileExplorer 消息处理程序

void CFileExplorer::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CFileExplorer::OnBnClickedStartmonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strWatchedDir.IsEmpty())
	{
		AfxMessageBox(L"请选择监视目录！");
		return;
	}
	this->StartWatch(m_strWatchedDir);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	GetDlgItem(IDC_STARTMONITOR)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CHOICEPATH)->EnableWindow(false);
	CString strBuffer = _T("正在监视...    ");
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer+m_strWatchedDir);

}


//开始监视线程
BOOL CFileExplorer::StartWatch( CString strPath )
{
	m_strWatchedDir = strPath;  
	DWORD ThreadId; 
	m_hWatchThread = ::CreateThread(NULL,0,ThreadWatchProc,this,0,&ThreadId);
	return (NULL!=m_hWatchThread);
}

void CFileExplorer::OnBnClickedButtonOption()
{
	CHSelectDlg sel;

	if(sel.DoModal() == IDOK)
	{
	}
}

//监视线程
DWORD WINAPI CFileExplorer::ThreadWatchProc( LPVOID lParam )
{
	CFileExplorer *obj = (CFileExplorer *)lParam;  

    obj->m_hDirectory = CreateFile(            //打开目录，得到目录的句柄
		obj->m_strWatchedDir,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
		); 

	if(obj->m_hDirectory ==INVALID_HANDLE_VALUE) 
		return false;

	char buf[(sizeof(FILE_NOTIFY_INFORMATION)+MAX_PATH)*2] = {0};

	FILE_NOTIFY_INFORMATION* pNotify=(FILE_NOTIFY_INFORMATION*)buf;

	DWORD dwBytesReturned;        
	while(true)
	{
		if(::ReadDirectoryChangesW(obj->m_hDirectory,
			pNotify,
			sizeof(buf),
			true,
			FILE_NOTIFY_CHANGE_FILE_NAME|
			FILE_NOTIFY_CHANGE_DIR_NAME|
			FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_SIZE|
			FILE_NOTIFY_CHANGE_LAST_WRITE|
			FILE_NOTIFY_CHANGE_LAST_ACCESS|
			FILE_NOTIFY_CHANGE_CREATION|
			FILE_NOTIFY_CHANGE_SECURITY,
			&dwBytesReturned,
			NULL,
			NULL))
		{
			TCHAR tmp[MAX_PATH], str1[MAX_PATH], str2[MAX_PATH];
			memset(tmp, 0, sizeof(tmp));

			int n =  260;
			char char_temp[MAX_PATH] = {0};
		    UnicodeToMultiByte(tmp,MAX_PATH,char_temp,n);

			WideCharToMultiByte(CP_ACP,0,pNotify->FileName,pNotify->FileNameLength/2,char_temp,99,NULL,NULL);
 
			 n = 260;
		    MultiByteToUnicode(char_temp,MAX_PATH,tmp,n);
			wcscpy_s(str1, MAX_PATH,tmp);
			if(pNotify->NextEntryOffset != 0)
			{
				PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pNotify+pNotify->NextEntryOffset);
				memset(tmp, 0, sizeof(tmp));
				n = 260;
			    UnicodeToMultiByte(tmp,MAX_PATH,char_temp,n);
				WideCharToMultiByte(CP_ACP,0,p->FileName,p->FileNameLength/2,char_temp,99,NULL,NULL);
				wcscpy_s(str2, MAX_PATH,tmp);
			}

			switch(pNotify->Action)
			{				
			case FILE_ACTION_ADDED:
				if(obj->m_bAddNew)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listen.InsertItem(0,obj->m_pszListIndex);
					obj->m_listen.SetItemText(0,2,_T("添加了新文件"));

					obj->m_listen.SetItemText(0,3,str1);
					obj->m_listen.SetItemText(0,1,strTT);

					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					wcscpy_s(PI->szIndex,MAX_PATH,obj->m_pszListIndex);
					wcscpy_s(PI->szTime,MAX_PATH,strTT);
					wcscpy_s(PI->szType,MAX_PATH,_T("添加了新文件"));
					wcscpy_s(PI->szDetail,MAX_PATH,str1);

					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_REMOVED:
				if(obj->m_bDelete)
				{
					CTime tt=CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listen.InsertItem(0,obj->m_pszListIndex);
					obj->m_listen.SetItemText(0,2,_T("删除了文件"));
					obj->m_listen.SetItemText(0,3,str1);
					obj->m_listen.SetItemText(0,1,strTT);

					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					wcscpy_s(PI->szIndex,MAX_PATH,obj->m_pszListIndex);
					wcscpy_s(PI->szTime,MAX_PATH,strTT);
					wcscpy_s(PI->szType,MAX_PATH,_T("删除了文件"));
					wcscpy_s(PI->szDetail,MAX_PATH,str1);
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_RENAMED_NEW_NAME:
				if(obj->m_bRename)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listen.InsertItem(0,obj->m_pszListIndex);
					obj->m_listen.SetItemText(0,2,_T("重命名了文件"));
					wcscat_s(str1,MAX_PATH,_T("->"));
					obj->m_listen.SetItemText(0,3,wcscat(str1,str2));
					obj->m_listen.SetItemText(0,1,strTT);

					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					wcscpy_s(PI->szIndex,MAX_PATH,obj->m_pszListIndex);
					wcscpy_s(PI->szTime,MAX_PATH,strTT);
					wcscpy_s(PI->szType,MAX_PATH,_T("重命名了文件"));
					wcscpy_s(PI->szDetail,wcscat(str1,str2));
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
				if(obj->m_bRename)
				{
					CTime tt=CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listen.InsertItem(0,obj->m_pszListIndex);
					obj->m_listen.SetItemText(0,2,_T("重命名了文件"));
					wcscat_s(str1,MAX_PATH,_T(" 改名为 "));
					obj->m_listen.SetItemText(0,3,wcscat(str1,str2));
					obj->m_listen.SetItemText(0,1,strTT);
					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					wcscpy_s(PI->szIndex,obj->m_pszListIndex);
					wcscpy_s(PI->szTime,strTT);
					wcscpy_s(PI->szType,_T("重命名了文件"));
					wcscpy_s(PI->szDetail,wcscat(str1,str2));
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_MODIFIED:
				if(obj->m_bModify)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listen.InsertItem(0,obj->m_pszListIndex);
					obj->m_listen.SetItemText(0,2,_T("修改了文件"));
					obj->m_listen.SetItemText(0,3,str1);
					obj->m_listen.SetItemText(0,1,strTT);

					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					wcscpy_s(PI->szIndex,obj->m_pszListIndex);
					wcscpy_s(PI->szTime,strTT);
					wcscpy_s(PI->szType,_T("修改了文件"));
					wcscpy_s(PI->szDetail,str1);
					obj->m_vecCI.push_back(PI);
				}
				break;  

			default:
				if(obj->m_bOther)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listen.InsertItem(0,obj->m_pszListIndex);
					obj->m_listen.SetItemText(0,2,_T("未知变化"));
					obj->m_listen.SetItemText(0,3,_T(""));
					obj->m_listen.SetItemText(0,1,strTT);

					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					wcscpy_s(PI->szIndex,obj->m_pszListIndex);
					wcscpy_s(PI->szTime,strTT);
					wcscpy_s(PI->szType,_T("未知变化"));
					wcscpy_s(PI->szDetail,_T(""));
					obj->m_vecCI.push_back(PI);
				}
				break;  
			}	
			obj->m_iListCount++;		
			_itow(obj->m_iListCount,obj->m_pszListIndex,10);		
		}
		else           
			break;           
	}
	obj->m_iListCount = 1;

	return 0;
}


BOOL CFileExplorer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listen.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_listen.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);
	m_listen.InsertColumn(0,_T("数量"),LVCFMT_LEFT,40);
	m_listen.InsertColumn(1,_T("时间"),LVCFMT_LEFT,80);
	m_listen.InsertColumn(2,_T("类型"),LVCFMT_LEFT,100);
	m_listen.InsertColumn(3,_T("变化的内容"),LVCFMT_LEFT,500);

	LOGFONT lf;
	::ZeroMemory(&lf,sizeof(lf)); 
	lf.lfHeight = 90;         
	lf.lfWeight = FW_NORMAL;   
	lf.lfItalic = false;	
	::lstrcpy(lf.lfFaceName,_T("Verdana")); 
	m_hFont.CreatePointFontIndirect(&lf);
	GetDlgItem(IDC_STATIC_PATH)->SetFont(&m_hFont);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	m_bAddNew = (::GetPrivateProfileInt(L"Selections",L"AddNew",0,L".\\Settings.ini")==0 ?false:true);
	m_bDelete = (::GetPrivateProfileInt(L"Selections",L"Delete",0,L".\\Settings.ini")==0 ?false:true);
	m_bRename = (::GetPrivateProfileInt(L"Selections",L"Rename",0,L".\\Settings.ini")==0 ?false:true);
	m_bModify = (::GetPrivateProfileInt(L"Selections",L"Modify",0,L".\\Settings.ini")==0 ?false:true);
	m_bOther  = (::GetPrivateProfileInt(L"Selections",L"Other",0,L".\\Settings.ini")==0 ?false:true);

	TCHAR buf[MAX_PATH];
	::GetPrivateProfileString(L"Settings",L"WatchDir",L"",buf,MAX_PATH,L".\\Settings.ini");
	m_strWatchedDir.Format(L"%s",buf);
	CString strBuffer = _T("你选择了监视 ");
	m_strWatchedDir.FreeExtra();
	strBuffer += m_strWatchedDir;
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer);

	m_iListCount = 1;
	m_pszListIndex = new TCHAR[MAX_PATH];
	_itow(m_iListCount,m_pszListIndex,10);

	//::RegisterHotKey(m_hWnd,1121,MOD_ALT,'W');
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFileExplorer::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strBuffer = _T("停止了监视 ");
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer+m_strWatchedDir);
	if(m_hWatchThread != NULL)
	{
		::TerminateThread(m_hWatchThread,0);
		m_hWatchThread = NULL;
	}
	if(m_hDirectory != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDirectory);
		m_hDirectory = INVALID_HANDLE_VALUE;
	}
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_STARTMONITOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CHOICEPATH)->EnableWindow(TRUE);
}

void CFileExplorer::OnBnClickedButtonChoicepath()
{
	// TODO: 在此添加控件通知处理程序代码

	CHBrowseFolder Dlg;
	if(Dlg.DoModal(this,NULL) == IDOK)
	{
		m_strWatchedDir.Format(L"%s",Dlg.GetDirPath());
		CString strBuffer = _T("你选择了监视 ");
		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer+m_strWatchedDir);
		::WritePrivateProfileString(L"Settings",L"WatchDir",m_strWatchedDir,L".\\Settings.ini");
	}
}

//控件颜色
HBRUSH CFileExplorer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


if(pWnd->GetDlgCtrlID() == IDC_STATIC_PATH)
		pDC->SetTextColor(RGB(255,0,255));
	return hbr;
}

void CFileExplorer::OnBnClickedButtonClearall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listen.DeleteAllItems();

	m_iListCount = 1;

	pCheckInfo PI;
	for (size_t i = 0; i < m_vecCI.size(); i++)
	{
		PI = NULL;
		PI = m_vecCI[i];
		if (NULL != PI)
			delete PI;
	}
	m_vecCI.clear();
}

void CFileExplorer::OnNMClickListen(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_listen.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if(nIndex == -1)
		return;

	TCHAR buf[MAX_PATH];
	m_listen.GetItemText(nIndex,3,buf,MAX_PATH);
	CString str;	
	str = this->m_strWatchedDir+L"\\"+buf;
	::ShellExecute(NULL,L"open",str,NULL,NULL,SW_SHOW);//直接打开这个程序
	*pResult = 0;
}

void CFileExplorer::OnBnClickedButtonExport()
{
	// 日志文件进行导出
	pCheckInfo PI;

for (size_t i =0;i< m_vecCI.size();i++)
{
   


}
 

}

void CFileExplorer::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}
