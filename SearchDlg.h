#pragma once
#include "HistoryCombo.h"

// CSearchDlg dialog

class CSearchInterface
{
public:
	virtual int DoSearch(CString sCode, int nFlag) = 0;
	virtual int DoSearchNext(bool bPre)	= 0;
};

class CSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchDlg();
	virtual BOOL OnInitDialog();
	void SetListCtrl(CListCtrl* p){m_pListCtrl = p;}
	void DoCreate(CWnd * p, CSearchInterface * p1);
// Dialog Data
	//enum { IDD =  };

	void CancelRealtimeMonitor();
	bool GetMonitorInfo(CString & sCode, int & nFlag);

protected:
	CHistoryCombo	m_histCombo;
	int		m_ncnt;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrl * m_pListCtrl;
	CSearchInterface * m_pSearchImpl;
	CString m_sSearchCode;
	int		m_nSearchFlg;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnEnChangeEditText();
	afx_msg void OnBnClickedRadioString();
	afx_msg void OnCbnSelchangeEditText();
	afx_msg void OnCbnEditchangeEditText();
	afx_msg void OnBnClickedCheckRuntimemonitor();
};
