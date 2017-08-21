#pragma once



// CLoginForm �� ���Դϴ�.

#include "NetworkManager.h"

class CLoginForm : public CFormView
{
	DECLARE_DYNCREATE(CLoginForm)

protected:
	CLoginForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CLoginForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	HBITMAP	m_hBack;
	HBITMAP	m_hOld;
	HDC		m_hBackDC;
	BITMAP	m_tBmp;
	PACKET	m_tPacket;

public:
	CString m_strID;
	CString m_strPass;
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedJoin();
	CString m_StrIP;
	afx_msg void OnBnClickedConnect();
};


