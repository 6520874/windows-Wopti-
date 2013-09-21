// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "MainDlg.h"
#include "testtabDlg.h"
#include "ClearMaster.h"
#include "BeautMaster.h"
#include "360Safe.h"
// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMainDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_YOUHUA, &CMainDlg::OnBnClickedButtonYouhua)
	ON_BN_CLICKED(IDC_BUTTON_MEIHUA, &CMainDlg::OnBnClickedButtonMeihua)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

void CMainDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
   CClearMaster  dlg;
   dlg.DoModal();


}

void CMainDlg::OnBnClickedButtonYouhua()
{
	// TODO: 在此添加控件通知处理程序代码
	CYouHuaDlg dlg;
	dlg.DoModal();
}

void CMainDlg::OnBnClickedButtonMeihua()
{
	// TODO: 在此添加控件通知处理程序代码
    CBeautMaster  dlg;
	dlg.DoModal();

}

   //C360SafeUI m_Dlg360Safe1;
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	//SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	//::SetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA, reinterpret_cast<LPARAM>(&m_Dlg360Safe1));
	//m_Dlg360Safe1.Subclass(GetSafeHwnd());
	//m_Dlg360Safe1.LoadSkin();

	 
	// by spf  ShellExecute(NULL,L"open",L"Follow.exe",NULL,NULL,SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMainDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO:  在此添加额外的初始化
}

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}
