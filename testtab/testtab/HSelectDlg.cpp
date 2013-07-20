// HSelectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "HSelectDlg.h"


// CHSelectDlg �Ի���

IMPLEMENT_DYNAMIC(CHSelectDlg, CDialog)

CHSelectDlg::CHSelectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHSelectDlg::IDD, pParent)
{

}

CHSelectDlg::~CHSelectDlg()
{
}

void CHSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SELECTALL, m_selectall);
	DDX_Control(pDX, IDC_BUTTON_REVERSELECT, m_reselectall);
	DDX_Control(pDX, IDC_CHECK_ADDNEW, m_addnew);
	DDX_Control(pDX, IDC_CHECK__DELFILE, m_deletefile);
	DDX_Control(pDX, IDC_CHECK_RENAME, m_renamefile);
	DDX_Control(pDX, IDC_CHECK_MODIFY, m_modifyfile);
	DDX_Control(pDX, IDC_CHECK_OTHERS, m_othersfile);
}


BEGIN_MESSAGE_MAP(CHSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CHSelectDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_REVERSELECT, &CHSelectDlg::OnBnClickedButtonReverselect)
END_MESSAGE_MAP()


// CHSelectDlg ��Ϣ�������

void CHSelectDlg::OnBnClickedButtonSelectall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_modifyfile.SetCheck(1);
	m_addnew.SetCheck(1);
	m_deletefile.SetCheck(1);
	m_renamefile.SetCheck(1);
	m_othersfile.SetCheck(1);


}

void CHSelectDlg::OnBnClickedButtonReverselect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_modifyfile.GetCheck())
	{

		m_modifyfile.SetCheck(0);
	}else
	{
		m_modifyfile.SetCheck(1);
	}

	if(m_addnew.GetCheck())
		m_addnew.SetCheck(0);
	else
		m_addnew.SetCheck(1);
	if(m_deletefile.GetCheck())
		m_deletefile.SetCheck(0);
	else
		m_deletefile.SetCheck(1);

	if(m_renamefile.GetCheck())
		m_renamefile.SetCheck(0);
	else
		m_renamefile.SetCheck(1);

	if(m_othersfile.GetCheck())
		m_othersfile.SetCheck(0);
	else
		m_othersfile.SetCheck(1);


}
