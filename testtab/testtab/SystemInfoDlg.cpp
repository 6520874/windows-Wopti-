// SystemInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "SystemInfoDlg.h"


// CSystemInfoDlg �Ի���

IMPLEMENT_DYNCREATE(CSystemInfoDlg, CDHtmlDialog)

CSystemInfoDlg::CSystemInfoDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CSystemInfoDlg::IDD, CSystemInfoDlg::IDH, pParent)
{

}

CSystemInfoDlg::~CSystemInfoDlg()
{
}

void CSystemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CSystemInfoDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CSystemInfoDlg, CDHtmlDialog)
	ON_BN_CLICKED(IDC_BUTTONAPPLY, &CSystemInfoDlg::OnBnClickedButtonapply)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CSystemInfoDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CSystemInfoDlg ��Ϣ�������

HRESULT CSystemInfoDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CSystemInfoDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CSystemInfoDlg::OnBnClickedButtonapply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������



}
