#pragma once

class CAfxThread : public CWinThread
{
 DECLARE_DYNCREATE(CAfxThread)
protected:
 CAfxThread();           // protected constructor used by dynamic creation


// Overrides
 // ClassWizard generated virtual function overrides
 //{{AFX_VIRTUAL(CAfxThread)
 public:
 virtual BOOL InitInstance();
 virtual int ExitInstance();


 //}}AFX_VIRTUAL

// Implementation
protected:
 virtual ~CAfxThread();

 // Generated message map functions
 //{{AFX_MSG(CAfxThread)
  // NOTE - the ClassWizard will add and remove member functions here.
 afx_msg void OnAfxTest1Msg(WPARAM wParam, LPARAM lParam);
 afx_msg void OnAfxTest2Msg(WPARAM wParam, LPARAM lParam);
 afx_msg void OnAfxHelloMsg(WPARAM wParam,LPARAM lParam);
 //}}AFX_MSG

 DECLARE_MESSAGE_MAP()
};

