// ClearMaster.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "ClearMaster.h"
#include "CleanRubbish.h"

// CClearMaster 对话框

IMPLEMENT_DYNAMIC(CClearMaster, CDialog)

CClearMaster::CClearMaster(CWnd* pParent /*=NULL*/)
	: CDialog(CClearMaster::IDD, pParent)
{

}

CClearMaster::~CClearMaster()
{
}

void CClearMaster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabClear);
}


BEGIN_MESSAGE_MAP(CClearMaster, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CClearMaster::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CClearMaster 消息处理程序

BOOL CClearMaster::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	TCITEM item;
	RECT      rc;
	item.mask = TCIF_TEXT;
	item.pszText = L"Regedit";
	item.cchTextMax = 200;
    m_RegeditDlg.Create (IDD_REGEDIT,&m_TabClear);
	m_TabClear.InsertItem (0,item.pszText);

	item.pszText = L"History";
	item.cchTextMax = 200;
	m_ClearHistoryDlg.Create (IDD_DIALOG_CLEARHISTORY,&m_TabClear);
	m_TabClear.InsertItem (1,item.pszText);

	item.pszText = L"Rubbish";
	item.cchTextMax = 200;
	m_Rubbish.Create (IDD_DIALOGCLEANRUBBISH,&m_TabClear);
	m_TabClear.InsertItem (2,item.pszText);

	GetClientRect(&rc);
	rc.top += 25;
	rc.left += 1;
	rc.right -= 1;
	rc.bottom -= 2;
	m_RegeditDlg.SetWindowPos (NULL,rc.left,rc.top,rc.right,rc.bottom,SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CClearMaster::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CRect r;
	m_TabClear.GetClientRect (&r);

	switch(m_TabClear.GetCurSel())
	{
	case 0:
		m_RegeditDlg.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_SHOWWINDOW);
		m_ClearHistoryDlg.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW);
        m_Rubbish.SetWindowPos(NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW);
		//m_para7.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW);
		break;
	case 1:
		m_RegeditDlg.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW);
		m_ClearHistoryDlg.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_SHOWWINDOW); 
        m_Rubbish.SetWindowPos(NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW);
		break;
	case 2:
		m_RegeditDlg.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW);
		m_ClearHistoryDlg.SetWindowPos (NULL,10,30,r.right -20,r.bottom -40,SWP_HIDEWINDOW); 
		m_Rubbish.SetWindowPos(NULL,10,30,r.right -20,r.bottom -40,SWP_SHOWWINDOW);
		break;
	}
}
