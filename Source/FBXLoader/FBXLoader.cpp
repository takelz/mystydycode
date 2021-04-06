#include "FBXLoader.h"
#include "FBXLoadMesh.h"
#include "FBXLoadBone.h"
#include "FBXLoadMaterial.h"
#include "FBXLoadAnimation.h"

namespace fbx {

FbxManager *g_pManager = NULL;
LogCallbackFunc g_LogFunc = NULL;

//-----------------------------------------------------
//
/// @brief 初期化
//
//-----------------------------------------------------
void LoadInitialize()
{
	g_pManager = FbxManager::Create();

	FbxIOSettings *pIO = FbxIOSettings::Create(g_pManager, IOSROOT);
	g_pManager->SetIOSettings(pIO);
	LogOutput("[FBXLoadInitialize]\nstart\n");
}

//-----------------------------------------------------
//
/// @brief 終了処理
//
//-----------------------------------------------------
void LoadTerminate()
{
	g_pManager->Destroy();
	LogOutput("[FBXLoadInitialize]\nend\n");
}

//-----------------------------------------------------
//
/// @brief FBX読み込み
/// @param [in] fileName ファイル名
/// @param [out] meshList メッシュ読み込み先
/// @param [out] boneListInfo ボーン読み込み先
/// @param [out] materialList マテリアル読み込み先
/// @param [out] anime アニメーション読み込み先
/// @return 読み込み結果
//
//-----------------------------------------------------
bool Load(const char *fileName, MeshList &meshList, BoneListInfo &boneListInfo, MaterialList &materialList, AnimationInfo &anime)
{
	LogOutput("[FBXLoad]\n");
	LogOutput("FileName: %s\n", fileName);

	// 読み込み初期化
	FbxImporter *pImporter = FbxImporter::Create(g_pManager, "");
	if (!pImporter->Initialize(fileName, -1, g_pManager->GetIOSettings()))
	{
		LogOutput("\nfailed...\n");
		return false;
	}

	// 読み込み
	FbxScene *pScene = FbxScene::Create(g_pManager, "originalScene");
	pImporter->Import(pScene);
	// 三角化
	FbxGeometryConverter geometryConverter(g_pManager);
	geometryConverter.Triangulate(pScene, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(pScene);
	// マテリアルで分割
	geometryConverter.SplitMeshesPerMaterial(pScene, true);

	// メッシュデータ取得
	GetMesh(pScene, meshList);

	// マテリアルデータ取得
	GetMaterial(pScene, materialList);

	// ボーンデータ取得
	GetBone(pScene, boneListInfo);

	// アニメーションデータ取得
	GetAnimation(pScene, anime);

	pImporter->Destroy();
	LogOutput("\nsucsess!\n");
	return true;
}

//-----------------------------------------------------
//
/// @brief ログ処理関数の設定
/// @param [in] func コールバック関数
//
//-----------------------------------------------------
void SetLogCallbackFunc(LogCallbackFunc func)
{
	g_LogFunc = func;
}

//-----------------------------------------------------
//
/// @brief ログ出力
/// @param[in] format 出力フォーマット
/// @param[in] ... 可変個引数
//
//-----------------------------------------------------
void LogOutput(const char *format, ...)
{
	if (g_LogFunc != NULL){
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsprintf_s(buf, sizeof(buf), format, args);
		g_LogFunc(buf);
		va_end(args);
	}
}

}; // fbx

// EOF