/**
 * @file DrawObject.h
 * @brief �f�o�b�O�`��
 * @author ryota.ssk
 */
#ifndef __DRAW_OBJECT_H__
#define __DRAW_OBJECT_H__

#ifdef _DEBUG
#define ENABLE_DRAW_OBJ 1
#else
#define ENABLE_DRAW_OBJ 0
#endif

#include "DX11util.h"

namespace DirectX {
namespace debug {

// �}�`�`��w��
struct DrawShapeArg
{
	DrawShapeArg() {}
	enum ShapeType {
		ST_BOX,
		ST_CIRCLE,
		ST_SPHERE
	};
	enum PoseType {
		PT_FIX,
		PT_ROTATION
	};
	enum CenterType {
		CT_ORIGIN,
		CT_SHIFT
	};

	union Pose {
		Pose() {}
		XMFLOAT3 pos; // Pose��FIX
		XMMATRIX mat; // Pose��ROTAION
	};
	union Size {
		Size() {}
		float radius;	// Shape��BOX�ȊO,Center��ORIGIN
		XMFLOAT3 vec;	// Shape��BOX,Center��ORIGIN
		struct Shift {	// Center��SHIFT
			XMFLOAT3 max;
			XMFLOAT3 min;
		} shift;
	};

	ShapeType shape;
	PoseType pose;
	CenterType center;
	Pose uPose;
	Size uSize;
};

#if ENABLE_DRAW_OBJ
bool InitializeDrawObject();
void TerminateDrawObject();

void FlushDrawObject(const XMFLOAT4X4 &viewproj);

//! @name DrawFunction
/// @{
void DrawLine(const XMFLOAT3 &start, const XMFLOAT3 &end, const XMFLOAT4 &color = XMFLOAT4(1, 1, 1, 1));
void DrawShape(const DrawShapeArg &arg, const XMFLOAT4 &color = XMFLOAT4(1, 1, 1, 1), bool wire = true);
/// @}
#else
inline bool InitializeDrawObject() { return true; }
inline void TerminateDrawObject() {}
inline void FlushDrawObject(const XMFLOAT4X4 &viewproj) {}
inline void DrawLine(const XMFLOAT3 &start, const XMFLOAT3 &end, const XMFLOAT4 &color = XMFLOAT4(1, 1, 1, 1)) {}
inline void DrawShape(const DrawShapeArg &arg, const XMFLOAT4 &color = XMFLOAT4(1, 1, 1, 1), bool wire = true) {}
#endif

} // debug
} // DirectX

#endif // __DRAW_OBJECT_H__