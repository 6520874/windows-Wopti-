#pragma once


// CSystemInfoDlg �Ի���

class CSystemInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemInfoDlg)

public:
	CSystemInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
