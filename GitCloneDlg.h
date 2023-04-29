
// GitCloneDlg.h : fichier d'en-tête
//

#pragma once


// boîte de dialogue de CGitCloneDlg
class CGitCloneDlg : public CDialog
{
// Construction
public:
	CGitCloneDlg(CWnd* pParent = nullptr);	// constructeur standard
	int MsgBox(CHAR* lpszText, CHAR* lpszCaption, DWORD dwStyle, INT lpszIcon);

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GITCLONE_DIALOG };

#endif
	CEdit	m_ctlEdURL;
	CListBox	m_url;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge de DDX/DDV


// Implémentation
protected:
	HICON m_hIcon;

	// Fonctions générées de la table des messages
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLbnSelchangeList1();
	void Telecharger(char* GitLink, bool Automatique);
	afx_msg void OnBnClickedAboutbox();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit3();
};
