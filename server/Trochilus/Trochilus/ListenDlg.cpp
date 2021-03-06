// ListenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Trochilus.h"
#include "ListenDlg.h"
#include "afxdialogex.h"


// CListenDlg 对话框

IMPLEMENT_DYNAMIC(CListenDlg, CDialogEx)

CListenDlg::CListenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListenDlg::IDD, pParent)
{

}

CListenDlg::~CListenDlg()
{
}

void CListenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LISTEN, m_listenList);
	DDX_Control(pDX, IDC_COMBO_PROTO, m_protoList);
}

void CListenDlg::InitView()
{
	m_Imagelist.Create(40,40,ILC_COLOR32|ILC_MASK,2,2);

	m_listenList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	
	m_listenList.InsertColumn(0,_T("Protocol"),LVCFMT_CENTER,130,-1);
	m_listenList.InsertColumn(1,_T("Port"),LVCFMT_CENTER,120,-1);
	m_listenList.InsertColumn(2,_T("Status"),LVCFMT_CENTER,115,-1);
	m_listenList.InsertColumn(3,_T("Error"),LVCFMT_CENTER,200,-1);
	
	m_Imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_LIS));
	m_listenList.SetImageList(&m_Imagelist);

	m_protoList.InsertString(0,_T("TCP"));
	m_protoList.SetItemData(0,2);
	m_protoList.InsertString(1,_T("HTTP"));
	m_protoList.SetItemData(1,4);
	m_protoList.InsertString(2,_T("DNS"));
	m_protoList.SetItemData(2,5);
	m_protoList.SetCurSel(0);

	SetDlgItemText(IDC_EDIT_PORT,_T("8081"));
}


BEGIN_MESSAGE_MAP(CListenDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CListenDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CListenDlg::OnBnClickedButtonStop)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LISTEN, &CListenDlg::OnNMRClickListListen)
	ON_CBN_SELENDCANCEL(IDC_COMBO_PROTO, &CListenDlg::OnCbnSelendcancelComboProto)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTO, &CListenDlg::OnCbnSelchangeComboProto)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CListenDlg 消息处理程序


BOOL CListenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	InitView();
	InitResize();

	return TRUE; 
}

void CListenDlg::InitResize()
{
	static CResizer::CBorderInfo s_bi[] = {
		{IDC_LIST_LISTEN,    
		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
		{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
		{CResizer::eFixed, IDC_MAIN, CResizer::eRight},
		{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},
		{IDC_STATIC1,
		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
		{CResizer::eFixed, IDC_MAIN, CResizer::eRight},
		{CResizer::eFixed, IDC_MAIN, CResizer::eRight}}
	};

	const int nSize = sizeof(s_bi)/sizeof(s_bi[0]);
	m_resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

void CListenDlg::OnBnClickedButtonStart()
{
	int nSelProto = m_protoList.GetCurSel();

	int nPort = GetDlgItemInt(IDC_EDIT_PORT);

	int nCount = m_listenList.GetItemCount();

	CString strPort;

	strPort.Format(_T("%d"),nPort);

	CString strProtocol;
	if (nSelProto == 0)
	{
		strProtocol = _T("TCP");
	}
	else if (nSelProto == 1)
	{
		strProtocol = _T("HTTP");
	}
	else if (nSelProto == 2)
	{
		strProtocol = _T("DNS");
	}

	m_listenList.InsertItem(nCount,strProtocol,0);
	m_listenList.SetItemText(nCount,1,strPort);

	int serial = AddCommService(nPort,m_protoList.GetItemData(nSelProto));

	if (serial)
	{
		m_listenList.SetItemData(nCount,serial);
		m_listenList.SetItemText(nCount,2,_T("Running"));
		m_listenList.SetItemText(nCount,3,_T("None"));
		m_listenList.SetItemColor(serial,RGB(255,0,0));
	}
	else
	{
// 		m_listenList.SetItemData(nCount,serial);
// 		m_listenList.SetItemText(nCount,2,_T("Stop"));
// 		m_listenList.SetItemText(nCount,3,_T("Port was used!"));
		m_listenList.DeleteItem(nCount);
		AfxMessageBox(_T("Listen to port faild!"));
	}
	
}


void CListenDlg::OnBnClickedButtonStop()
{
	CString str;
	POSITION pos=m_listenList.GetFirstSelectedItemPosition();
	if(pos == NULL)
		AfxMessageBox(L"No item has selected!");
	else
	{
		int nIdx = -1;
		nIdx = m_listenList.GetNextSelectedItem(pos);

		DWORD serial = m_listenList.GetItemData(nIdx);
		DeleteCommService(serial);
		m_listenList.DeleteItem(nIdx);
	}

}
void CListenDlg::OnNMRClickListListen(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// 
// 	POSITION pos=m_listenList.GetFirstSelectedItemPosition();
// 
// 	if (!pos)
// 		return;
// 
// 	CPoint point;
// 
// 	::GetCursorPos(&point);
// 
// 	CMenu menu;
// 
// 	menu.LoadMenu(IDR_MENU_LIS);
// 
// 	CMenu* pMenu = menu.GetSubMenu(0);
// 
// 	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this );
// 
// 	*pResult = 0;
}


void CListenDlg::OnCbnSelendcancelComboProto()
{

	// TODO: 在此添加控件通知处理程序代码
}


void CListenDlg::OnCbnSelchangeComboProto()
{
	int nSel = m_protoList.GetCurSel();
	if (nSel == 2)
	{
		SetDlgItemText(IDC_EDIT_PORT,_T("53"));
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	}
	else if(nSel == 1)
	{
		SetDlgItemText(IDC_EDIT_PORT,_T("8081"));
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	}
	else if(nSel == 0)
	{
		SetDlgItemText(IDC_EDIT_PORT,_T("8081"));
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	}
}


void CListenDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	m_resizer.Move();
	// TODO: 在此处添加消息处理程序代码
}
