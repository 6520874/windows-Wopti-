
#pragma once

#include  "360Safe.h"

// CMainDlg 对话框


class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

     HICON m_hIcon;
// 对话框数据
	enum { IDD = IDD_DIALOGMAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonYouhua();
	afx_msg void OnBnClickedButtonMeihua();
	afx_msg void OnBnClickedButton1();

	afx_msg void OnPaint();
};
