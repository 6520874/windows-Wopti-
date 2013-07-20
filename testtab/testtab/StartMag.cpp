// StartMag.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "StartMag.h"


// CStartMag �Ի���

IMPLEMENT_DYNAMIC(CStartMagDlg, CDialog)

CStartMagDlg::CStartMagDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStartMagDlg::IDD, pParent)
{

}

CStartMagDlg::~CStartMagDlg()
{
}

void CStartMagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_startItemList);
}


void CStartMagDlg::GetRunRegpath()
{
 
	HKEY hTestKey;

	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 
 
	DWORD i, retCode; 

	TCHAR  ValueName[MAX_VALUE_NAME] ={0};
	DWORD cchValue = MAX_VALUE_NAME; 
    
	TCHAR   ValueData[MAX_VALUE_NAME] ={0};
	DWORD   ccValueSize  = MAX_VALUE_NAME;


	if( !RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0,
		KEY_READ,
		&hTestKey) == ERROR_SUCCESS
		)
	{
		AfxMessageBox(L"ע����ʧ��");
		return ;
	}

	retCode = RegQueryInfoKey(
		hTestKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 


	for(int i =0;i<cValues;i++)
	{
       RegEnumValue(hTestKey,i,ValueName,&cchValue,0,0,(LPBYTE)ValueData,&ccValueSize);
	     cchValue = MAX_VALUE_NAME;
		 ccValueSize = MAX_VALUE_NAME;
	   int index = m_startItemList.InsertItem(i,ValueName,0);
	   m_startItemList.SetItemText(index,1,ValueData);

	}

}

BEGIN_MESSAGE_MAP(CStartMagDlg, CDialog)
END_MESSAGE_MAP()


// CStartMag ��Ϣ�������

BOOL CStartMagDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_startItemList.InsertColumn(0,L"����������",LVCFMT_CENTER,150);
	m_startItemList.InsertColumn(1,L"�ļ�λ��",LVCFMT_CENTER,350);
    m_startItemList.InsertColumn(2,L"�ļ�����",LVCFMT_CENTER,100);
    m_startItemList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 
 

	
    //����������   ע������д��һ����
	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Shared Tools\MSConfig\startupreg
  
	GetRunRegpath();

 return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
