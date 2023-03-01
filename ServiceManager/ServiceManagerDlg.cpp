
// ServiceManagerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ManagerApp.h"
#include "ServiceManagerDlg.h"
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

ServiceReportRecord::ServiceReportRecord(const CoreServiceInfo& info):
	token(info.token)
{
	m_arrItems.SetSize(0,6);

	AddItem(&m_itemName);
	AddItem(&m_itemToken);
	AddItem(&m_itemPort);
	AddItem(&m_itemStatus);
	AddItem(&m_itemTimestamp);
	AddItem(&m_itemSequence);

	m_itemName.SetValue(_convert.from_bytes(info.serviceName).c_str());
	m_itemToken.SetValue(_convert.from_bytes(info.token).c_str());
	m_itemPort.SetValue( std::to_wstring(info.bindPort).c_str());
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

// ServiceManagerDlg dialog

ServiceManagerDlg::ServiceManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVICEMANAGER_DIALOG, pParent)
	, m_strNewServiceName(_T(""))
	, m_bindPort(0)
{
	GetServiceManager()->AttachObserver(this);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ServiceManagerDlg::OnServiceAdded(const CoreServiceInfo& info)
{
	CoreServiceInfo _info = info;
	
	SendMessage(WM_SERVICE_MODIFY, SERVICE_ADDED, (LPARAM)&_info);
}

void ServiceManagerDlg::OnServiceRemoved(const CoreServiceInfo& info)
{
	CoreServiceInfo _info = info;
	SendMessage(WM_SERVICE_MODIFY, SERVICE_REMOVED, (LPARAM)&_info);
}

void ServiceManagerDlg::OnServiceChanged(const CoreServiceInfo& info)
{
	CoreServiceInfo _info = info;
	SendMessage(WM_SERVICE_MODIFY, SERVICE_CHANGED, (LPARAM)&_info);
}

void ServiceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICES, m_ctrlListService);
	DDX_Text(pDX, IDC_EDIT_NEW_SERVICE_NAME, m_strNewServiceName);
	DDX_Text(pDX, IDC_EDIT_SERVICE_PORT, m_bindPort);
}

BEGIN_MESSAGE_MAP(ServiceManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_NEW_SERVICE, &ServiceManagerDlg::OnBnClickedBtnNewService)
	ON_MESSAGE(WM_SERVICE_MODIFY, &ServiceManagerDlg::OnServiceModify)
	ON_BN_CLICKED(IDC_BUTTON_NEW_APP, &ServiceManagerDlg::OnBnClickedButtonNewApp)
END_MESSAGE_MAP()


// ServiceManagerDlg message handlers

BOOL ServiceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitReportControl();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ServiceManagerDlg::OnPaint()
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
HCURSOR ServiceManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ServiceManagerDlg::OnBnClickedBtnNewService()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	std::wstring szServName(m_strNewServiceName.GetString());
	std::string servName = _convert.to_bytes(szServName.c_str());
	GetServiceManager()->StartProgram(EXE_PATH,servName, m_bindPort);
}


void ServiceManagerDlg::AddItem(const CoreServiceInfo& info)
{
	m_ctrlListService.AddRecordEx(new ServiceReportRecord(info));
	m_ctrlListService.Populate();
}

void ServiceManagerDlg::RemoveItem(const CoreServiceInfo& info)
{
	auto pRecords = m_ctrlListService.GetRecords();
	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		ServiceReportRecord* pRecord = (ServiceReportRecord*)pRecords->GetAt(i);
		if (pRecord->CheckToken(info.token))
		{
			m_ctrlListService.RemoveRecordEx(pRecord);
			return;
		}
	}
}

void ServiceManagerDlg::UpdateItem(const CoreServiceInfo& info)
{
	auto pRecords = m_ctrlListService.GetRecords();
	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		ServiceReportRecord* pRecord = (ServiceReportRecord*)pRecords->GetAt(i);
		if (pRecord->CheckToken(info.token))
		{
			pRecord->UpdateStatus(info);
			m_ctrlListService.RedrawControl();
			return;
		}
	}
		
}

void ServiceManagerDlg::InitReportControl()
{
	m_ctrlListService.ResetContent();
	int colIndex = 0;
	auto* pColum = m_ctrlListService.AddColumn(new CXTPReportColumn(colIndex++, L"Name", 200));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_ctrlListService.AddColumn(new CXTPReportColumn(colIndex++, L"Token", 500));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_ctrlListService.AddColumn(new CXTPReportColumn(colIndex++, L"Service Port", 100));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_ctrlListService.AddColumn(new CXTPReportColumn(colIndex++, L"Status", 100));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_ctrlListService.AddColumn(new CXTPReportColumn(colIndex++, L"TimeStamp", 200));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	pColum = m_ctrlListService.AddColumn(new CXTPReportColumn(colIndex++, L"Seq", 100));
	pColum->SetEditable(FALSE);
	pColum->SetAllowDrag(FALSE);
	m_ctrlListService.AllowEdit(FALSE);
	m_ctrlListService.AllowSort(FALSE);
	m_ctrlListService.SetMultipleSelection(FALSE);
	m_ctrlListService.InitialSelectionEnable(TRUE);
}

afx_msg LRESULT ServiceManagerDlg::OnServiceModify(WPARAM wParam, LPARAM lParam)
{
	CoreServiceInfo* pInfo = (CoreServiceInfo*)lParam;
	if (wParam == SERVICE_ADDED)
	{
		AddItem(*pInfo);
	}
	else if (wParam == SERVICE_REMOVED)
	{
		RemoveItem(*pInfo);
	}else if(wParam == SERVICE_CHANGED)
	{
		UpdateItem(*pInfo);
	}
	return 0;
}


void ServiceManagerDlg::OnBnClickedButtonNewApp()
{
	// TODO: Add your control notification handler code here
	
}
