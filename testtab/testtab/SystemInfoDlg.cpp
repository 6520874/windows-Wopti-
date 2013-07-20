// SystemInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "SystemInfoDlg.h"


// CSystemInfoDlg 对话框

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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CSystemInfoDlg, CDHtmlDialog)
	ON_BN_CLICKED(IDC_BUTTONAPPLY, &CSystemInfoDlg::OnBnClickedButtonapply)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CSystemInfoDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CSystemInfoDlg 消息处理程序

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
	// TODO: 在此添加控件通知处理程序代码



}
