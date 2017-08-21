#pragma once


// CMemberShipDlg 대화 상자입니다.
#include "NetworkManager.h"

class CMemberShipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMemberShipDlg)

public:
	CMemberShipDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMemberShipDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MembershipDiaglog };
#endif

private:
	PACKET	m_tPacket;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strID;
	CString m_strPass;
	afx_msg void OnBnClickedJoin();
};
