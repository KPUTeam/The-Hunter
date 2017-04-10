
// Types.h
// 인클루드할 헤더파일, 네임스페이스
// 사용자 정의 자료형, 헤더를 모아둠.

#define	_XM_NO_INTRINSICS_

#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <crtdbg.h>
#include <conio.h>
#include <functional>
#include <algorithm>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "DirectXTex")

//사용자 정의 헤더파일
#include "Dx11Macro.h"
#include "Dx11Flag.h"

DX11_BEGIN

typedef XMVECTOR	Vec;
typedef XMVECTOR*	PVec;

//Resolution
typedef struct _tagResolution
{
	UINT	iWidth;
	UINT	iHeight;

	_tagResolution() :
		iWidth(0),
		iHeight(0)
	{
	}

	_tagResolution(UINT iW, UINT iH) :
		iWidth(iW),
		iHeight(iH)
	{
	}
}RESOLUTION, *PRESOLUTION;

const RESOLUTION	g_tRS[RT_MAX] =
{
	RESOLUTION(520, 640),
	RESOLUTION(800, 600),
	RESOLUTION(1280, 720),
	RESOLUTION(1920, 1080),
};

// Window SDK에서 지원하는 DirectXMath는 수학적인 연산을 제공하지 않는다.
// 따라서 Vector 구조체와 그 연산 관련 함수를 모두 만들어주어야 한다.

// 다음은 Vector2, Vector3, Vector4 구조체와 그 멤버함수를 정의한 코드이다.


