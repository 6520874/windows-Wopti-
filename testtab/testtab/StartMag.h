#pragma once
#include "afxcmn.h"


// CStartMag �Ի���

class CStartMagDlg : public CDialog
{
	DECLARE_DYNAMIC(CStartMagDlg)

public:
	CStartMagDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartMagDlg();

// �Ի�������
	enum { IDD = IDD_STARTMANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void GetRunRegpath();

private:
	CListCtrl m_startItemList;
};
