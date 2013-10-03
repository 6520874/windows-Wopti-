// FileView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "FileView.h"


// CFileView �Ի���

IMPLEMENT_DYNAMIC(CFileView, CDialog)

CFileView::CFileView(CWnd* pParent /*=NULL*/)
	: CDialog(CFileView::IDD, pParent)
{

}

CFileView::~CFileView()
{
}

void CFileView::DoDataExchange(CDataExchange* pDX)
{
 CDialog::DoDataExchange(pDX);
 DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CFileView, CDialog)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, &CFileView::OnTvnItemexpandedTree1)
END_MESSAGE_MAP()


// CFileView ��Ϣ�������
//��ʼ�������̣��������Ϊ����
void CFileView::InitDriverTree()
	{
	TCHAR  *pDriver,buf[50]={0};
	//�õ����������̺�
	GetLogicalDriveStrings(sizeof(buf),buf);
	//���������Ϊ��
	for(pDriver=buf;*pDriver;pDriver+=wcslen(pDriver)+1)
		{
		//Ҷ�ӽڵ�ṹ��
		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = pDriver; 

		HTREEITEM hDriver = m_tree.InsertItem(&tvInsert);
		//���ýڵ�����Ϊ1����ʾ�ýڵ��Ѿ�չ���������ٴ�չ��ʱ�����ٽ��а󶨣�

		m_tree.SetItemData(hDriver,1);
		//�Դ�������Ϊ���������²����ļ����а�
		InsertNode(pDriver,hDriver);
		}
	}

//����ڵ㺯��������1�Ǹ��ڵ�·��������2�Ǹ��ڵ���
void CFileView::InsertNode(CString szPath, HTREEITEM hNode)
	{
	HANDLE hFile;
	WIN32_FIND_DATA wData;

	szPath+="\\*";
	hFile=FindFirstFile(szPath,&wData);
	//����ʧ��
	if(hFile==INVALID_HANDLE_VALUE)
		return;
	do
		{
		//����2�������ļ���"."��".."
		if(wData.cFileName[0]=='.')
			continue;
		//������ҵ����ļ��Ǹ��ļ���
		if(wData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			HTREEITEM hTemp=m_tree.InsertItem(wData.cFileName,0,0,hNode,TVI_SORT);
			//���һ����ʱ�ڵ�����ʾ+��
			m_tree.InsertItem(NULL,0,0,hTemp,TVI_SORT); 
			}
		else
			{
			m_tree.InsertItem(wData.cFileName,0,0,hNode,TVI_SORT);
			}

		}while(FindNextFile(hFile,&wData));
	}
//�ú����õ�Ҷ�ӽڵ����ڵ�ȫ·��
CString CFileView::GetFullPath(HTREEITEM hNode)
	{
	CString szRet=m_tree.GetItemText(hNode);
	while(hNode=m_tree.GetParentItem(hNode))
		szRet=m_tree.GetItemText(hNode)+_T("\\")+szRet;
	return szRet;
	}

void CFileView::OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж���չ�����Ǻ�£
	Invalidate(FALSE);
	if(TVE_EXPAND==pNMTreeView->action)
		{
		HTREEITEM  hNode=pNMTreeView->itemNew.hItem; 
		//�жϽڵ������Ƿ�Ϊ0����û��չ����������а�
		if(!m_tree.GetItemData(hNode))
			{
			m_tree.DeleteItem(m_tree.GetChildItem(hNode));
			m_tree.SetItemData(hNode,1);
			InsertNode(GetFullPath(hNode),hNode);
			}
		}
	*pResult = 0;
	*pResult = 0;
}

BOOL CFileView::OnInitDialog()
	{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
   InitDriverTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
	}
