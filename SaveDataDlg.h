#pragma once

#include "sniffer.h"
// CSaveDataDlg dialog
#include <map>
using namespace std;

class CSaveDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveDataDlg)

public:
	CSaveDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveDataDlg();

	void InitData(map<int,RAW_PACKET*> *);
// Dialog Data
//	enum { IDD =  };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditOffset();
	afx_msg void OnEnChangeEditLen();
	virtual BOOL OnInitDialog();
	map<int,RAW_PACKET*> * m_pInitData;

};