typedef struct _tagVector2 :
	public XMFLOAT2
{
	_tagVector2()
	{
		x = 0.f;
		y = 0.f;
	}

	_tagVector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	_tagVector2 operator +(const _tagVector2& v) const
	{
		_tagVector2 v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		return v1;
	}

	_tagVector2 operator +(int iValue) const
	{
		_tagVector2 v1;
		v1.x = x + (float)iValue;
		v1.y = y + (float)iValue;
		return v1;
	}

	void operator +=(const _tagVector2& v)
	{
		x += v.x;
		y += v.y;
	}
	void operator +=(float fValue)
	{
		x += fValue;
		y += fValue;
	}

	void operator +=(int iValue)
	{
		x += (float)iValue;
		y += (float)iValue;
	}

	_tagVector2 operator -(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		return v1;
	}

	_tagVector2 operator -(float fValue)	const
	{
		_tagVector2	v1;
		v1.x = x - fValue;
		v1.y = y - fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector2 operator -(int iValue)	const
	{
		_tagVector2	v1;
		v1.x = x - (float)iValue;
		v1.y = y - (float)iValue;
		return v1;
	}

	void operator -=(const _tagVector2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	void operator -=(float fValue)
	{
		x -= fValue;
		y -= fValue;
	}

	void operator -=(int iValue)
	{
		x -= (float)iValue;
		y -= (float)iValue;
	}

	_tagVector2 operator *(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		return v1;
	}

	_tagVector2 operator *(float fValue)	const
	{
		_tagVector2	v1;
		v1.x = x * fValue;
		v1.y = y * fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector2 operator *(int iValue)	const
	{
		_tagVector2	v1;
		v1.x = x * (float)iValue;
		v1.y = y * (float)iValue;
		return v1;
	}

	void operator *=(const _tagVector2& v)
	{
		x *= v.x;
		y *= v.y;
	}

	void operator *=(float fValue)
	{
		x *= fValue;
		y *= fValue;
	}

	void operator *=(int iValue)
	{
		x *= (float)iValue;
		y *= (float)iValue;
	}

	_tagVector2 operator /(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		return v1;
	}

	_tagVector2 operator /(float fValue)	const
	{
		_tagVector2	v1;
		v1.x = x / fValue;
		v1.y = y / fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector2 operator /(int iValue)	const
	{
		_tagVector2	v1;
		v1.x = x / (float)iValue;
		v1.y = y / (float)iValue;
		return v1;
	}

	void operator /=(const _tagVector2& v)
	{
		x /= v.x;
		y /= v.y;
	}

	void operator /=(float fValue)
	{
		x /= fValue;
		y /= fValue;
	}

	void operator /=(int iValue)
	{
		x /= (float)iValue;
		y /= (float)iValue;
	}

	void operator =(const POINT& pt)
	{
		x = pt.x;
		y = pt.y;
	}

	bool operator ==(const _tagVector2& v)	const
	{
		return x == v.x && y == v.y;
	}

	bool operator !=(const _tagVector2& v)	const
	{
		return x != v.x || y != v.y;
	}


	float Length()	const
	{
		return sqrtf(x * x + y * y);
	}

	float Distance(const _tagVector2& v) const
	{
		_tagVector2 v1 = *this - v;
		return v1.Length();
	}

	_tagVector2 Normalize()	const
	{
		_tagVector2	v;
		float	fLength = Length();
		v.x = x / fLength;
		v.y = y / fLength;
		return v;
	}

	float Dot(const _tagVector2& v)	const
	{
		return x * v.x + y * v.y;
	}
}VECTOR2, *PVECTOR2;

typedef struct _tagVector3 :
	public XMFLOAT3
{
	_tagVector3()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	_tagVector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	_tagVector3 operator +(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		v1.z = z + v.z;
		return v1;
	}

	_tagVector3 operator +(float fValue)	const
	{
		_tagVector3	v1;
		v1.x = x + fValue;
		v1.y = y + fValue;
		v1.z = z + fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector3 operator +(int iValue)	const
	{
		_tagVector3	v1;
		v1.x = x + (float)iValue;
		v1.y = y + (float)iValue;
		v1.z = z + (float)iValue;
		return v1;
	}

	void operator +=(const _tagVector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator +=(float fValue)
	{
		x += fValue;
		y += fValue;
		z += fValue;
	}

	void operator +=(int iValue)
	{
		x += (float)iValue;
		y += (float)iValue;
		z += (float)iValue;
	}

	_tagVector3 operator -(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		v1.z = z - v.z;
		return v1;
	}

	_tagVector3 operator -(float fValue)	const
	{
		_tagVector3	v1;
		v1.x = x - fValue;
		v1.y = y - fValue;
		v1.z = z - fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector3 operator -(int iValue)	const
	{
		_tagVector3	v1;
		v1.x = x - (float)iValue;
		v1.y = y - (float)iValue;
		v1.z = z - (float)iValue;
		return v1;
	}

	void operator -=(const _tagVector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	void operator -=(float fValue)
	{
		x -= fValue;
		y -= fValue;
		z -= fValue;
	}

	void operator -=(int iValue)
	{
		x -= (float)iValue;
		y -= (float)iValue;
		z -= (float)iValue;
	}

	_tagVector3 operator *(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		v1.z = z * v.z;
		return v1;
	}

	_tagVector3 operator *(float fValue)	const
	{
		_tagVector3	v1;
		v1.x = x * fValue;
		v1.y = y * fValue;
		v1.z = z * fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector3 operator *(int iValue)	const
	{
		_tagVector3	v1;
		v1.x = x * (float)iValue;
		v1.y = y * (float)iValue;
		v1.z = z * (float)iValue;
		return v1;
	}

	void operator *=(const _tagVector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	void operator *=(float fValue)
	{
		x *= fValue;
		y *= fValue;
		z *= fValue;
	}

	void operator *=(int iValue)
	{
		x *= (float)iValue;
		y *= (float)iValue;
		z *= (float)iValue;
	}

	_tagVector3 operator /(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		v1.z = z / v.z;
		return v1;
	}

	_tagVector3 operator /(float fValue)	const
	{
		_tagVector3	v1;
		v1.x = x / fValue;
		v1.y = y / fValue;
		v1.z = z / fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector3 operator /(int iValue)	const
	{
		_tagVector3	v1;
		v1.x = x / (float)iValue;
		v1.y = y / (float)iValue;
		v1.z = z / (float)iValue;
		return v1;
	}

	void operator /=(const _tagVector3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

	void operator /=(float fValue)
	{
		x /= fValue;
		y /= fValue;
		z /= fValue;
	}

	void operator /=(int iValue)
	{
		x /= (float)iValue;
		y /= (float)iValue;
		z /= (float)iValue;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	_tagVector3 Normalize()	const
	{
		_tagVector3	v;
		float	fLength = Length();
		v.x = x / fLength;
		v.y = y / fLength;
		v.z = z / fLength;
		return v;
	}

	float Distance(const _tagVector3& v) const
	{
		_tagVector3 v1 = *this - v;
		return v1.Length();
	}

	float Dot(const _tagVector3& v)	const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	_tagVector3 Cross(const _tagVector3& v)
	{
		return _tagVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	//XMVECTOR3 -> XMFLOAT3
	_tagVector3 Convert(const Vec& v)
	{
		XMStoreFloat3(this, v);
		return *this;
	}

	//XMFLOAT3 -> XMVECTOR3
	Vec Convert() const
	{
		return XMLoadFloat3(this);
	}
}VECTOR3, *PVECTOR3;

const _tagVector3 Vec3Zero = _tagVector3(0.f, 0.f, 0.f);
const _tagVector3 Vec3One = _tagVector3(1.f, 1.f, 1.f);
const _tagVector3 Vec3X = _tagVector3(1.f, 0.f, 0.f);
const _tagVector3 Vec3Y = _tagVector3(0.f, 1.f, 0.f);
const _tagVector3 Vec3Z = _tagVector3(0.f, 0.f, 1.f);

typedef struct _tagVector4 :
	public XMFLOAT4
{
	_tagVector4()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	}

	_tagVector4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	_tagVector4 operator +(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		v1.z = z + v.z;
		v1.w = w + v.w;
		return v1;
	}

	_tagVector4 operator +(float fValue)	const
	{
		_tagVector4	v1;
		v1.x = x + fValue;
		v1.y = y + fValue;
		v1.z = z + fValue;
		v1.w = w + fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector4 operator +(int iValue)	const
	{
		_tagVector4	v1;
		v1.x = x + (float)iValue;
		v1.y = y + (float)iValue;
		v1.z = z + (float)iValue;
		v1.w = w + (float)iValue;
		return v1;
	}

	void operator +=(const _tagVector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}

	void operator +=(float fValue)
	{
		x += fValue;
		y += fValue;
		z += fValue;
		w += fValue;
	}

	void operator +=(int iValue)
	{
		x += (float)iValue;
		y += (float)iValue;
		z += (float)iValue;
		w += (float)iValue;
	}

	_tagVector4 operator -(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		v1.z = z - v.z;
		v1.w = w - v.w;
		return v1;
	}

	_tagVector4 operator -(float fValue)	const
	{
		_tagVector4	v1;
		v1.x = x - fValue;
		v1.y = y - fValue;
		v1.z = z - fValue;
		v1.w = w - fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector4 operator -(int iValue)	const
	{
		_tagVector4	v1;
		v1.x = x - (float)iValue;
		v1.y = y - (float)iValue;
		v1.z = z - (float)iValue;
		v1.w = w - (float)iValue;
		return v1;
	}

	void operator -=(const _tagVector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}

	void operator -=(float fValue)
	{
		x -= fValue;
		y -= fValue;
		z -= fValue;
		w -= fValue;
	}

	void operator -=(int iValue)
	{
		x -= (float)iValue;
		y -= (float)iValue;
		z -= (float)iValue;
		w -= (float)iValue;
	}

	_tagVector4 operator *(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		v1.z = z * v.z;
		v1.w = w * v.w;
		return v1;
	}

	_tagVector4 operator *(float fValue)	const
	{
		_tagVector4	v1;
		v1.x = x * fValue;
		v1.y = y * fValue;
		v1.z = z * fValue;
		v1.w = w * fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector4 operator *(int iValue)	const
	{
		_tagVector4	v1;
		v1.x = x * (float)iValue;
		v1.y = y * (float)iValue;
		v1.z = z * (float)iValue;
		v1.w = w * (float)iValue;
		return v1;
	}

	void operator *=(const _tagVector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
	}

	void operator *=(float fValue)
	{
		x *= fValue;
		y *= fValue;
		z *= fValue;
		w *= fValue;
	}

	void operator *=(int iValue)
	{
		x *= (float)iValue;
		y *= (float)iValue;
		z *= (float)iValue;
		w *= (float)iValue;
	}

	_tagVector4 operator /(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		v1.z = z / v.z;
		v1.w = w / v.w;
		return v1;
	}

	_tagVector4 operator /(float fValue)	const
	{
		_tagVector4	v1;
		v1.x = x / fValue;
		v1.y = y / fValue;
		v1.z = z / fValue;
		v1.w = w / fValue;
		return v1;
	}

	// 함수 뒤에 const를 붙여주게 되면 이 함수에서는 멤버변수의
	// 값을 변경할 수 없다.
	_tagVector4 operator /(int iValue)	const
	{
		_tagVector4	v1;
		v1.x = x / (float)iValue;
		v1.y = y / (float)iValue;
		v1.z = z / (float)iValue;
		v1.w = w / (float)iValue;
		return v1;
	}

	void operator /=(const _tagVector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
	}

	void operator /=(float fValue)
	{
		x /= fValue;
		y /= fValue;
		z /= fValue;
		w /= fValue;
	}

	void operator /=(int iValue)
	{
		x /= (float)iValue;
		y /= (float)iValue;
		z /= (float)iValue;
		w /= iValue;
	}

	void operator =(const _tagVector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	_tagVector4 Normalize()	const
	{
		_tagVector4	v;
		float	fLength = Length();
		v.x = x / fLength;
		v.y = y / fLength;
		v.z = z / fLength;
		v.w = w / fLength;
		return v;
	}

	float Dot(const _tagVector4& v)	const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	//XMVECTOR4 -> XMFLOAT4
	_tagVector4 Convert(const Vec& v)
	{
		XMStoreFloat4(this, v);
		return *this;
	}

	//XMFLOAT4 -> XMVECTOR4
	Vec Convert()
	{
		return XMLoadFloat4(this);
	}
}VECTOR4, *PVECTOR4;


// 구조체명을 편하기 쓰기위해 typedef을 해줌.
typedef VECTOR2		Vec2;
typedef VECTOR2*	PVec2;
typedef VECTOR3		Vec3;
typedef VECTOR3*	PVec3;
typedef VECTOR4		Vec4;
typedef VECTOR4*	PVec4;
typedef XMMATRIX	Matrix;

// 조명
const Vec4	White = Vec4(1.f, 1.f, 1.f, 1.f);
const Vec4	Black = Vec4(0.f, 0.f, 0.f, 1.f);
const Vec4	Red = Vec4(1.f, 0.f, 0.f, 1.f);
const Vec4	Green = Vec4(0.f, 1.f, 0.f, 1.f);
const Vec4	Blue = Vec4(0.f, 0.f, 1.f, 1.f);
const Vec4	Yellow = Vec4(1.f, 1.f, 0.f, 1.f);

// Color Vertex
typedef struct _tagColorVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec4		vColor;
}COLORVERTEX, *PCOLORVERTEX;


// Texture Vertex
typedef struct _tagTexVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vUV;
}TEXVERTEX, *PTEXVERTEX;


// Bump Vertex
typedef struct _tagBumpVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vUV;
	Vec3		vTangent;
	Vec3		vBinormal;

	_tagBumpVertex() :
		vPos(Vec3Zero),
		vNormal(Vec3Zero),
		vUV(0.f, 0.f),
		vTangent(Vec3Zero),
		vBinormal(Vec3Zero)
	{
	}

	_tagBumpVertex(const Vec3& vP, const Vec3& vN, const Vec2& vU,  
		const Vec3& vT, const Vec3& vB) :
		vPos(vP),
		vNormal(vN),
		vUV(vU),
		vTangent(vT),
		vBinormal(vB)
	{
	}

	_tagBumpVertex(float x, float y, float z, float nx, float ny, float nz,
		float ux, float uy, float tx, float ty, float tz, float bx, float by, float bz) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vUV(ux, uy),
		vTangent(tx, ty, tz),
		vBinormal(bx, by, bz)
	{
	}
}BUMPVERTEX, *PBUMPVERTEX;


// Animation Vertex
typedef struct _tagAnimationTexVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vUV;
	Vec4		vWeights;
	Vec4		vIndices;
}ANIMTEXVERTEX, *PANIMTEXVERTEX;

typedef struct _tagAnimationBumpVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vUV;
	Vec4		vWeights;
	Vec4		vIndices;
	Vec3		vTangent;
}ANIMBUMPVERTEX, *PANIMBUMPVERTEX;


// UI Vertex
typedef struct _tagUIVertex
{
	Vec3	vPos;
	Vec2	vUV;

	_tagUIVertex() :
		vPos(0.f, 0.f, 0.f),
		vUV(0.f, 0.f)
	{
	}

	_tagUIVertex(const Vec3& _vPos, const Vec2& _vUV) :
		vPos(_vPos),
		vUV(_vUV)
	{
	}

	_tagUIVertex(float x, float y, float z, float u, float v) :
		vPos(x, y, z),
		vUV(u, v)
	{
	}
}UIVERTEX, *PUIVERTEX;


// Transform Constant Buffer
typedef struct _tagTransformCBuffer
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matWV;
	Matrix matWVP;
}TRANSFORMCBUFFER, *PTRANSFORMCBUFFER;

// Key Info
typedef struct _tagKeyInfo {

	vector<UINT>	vecKey;		// 해당 키
	string			strName;	// 키의 이름
	bool			bDown;		// 버튼 누름
	bool			bPush;		// 버튼 누르는 중
	bool			bUp;		// 버튼 땜
}KEYINFO, *PKEYINFO;


// Light Info
typedef struct _tagLightInfo
{
	Vec4	vDif;
	Vec4	vAmb;
	Vec4	vSpc;
	int		iLightType;
	Vec3	vLightEmpty;

}LIGHTINFO, *PLIGHTINFO;


// Material Info
typedef struct _tagMaterial
{
	Vec4	vDif;
	Vec4	vAmb;
	Vec4	vSpc;
	Vec4	vEmv;

	_tagMaterial() :
		vDif(White),
		vAmb(White),
		vSpc(White),
		vEmv(White)
	{
	}
}MATERIAL, *PMATERIAL;

// Spot Light Info
typedef struct _tagSpotlight
{
	Vec3	vPos;
	float	fRange;
	Vec3	vDir;
	float	fSpot;
}SPOTLIGHT, *PSPOTLIGHT;


// Spot Light Info
typedef struct _tagFogInfo
{
	Vec4	vColor;
	float	fStart;
	Vec3	vEyePos;
	float	fRange;
	Vec3	vFogEmpty;
}FOGINFO, *PFOGINFO;


// UV Frame
typedef struct _tagUVFrame
{
	UINT	iFrameX;
	UINT	iFrameY;
	UINT	iMaxFrameX;
	UINT	iMaxFrameY;
	UINT	iAnimType;
	Vec3	vEmpty;
}UVFRAME, *PUVFRAME;


DX11_END