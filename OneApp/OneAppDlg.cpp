
// OneAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "OneApp.h"
#include "OneAppDlg.h"
#include "afxdialogex.h"
#include "ServicesManager.h"
#include "CoreServiceInfo.h"
#include <codecvt>

#ifdef _DEBUG
#define new DEBUG_NEW
#define EXE_PATH L"C:\\Workspaces\\One\\Debug64\\OneAppD.exe"
#else
#define EXE_PATH L"C:\\Workspaces\\One\\Debug64\\OneApp.exe"
#endif

// OneAppDlg dialog
#define ID_TIMER_CHECK_SERVICE 100
#define WM_SERVICE_MODIFY 1110
#define SERVICE_ADDED 0
#define SERVICE_REMOVED 1
#define SERVICE_CHANGED 2

enum ServiceReportColumn
{
	Column_Name,
	Column_Token,
	Column_Port,
	Column_Status,
	Column_TimeStamp,
	Column_Sequence
};

std::vector<std::pair<CString, int>> _services = {
	{L"OpcUaProxy Service", 1101},
	{L"DataModel Service", 1102},
	{L"Topology Service", 1103},
	{L"History Service", 1104},
	{L"AlarmEvent Service", 1105},
	{L"Tagging Service", 1106},
};

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> _convert;

class ServiceReportRecord : public CXTPReportRecord
{
public:
	ServiceReportRecord(const CoreServiceInfo& info);
	~ServiceReportRecord();

	void UpdateStatus(const CoreServiceInfo& info);

	bool CheckToken(const std::string& token);
private:
	std::string token;
	CXTPReportRecordItemText m_itemName;
	CXTPReportRecordItemText m_itemToken;
	CXTPReportRecordItemText m_itemPort;
	CXTPReportRecordItemText m_itemStatus;
	CXTPReportRecordItemText m_itemTimestamp;
	CXTPReportRecordItemText m_itemSequence;
};

ServiceReportRecord::ServiceReportRecord(const CoreServiceInfo& info) :
	token(info.token)
{
	m_arrItems.SetSize(0, 6);

	AddItem(&m_itemName);
	AddItem(&m_itemToken);
	AddItem(&m_itemPort);
	AddItem(&m_itemStatus);
	AddItem(&m_itemTimestamp);
	AddItem(&m_itemSequence);

	m_itemName.SetValue(_convert.from_bytes(info.serviceName).c_str());
	m_itemToken.SetValue(_convert.from_bytes(info.token).c_str());
	m_itemPort.SetValue(std::to_wstring(info.bindPort).c_str());
	m_itemStatus.SetValue(_convert.from_bytes(info.serviceStatus).c_str());
	CTime tmp(info.timestamp);
	m_itemTimestamp.SetValue(tmp.Format(L"%F %T"));
	m_itemSequence.SetValue(std::to_wstring(info.seq).c_str());
}

ServiceReportRecord::~ServiceReportRecord()
{
	m_arrItems.RemoveAll();
}

void ServiceReportRecord::UpdateStatus(const CoreServiceInfo& info)
{
	if (!CheckToken(info.token))
		return;

	m_itemStatus.SetValue(_convert.from_bytes(info.serviceStatus).c_str());
	CTime tmp(info.timestamp);
	m_itemTimestamp.SetValue(tmp.Format(L"%F %T"));
	m_itemSequence.SetValue(std::to_wstring(info.seq).c_str());
}

bool ServiceReportRecord::CheckToken(const std::string& strToken)
{
	if (strToken.compare(token) == 0)
		return true;

	return false;
}

OneAppDlg::OneAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ONEAPP_DIALOG, pParent)
{
	GetServiceManager()->AttachObserver(this);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void OneAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_LIST_SERVICE, m_wndReportControl);
	DDX_Control(pDX, IDC_COMBO_SELECT_SERVICE, m_ctrlServiceList);
}

void OneAppDlg::OnServiceAdded(const CoreServiceInfo& info)
{
	CoreServiceInfo _info = info;

	SendMessage(WM_SERVICE_MODIFY, SERVICE_ADDED, (LPARAM)&_info);
}

void OneAppDlg::OnServiceRemoved(const CoreServiceInfo& info)
{
	CoreServiceInfo _info = info;
	SendMessage(WM_SERVICE_MODIFY, SERVICE_REMOVED, (LPARAM)&_info);
}

void OneAppDlg::OnServiceChanged(const CoreServiceInfo& info)
{
	CoreServiceInfo _info = info;
	SendMessage(WM_SERVICE_MODIFY, SERVICE_CHANGED, (LPARAM)&_info);
}

