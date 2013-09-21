// SystemInfoDlg1.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "SystemInfoDlg.h"


// CSystemInfoDlg 对话框

IMPLEMENT_DYNAMIC(CSystemInfoDlg, CDialog)

CSystemInfoDlg::CSystemInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemInfoDlg::IDD, pParent)
{

}

CSystemInfoDlg::~CSystemInfoDlg()
{
}

void CSystemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemInfoDlg, CDialog)
END_MESSAGE_MAP()


// CSystemInfoDlg 消息处理程序
