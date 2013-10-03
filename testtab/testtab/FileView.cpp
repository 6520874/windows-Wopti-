// FileView.cpp : 实现文件
//

#include "stdafx.h"
#include "testtab.h"
#include "FileView.h"


// CFileView 对话框

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


// CFileView 消息处理程序
//初始化驱动盘，并添加作为树根
void CFileView::InitDriverTree()
	{
	TCHAR  *pDriver,buf[50]={0};
	//得到所有驱动盘号
	GetLogicalDriveStrings(sizeof(buf),buf);
	//依次添加作为根
	for(pDriver=buf;*pDriver;pDriver+=wcslen(pDriver)+1)
		{
		//叶子节点结构体
		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = pDriver; 

		HTREEITEM hDriver = m_tree.InsertItem(&tvInsert);
		//设置节点数据为1，表示该节点已经展开过，在再次展开时不用再进行绑定！

		m_tree.SetItemData(hDriver,1);
		//以此驱动盘为根，在其下查找文件进行绑定
		InsertNode(pDriver,hDriver);
		}
	}

//插入节点函数，参数1是父节点路径，参数2是父节点句柄
void CFileView::InsertNode(CString szPath, HTREEITEM hNode)
	{
	HANDLE hFile;
	WIN32_FIND_DATA wData;

	szPath+="\\*";
	hFile=FindFirstFile(szPath,&wData);
	//查找失败
	if(hFile==INVALID_HANDLE_VALUE)
		return;
	do
		{
		//过滤2个特殊文件夹"."和".."
		if(wData.cFileName[0]=='.')
			continue;
		//如果查找到的文件是个文件夹
		if(wData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			HTREEITEM hTemp=m_tree.InsertItem(wData.cFileName,0,0,hNode,TVI_SORT);
			//添加一个临时节点来显示+号
			m_tree.InsertItem(NULL,0,0,hTemp,TVI_SORT); 
			}
		else
			{
			m_tree.InsertItem(wData.cFileName,0,0,hNode,TVI_SORT);
			}

		}while(FindNextFile(hFile,&wData));
	}
//该函数得到叶子节点所在的全路径
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
	// TODO: 在此添加控件通知处理程序代码
	//判断是展开还是合拢
	Invalidate(FALSE);
	if(TVE_EXPAND==pNMTreeView->action)
		{
		HTREEITEM  hNode=pNMTreeView->itemNew.hItem; 
		//判断节点数据是否为0，即没有展开过，则进行绑定
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

	// TODO:  在此添加额外的初始化
   InitDriverTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
	}
