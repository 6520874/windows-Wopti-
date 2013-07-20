// CleanRubbish.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "CleanRubbish.h"


// CCleanRubbish 对话框

IMPLEMENT_DYNAMIC(CCleanRubbish, CDialog)

CCleanRubbish::CCleanRubbish(CWnd* pParent /*=NULL*/)
	: CDialog(CCleanRubbish::IDD, pParent)
{

}

CCleanRubbish::~CCleanRubbish()
{
}

void CCleanRubbish::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CLEAN_TEMP, m_chekTemp);
	DDX_Control(pDX, IDC_CHECK_CELANLOG, m_checklog);
	DDX_Control(pDX, IDC_CHECK_CLEANGID, m_checkgid);
	DDX_Control(pDX, IDC_CHECK_CLEAN_OLD, m_checkold);
}


BEGIN_MESSAGE_MAP(CCleanRubbish, CDialog)
	ON_BN_CLICKED(IDC_BUTTONSTARTSCAN, &CCleanRubbish::OnBnClickedButtonstartscan)
	ON_BN_CLICKED(IDC_BUTTON_STOPSCAN, &CCleanRubbish::OnBnClickedButtonStopscan)
END_MESSAGE_MAP()


// CCleanRubbish 消息处理程序


void   CCleanRubbish::SelectChoice()
{
	if(m_chekTemp.GetCheck())
	{


	}

	if(m_checklog.GetCheck())
	{

	}

	if(m_checkgid.GetCheck())
	{

	}

	if(m_checkold.GetCheck())
	{

	}
   
}

UINT __cdecl ClearRubbishThread( LPVOID pParam )
{

	CCleanRubbish * dlg = (CCleanRubbish*)pParam;
	
	dlg->SelectChoice();
	return 0;

}


void CCleanRubbish::OnBnClickedButtonstartscan()
{
	// TODO: 在此添加控件通知处理程序代码
  
   m_Windlg =  AfxBeginThread(ClearRubbishThread,this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);

	

}

void CCleanRubbish::OnBnClickedButtonStopscan()
{
	// TODO: 在此添加控件通知处理程序代码
	TerminateThread(m_Windlg->m_hThread,0);
}
