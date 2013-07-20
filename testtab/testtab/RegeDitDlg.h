#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CRegeDitDlg �Ի���

class CRegeDitDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegeDitDlg)
public:
	CRegeDitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegeDitDlg();

// �Ի�������
	enum { IDD = IDD_REGEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL    OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
CButton m_Button_nostartmenu;
CButton m_Buttonbigfile;
CButton  m_Button_invalidSoft;
CStatic  m_Staticscanregpath;
CListCtrl m_clearitmelist;
int  m_Ilistctrlindex; 
int  m_IvaildstartmenuNum;
CWinThread * m_pThread;
CStatic m_validMenu;
int    m_j ;

public:
	afx_msg void OnBnClickedClearreg();
	afx_msg void OnBnClickedOpenreg();
	afx_msg void OnUpdateDleteitem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenitem(CCmdUI *pCmdUI);
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStopScan();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString ReadShortcut(CString lpwLnkFile);     //��ȡ��ݷ�ʽ(com���)
	void DisplayPath(CString   path);             //·������
	void DisplayCFileSize(CString   path);        //ѡ��C���ļ���С
	void DisplayRegSoftFile();                    //ѡ��ע��������Ϣ
    void CheckChoice();                           //ѡ�������ѡ��
	BOOL IsExist(HKEY hKey,LPCTSTR lpSubKey);     //�Ƿ����
	BOOL FindSubKey(HKEY hKey,LPCTSTR lpSubKey);  //��������
	void DispayRegKeyPath(HKEY h1,TCHAR s[]);
	void GetKeyToValue(HKEY  h1);                 //��ȡһ�����Ӧ��ֵ
    static 	UINT __cdecl Clearthread( LPVOID pParam );
private:
	CTreeCtrl m_DiskTree;
};
