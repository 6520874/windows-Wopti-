			// UsbShaDuDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "UsbShaDuDlg.h"


// CUsbShaDuDlg �Ի���

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
	ON_MESSAGE(WM_DEVICECHANGE,DevChange)                //����ƶ��豸
	ON_BN_CLICKED(IDC_BUTTON_STOPSCAN, &CUsbShaDuDlg::OnBnClickedButtonStopscan)
	ON_STN_CLICKED(IDC_STATIC_USBSTATE, &CUsbShaDuDlg::OnStnClickedStaticUsbstate)

	ON_WM_DEVMODECHANGE()
END_MESSAGE_MAP()


// CUsbShaDuDlg ��Ϣ�������

BOOL CUsbShaDuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	m_ListVirus.InsertColumn(0,L"������",LVCFMT_CENTER,250);
	m_ListVirus.InsertColumn(1,L"·��",LVCFMT_CENTER,450);
	m_ListVirus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  //��������ȫ��ѡ��
	m_ListVirus.SetBkColor(RGB(255, 255 ,0));	//���ñ���ɫΪ��ɫ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUsbShaDuDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CUsbShaDuDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_USBSTATE)
		pDC->SetTextColor(RGB(255,0,0));

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_SCANPATH)
		pDC->SetTextColor(RGB(255,0,0));
	return hbr;

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

}


void CUsbShaDuDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CUsbShaDuDlg::OnBnClickedButtonScan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//1�ƽⲡ��ע�����ļ���׺������
	//2�ƽ��ϵͳ�ļ�������
	//3ɨ����������

	GetDlgItem(IDC_BUTTON_STOPSCAN)->EnableWindow(TRUE);
	m_pThread = AfxBeginThread(
		AfxUsbCleanThread,         //�߳���������
		this,          //�߳���������
		THREAD_PRIORITY_NORMAL,       //�߳����ȼ�
		0,            //Windowsϵͳһ���߳�ջ��СΪ1 MB�������̵߳���Ŀ�������ڴ��ջ�ռ��С�й�
		0,            //�̴߳�����־���磺CREATE_SUSPENDED
		NULL);           //ϵͳ��ȫ������NULL

	if ( m_pThread ) 
	{

	}

}


UINT  __cdecl  CUsbShaDuDlg::AfxUsbCleanThread( LPVOID pParam )
{
	CUsbShaDuDlg  * dl = (CUsbShaDuDlg*)pParam;
	//ע����ļ���׺������
	//����winweb.exe ����
	//explorer  �µ�iconhandle.dllģ��


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

		if( FILE_ATTRIBUTE_HIDDEN == GetFileAttributesW(myfile.GetFilePath())) //����ļ������صģ���ô�ָ�����
		{
			if(!SetFileAttributesW(myfile.GetFilePath(),FILE_ATTRIBUTE_NORMAL))
			{
				AfxMessageBox(L"�����ļ�����ʧ��");
				return;
			}
		}

		//ͼ����һ���ļ��У����ļ���+.exe���ж�Ϊ�����ļ���
		if( myfile.IsDirectory())
		{ 
			if(-1 != myfile.GetFileName().Find(_T("exe")))
			{
				m_ListVirus.SetItemText(m_index,0,_T("virus"));
				m_ListVirus.SetItemText(m_index++,1,myfile.GetFilePath());
			
			}
			DisplayPath(myfile.GetFilePath());//�ݹ�			
		}	

	}
	myfile.Close();
}




void CUsbShaDuDlg::OnBnClickedButtonStopscan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_pThread!=NULL)
	{
		::TerminateThread(m_pThread->m_hThread,1);
		m_pThread =NULL;
	}

	else
	{
		AfxMessageBox(L"���ȵ����ʼɨ��");
	}
	GetDlgItem(IDC_BUTTON_STOPSCAN)->EnableWindow(FALSE);
}




void CUsbShaDuDlg::OnStnClickedStaticUsbstate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CUsbShaDuDlg::OnDevModeChange(LPTSTR lpDeviceName)
{

	m_Staticusbstate.SetWindowText(L"��⵽u�̲���"); 
	CDialog::OnDevModeChange(lpDeviceName);
	// TODO: �ڴ˴������Ϣ����������
}
