// testtab.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "testtab.h"
#include "testtabDlg.h"
#include "MainDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtesttabApp

BEGIN_MESSAGE_MAP(CtesttabApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtesttabApp ����

CtesttabApp::CtesttabApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CtesttabApp ����

CtesttabApp theApp;


// CtesttabApp ��ʼ��

BOOL CtesttabApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    //skinppLoadSkin(L"Aura.ssk");
     //skinppLoadSkinFromRes(m_hInstance,NULL,NULL,);
	 //skinppRemoveSkin();
	CMainDlg dlg;
	m_pMainWnd = &dlg;

	dlg.Create(IDD_DIALOGMAIN ); //����Ϊ��ģ̬�Ի���
	//dlg.DoModal();
	dlg.ShowWindow(SW_HIDE);        //������Ϻ󣬿������öԻ������ʾ��ʽ������Ϊ��SW_SHOW����
	//�ڴˣ�����ʹ�á�SW_HIDE�����Ի������أ������ڽ����б�����Ȼ���Կ���
	dlg.RunModalLoop();                  //��Ϣѭ��
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
