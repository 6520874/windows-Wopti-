// BeautMaster.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "BeautMaster.h"


// CBeautMaster 对话框

IMPLEMENT_DYNAMIC(CBeautMaster, CDialog)

CBeautMaster::CBeautMaster(CWnd* pParent /*=NULL*/)
	: CDialog(CBeautMaster::IDD, pParent)
{

}

CBeautMaster::~CBeautMaster()
{
}

void CBeautMaster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabXiuFu);
}


BEGIN_MESSAGE_MAP(CBeautMaster, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBeautMaster::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CBeautMaster 消息处理程序

void CBeautMaster::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	m_TabXiuFu.GetClientRect (&rc);

	switch(m_TabXiuFu.GetCurSel())
	{
	case 0:
		m_USBCleanDlg.SetWindowPos (NULL,rc.left,rc.top,rc.right,rc.bottom,SWP_SHOWWINDOW);
		break;
	}
}

BOOL CBeautMaster::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCITEM item;
	RECT      rc;
	item.mask = TCIF_TEXT;
	item.pszText = L"U盘杀毒";
	item.cchTextMax = 200;
	m_TabXiuFu.InsertItem (0,item.pszText);
	m_USBCleanDlg.Create(IDD_DIALOG_USBSHADU,&m_TabXiuFu);
	GetClientRect(&rc);
	rc.top += 25;
	rc.left += 1;
	rc.right -= 1;
	rc.bottom -= 2;
	m_USBCleanDlg.SetWindowPos (NULL,rc.left,rc.top,rc.right,rc.bottom,SWP_SHOWWINDOW);
	
	return TRUE; 
}
