			// UsbShaDuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "UsbShaDuDlg.h"


// CUsbShaDuDlg 对话框

IMPLEMENT_DYNAMIC(CUsbShaDuDlg, CDialog)

CUsbShaDuDlg::CUsbShaDuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbShaDuDlg::IDD, pParent)
{
      m_index = 0;
}

CUsbShaDuDlg::~CUsbShaDuDlg()
{
}

void CUsbShaDuDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_STATIC_USBSTATE, m_Staticusbstate);
	DDX_Control(pDX, IDC_STATIC_SCANPATH, m_Staticscanpath);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_Buttonstartscan);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTVIRUS, m_ListVirus);
}


BEGIN_MESSAGE_MAP(CUsbShaDuDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CUsbShaDuDlg::OnBnClickedButtonScan)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DEVICECHANGE,DevChange)                //检测移动设备
	ON_BN_CLICKED(IDC_BUTTON_STOPSCAN, &CUsbShaDuDlg::OnBnClickedButtonStopscan)
	ON_STN_CLICKED(IDC_STATIC_USBSTATE, &CUsbShaDuDlg::OnStnClickedStaticUsbstate)

	ON_WM_DEVMODECHANGE()
END_MESSAGE_MAP()


// CUsbShaDuDlg 消息处理程序

BOOL CUsbShaDuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化


	m_ListVirus.InsertColumn(0,L"病毒名",LVCFMT_CENTER,250);
	m_ListVirus.InsertColumn(1,L"路径",LVCFMT_CENTER,450);
	m_ListVirus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  //设置网格，全行选中
	m_ListVirus.SetBkColor(RGB(255, 255 ,0));	//设置背景色为白色

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUsbShaDuDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CUsbShaDuDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	// TODO:  在此更改 DC 的任何属性
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_USBSTATE)
		pDC->SetTextColor(RGB(255,0,0));

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_SCANPATH)
		pDC->SetTextColor(RGB(255,0,0));
	return hbr;

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

}


void CUsbShaDuDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CUsbShaDuDlg::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	//1破解病毒注册表的文件后缀名隐藏
	//2破解对系统文件的隐藏
	//3扫描宿主进程

	GetDlgItem(IDC_BUTTON_STOPSCAN)->EnableWindow(TRUE);
	m_pThread = AfxBeginThread(
		AfxUsbCleanThread,         //线程启动函数
		this,          //线程启动函数
		THREAD_PRIORITY_NORMAL,       //线程优先级
		0,            //Windows系统一般线程栈大小为1 MB，创建线程的数目与物理内存和栈空间大小有关
		0,            //线程创建标志，如：CREATE_SUSPENDED
		NULL);           //系统安全描述，NULL

	if ( m_pThread ) 
	{

	}

}


UINT  __cdecl  CUsbShaDuDlg::AfxUsbCleanThread( LPVOID pParam )
{
	CUsbShaDuDlg  * dl = (CUsbShaDuDlg*)pParam;
	//注册表文件后缀名开启
	//结束winweb.exe 进程
	//explorer  下的iconhandle.dll模块


	dl->DisplayPath(L"c:");
	return 0;

}

LRESULT CUsbShaDuDlg::DevChange(WPARAM wParam, LPARAM lParam)
{
	

	return NULL;
}




void  CUsbShaDuDlg::DisplayPath(CString   UsbPath)
{
	CFileFind myfile;
	UsbPath +=L"\\*.*";
	BOOL  exist = myfile.FindFile(UsbPath);
   
	while(exist)
	{ 	
		exist = myfile.FindNextFile();

		if (myfile.IsDots())
		{
			continue;
		}

		m_Staticscanpath.SetWindowText(myfile.GetFilePath());  

		if( FILE_ATTRIBUTE_HIDDEN == GetFileAttributesW(myfile.GetFilePath())) //如果文件是隐藏的，那么恢复正常
		{
			if(!SetFileAttributesW(myfile.GetFilePath(),FILE_ATTRIBUTE_NORMAL))
			{
				AfxMessageBox(L"设置文件属性失败");
				return;
			}
		}

		//图标是一个文件夹，且文件名+.exe的判定为病毒文件，
		if( myfile.IsDirectory())
		{ 
			if(-1 != myfile.GetFileName().Find(_T("exe")))
			{
				m_ListVirus.SetItemText(m_index,0,_T("virus"));
				m_ListVirus.SetItemText(m_index++,1,myfile.GetFilePath());
			
			}
			DisplayPath(myfile.GetFilePath());//递归			
		}	

	}
	myfile.Close();
}




void CUsbShaDuDlg::OnBnClickedButtonStopscan()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_pThread!=NULL)
	{
		::TerminateThread(m_pThread->m_hThread,1);
		m_pThread =NULL;
	}

	else
	{
		AfxMessageBox(L"请先点击开始扫描");
	}
	GetDlgItem(IDC_BUTTON_STOPSCAN)->EnableWindow(FALSE);
}




void CUsbShaDuDlg::OnStnClickedStaticUsbstate()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CUsbShaDuDlg::OnDevModeChange(LPTSTR lpDeviceName)
{

	m_Staticusbstate.SetWindowText(L"检测到u盘插入"); 
	CDialog::OnDevModeChange(lpDeviceName);
	// TODO: 在此处添加消息处理程序代码
}
