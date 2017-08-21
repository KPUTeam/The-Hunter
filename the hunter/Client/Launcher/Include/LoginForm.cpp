// LoginForm.cpp : 구현 파일입니다.
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


// CLoginForm 진단입니다.

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


// CLoginForm 메시지 처리기입니다.


void CLoginForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CDC* pDC = GetDC();

	// 배경 이미지를 로딩한다.
	m_hBackDC = CreateCompatibleDC(pDC->GetSafeHdc());
	m_hBack = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	GetObject(m_hBack, sizeof(BITMAP), &m_tBmp);
	m_hOld = (HBITMAP)SelectObject(m_hBackDC, m_hBack);
}


void CLoginForm::OnDraw(CDC* pDC)
{
	// 배경 이미지를 출력함
//	BitBlt(pDC->GetSafeHdc(), 0, 0, m_tBmp.bmWidth, m_tBmp.bmHeight, m_hBackDC, 0, 0, SRCCOPY);
}


void CLoginForm::OnBnClickedLogin()
{
	// 로그인 버튼을 눌렀을때
	UpdateData(TRUE);
	memset(&m_tPacket, 0, sizeof(m_tPacket));
	m_tPacket.eHeader = PH_LOGIN;

	// 패킷을 조립한다.
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

	// 5. 패킷 조립이 다 되었으면 보낸다.
	ipNumber = m_StrIP;
	SOCKET	hSocket = CNetworkManager::GetInst()->GetSocket();
	send(hSocket, (char*)&m_tPacket, m_tPacket.iSize + 8, 0);
}


void CLoginForm::OnBnClickedJoin()
{
	// 회원가입 버튼을 눌렀을때
	// 회원가입 창을 연다.
	CMemberShipDlg	MemberDlg;
	MemberDlg.DoModal();
}


void CLoginForm::OnBnClickedConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	char	strIP[16] = {};
	strcpy_s(strIP, m_StrIP);

	if (!CNetworkManager::GetInst()->Init(strIP, LOGINSERVERPORT))
		return;
}
