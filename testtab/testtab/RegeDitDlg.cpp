// RegeDitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "RegeDitDlg.h"


/***************
/* ע�������˼·
/* ���ҹ̶�һ�����µ�ֵ
�����ַ�����ֵ 
������//���ŵ��ַ���
�еĻ����������·���ڱ��ص����Ƿ���ڣ�
��������ڣ���Ϊ��������
/************************************************************************/

// CRegeDitDlg �Ի���

IMPLEMENT_DYNAMIC(CRegeDitDlg, CDialog)

CRegeDitDlg::CRegeDitDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRegeDitDlg::IDD, pParent)
{

	m_Ilistctrlindex = 0;
	m_IvaildstartmenuNum = 0;
	m_j  = 0;
}

CRegeDitDlg::~CRegeDitDlg()
{
}

void CRegeDitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECKNOSTARTMENU, m_Button_nostartmenu);
	DDX_Control(pDX, IDC_STATIC_SCANREGPATH, m_Staticscanregpath);
	DDX_Control(pDX, IDC_CHECKBIGFILE, m_Buttonbigfile);
	DDX_Control(pDX, IDC_LIST2, m_clearitmelist);
	DDX_Control(pDX, IDC_STATIC_VAILDSTARTMENU, m_validMenu);
	DDX_Control(pDX,IDC_CHECKSOFTINFO,m_Button_invalidSoft);
	DDX_Control(pDX, IDC_TREE2, m_DiskTree);
}


BEGIN_MESSAGE_MAP(CRegeDitDlg, CDialog)

	ON_BN_CLICKED(IDC_CLEARREG, &CRegeDitDlg::OnBnClickedClearreg)
	ON_BN_CLICKED(IDC_OPENREG, &CRegeDitDlg::OnBnClickedOpenreg)
	ON_WM_CTLCOLOR()
	ON_UPDATE_COMMAND_UI(ID_DLETEITEM, &CRegeDitDlg::OnUpdateDleteitem)
	ON_UPDATE_COMMAND_UI(ID_OPENITEM, &CRegeDitDlg::OnUpdateOpenitem)
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CRegeDitDlg::OnNMRClickList)
	ON_BN_CLICKED(IDC_BUTTON_STOPSCAN, &CRegeDitDlg::OnBnClickedButtonStopScan)
END_MESSAGE_MAP()


// CRegeDitDlg ��Ϣ�������

BOOL CRegeDitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_clearitmelist.InsertColumn(0,L"������",LVCFMT_CENTER,450);
	m_clearitmelist.InsertColumn(1,L"·��",LVCFMT_CENTER,450);
	m_clearitmelist.InsertColumn(2,L"��СMB",LVCFMT_CENTER,150);
	m_clearitmelist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  //��������ȫ��ѡ��
	m_clearitmelist.SetBkColor(RGB(255, 255 ,0));	//���ñ���ɫΪ��ɫ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CRegeDitDlg::OnBnClickedClearreg()
{

	//ֻ�ܵ�һ���������
	//��һ��ui�߳�
	m_pThread = AfxBeginThread(Clearthread,this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);
	m_Staticscanregpath.SetWindowText(L"ɨ�����");
	GetDlgItem(IDC_BUTTON_STOPSCAN)->EnableWindow(TRUE);

}


CString CRegeDitDlg::ReadShortcut(CString lpwLnkFile)
{
	bool bReturn = true;
	TCHAR lpDescFile[MAX_PATH]  = {0};
	IShellLink *pShellLink =NULL;

	if(bReturn)
	{
		bReturn = (CoInitialize(NULL) == S_OK);

		if(bReturn)
		{
			bReturn = CoCreateInstance (CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
				IID_IShellLink, (void **)&pShellLink) >= 0;

			if(bReturn)
			{
				IPersistFile *ppf =NULL;
				bReturn = pShellLink->QueryInterface(IID_IPersistFile, (void **)&ppf) >= 0;
				if(bReturn)
				{
					bReturn = ppf->Load(lpwLnkFile, TRUE) >= 0;
					if(bReturn)
					{
						pShellLink->GetPath(lpDescFile, MAX_PATH, NULL, 0);
					}
					ppf->Release ();
				}
				pShellLink->Release ();
			}
			CoUninitialize();
		}
	}
	return lpDescFile;
}

