// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "MainDlg.h"
#include "testtabDlg.h"
#include "ClearMaster.h"
#include "BeautMaster.h"
#include "360Safe.h"
// CMainDlg �Ի���

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


// CMainDlg ��Ϣ�������

void CMainDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   CClearMaster  dlg;
   dlg.DoModal();


}

void CMainDlg::OnBnClickedButtonYouhua()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CYouHuaDlg dlg;
	dlg.DoModal();
}

void CMainDlg::OnBnClickedButtonMeihua()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//::SetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA, reinterpret_cast<LPARAM>(&m_Dlg360Safe1));
	//m_Dlg360Safe1.Subclass(GetSafeHwnd());
	//m_Dlg360Safe1.LoadSkin();

	 
	// by spf  ShellExecute(NULL,L"open",L"Follow.exe",NULL,NULL,SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMainDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
}

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}
