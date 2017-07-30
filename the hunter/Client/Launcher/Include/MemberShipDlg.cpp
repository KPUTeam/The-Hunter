// MemberShipDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Launcher.h"
#include "MemberShipDlg.h"
#include "afxdialogex.h"


// CMemberShipDlg ��ȭ �����Դϴ�.

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


// CMemberShipDlg �޽��� ó�����Դϴ�.


void CMemberShipDlg::OnBnClickedJoin()
{
	// ȸ������ â���� ȸ�������� ������ ��
	UpdateData(TRUE);

	memset(&m_tPacket, 0, sizeof(m_tPacket));

	m_tPacket.eHeader = PH_JOIN;

	// 1. ID ���̸� �ִ´�.
	int iLength = m_strID.GetLength();
	memcpy(m_tPacket.strPacket, &iLength, 4);
	m_tPacket.iSize += 4;

	// 2. ID�� �ִ´�.
	memcpy(m_tPacket.strPacket + m_tPacket.iSize, m_strID.GetString(), iLength);
	m_tPacket.iSize += iLength;

	// 3. Pass ���̸� �ִ´�.
	iLength = m_strPass.GetLength();
	memcpy(m_tPacket.strPacket + m_tPacket.iSize, &iLength, 4);
	m_tPacket.iSize += 4;

	// 4. Pass�� �ִ´�.
	memcpy(m_tPacket.strPacket + m_tPacket.iSize, m_strPass.GetString(), iLength);
	m_tPacket.iSize += iLength;

	// 5. ��Ŷ ������ �� �Ǿ����� ��Ŷ�� ������!
	SOCKET	hSocket = CNetworkManager::GetInst()->GetSocket();
	send(hSocket, (char*)&m_tPacket, m_tPacket.iSize + 8, 0);
}