void  CRegeDitDlg::DisplayPath(CString   path)
{

	CFileFind myfile;
	CFileFind  myshortcut;

	path +=L"\\*.*";
	BOOL  exist = myfile.FindFile(path);

	while(exist)
	{ 
		CString  destpath;
		exist = myfile.FindNextFile();

		if (myfile.IsDots())
		{
			continue;
		}

		m_Staticscanregpath.SetWindowText(myfile.GetFilePath());
		if( myfile.IsDirectory())
		{ 
			DisplayPath(myfile.GetFilePath());//�ݹ�
		}	

		if(exist)
		{
			destpath = ReadShortcut(myfile.GetFilePath());

			if (destpath=="")
			{
				continue;
			}

			if(!myshortcut.FindFile( destpath))             //��ݷ�ʽĿ���ļ�������
			{ 

				TCHAR  tsznumstart[10] ={0};
				int index = m_clearitmelist.InsertItem(m_Ilistctrlindex++,myfile.GetFilePath(),0);
				m_clearitmelist.SetItemText(index,1,destpath.GetBuffer());
				m_clearitmelist.SetItemText(index,2,L"��Ч�Ŀ�ʼ�˵�");
				_itow_s(m_IvaildstartmenuNum++,tsznumstart,10);
				m_validMenu.SetWindowText(tsznumstart);
			}
		}
	}

	myfile.Close();
	myshortcut.Close();
}


UINT __cdecl CRegeDitDlg::Clearthread( LPVOID pParam )
{
	
	CRegeDitDlg* dl = (CRegeDitDlg*)pParam;
	dl->CheckChoice();

	return 0;
}



void  CRegeDitDlg::DisplayCFileSize(CString   path)
{
	CFileFind myfile;
	TCHAR tszsize[1000] = {0};
	path +=L"\\*.*";
	BOOL  exist = myfile.FindFile(path);

	while(exist)
	{ 
		CString  destpath;
		exist = myfile.FindNextFile();	
		CString csSystempath = L"WINDOWS";
		CString csSearchPath =  myfile.GetFileName();

		if(-1!=csSearchPath.Find(csSystempath))
		{
			continue;
		}

		if (myfile.IsDots())
		{
			continue;
		}

		m_Staticscanregpath.SetWindowText(myfile.GetFilePath());

		if( myfile.IsDirectory())
		{ 
	     //m_DiskTree.InsertItem(TVIF_TEXT,myfile.GetFileName(),0, 0, 0, 0, 0,hCountry, NULL);
		 DisplayCFileSize(myfile.GetFileName());//�ݹ�
		}

		else
		{
	     	//m_DiskTree.InsertItem(TVIF_TEXT,myfile.GetFileName(),0,0,0,0,0,hCountry, NULL);
			if(myfile.GetLength()/(1024*1024)>100)  //����100mb
			{
				//��ʾ����
				int index  =m_clearitmelist.InsertItem(m_Ilistctrlindex++,myfile.GetFileName(),0);
				m_clearitmelist.SetItemText(index,1,myfile.GetFilePath());
				_ltow_s((ULONGLONG)myfile.GetLength()/(1024*1024),tszsize,10);
				m_clearitmelist.SetItemText(index,2,tszsize);

			}
		}
	}
	myfile.Close();

}

void CRegeDitDlg::OnBnClickedOpenreg()
{

	ShellExecute(NULL,L"open",L"C://windows//system32//regedit.exe",NULL,NULL,SW_SHOWNORMAL);
}


void CRegeDitDlg::CheckChoice()
{
	if(m_Button_nostartmenu.GetCheck())
	{
		DisplayPath(L"C:\\Documents and Settings\\All Users\\����ʼ���˵�");
	}

	TCHAR szkeyName[MAX_VALUE_NAME] = {L"SOFTWARE"};

	/*TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText =L"c:";

	HTREEITEM hCountry = m_DiskTree.InsertItem(&tvInsert);
	*/
	if(m_Buttonbigfile.GetCheck())
	{
		DisplayCFileSize(L"C:");
	}

	if(m_Button_invalidSoft.GetCheck())
	{
		DispayRegKeyPath(HKEY_LOCAL_MACHINE,szkeyName);
	} 
}


HBRUSH CRegeDitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_SCANREGPATH)
		pDC->SetTextColor(RGB(255,0,0));

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CRegeDitDlg::OnUpdateDleteitem(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

	TCHAR  szBuf[MAX_PATH] ={0};

	int index = m_clearitmelist.GetSelectionMark();
	m_clearitmelist.GetItemText(index,1, szBuf, MAX_PATH);

	if(!DeleteFile(szBuf))
	{
		AfxMessageBox(L"ɾ��ʧ��");
		return ;
	}

	m_clearitmelist.DeleteItem(index);
}


void CRegeDitDlg::OnUpdateOpenitem(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	TCHAR  strPath[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	int index = m_clearitmelist.GetSelectionMark();
	m_clearitmelist.GetItemText(index, 1, szPath, MAX_PATH);

	wsprintfW(strPath,L"Explorer /select, %s", szPath);

	char szExePath[MAX_PATH] = {0};

	int exepath_len = 256;
	UnicodeToMultiByte(strPath,256,szExePath,exepath_len);
	WinExec(szExePath, SW_SHOWNORMAL);
}



void CRegeDitDlg::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	GetCursorPos( & point );
	CMenu m_menu;
	VERIFY( m_menu.LoadMenu( IDR_MENU1) );
	CMenu* popup = m_menu.GetSubMenu(0);
	ASSERT( popup != NULL );
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	*pResult = 0;

}	


