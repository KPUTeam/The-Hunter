// LoginForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Launcher.h"
#include "LoginForm.h"
#include "MemberShipDlg.h"

// CLoginForm
#define LOGINSERVERPORT 9000

IMPLEMENT_DYNCREATE(CLoginForm, CFormView)

CString	 ipNumber = {};
CLoginForm::CLoginForm()
	: CFormView(IDD_LOGIN_DIALOG)
	, m_strID(_T(""))
	, m_strPass(_T(""))
	, m_StrIP(_T(""))
{

}

CLoginForm::~CLoginForm()
{
	SelectObject(m_hBackDC, m_hOld);
	DeleteObject(m_hBack);
	DeleteDC(m_hBackDC);
}

void CLoginForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
	DDX_Text(pDX, IDC_EDIT_IP, m_StrIP);
}

BEGIN_MESSAGE_MAP(CLoginForm, CFormView)
	ON_BN_CLICKED(ID_LOGIN, &CLoginForm::OnBnClickedLogin)
	ON_BN_CLICKED(ID_JOIN, &CLoginForm::OnBnClickedJoin)
	ON_BN_CLICKED(ID_CONNECT, &CLoginForm::OnBnClickedConnect)
END_MESSAGE_MAP()


// CLoginForm �����Դϴ�.

#ifdef _DEBUG
void CLoginForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLoginForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLoginForm �޽��� ó�����Դϴ�.


void CLoginForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CDC* pDC = GetDC();

	// ��� �̹����� �ε��Ѵ�.
	m_hBackDC = CreateCompatibleDC(pDC->GetSafeHdc());
	m_hBack = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	GetObject(m_hBack, sizeof(BITMAP), &m_tBmp);
	m_hOld = (HBITMAP)SelectObject(m_hBackDC, m_hBack);
}


void CLoginForm::OnDraw(CDC* pDC)
{
	// ��� �̹����� �����
//	BitBlt(pDC->GetSafeHdc(), 0, 0, m_tBmp.bmWidth, m_tBmp.bmHeight, m_hBackDC, 0, 0, SRCCOPY);
}


void CLoginForm::OnBnClickedLogin()
{
	// �α��� ��ư�� ��������
	UpdateData(TRUE);
	memset(&m_tPacket, 0, sizeof(m_tPacket));
	m_tPacket.eHeader = PH_LOGIN;

	// ��Ŷ�� �����Ѵ�.
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

	// 5. ��Ŷ ������ �� �Ǿ����� ������.
	ipNumber = m_StrIP;
	SOCKET	hSocket = CNetworkManager::GetInst()->GetSocket();
	send(hSocket, (char*)&m_tPacket, m_tPacket.iSize + 8, 0);
}


void CLoginForm::OnBnClickedJoin()
{
	// ȸ������ ��ư�� ��������
	// ȸ������ â�� ����.
	CMemberShipDlg	MemberDlg;
	MemberDlg.DoModal();
}


void CLoginForm::OnBnClickedConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	char	strIP[16] = {};
	strcpy_s(strIP, m_StrIP);

	if (!CNetworkManager::GetInst()->Init(strIP, LOGINSERVERPORT))
		return;
}
