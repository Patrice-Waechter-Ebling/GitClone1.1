
// GitCloneDlg.cpp : fichier d'implémentation
//

#include "pch.h"
#include "framework.h"
#include "GitClone.h"
#include "GitCloneDlg.h"
#include "afxdialogex.h"
#include "CTDropTarget.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

#pragma comment(lib, "User32.lib")

#pragma warning(disable:26495)
#pragma warning(disable:6031)
#pragma warning(disable:6284)
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CTDropTarget gDropTarget;

void ListerRepertoire(HWND hDlg,char* Chemin) {
	SendMessage(GetDlgItem(hDlg, 5001), LB_RESETCONTENT, 0, 0);
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH, Chemin);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*")); 
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {return;	}
	do
		{
			SetWindowText(GetDlgItem(hDlg, 5000), ffd.cFileName);
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				SendMessage(GetDlgItem(hDlg,5001),LB_ADDSTRING,0,(LPARAM) ffd.cFileName);
			}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	if (GetLastError() == ERROR_NO_MORE_FILES) {
		SetWindowText(GetDlgItem(hDlg, 5000), "Énumération terminée");
	}
	FindClose(hFind);
}
// boîte de dialogue CAboutDlg utilisée pour la boîte de dialogue 'À propos de' pour votre application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

