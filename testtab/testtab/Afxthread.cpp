#include "StdAfx.h"
#include "Afxthread.h"


IMPLEMENT_DYNAMIC(CAfxThread, CWinThread)
CAfxThread::CAfxThread()
{
}

CAfxThread::~CAfxThread()
{
}

BOOL CAfxThread::InitInstance()
{
 // TODO:  perform and per-thread initialization here
/*
//实现用户界面线程 
 CFrameWnd* pWnd = NULL;

 pWnd = new CFrameWnd;
 if ( pWnd ) 
 {
  pWnd->Create(NULL,"UI Thread Window");
  pWnd->ShowWindow(SW_SHOW);
  pWnd->UpdateWindow();
  m_pMainWnd = pWnd;
 }
 return TRUE;
*/
/* CMultiThreadDlg dlg;
 m_pMainWnd = &dlg;

 dlg.DoModal();
 
 return FALSE;*/

 return TRUE;
}

int CAfxThread::ExitInstance()
{
 // TODO:  perform any per-thread cleanup here
 AfxMessageBox( _T("Oh ! I Know It! Bye!") );
 return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAfxThread, CWinThread)
 //{{AFX_MSG_MAP(CAfxThread)
  // NOTE - the ClassWizard will add and remove mapping macros here.
 ON_THREAD_MESSAGE(WM_AFX_TEST1_MSG,OnAfxTest1Msg)
 ON_THREAD_MESSAGE(WM_AFX_TEST2_MSG,OnAfxTest2Msg)
 ON_THREAD_MESSAGE(WM_AFX_HELLO_MSG,OnAfxHelloMsg)
 //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAfxThread message handlers

void CAfxThread::OnAfxTest1Msg( WPARAM wParam, LPARAM lParam )
{

  MessageBox(NULL,_T("Hello World!"),_T("Test"),MB_OK);
}

void CAfxThread::OnAfxTest2Msg( WPARAM wParam, LPARAM lParam )
{
 MessageBox(NULL,_T("Hello World!"),_T("Test"),MB_OK);
}

void CAfxThread::OnAfxHelloMsg(WPARAM wParam,LPARAM lParam)
{
 AfxMessageBox( _T("Hello! I'm A Thread!") );
 ::PostThreadMessage( AfxGetApp()->m_nThreadID, WM_AFX_HELLO_MSG, m_nThreadID, 0 );
}


