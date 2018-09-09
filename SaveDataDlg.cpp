// SaveDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SaveDataDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "MyAnalysiser.h"
#include <time.h>
// CSaveDataDlg dialog

IMPLEMENT_DYNAMIC(CSaveDataDlg, CDialogEx)

CSaveDataDlg::CSaveDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SAVEDATA, pParent)
{
	m_pInitData = NULL;
}

CSaveDataDlg::~CSaveDataDlg()
{
}

void CSaveDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveDataDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSaveDataDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_OFFSET, &CSaveDataDlg::OnEnChangeEditOffset)
	ON_EN_CHANGE(IDC_EDIT_LEN, &CSaveDataDlg::OnEnChangeEditLen)
END_MESSAGE_MAP()


// CSaveDataDlg message handlers
void CSaveDataDlg::InitData(map<int,RAW_PACKET*> * p)
{
	m_pInitData = p;
}

BOOL CSaveDataDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (!m_pInitData || m_pInitData->size() < 1)
	{
		EndDialog(IDCANCEL);
	}
	SetDlgItemText(IDC_EDIT_LEN, "-1");
	SetDlgItemText(IDC_EDIT_PATH, "d:\\data\\");
	SetDlgItemText(IDC_EDIT_OFFSET, "0");

	OnEnChangeEditOffset();
	return TRUE;
}

void CSaveDataDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	char sPath[200] = {0};
	GetDlgItemText(IDC_EDIT_PATH, sPath, 200);
	int nOffset = GetDlgItemInt(IDC_EDIT_OFFSET);
	int nsize = GetDlgItemInt(IDC_EDIT_LEN);
	time_t timer = time(NULL);
	char * sTime = ctime(&timer);
	CString str = sTime;
	str.Replace(":", "");
	str.Replace(" ", "");
	str.Replace("\n", "");
	bool bHasErr = false;

	if (IsDlgButtonChecked(IDC_CHECK_UNION))
	{
		auto iter = m_pInitData->begin();
		char sfile[260] = {0};
		sprintf(sfile, "%s\\%s_%d__.dat", sPath, str, iter->first);

		if(!CMyAnalysiser::GetInstance()->SaveBuffer(sfile, m_pInitData, nOffset, nsize))
		{
			bHasErr = true;
		}
	}
	else
	{
		for (auto iter = m_pInitData->begin(); iter != m_pInitData->end(); iter++)
		{
			char sfile[260] = {0};
			sprintf(sfile, "%s\\%s_%d.dat", sPath, str, iter->first);

			if(!CMyAnalysiser::GetInstance()->SaveBuffer(sfile, iter->second, nOffset, nsize))
			{
				bHasErr = true;
				break;
			}
		}
	}
	
	if (!bHasErr)
	{
		CDialogEx::OnOK();
	}
	else
	{
		CString sCode;
		sCode.Format("err:%d", GetLastError());
		AfxMessageBox(sCode);
	}
}


void CSaveDataDlg::OnEnChangeEditOffset()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int nOffset = GetDlgItemInt(IDC_EDIT_OFFSET);
	int nlen = GetDlgItemInt(IDC_EDIT_LEN);
	
	char sBuf[200] = {0};
	int nSize = nlen < 0 ? 100 : nlen < 100 ? nlen : 100;
	CMyAnalysiser::GetInstance()->DispBuffer(m_pInitData->begin()->second, sBuf, nSize);
	SetDlgItemText(IDC_EDIT_SAMPLE, &sBuf[nOffset]);

}


void CSaveDataDlg::OnEnChangeEditLen()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	OnEnChangeEditOffset();
}