// Implémentation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CGitCloneDlg::CGitCloneDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GITCLONE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGitCloneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGitCloneDlg)
	DDX_Control(pDX, IDC_EDIT1, m_ctlEdURL);
	DDX_Control(pDX, IDC_LIST1, m_url);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGitCloneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_EDIT1, &CGitCloneDlg::OnEnChangeEdit1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CGitCloneDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDM_ABOUTBOX, &CGitCloneDlg::OnBnClickedAboutbox)
	ON_BN_CLICKED(IDOK, &CGitCloneDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CGitCloneDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CGitCloneDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CGitCloneDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT3, &CGitCloneDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


BOOL CGitCloneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône
	DragAcceptFiles();
	BOOL fRet = gDropTarget.Register(&m_ctlEdURL);
	GetCurrentDirectory(256, gDropTarget.curdir);
	CreateStatusWindow(WS_CHILD | WS_VISIBLE, "Patrice Waechter-Ebling 2022-2023     P.O.O.      C++        v:1.0", this->m_hWnd, 6000);
	SetDlgItemText(IDC_EDIT3, gDropTarget.curdir);
	RECT rc{};
	HWND hCon = GetDlgItem(IDC_CONTENEUR)->m_hWnd;
	GetDlgItem(IDC_CONTENEUR) ->GetClientRect(&rc);
	CreateWindow(WC_LISTBOX, nullptr, WS_CHILD | WS_VISIBLE|WS_BORDER|LBS_NOINTEGRALHEIGHT, 0, 32, rc.right, rc.bottom - 0x38, hCon, (HMENU)5001, AfxGetApp()->m_hInstance, 0);
	CreateStatusWindow(WS_CHILD | WS_VISIBLE, "Énumérateur de dossiers",hCon , 0x1388);
	ListerRepertoire(hCon,gDropTarget.curdir);


	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

void CGitCloneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		OnBnClickedAboutbox();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
void CGitCloneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
HCURSOR CGitCloneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGitCloneDlg::OnDropFiles(HDROP hDropInfo)
{
	CString    sFile;
	DWORD      nBuffer = 0;
	UINT nFilesDropped = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (nFilesDropped > 0)
	{
		nBuffer = DragQueryFile(hDropInfo, 0, NULL, 0);
		DragQueryFile(hDropInfo, 0, sFile.GetBuffer(nBuffer + 1), nBuffer + 1);
		sFile.ReleaseBuffer();
	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

void CGitCloneDlg::OnEnChangeEdit1()
{
		CString Target;
		GetDlgItemText(IDC_EDIT1, Target);
		m_url.AddString(Target+ ".git");
		Target.Replace("https://github.com/", "\\");
		Target.Replace("/", "_");
		SetDlgItemText(IDC_EDIT2, Target);
}

void CGitCloneDlg::OnLbnSelchangeList1()
{
	char temp[260]{};
	m_url.GetText(m_url.GetCurSel(), temp);
	Telecharger(temp,false);
}

void CGitCloneDlg::Telecharger(char* GitLink,bool Automatique)
{
	CString Target=GitLink;
	Target.Replace("https://github.com/", "\\");
	Target.Replace(".git", "");
	Target.Replace("/", "_");
	SetDlgItemText(IDC_EDIT2, Target);
	char tmpPath[260]{};
	wsprintf(tmpPath, "%s%s", gDropTarget.curdir, Target.MakeUpper());
	SetDlgItemText(6000, tmpPath);
	SECURITY_ATTRIBUTES secattr;
	ZeroMemory(&secattr, sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;
	HANDLE rPipe, wPipe;
	CreatePipe(&rPipe, &wPipe, &secattr, 0);
	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	PROCESS_INFORMATION pInfo{};
	ZeroMemory(&pInfo, sizeof(pInfo));
	sInfo.cb = sizeof(sInfo);
	sInfo.dwFlags = STARTF_USESTDHANDLES;
	sInfo.hStdInput = NULL;
	sInfo.hStdOutput = wPipe;
	sInfo.hStdError = wPipe;
	CHAR tempEx[0x105]{};
	wsprintf(tempEx, "git clone %s %s", GitLink, tmpPath);
	if (Automatique == false) {
		if (MsgBox(tempEx, "Voulez vous executer cette requette?", MB_YESNO | MB_DEFBUTTON2, IDI_ICON2) != 0x06) { return; }
	}
	CreateProcess(0, (LPSTR)tempEx, 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_FORCEDOS/* | CREATE_NO_WINDOW */ , 0, 0, &sInfo, &pInfo);
	if (CloseHandle(wPipe)) return;
	CHAR buf[100]{};
	DWORD reDword;
	BOOL res;
	do { 
		res = ::ReadFile(rPipe, buf, 100, &reDword, 0); 
		SetDlgItemText(6000, buf);
	} while (res);
	CloseHandle(rPipe);
//	NavigerExplorateur(GetDlgItem(IDC_CONTENEUR)->m_hWnd, tmpPath);

}


void CGitCloneDlg::OnBnClickedAboutbox()
{
		ShellAbout(this->m_hWnd, AfxGetAppName(), "(C)Patrice Waechter-Ebling 2022-2023\tv:1.1\tP.O.O.\tC++\nLiaisons dynamiques OLE", AfxGetApp()->LoadIconA(IDR_MAINFRAME));
}


void CAboutDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	CreateStatusWindow(WS_CHILD | WS_VISIBLE, __FILE__, this->m_hWnd, 6000);
}
int CGitCloneDlg::MsgBox(CHAR* lpszText, CHAR* lpszCaption, DWORD dwStyle, INT lpszIcon)
{
	MSGBOXPARAMS lpmbp{};
	lpmbp.hInstance =AfxGetApp()->m_hInstance;
	lpmbp.cbSize = sizeof(MSGBOXPARAMS);
	lpmbp.hwndOwner =(HWND) this->m_hWnd;
	lpmbp.dwLanguageId = MAKELANGID(0x0800, 0x0800); //par defaut celui du systeme
	lpmbp.lpszText = lpszText;
	if (lpszCaption != NULL) {
		lpmbp.lpszCaption = lpszCaption;
	}
	else {
		lpmbp.lpszCaption = AfxGetApp()->m_pszAppName;
	}
	lpmbp.dwStyle = dwStyle | 0x00000080L;
	if (lpszIcon == NULL) {
		lpmbp.lpszIcon = (LPCTSTR)(int*)IDI_ICON1;
	}
	else 
	{
		lpmbp.lpszIcon = (LPCTSTR)(INT)lpszIcon;
	}
	lpmbp.lpfnMsgBoxCallback = 0;
	return  MessageBoxIndirect(&lpmbp);
}


void CGitCloneDlg::OnBnClickedOk()
{
	char tmp[260]{};
	for (int x = 0; x < m_url.GetCount(); x++) 
	{
		m_url.GetText(x, tmp);
		Telecharger(tmp,true);
	}
	CDialog::OnOK();
}

void CGitCloneDlg::OnBnClickedButton2()
{
	if (MsgBox("Vider la liste de clonage\n Aucun retour possible une fois les tampons vides\n\nVoulez-vous executer cette requette?",NULL, MB_YESNO | MB_DEFBUTTON2, IDI_ICON2) == 0x06) 
	{
		m_url.ResetContent();
	}
}

void CGitCloneDlg::OnBnClickedButton1()
{
	BROWSEINFO bi{};
	ITEMIDLIST* il = nullptr;
	CHAR buffer[MAX_PATH]{};
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = &buffer[0];
	bi.lpszTitle = AfxGetAppName();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_STATUSTEXT | BIF_EDITBOX;
	bi.lpfn = NULL;
	il = SHBrowseForFolder(&bi);
	if (il == NULL) { 
		MsgBox((CHAR*)"Aucun dossier ni lecteur n'a ete selectionne\nCommutation vers le dossier du programme", NULL, MB_OK, IDI_ICON2); 
		GetCurrentDirectory(256, gDropTarget.curdir);
		SetDlgItemText(IDC_EDIT3, gDropTarget.curdir);
	}
	if (SHGetPathFromIDList(il, &gDropTarget.curdir[0]))
	{
		SetDlgItemText(IDC_EDIT3, gDropTarget.curdir);
//		NavigerExplorateur(GetDlgItem(IDC_CONTENEUR)->m_hWnd,gDropTarget.curdir);
	}

}


void CGitCloneDlg::OnClose()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	CoUninitialize();
//	LibererExplorateur();
	CDialog::OnClose();
}


void CGitCloneDlg::OnBnClickedCancel()
{
	OnClose();
	CDialog::OnCancel();
}


void CGitCloneDlg::OnEnChangeEdit3()
{
	char tmp[0x105];
	GetDlgItemText(IDC_EDIT3, tmp, 0x104);
	GetDlgItem(IDC_CONTENEUR)->SetDlgItemTextA(5000, tmp);
	//NavigerExplorateur(GetDlgItem(IDC_CONTENEUR)->m_hWnd, tmp);


}
