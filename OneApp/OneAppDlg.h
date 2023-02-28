#pragma once
#include "afxdialogex.h"

// OneAppDlg dialog

class OneAppDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OneAppDlg)

public:
	OneAppDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OneAppDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ONEAPP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
