#pragma once
#include "afxcmn.h"
#include "UsbShaDuDlg.h"
// CBeautMaster �Ի���

class CBeautMaster : public CDialog
{
	DECLARE_DYNAMIC(CBeautMaster)

public:
	CBeautMaster(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBeautMaster();

// �Ի�������
	enum { IDD = IDD_MEIHUA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabXiuFu;
    CUsbShaDuDlg m_USBCleanDlg;
	virtual BOOL OnInitDialog();
};
