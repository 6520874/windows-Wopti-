#pragma once


// CSystemInfoDlg 对话框

class CSystemInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemInfoDlg)

public:
	CSystemInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
