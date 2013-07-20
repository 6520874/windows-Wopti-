// ClearMaster.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "ClearMaster.h"
#include "CleanRubbish.h"

// CClearMaster �Ի���

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


// CClearMaster ��Ϣ�������

BOOL CClearMaster::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CClearMaster::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
