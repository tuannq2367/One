///////////////////////////////////////////////////////////////////////////////
// This file is a part of the HMIViewer Project.
// Copyright (c) 2016 Applied Technical Systems Co. Ltd.,
// All Rights Reserved.
// 
// AUTHOR(s): HoangDT
//
// DESCRIPTION:
// SingleInstance.cpp:
///////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "SingleInstance.h"

struct CWindowHwnd
{
	HWND hMainWnd;
};

//Class which be used as a static to ensure that we
//only close the file mapping at the very last chance
class InstanceData
{
public:
	InstanceData();
	~InstanceData();

protected:
	HANDLE hInstanceData;
	friend class CSingleInstance;
};

InstanceData::InstanceData()
{
	hInstanceData = NULL;
}

InstanceData::~InstanceData()
{
	if (hInstanceData != NULL)
	{
		::CloseHandle(hInstanceData);
		hInstanceData = NULL;
	}
}

static InstanceData instanceData;

CSingleInstance::CSingleInstance():
m_instanceMutex(FALSE, _T("InstanceMutex"))
{
	// Only one instance of this class created
	VERIFY(instanceData.hInstanceData == NULL);

	m_hPrevInstance = NULL;
}

CSingleInstance::~CSingleInstance()
{
	if (m_hPrevInstance)
	{
		::CloseHandle(m_hPrevInstance);
		m_hPrevInstance = NULL;
	}
}

CSingleInstance* CSingleInstance::GetInstance()
{
	static CSingleInstance _instance;
	return &_instance;
}

BOOL CSingleInstance::IsRunning()
{
	ASSERT(m_hPrevInstance == NULL); 	
	m_hPrevInstance = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, CreateMMFileName());

	return (m_hPrevInstance != NULL);
}

BOOL CSingleInstance::ActivateFirstInstance()
{
	if (m_hPrevInstance != NULL) 
	{
		// Open up the MMF
		int nMMFSize = sizeof(CWindowHwnd);
		CWindowHwnd* pInstanceData = (CWindowHwnd*)::MapViewOfFile(m_hPrevInstance, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, nMMFSize);
		if (pInstanceData != NULL)
		{
			// Lock the data prior to reading from it
			CSingleLock locked(&m_instanceMutex, TRUE);

			//activate the old window
			HWND hWindow = pInstanceData->hMainWnd;
			CWnd wndPrev;
			if (!hWindow || !::IsWindow(hWindow))
			{
				return FALSE;
			}
			wndPrev.Attach(hWindow);
			CWnd* pWndChild = wndPrev.GetLastActivePopup();
			if (wndPrev.IsIconic())
				wndPrev.ShowWindow(SW_RESTORE);

			pWndChild->SetForegroundWindow();			
			wndPrev.Detach();

			VERIFY(::UnmapViewOfFile(pInstanceData));
			::CloseHandle(m_hPrevInstance);
			m_hPrevInstance = NULL;

			return TRUE;
		}
		else
			VERIFY(FALSE);  

		::CloseHandle(m_hPrevInstance);
		m_hPrevInstance = NULL;
	}
	else
		VERIFY(FALSE); 

	return FALSE;
}

BOOL CSingleInstance::SetSingleInstance()
{
	VERIFY(IsRunning() == FALSE);

	//If this is the first instance then copy in our info into the shared memory
	if (m_hPrevInstance == NULL)
	{
		//Create the MMF
		int nMMFSize = sizeof(CWindowHwnd); 		
		instanceData.hInstanceData = ::CreateFileMapping(INVALID_HANDLE_VALUE/*(HANDLE)0xFFFFFFFF*/, NULL, PAGE_READWRITE, 0, nMMFSize, CreateMMFileName());
		DWORD dw = GetLastError(); 

		VERIFY(instanceData.hInstanceData != NULL); //Creating the MMF should work
	}

	return (instanceData.hInstanceData != NULL);
}

BOOL CSingleInstance::WriteInstanceData(HWND hMainWindow)
{
	if (instanceData.hInstanceData)
	{
		//Open the MMF
		int nMMFSize = sizeof(CWindowHwnd); 	
		CWindowHwnd* pInstanceData = (CWindowHwnd*) ::MapViewOfFile(instanceData.hInstanceData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, nMMFSize);
		if(pInstanceData == NULL)
		{
			return FALSE;
		}

		// Lock the data prior to updating it
		CSingleLock locked(&m_instanceMutex, TRUE);

		pInstanceData->hMainWnd = hMainWindow;

		VERIFY(::UnmapViewOfFile(pInstanceData));
	}

	return (instanceData.hInstanceData != NULL);
}

BOOL CSingleInstance::InitializeOnce()
{
	VERIFY(IsRunning() == FALSE);

	//If this is the first instance then copy in our info into the shared memory
	if (m_hPrevInstance == NULL)
	{
		//Create the MMF
		int nMMFSize = sizeof(CWindowHwnd); 		
		instanceData.hInstanceData = ::CreateFileMapping(INVALID_HANDLE_VALUE/*(HANDLE)0xFFFFFFFF*/, NULL, PAGE_READWRITE, 0, nMMFSize, CreateMMFileName());
		DWORD dw = GetLastError(); 

		VERIFY(instanceData.hInstanceData != NULL); //Creating the MMF should work

		//Open the MMF
		CWindowHwnd* pInstanceData = (CWindowHwnd*) ::MapViewOfFile(instanceData.hInstanceData, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, nMMFSize);
		VERIFY(pInstanceData != NULL);   

		// Lock the data prior to updating it
		CSingleLock locked(&m_instanceMutex, TRUE);

		ASSERT(AfxGetMainWnd() != NULL);
		ASSERT(AfxGetMainWnd()->GetSafeHwnd() != NULL);
		pInstanceData->hMainWnd = AfxGetMainWnd()->GetSafeHwnd();

		VERIFY(::UnmapViewOfFile(pInstanceData));
	}

	return (m_hPrevInstance == NULL);
}

BOOL CSingleInstance::ExitInstance()
{
	return WriteInstanceData(0);
}

CString CSingleInstance::CreateMMFileName()
{
	CString suffix(_T("")); // = _T("Global\\");
	suffix += AfxGetAppName();
	suffix += _T("_MMF_");
	return suffix;
}

