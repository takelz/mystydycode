/**
 * @file ModelCommon.cpp
 * @brief モデル汎用処理
 * @author ryota.ssk
 */
#include "ModelCommon.h"


void PushDirectory(const char *file, char *directory)
{
	// 読み込みディレクトリ移動
	GetCurrentDirectoryA(MAX_PATH, directory);
	char loadDirectory[MAX_PATH];
	_fullpath(loadDirectory, file, sizeof(loadDirectory));
	PathRemoveFileSpecA(loadDirectory);
	SetCurrentDirectoryA(loadDirectory);
}

void PopDirectory(const char *directory)
{
	SetCurrentDirectoryA(directory);
}

DirectX::XMMATRIX CalcRotateFromOrder(const DirectX::XMMATRIX &rotX, const DirectX::XMMATRIX &rotY, const DirectX::XMMATRIX &rotZ, fbx::RotateOrder order)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
	switch (order) {
	case fbx::RotateOrder::XYZ: mat = rotX * rotY * rotZ; break;
	case fbx::RotateOrder::ZXY: mat = rotZ * rotX * rotY; break;
	}
	return mat;
}


// EOF