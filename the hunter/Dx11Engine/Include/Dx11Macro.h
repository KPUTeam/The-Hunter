
// Macro.h
// 매크로함수를 저장

#pragma once
#define	DX11_BEGIN	namespace DX11 {
#define	DX11_END	}
#define	DX11_USING	using namespace DX11;

#define SAFE_DELETE(p)	if(p)	{ delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p)	if(p) { delete[] p; p = NULL;}
#define	SAFE_RELEASE(p)	if(p)	{p->Release(); p = NULL;}

// 싱글톤 선언 매크로
// 클래스 형식으로 작성하였다.
// static 타입으로 포인터 변수를 선언
#define DX11_DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_pInst;\
public:\
	static Type*	GetInst()\
	{\
		if(!m_pInst)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_pInst);\
	}\
private:\
	Type();\
	~Type();

//싱글톤 static 변수를 정의
#define	DX11_DEFINITION_SINGLE(Type)	Type* Type::m_pInst	= NULL;


#define	DX11_GET_SINGLE(Type)	DX11::Type::GetInst()
#define	DX11_DESTROY_SINGLE(Type)	DX11::Type::DestroyInst()
#define	GET_SINGLE(Type)	Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()

#define	DX11_DEVICE		DX11::CDx11Device::GetInst()->GetDevice()
#define	DX11_CONTEXT	DX11::CDx11Device::GetInst()->GetContext()
#define	DX11_RESOLUTION	DX11::CDx11Device::GetInst()->GetResolutionType()