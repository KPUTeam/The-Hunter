#pragma once

#include "Global.h"

class CIocp
{
public:
	CIocp();
	~CIocp();

private:
	HANDLE	m_hComPort;
	vector<class CThread*>	m_vecThread;
	
public:
	HANDLE GetPort()	const;

public:
	bool CreateIOCP();
};

