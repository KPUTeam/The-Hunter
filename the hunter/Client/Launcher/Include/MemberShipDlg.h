#pragma once


// CMemberShipDlg ��ȭ �����Դϴ�.
#include "NetworkManager.h"

class CMemberShipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMemberShipDlg)

public:
	CMemberShipDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMemberShipDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MembershipDiaglog };
#endif

private:
	PACKET	m_tPacket;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strID;
	CString m_strPass;
	afx_msg void OnBnClickedJoin();
};
