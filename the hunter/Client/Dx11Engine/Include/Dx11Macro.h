
#pragma once

#ifdef DX11_IMPORT
#define	DX11_DLL	__declspec(dllimport)
#else
#define	DX11_DLL	__declspec(dllexport)
#endif // MOSQUITO_IMPORT

#define	DX11_BEGIN		namespace Dx11 {
#define	DX11_END		}
#define	DX11_USING		using namespace Dx11;

#define	SAFE_DELETE(p)	if(p)	{delete p; p = NULL;}
#define	SAFE_DELETE_ARRAY(p)	if(p)	{delete [] p; p = NULL;}
#define	SAFE_RELEASE(p)	if(p)	{p->Release(); p = NULL;}

#define	DX11_SINGLE_DECLARE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if(!m_pInst)\
				m_pInst	= new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();

#define	SINGLE_DECLARE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if(!m_pInst)\
				m_pInst	= new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();

#define	DX11_SINGLE_DEFINITION(Type)	Dx11::Type* Dx11::Type::m_pInst	= NULL;
#define	SINGLE_DEFINITION(Type)	Type* Type::m_pInst	= NULL;

#define	DX11_GET_SINGLE(Type)			Dx11::Type::GetInst()
#define	DX11_DESTROY_SINGLE(Type)		Dx11::Type::DestroyInst()

#define	GET_SINGLE(Type)			Type::GetInst()
#define	DESTROY_SINGLE(Type)		Type::DestroyInst()

#define	DX11_DEVICE		Dx11::CDx11Device::GetInst()->GetDevice()
#define	DX11_CONTEXT	Dx11::CDx11Device::GetInst()->GetContext()

#define	DX11_GRAVITY	Dx11::CDx11Core::GetInst()->GetGravity()
