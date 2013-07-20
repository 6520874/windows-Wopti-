#pragma once
#include "afxcmn.h"


// CUsbShaDuDlg 对话框

class CUsbShaDuDlg : public CDialog
{
	DECLARE_DYNAMIC(CUsbShaDuDlg)

public:
	CUsbShaDuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUsbShaDuDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_USBSHADU};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    
	DECLARE_MESSAGE_MAP()
public:

	LRESULT DevChange(WPARAM wParam, LPARAM lParam);
	LRESULT ONQQQ(WPARAM wParam, LPARAM lParam);
	void  DisplayPath(CString path);
	static 	UINT __cdecl CUsbShaDuDlg::AfxUsbCleanThread( LPVOID pParam ); //u盘杀毒线程函数
	afx_msg void OnBnClickedButtonStopscan();
	afx_msg void OnStnClickedStaticUsbstate();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CStatic m_Staticusbstate;
	CStatic m_Staticscanpath;
	CWinThread *m_pThread;
	FARPROC m_OldEditProc;
	CButton m_Buttonstartscan;
	int      m_index;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CListCtrl m_ListVirus;
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
};
