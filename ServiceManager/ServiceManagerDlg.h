
// ServiceManagerDlg.h : header file
//

#pragma once
#include "Observers.h"

// ServiceManagerDlg dialog
class ServiceManagerDlg : public CDialogEx, public ServiceObserver
{
// Construction
public:
	ServiceManagerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICEMANAGER_DIALOG };
#endif

	virtual void OnServiceAdded(const CoreServiceInfo& info) override;
	virtual void OnServiceRemoved(const CoreServiceInfo& info) override;
	virtual void OnServiceChanged(const CoreServiceInfo& info) override;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNewService();
private:
	void AddItem(const CoreServiceInfo& info);
	void RemoveItem(const CoreServiceInfo& info);
	void UpdateItem(const CoreServiceInfo& info);
	void InitReportControl();
	CXTPReportControl m_ctrlListService;
	CString m_strNewServiceName;
	int m_bindPort;
protected:
	afx_msg LRESULT OnServiceModify(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonNewApp();
};
