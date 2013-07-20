#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CSystemInfoDlg 对话框

class CSystemInfoDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CSystemInfoDlg)

public:
	CSystemInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemInfoDlg();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOGSYSTEMINFO, IDH = IDR_HTML_SYSTEMINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedButtonapply();
};
