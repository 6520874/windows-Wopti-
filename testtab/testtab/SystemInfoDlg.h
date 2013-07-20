#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CSystemInfoDlg �Ի���

class CSystemInfoDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CSystemInfoDlg)

public:
	CSystemInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemInfoDlg();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOGSYSTEMINFO, IDH = IDR_HTML_SYSTEMINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedButtonapply();
};