BEGIN_MESSAGE_MAP(OneAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_SERVICE, &OneAppDlg::OnBnClickedBtnStartService)
	ON_MESSAGE(WM_SERVICE_MODIFY, &OneAppDlg::OnServiceModify)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_SERVICE, &OneAppDlg::OnSelchangeComboSelectService)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// OneAppDlg message handlers

BOOL OneAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitReportControl();

	InitServiceList();

	SetTimer(ID_TIMER_CHECK_SERVICE,2500, nullptr);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void OneAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR OneAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void OneAppDlg::InitReportControl()
{
	m_wndReportControl.ResetContent();
	int colIndex = 0;
	auto* pColum = m_wndReportControl.AddColumn(new CXTPReportColumn(colIndex++, L"Name", 200));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_wndReportControl.AddColumn(new CXTPReportColumn(colIndex++, L"Token", 500));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_wndReportControl.AddColumn(new CXTPReportColumn(colIndex++, L"Service Port", 100));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_wndReportControl.AddColumn(new CXTPReportColumn(colIndex++, L"Status", 100));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_wndReportControl.AddColumn(new CXTPReportColumn(colIndex++, L"TimeStamp", 200));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_wndReportControl.AddColumn(new CXTPReportColumn(colIndex++, L"Seq", 100));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	m_wndReportControl.AllowEdit(FALSE);
	m_wndReportControl.AllowSort(FALSE);
	m_wndReportControl.SetMultipleSelection(FALSE);
	m_wndReportControl.InitialSelectionEnable(TRUE);
}

void OneAppDlg::InitServiceList()
{
	for (auto it: _services)
	{
		int ind = m_ctrlServiceList.AddString(it.first);
		m_ctrlServiceList.SetItemData(ind, it.second);
	}

	m_ctrlServiceList.SetCurSel(-1);
}

void OneAppDlg::AddItem(const CoreServiceInfo& info)
{
	m_wndReportControl.AddRecordEx(new ServiceReportRecord(info));
	m_wndReportControl.Populate();
}

void OneAppDlg::RemoveItem(const CoreServiceInfo& info)
{
	auto pRecords = m_wndReportControl.GetRecords();
	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		ServiceReportRecord* pRecord = (ServiceReportRecord*)pRecords->GetAt(i);
		if (pRecord->CheckToken(info.token))
		{
			m_wndReportControl.RemoveRecordEx(pRecord);
			return;
		}
	}
}

void OneAppDlg::UpdateItem(const CoreServiceInfo& info)
{
	auto pRecords = m_wndReportControl.GetRecords();
	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		ServiceReportRecord* pRecord = (ServiceReportRecord*)pRecords->GetAt(i);
		if (pRecord->CheckToken(info.token))
		{
			pRecord->UpdateStatus(info);
			m_wndReportControl.RedrawControl();
			return;
		}
	}
}

void OneAppDlg::OnBnClickedBtnStartService()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	int ind = m_ctrlServiceList.GetCurSel();
	if (ind == -1)
		return;

	int nPort = m_ctrlServiceList.GetItemData(ind);
	CString servName;
	m_ctrlServiceList.GetLBText(ind, servName);
	GetServiceManager()->StartProgram(EXE_PATH, _convert.to_bytes(servName), nPort);
}


afx_msg LRESULT OneAppDlg::OnServiceModify(WPARAM wParam, LPARAM lParam)
{
	CoreServiceInfo* pInfo = (CoreServiceInfo*)lParam;
	if (wParam == SERVICE_ADDED)
	{
		AddItem(*pInfo);
	}
	else if (wParam == SERVICE_REMOVED)
	{
		RemoveItem(*pInfo);
	}
	else if (wParam == SERVICE_CHANGED)
	{
		UpdateItem(*pInfo);
	}
	return 0;
}


void OneAppDlg::OnSelchangeComboSelectService()
{
	// TODO: Add your control notification handler code here
	int ind = m_ctrlServiceList.GetCurSel();
	int nPort = m_ctrlServiceList.GetItemData(ind);
	SetDlgItemText(IDC_EDIT_PORT, std::to_wstring(nPort).c_str());
}


void OneAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TIMER_CHECK_SERVICE)
	{
		GetServiceManager()->CheckServicesTimeout();
	} else
	{	
		__super::OnTimer(nIDEvent);
	}
}


void OneAppDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(ID_TIMER_CHECK_SERVICE);
	__super::PostNcDestroy();
}
