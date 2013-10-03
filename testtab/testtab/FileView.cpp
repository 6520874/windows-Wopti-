// FileView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testtab.h"
#include "FileView.h"
#include <stack>

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
    ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CFileView::OnNMRClickTree1)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CFileView::OnTvnSelchangedTree1)
    ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CFileView::OnNMDblclkTree1)
    ON_UPDATE_COMMAND_UI(ID_DLETEITEM, &CFileView::OnUpdateDleteitem)
    ON_UPDATE_COMMAND_UI(ID_OPENITEM, &CFileView::OnUpdateOpenitem)
    ON_UPDATE_COMMAND_UI(ID_DDDD_32795, &CFileView::OnRenName)
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

void CFileView::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
// TODO: �ڴ���ӿؼ�֪ͨ����������
CPoint point;
::GetCursorPos(&point);
CMenu menu;
menu.LoadMenu(IDR_MENU1);
CMenu* pPopup = menu.GetSubMenu(0);
pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
*pResult = 0;
}

void CFileView::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{

}

void CFileView::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
 {
 
    
 }

void CFileView::OnUpdateDleteitem(CCmdUI *pCmdUI)
    {
    // TODO: �ڴ������������û����洦��������


    std::stack<CString>   strStack;
    CString strsel = m_tree.GetItemText(m_tree.GetSelectedItem());
    strStack.push(strsel);
    HTREEITEM  h1 = m_tree.GetParentItem(m_tree.GetSelectedItem());
    CString  str;
    do 
        {
        CString strsub = m_tree.GetItemText(h1);
        strStack.push(strsub);    
        h1 = m_tree.GetParentItem(h1);
        } while (h1);

        while(!strStack.empty())
            {
            str +=  strStack.top();
            str += _T("\\");
            strStack.pop();
            }

   
     CString cs = _T("ȷ��Ҫɾ��");
     cs += str;

    if(IDYES == ::MessageBox(m_hWnd,cs,_T("��ʾ"),MB_YESNO))
      { 
    

      }   
    }

void CFileView::OnUpdateOpenitem(CCmdUI *pCmdUI)
    {
     
    std::stack<CString>   strStack;
    CString strsel = m_tree.GetItemText(m_tree.GetSelectedItem());
    strStack.push(strsel);
    HTREEITEM  h1 = m_tree.GetParentItem(m_tree.GetSelectedItem());
    CString  str;
    do 
        {
        CString strsub = m_tree.GetItemText(h1);
        strStack.push(strsub);    
        h1 = m_tree.GetParentItem(h1);
        } while (h1);

        while(!strStack.empty())
            {
            str +=  strStack.top();
            str += _T("\\");
            strStack.pop();
            }

        ShellExecute(m_hWnd,L"open",str,NULL,NULL,SW_NORMAL);	   
    }

void CFileView::OnRenName(CCmdUI *pCmdUI)
    {
    // TODO: �ڴ������������û����洦��������
    std::stack<CString>   strStack;
    CString strsel = m_tree.GetItemText(m_tree.GetSelectedItem());
    strStack.push(strsel);
    HTREEITEM  h1 = m_tree.GetParentItem(m_tree.GetSelectedItem());
    CString  str;
    do 
        {
        CString strsub = m_tree.GetItemText(h1);
        strStack.push(strsub);    
        h1 = m_tree.GetParentItem(h1);
        } while (h1);

        while(!strStack.empty())
            {
            str +=  strStack.top();
            str += _T("\\");
            strStack.pop();
            }

     
       //csFind.Rename(strsel,)
    }
