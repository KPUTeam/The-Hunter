
// Flag.h
// 열거형으로 만든 플래그 값 저장

#pragma once

// Resolution Type
enum RESOLUTION_TYPE
{
	RT_SMINI,
	RT_MINI,
	RT_HD,
	RT_FHD,
	RT_MAX
};

// Shader Type
enum SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_MAX
};

// Component Type
enum COMPONENT_TYPE
{
	CT_TRANSFORM,
	CT_RENDERER,
	CT_SCRIPT,
	CT_CAMERA,
	CT_MATERIAL,
	CT_LIGHT,
	CT_COLLIDER,
	CT_UI,
	CT_MOUSE,
	CT_BILLBOARD,
	CT_ANIMATION2D
};


// Collider SubType
enum COLLIDER_SUB_TYPE
{
	CST_SPHERE,
	CST_RECT,
	CST_POINT,
	CST_AABB,
	CST_OBB,
	CST_LAY
};

// Axis
enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

// Object Type
enum OBJECT_TYPE
{
	OT_NORMAL,
	OT_PROTOTYPE,
	OT_CLONE
};

// Schedule Option
enum SCHEDULE_OPTION
{
	SO_ONCE,
	SO_LOOP
};

// Transform Parent Flag
enum TRANSFORM_PARENT_FLAG
{
	TPF_NONE,
	TPF_SCALE = 0x00000001,	//001 = 1
	TPF_ROT = 0x00000002,	//010 = 2
	TPF_POS = 0x00000004,	//100 = 4
	TPF_ALL = TPF_SCALE | TPF_ROT | TPF_POS //111 = 7

};

// Light Type
enum LIGHT_TYPE
{
	LT_DIR,
	LT_POINT,
	LT_SPOT
};

// Render State Type
enum RENDER_STATE_TYPE
{
	RST_DEPTHSTENCIL,
	RST_RASTERIZER,
	RST_BLEND,
	RST_END
};

// Rendering Type
enum RENDER_TYPE_FLAG
{
	RTF_NORMAL,
	RTF_ALPHA1,
	RTF_ALPHA2,
	RTF_ALPHA3,
	RTF_UI,
	RTF_END
};

// Button State
enum BUTTON_STATE
{
	BS_NORMAL,
	BS_MOUSEON,
	BS_CLICK,
	BS_END
};

// Scene Change
enum SCENE_CHANGE
{
	SC_NONE,
	SC_CHANGE
};

// UIBar Direction
enum UIBAR_DIR
{
	UBD_LEFT,
	UBD_RIGHT,
	UBD_DOWN,
	UBD_UP,
	UBD_END
};

// Camera View Type
enum CAMERA_VIEW_TYPE
{
	CVT_FIRST,
	CVT_THIRD
};

// Rotation Flag
enum ROTATION_FLAG
{
	RF_XYZ,
	RF_AXIS,
	RF_BILLBOARD
};

// UIBar Flag
enum BAR_FLAG
{
	BF_NORMAL,
	BF_SHADOW,
	BF_EDGE
};

// Animation2D Type
enum ANIMATION2D_TYPE
{
	A2D_FRAME,
	A2D_ATLAS_H,
	A2D_ATLAS_ALL
};

// Animation2D Option
enum ANIMATION2D_OPTION
{
	A2DO_ONCE,
	A2DO_LOOP,
	A2DO_TIME
};

