// OneAppDlg.cpp : implementation file
//

#include "pch.h"
#include "OneApp.h"
#include "OneAppDlg.h"


// OneAppDlg dialog

IMPLEMENT_DYNAMIC(OneAppDlg, CDialogEx)

OneAppDlg::OneAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ONEAPP, pParent)
{

}

OneAppDlg::~OneAppDlg()
{
}

void OneAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OneAppDlg, CDialogEx)
END_MESSAGE_MAP()


// OneAppDlg message handlers


BOOL OneAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
