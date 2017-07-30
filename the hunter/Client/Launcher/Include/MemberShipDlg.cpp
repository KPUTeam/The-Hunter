// MemberShipDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Launcher.h"
#include "MemberShipDlg.h"
#include "afxdialogex.h"


// CMemberShipDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMemberShipDlg, CDialogEx)

CMemberShipDlg::CMemberShipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MembershipDiaglog, pParent)
	, m_strID(_T(""))
	, m_strPass(_T(""))
{

}

CMemberShipDlg::~CMemberShipDlg()
{
}

void CMemberShipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
}


BEGIN_MESSAGE_MAP(CMemberShipDlg, CDialogEx)
	ON_BN_CLICKED(ID_JOIN, &CMemberShipDlg::OnBnClickedJoin)
END_MESSAGE_MAP()


// CMemberShipDlg 메시지 처리기입니다.


void CMemberShipDlg::OnBnClickedJoin()
{
	// 회원가입 창에서 회원가입을 눌렀을 때
	UpdateData(TRUE);

	memset(&m_tPacket, 0, sizeof(m_tPacket));

	m_tPacket.eHeader = PH_JOIN;

	// 1. ID 길이를 넣는다.
	int iLength = m_strID.GetLength();
	memcpy(m_tPacket.strPacket, &iLength, 4);
	m_tPacket.iSize += 4;

	// 2. ID를 넣는다.
	memcpy(m_tPacket.strPacket + m_tPacket.iSize, m_strID.GetString(), iLength);
	m_tPacket.iSize += iLength;

	// 3. Pass 길이를 넣는다.
	iLength = m_strPass.GetLength();
	memcpy(m_tPacket.strPacket + m_tPacket.iSize, &iLength, 4);
	m_tPacket.iSize += 4;

	// 4. Pass를 넣는다.
	memcpy(m_tPacket.strPacket + m_tPacket.iSize, m_strPass.GetString(), iLength);
	m_tPacket.iSize += iLength;

	// 5. 패킷 조립이 다 되었으면 패킷을 보낸다!
	SOCKET	hSocket = CNetworkManager::GetInst()->GetSocket();
	send(hSocket, (char*)&m_tPacket, m_tPacket.iSize + 8, 0);
}
