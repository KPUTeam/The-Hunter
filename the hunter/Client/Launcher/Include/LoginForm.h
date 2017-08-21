#pragma once



// CLoginForm 폼 뷰입니다.

#include "NetworkManager.h"

class CLoginForm : public CFormView
{
	DECLARE_DYNCREATE(CLoginForm)

protected:
	CLoginForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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


