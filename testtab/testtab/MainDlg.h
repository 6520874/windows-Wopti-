
#pragma once

#include  "360Safe.h"

// CMainDlg �Ի���


class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

     HICON m_hIcon;
// �Ի�������
	enum { IDD = IDD_DIALOGMAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonYouhua();
	afx_msg void OnBnClickedButtonMeihua();
	afx_msg void OnBnClickedButton1();

	afx_msg void OnPaint();
};