void CRegeDitDlg::GetKeyToValue(HKEY hKey)
{


	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 
	CFileFind  f1;
	DWORD  retCode; 

	TCHAR  ValueName[MAX_VALUE_NAME] ={0};
	DWORD cchValue = MAX_VALUE_NAME; 

	TCHAR   ValueData[MAX_VALUE_NAME] ={0};
	DWORD   ccValueSize  = MAX_VALUE_NAME;


	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 


	for(DWORD i =0,j=0;i<cValues;i++)
	{
		RegEnumValue(hKey,i,ValueName,&cchValue,0,0,(LPBYTE)ValueData,&ccValueSize);
		cchValue = MAX_VALUE_NAME;
		ccValueSize = MAX_VALUE_NAME;

		CString s;
	    s.Format(_T("%s"),ValueData);

		if(-1 !=s.Find(L"\\") &&( -1==s.Find(L"%")) &&(-1==s.Find(L"@")))    //�����Ƿ��ж�Ӧ��
		{
             
			if( (s.GetLength()-1)== s.ReverseFind('\\') )    //�����ַ�ĩβΪ//��,ȥ��//��CFileFind::find�����ҵ�
			{
				s.SetAt(s.GetLength()-1,'\0');
			}
		     
			if(!f1.FindFile(s.GetBuffer()))    //���Բ��������ֵ��Ӧ��
			{
				int index = m_clearitmelist.InsertItem(j++,ValueName,0);
				m_clearitmelist.SetItemText(index,1,ValueData);

			}

		}
	}



}


void CRegeDitDlg::DispayRegKeyPath(HKEY  h1,TCHAR  szKeyName[])
{
	TCHAR  szSubKeyName[50] ={0};
	DWORD   ccKeySize = MAX_PATH;
	TCHAR sztemp[MAX_VALUE_NAME] = {0};
	HKEY   hkey;

	if(ERROR_SUCCESS !=RegOpenKeyEx(h1,szKeyName,0,KEY_READ,&hkey))
	{
		AfxMessageBox(L"��ע���ʧ��");
		return ;
	}

	wcscpy_s(sztemp,MAX_PATH,szKeyName);

	for (int i=0;;i++)
	{
		if(ERROR_SUCCESS!=RegEnumKey(hkey,i,szSubKeyName,ccKeySize))
		{
			break;
		}

		wcscat_s(szKeyName,MAX_PATH,L"\\");
		wcscat_s(szKeyName,MAX_PATH,szSubKeyName);

		if(FindSubKey(h1,szKeyName))   //������Ӽ��Ļ�
		{
			DispayRegKeyPath(h1,szKeyName);
		}
		else
		{

			GetKeyToValue(hkey);
			wcscpy_s(szKeyName,MAX_PATH,sztemp);   

		}
	}
}



/**********************************************************************
* �������ƣ� // IsExist
* ���������� // �Ƿ��������
* ���ʵı� //
* �޸ĵı� //
* ��������� // HKEY hKey��LPCTSTR lpSubKey
* ��������� // BOOL
* �� �� ֵ�� // return FALSE;
* ����˵���� //
* �޸�����        �汾��     �޸���	      �޸�����
* -----------------------------------------------
* 2012/5/1 	   V1.0	      ������        XXXX
************************************************************************/

BOOL CRegeDitDlg::IsExist(HKEY hKey,LPCTSTR lpSubKey)  
{
	HKEY phkResult;
	if( ERROR_SUCCESS == RegOpenKeyEx(hKey,lpSubKey,0,
		KEY_EXECUTE,&phkResult))
	{
		RegCloseKey(phkResult);
		return TRUE;
	}
	return FALSE;
}


BOOL  CRegeDitDlg::FindSubKey(HKEY hKey,LPCTSTR lpSubKey)
{
	if(IsExist(hKey,lpSubKey))
	{
		HKEY phkResult;
		TCHAR  sz[MAX_PATH];
		if( ERROR_SUCCESS == RegOpenKeyEx(hKey,lpSubKey,0,
			KEY_READ,&phkResult))
		{

			if( ERROR_SUCCESS == RegEnumKey(phkResult,0,sz,MAX_PATH))
			{
				RegCloseKey(phkResult);
				return TRUE;
			}
			RegCloseKey(phkResult);
		}
	}
	return FALSE;
}

void CRegeDitDlg::OnBnClickedButtonStopScan()
{
	::TerminateThread(m_pThread->m_hThread,0);
	GetDlgItem(IDC_BUTTON_STOPSCAN)->EnableWindow(FALSE);
}
