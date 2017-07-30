#pragma once

// Resolution
typedef struct _tagResolution
{
	UINT	iWidth;
	UINT	iHeight;

public:
	_tagResolution() :
		iWidth(0),
		iHeight(0)
	{
	}

	_tagResolution(UINT w, UINT h) :
		iWidth(w),
		iHeight(h)
	{
	}
}RESOLUTION, *PRESOLUTION;

static RESOLUTION	g_tRS[RT_MAX] =
{
	RESOLUTION(800, 600),
	RESOLUTION(1280, 720),
	RESOLUTION(1920, 1080)
};

typedef struct _tagVector2 :
	public XMFLOAT2
{
	_tagVector2()
	{
		x = y = 0.f;
	}

	_tagVector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	_tagVector2 operator +(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		return v1;
	}

	_tagVector2 operator +(float f)	const
	{
		_tagVector2	v;
		v.x = x + f;
		v.y = y + f;
		return v;
	}

	void operator +=(const _tagVector2& v)
	{
		x += v.x;
		y += v.y;
	}

	void operator +=(float f)
	{
		x += f;
		y += f;
	}

	_tagVector2 operator -(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		return v1;
	}

	_tagVector2 operator -(float f)	const
	{
		_tagVector2	v;
		v.x = x - f;
		v.y = y - f;
		return v;
	}

	void operator -=(const _tagVector2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	void operator -=(float f)
	{
		x -= f;
		y -= f;
	}

	_tagVector2 operator *(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		return v1;
	}

	_tagVector2 operator *(float f)	const
	{
		_tagVector2	v;
		v.x = x * f;
		v.y = y * f;
		return v;
	}

	void operator *=(const _tagVector2& v)
	{
		x *= v.x;
		y *= v.y;
	}

	void operator *=(float f)
	{
		x *= f;
		y *= f;
	}


	_tagVector2 operator /(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		return v1;
	}

	_tagVector2 operator /(float f)	const
	{
		_tagVector2	v;
		v.x = x / f;
		v.y = y / f;
		return v;
	}

	void operator /=(const _tagVector2& v)
	{
		x /= v.x;
		y /= v.y;
	}

	void operator /=(float f)
	{
		x /= f;
		y /= f;
	}

	void operator =(const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
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

	_tagVector2 Normalize()
	{
		_tagVector2	v;
		float	f = Length();
		v.x = x / f;
		v.y = y / f;
		return v;
	}

	float Dot(const _tagVector2& v)
	{
		return x * v.x + y * v.y;
	}
}Vector2, *PVector2;


typedef struct _tagVector3 :
	public XMFLOAT3
{
	_tagVector3()
	{
		x = y = z = 0.f;
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

	_tagVector3 operator +(float f)	const
	{
		_tagVector3	v;
		v.x = x + f;
		v.y = y + f;
		v.z = z + f;
		return v;
	}

	void operator +=(const _tagVector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator +=(float f)
	{
		x += f;
		y += f;
		z += f;
	}

	_tagVector3 operator -(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		v1.z = z - v.z;
		return v1;
	}

	_tagVector3 operator -(float f[3])	const
	{
		_tagVector3	v;
		v.x = x - f[0];
		v.y = y - f[1];
		v.z = z - f[2];
		return v;
	}

	_tagVector3 operator -(float f)	const
	{
		_tagVector3	v;
		v.x = x - f;
		v.y = y - f;
		v.z = z - f;
		return v;
	}

	void operator -=(const _tagVector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	void operator -=(float f)
	{
		x -= f;
		y -= f;
		z -= f;
	}

	_tagVector3 operator *(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		v1.z = z * v.z;
		return v1;
	}

	_tagVector3 operator *(float f)	const
	{
		_tagVector3	v;
		v.x = x * f;
		v.y = y * f;
		v.z = z * f;
		return v;
	}

	void operator *=(const _tagVector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	void operator *=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
	}


	_tagVector3 operator /(const _tagVector3& v)
	{
		_tagVector3	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		v1.z = z / v.z;
		return v1;
	}

	_tagVector3 operator /(float f)
	{
		_tagVector3	v;
		v.x = x / f;
		v.y = y / f;
		v.z = z / f;
		return v;
	}

	float operator [](UINT idx)	const
	{
		if (idx == 0)
			return x;

		else if (idx == 1)
			return y;

		else if (idx == 2)
			return z;

		assert(false);
		return 0;
	}

	float operator [](AXIS idx)	const
	{
		if (idx == 0)
			return x;

		else if (idx == 1)
			return y;

		else if (idx == 2)
			return z;

		assert(false);
		return 0;
	}

	void operator /=(const _tagVector3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

	void operator /=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
	}

	bool operator ==(const _tagVector3& v)	const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator !=(const _tagVector3& v)	const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	_tagVector3 Normalize()	const
	{
		_tagVector3	v;
		float	f = Length();
		v.x = x / f;
		v.y = y / f;
		v.z = z / f;
		return v;
	}

	float Dot(const _tagVector3& v)	const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	_tagVector3 Cross(const _tagVector3& v)	const
	{
		//_tagVector3 vCross;
		//vCross.Convert(XMVector3Cross(Convert(), v.Convert()));
		//return vCross;
		return _tagVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	float GetAngle(_tagVector3 v)	const
	{
		v = v.Normalize();
		_tagVector3 v1 = Normalize();
		float fDot = v1.Dot(v);
		return acosf(fDot);
	}

	float Distance(const _tagVector3& v)	const
	{
		_tagVector3	_v = *this - v;
		return _v.Length();
	}

	XMVECTOR Convert()	const
	{
		return XMLoadFloat3(this);
	}

	void Convert(const XMVECTOR& v)
	{
		XMStoreFloat3(this, v);
	}

	void Max(const _tagVector3 v)
	{
		x = x < v.x ? v.x : x;
		y = y < v.y ? v.y : y;
		z = z < v.z ? v.z : z;
	}

	void Min(const _tagVector3 v)
	{
		x = x > v.x ? v.x : x;
		y = y > v.y ? v.y : y;
		z = z > v.z ? v.z : z;
	}
}Vector3, *PVector3;

typedef struct _tagVector4 :
	public XMFLOAT4
{
	_tagVector4()
	{
		x = y = z = w = 0.f;
	}

	_tagVector4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	_tagVector4(const _tagVector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	_tagVector4(const _tagVector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0.f;
	}

	_tagVector4(float f[4])
	{
		x = f[0];
		y = f[1];
		z = f[2];
		w = f[3];
	}

	_tagVector4(int i[4])
	{
		x = i[0];
		y = i[1];
		z = i[2];
		w = i[3];
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

	_tagVector4 operator +(float f)	const
	{
		_tagVector4	v;
		v.x = x + f;
		v.y = y + f;
		v.z = z + f;
		v.w = w + f;
		return v;
	}

	void operator +=(const _tagVector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}

	void operator +=(float f)
	{
		x += f;
		y += f;
		z += f;
		w += f;
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

	_tagVector4 operator -(float f)	const
	{
		_tagVector4	v;
		v.x = x - f;
		v.y = y - f;
		v.z = z - f;
		v.w = w - f;
		return v;
	}

	void operator -=(const _tagVector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}

	void operator -=(float f)
	{
		x -= f;
		y -= f;
		z -= f;
		w -= f;
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

	_tagVector4 operator *(float f)	const
	{
		_tagVector4	v;
		v.x = x * f;
		v.y = y * f;
		v.z = z * f;
		v.w = w * f;
		return v;
	}

	void operator *=(const _tagVector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
	}

	void operator *=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
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

	_tagVector4 operator /(float f)	const
	{
		_tagVector4	v;
		v.x = x / f;
		v.y = y / f;
		v.z = z / f;
		v.w = w / f;
		return v;
	}

	void operator /=(const _tagVector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
	}

	void operator /=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
	}

	void operator =(const _tagVector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	void operator =(const _tagVector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	void operator =(float f[4])
	{
		x = f[0];
		y = f[1];
		z = f[2];
		w = f[3];
	}

	void operator =(int i[4])
	{
		x = i[0];
		y = i[1];
		z = i[2];
		w = i[3];
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	void Normalize()
	{
		x /= Length();
		y /= Length();
		z /= Length();
		w /= Length();
	}

	XMVECTOR Convert()	const
	{
		return XMLoadFloat4(this);
	}

	void Convert(const XMVECTOR& v)
	{
		XMStoreFloat4(this, v);
	}
}Vector4, *PVector4;

typedef Vector2		Vec2;
typedef Vector2*	PVec2;
typedef Vector3		Vec3;
typedef Vector3*	PVec3;
typedef Vector4		Vec4;
typedef Vector4*	PVec4;
typedef Vector4		Quaternion;
typedef	Vector4*	PQuaternion;
typedef XMVECTOR	Vector;
typedef XMMATRIX	Matrix;
typedef XMMATRIX*	PMatrix;


const Vec3	VEC3ZERO = Vec3(0.f, 0.f, 0.f);
const Vec3	VEC3X = Vec3(1.f, 0.f, 0.f);
const Vec3	VEC3Y = Vec3(0.f, 1.f, 0.f);
const Vec3	VEC3Z = Vec3(0.f, 0.f, 1.f);
const Vec3	VEC3ONE = Vec3(1.f, 1.f, 1.f);

const Vec4	VEC4ZERO = Vec4(0.f, 0.f, 0.f, 0.f);
const Vec4	VEC4X = Vec4(1.f, 0.f, 0.f, 0.f);
const Vec4	VEC4Y = Vec4(0.f, 1.f, 0.f, 0.f);
const Vec4	VEC4Z = Vec4(0.f, 0.f, 1.f, 0.f);
const Vec4	VEC4ONE = Vec4(1.f, 1.f, 1.f, 1.f);
const Vec4	VECRed = Vec4(1.f, 0.f, 0.f, 1.f);
const Vec4	VECGreen = Vec4(0.f, 1.f, 0.f, 1.f);
const Vec4	VECBlue = Vec4(0.f, 0.f, 1.f, 1.f);
const Vec4	VECBlack = Vec4(0.f, 0.f, 0.f, 1.f);

// Color Vertex
typedef struct _tagColorVertex
{
	Vec3		vPos;
	Vec4		vColor;
}COLORVERTEX, *PCOLORVERTEX;

// Position Vertex
typedef struct _tagPosVertex
{
	Vec3		vPos;
}POSVERTEX, *PPOSVERTEX;

// Tex Vertex
typedef struct _tagTexVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vTex;

	_tagTexVertex() :
		vPos({}),
		vNormal({}),
		vTex({})
	{
	}

	_tagTexVertex(const Vec3& _vPos, const Vec3& _vNormal, const Vec2& _vTex) :
		vPos(_vPos),
		vNormal(_vNormal),
		vTex(_vTex)
	{
	}

	_tagTexVertex(float fPX, float fPY, float fPZ,
		float fNX, float fNY, float fNZ, float fTX, float fTY) :
		vPos(Vec3(fPX, fPY, fPZ)),
		vNormal(Vec3(fNX, fNY, fNZ)),
		vTex(Vec2(fTX, fTY))
	{
	}
}TEXVERTEX, *PTEXVERTEX;

// 2D Vertex
typedef struct _tagVertex2D
{
	Vec3		vPos;
	Vec2		vTex;

	_tagVertex2D() :
		vPos({}),
		vTex({})
	{
	}

	_tagVertex2D(const Vec3& _vPos, const Vec2& _vTex) :
		vPos(_vPos),
		vTex(_vTex)
	{
	}

	_tagVertex2D(float fPX, float fPY, float fPZ,
		float fTX, float fTY) :
		vPos(Vec3(fPX, fPY, fPZ)),
		vTex(Vec2(fTX, fTY))
	{
	}
}VERTEX2D, *PVERTEX2D;

// Tex Vertex
typedef struct _tagBumpVertex
{
	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vTex;
	Vec3		vTangent;
	Vec3		vBinormal;

	_tagBumpVertex() :
		vPos({}),
		vNormal({}),
		vTangent({}),
		vBinormal({}),
		vTex({})
	{
	}

	_tagBumpVertex(const Vec3& _vPos, const Vec3& _vNormal,
		const Vec3& _vTangent, const Vec2& _vTex) :
		vPos(_vPos),
		vNormal(_vNormal),
		vTangent(_vTangent),
		vTex(_vTex)
	{
	}

	_tagBumpVertex(float fPX, float fPY, float fPZ,
		float fNX, float fNY, float fNZ,
		float fTX, float fTY, float fTZ,
		float fU, float fV) :
		vPos(Vec3(fPX, fPY, fPZ)),
		vNormal(Vec3(fNX, fNY, fNZ)),
		vTangent(Vec3(fTX, fTY, fTZ)),
		vTex(Vec2(fU, fV))
	{
	}
}BUMPVERTEX, *PBUMPVERTEX;

typedef struct _tagAnimationTexVertex
{
	Vec3	vPos;
	Vec3	vNormal;
	Vec2	vTex;
	Vec4	vWeights;
	Vec4	vIndices;
}ANIMTEXVERTEX, *PANIMTEXVERTEX;

typedef struct _tagAnimationBumpVertex
{
	Vec3	vPos;
	Vec3	vNormal;
	Vec2	vTex;
	Vec3	vTangent;
	Vec3	vBinormal;
	Vec4	vWeights;
	Vec4	vIndices;
}ANIMBUMPVERTEX, *PANIMBUMPVERTEX;

// Transform Constant Buffer
typedef struct _tagTransformCBuffer
{
	Matrix		matWorld;
	Matrix		matView;
	Matrix		matProj;
	Matrix		matWV;
	Matrix		matWVP;
}TRANSFORMCBUFFER, *PTRANSFORMCBUFFER;

// Light Constant Buffer
typedef struct _tagLightInfo
{
	int		iLightType;
	Vec3	vEmpty;
	Vec4	vDif;
	Vec4	vAmb;
	Vec4	vSpc;
}LIGHTINFO, *PLIGHTINFO;

// Point Light Constant Buffer
typedef struct _tagLightPoint
{
	Vec3	vPos;
	float	fRange;
	Vec4	vAtt;
}LIGHTPOINT, *PLIGHTPOINT;

// Material Constant Buffer
typedef struct _tagMaterial
{
	Vec4	vDif;
	Vec4	vAmb;
	Vec4	vSpc;
	Vec4	vEmv;
	float	fFadeAmount;
	Vec3	vMtrlEmpty;

}MATERIAL, *PMATERIAL;

// Mesh Data
typedef struct _tagMeshData
{
	vector<BUMPVERTEX>	vecVertices;
	vector<UINT>		vecIndices;
}MESHDATA, *PMESHDATA;

// UVFrame
typedef struct _tagUVFrame
{
	UINT	iFrameX;
	UINT	iFrameY;
	UINT	iFrameMaxX;
	UINT	iFrameMaxY;
	UINT	iAniType;
	Vec3	vEmpty;
}UVFRAME, *PUVFRAME;

// Ray
typedef struct _tagRay
{
	Vec3	vPos;
	Vec3	vDir;
	Vec3	vIntersect;
	float	fDist;
}RAY, *PRAY;

// Key Frame
typedef struct _tagAnimationKeyFrame
{
	float		fAnimationTime;
	Vec3		vTranslate;
	Vec3		vScale;
	Quaternion	qRot;
}ANIMATIONKEYFRAME, *PANIMATIONKEYFRAME;

// Bone
typedef struct _tagBone
{
	string		strName;
	int			iParentIndex;
	int			iDepth;
	Matrix		matTransform;
	Matrix		matOffset;
	Matrix		matBone;
	vector<ANIMATIONKEYFRAME>	vecKeyFrame;
}BONE, *PBONE;

// Animation Clip
typedef struct _tagAnimationClip
{
	string	strName;
	unsigned __int64	iStartFrame;
	unsigned __int64	iEndFrame;
	unsigned __int64	iFrameLength;
	double	dStartTime;
	double	dEndTime;
	double	dTimeLength;
}ANIMATIONCLIP, *PANIMATIONCLIP;

//Bone ConstantBuffer
typedef struct _tagBoneCBuffer
{
	Matrix	pBones[90];
	int		iBoneCount;
	Vec3	vEmpty;
}BONECBUFFER, *PBONECBUFFER;


// 직선과 평명
typedef struct _tagtriangleRay
{
	Vec3 Normal = {};
	Vec3 Pos = {};
}TRIANGLE2RAY, *PTRIANGLE2RAY;

// Frustrum
struct Frustrum
{
	XMVECTOR	m_vPlane[6];
	Vec3		vPos[8];
	Vec3		vCenter;

	bool IsInSphere(const Vec3& vPos, float fRadius)
	{
		for (UINT i = 0; i < 6; ++i)
		{
			XMVECTOR v = XMPlaneDotCoord(m_vPlane[i], vPos.Convert());
			if (v.vector4_f32[0] > fRadius)
			{
				return false;
			}
		}
		return true;
	}

	bool IsInSphereXZ(const Vec3& vPos, float fRadius)
	{
		for (UINT i = 0; i < 4; ++i)
		{
			XMVECTOR v = XMPlaneDotCoord(m_vPlane[i], vPos.Convert());
			if (v.vector4_f32[0] > fRadius)
			{
				return false;
			}
		}
		return true;
	}

	bool IsIn(const Vec3& vPos)
	{
		for (UINT i = 0; i < 4; ++i)
		{
			XMVECTOR v = XMPlaneDotCoord(m_vPlane[i], vPos.Convert());
			if (v.vector4_f32[0] > 0)
			{
				return false;
			}
		}
		return true;
	}
};