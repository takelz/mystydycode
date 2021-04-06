/**
 * @file ModelCommon.h
 * @brief ÉÇÉfÉãîƒópèàóù
 * @author ryota.ssk
 */
#ifndef __MODEL_COMMON_H__
#define __MODEL_COMMON_H__

#include <DirectX/DX11Util.h>
#include <FBXLoader/FBXLoader.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

void PushDirectory(const char *file, char *directory);
void PopDirectory(const char *directory);
DirectX::XMMATRIX CalcRotateFromOrder(const DirectX::XMMATRIX &rotX, const DirectX::XMMATRIX &rotY, const DirectX::XMMATRIX &rotZ, fbx::RotateOrder order);

#endif // __MODEL_COMMON_H__
