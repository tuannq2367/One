///////////////////////////////////////////////////////////////////////////////
// This file is a part of the HMIViewer Project.
// Copyright (c) 2016 Applied Technical Systems Co. Ltd., 
// All Rights Reserved.
// 
// AUTHOR(s): 
//
// DESCRIPTION:
// SingleInstance.h:
///////////////////////////////////////////////////////////////////////////////

#ifndef SingleInstance_h__
#define SingleInstance_h__

class CSingleInstance : public CObject
{
public:
	~CSingleInstance();

	static CSingleInstance* GetInstance();
	BOOL IsRunning();
	BOOL SetSingleInstance();
	BOOL WriteInstanceData(HWND hMainWindow);
	BOOL InitializeOnce();
	BOOL ExitInstance();
	BOOL ActivateFirstInstance();

protected:
	CSingleInstance();

private:
	CString CreateMMFileName();
	CMutex m_instanceMutex;
	HANDLE m_hPrevInstance; // MMF Handle

};

#endif // SingleInstance_h__