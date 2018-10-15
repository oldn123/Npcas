// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CSearchDlg dialog

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SEARCH, pParent)
{
	m_pSearchImpl = NULL;
	m_nSearchFlg = 0;
	m_ncnt = 0;
}

CSearchDlg::~CSearchDlg()
{
}

BOOL CSearchDlg::OnInitDialog()
{
	__super::OnInitDialog();
	CheckDlgButton(IDC_RADIO_STRING, 1);
	m_nSearchFlg = 2;
	m_histCombo.LoadHistory("Npcas", "SearchHist");
	return TRUE;
}

void CSearchDlg::DoCreate(CWnd * p, CSearchInterface * p1)
{
	m_pSearchImpl = p1, 
	Create(IDD_DIALOG_SEARCH, p);
	ShowWindow(SW_SHOW);
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_histCombo);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CSearchDlg::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CSearchDlg::OnBnClickedButtonNext)
	ON_EN_CHANGE(IDC_EDIT_TEXT, &CSearchDlg::OnEnChangeEditText)
	ON_BN_CLICKED(IDC_RADIO_STRING, &CSearchDlg::OnBnClickedRadioString)
	ON_BN_CLICKED(IDC_RADIO_NUM, &CSearchDlg::OnBnClickedRadioString)
	ON_BN_CLICKED(IDC_RADIO_BIN, &CSearchDlg::OnBnClickedRadioString)
	ON_CBN_SELCHANGE(IDC_EDIT_TEXT, &CSearchDlg::OnCbnSelchangeEditText)
	ON_CBN_EDITCHANGE(IDC_EDIT_TEXT, &CSearchDlg::OnCbnEditchangeEditText)
	ON_BN_CLICKED(IDC_CHECK_RUNTIMEMONITOR, &CSearchDlg::OnBnClickedCheckRuntimemonitor)
END_MESSAGE_MAP()


// CSearchDlg message handlers

bool CSearchDlg::GetMonitorInfo(CString & sCode, int & nFlag)
{
	if (GetSafeHwnd() && IsDlgButtonChecked(IDC_CHECK_RUNTIMEMONITOR))
	{
		if (m_sSearchCode.GetLength())
		{
			sCode = m_sSearchCode;
			nFlag = m_nSearchFlg;
			return true;
		}
	}
	return false;
}

void CSearchDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	if (m_pSearchImpl)
	{
		if (m_sSearchCode.GetLength())
		{
			m_histCombo.SaveHistory();
		}
		
		m_ncnt = m_pSearchImpl->DoSearch(m_sSearchCode, m_nSearchFlg);	
		CString sText;
		sText.Format("[%d][%d]", 1, m_ncnt);
		SetWindowText(sText);
	}
}


void CSearchDlg::OnBnClickedButtonPre()
{
	// TODO: Add your control notification handler code here
	if (m_pSearchImpl)
	{
		int nIdx = m_pSearchImpl->DoSearchNext(true);
		CString sText;
		sText.Format("[%d][%d]", nIdx+1, m_ncnt);
		SetWindowText(sText);
	}
}


void CSearchDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	if (m_pSearchImpl)
	{
		int nIdx = m_pSearchImpl->DoSearchNext(false);
		CString sText;
		sText.Format("[%d][%d]", nIdx+1, m_ncnt);
		SetWindowText(sText);
	}
}


void CSearchDlg::OnEnChangeEditText()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	GetDlgItemText(IDC_EDIT_TEXT, m_sSearchCode);
	// TODO:  Add your control notification handler code here
}


void CSearchDlg::OnBnClickedRadioString()
{
	// TODO: Add your control notification handler code here
	m_nSearchFlg = IsDlgButtonChecked(IDC_RADIO_NUM) ? 1 : 
		IsDlgButtonChecked(IDC_RADIO_STRING) ? 2 : 
		IsDlgButtonChecked(IDC_RADIO_BIN) ? 3 : 0;
}



void CSearchDlg::OnCbnSelchangeEditText()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_TEXT, m_sSearchCode);
}


void CSearchDlg::OnCbnEditchangeEditText()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_TEXT, m_sSearchCode);
}

void CSearchDlg::CancelRealtimeMonitor()
{
	CheckDlgButton(IDC_CHECK_RUNTIMEMONITOR, 0);
}

void CSearchDlg::OnBnClickedCheckRuntimemonitor()
{
	// TODO: Add your control notification handler code here
	if (IsDlgButtonChecked(IDC_CHECK_RUNTIMEMONITOR))
	{
		if (m_sSearchCode.GetLength())
		{
			m_histCombo.SaveHistory();
		}
	}
}
