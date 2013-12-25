#pragma once
#include "afxcmn.h"

#include "ProcessConsumption.h"
// Cdlg1 �Ի���

class CTaskMgrDlg : public CDialog
{
	DECLARE_DYNAMIC(CTaskMgrDlg)

public:
	CTaskMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskMgrDlg();

// �Ի�������
	enum { IDD = IDD_TASKMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CListCtrl m_list;
	CImageList  m_ImageList;
		CProcessConsumption   m_process;
public:
	DWORD GetProcessMemoyUse(DWORD dwProcessID);
	CString GetProcessPath(DWORD dwprocessID);
	BOOL EnableDebugPriv();                //���н�����Ȩ
	BOOL EmuProcess();     
	int  GetCPU();
	afx_msg void OnUpdateLocatepro(CCmdUI *pCmdUI);
	afx_msg void OnUpdateKillpro(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedClearmem();
    afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdateBaidusearch(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePopupKillall(CCmdUI *pCmdUI);
	double GetCpuUsage( int iProcessID );
	__int64 FileTimeToInt64(const FILETIME& time);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void RefreshData();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheckDll();
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);

};
DWORD WINAPI RefreshDataThread(LPVOID lpParameter);