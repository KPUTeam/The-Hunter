
// Launcher.h : Launcher ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CLauncherApp:
// �� Ŭ������ ������ ���ؼ��� Launcher.cpp�� �����Ͻʽÿ�.
//

class CLauncherApp : public CWinApp
{
public:
	CLauncherApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CLauncherApp theApp;
