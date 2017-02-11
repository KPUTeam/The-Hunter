
#pragma once

// Resolution Type
enum RESOLUTION_TYPE
{
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
	CT_LIGHT
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

// TRANSFORM PARENT Flag
enum TRANSFORM_PARENT_FLAG
{
	TPF_NONE,
	TPF_SCALE = 0x00000001,
	TPF_ROT	= 0x00000002,
	TPF_POS = 0x00000004,
	TPF_ALL = TPF_SCALE | TPF_ROT | TPF_POS
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
