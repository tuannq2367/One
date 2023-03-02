
// OneAppDlg.h : header file
//

#pragma once
#include "Observers.h"

// OneAppDlg dialog
class OneAppDlg : public CDialogEx, public ServiceObserver
{
// Construction
public:
	OneAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ONEAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
	virtual void OnServiceAdded(const CoreServiceInfo& info) override;
	virtual void OnServiceRemoved(const CoreServiceInfo& info) override;
	virtual void OnServiceChanged(const CoreServiceInfo& info) override;
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void InitReportControl();
	void InitServiceList();
	CXTPReportControl m_wndReportControl;

	void AddItem(const CoreServiceInfo& info);
	void RemoveItem(const CoreServiceInfo& info);
	void UpdateItem(const CoreServiceInfo& info);
public:
	afx_msg void OnBnClickedBtnStartService();
protected:
	afx_msg LRESULT OnServiceModify(WPARAM wParam, LPARAM lParam);
private:
	CComboBox m_ctrlServiceList;
public:
	afx_msg void OnSelchangeComboSelectService();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void PostNcDestroy();
};
