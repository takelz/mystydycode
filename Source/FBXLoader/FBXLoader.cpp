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
/// @brief ������
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
/// @brief �I������
//
//-----------------------------------------------------
void LoadTerminate()
{
	g_pManager->Destroy();
	LogOutput("[FBXLoadInitialize]\nend\n");
}

//-----------------------------------------------------
//
/// @brief FBX�ǂݍ���
/// @param [in] fileName �t�@�C����
/// @param [out] meshList ���b�V���ǂݍ��ݐ�
/// @param [out] boneListInfo �{�[���ǂݍ��ݐ�
/// @param [out] materialList �}�e���A���ǂݍ��ݐ�
/// @param [out] anime �A�j���[�V�����ǂݍ��ݐ�
/// @return �ǂݍ��݌���
//
//-----------------------------------------------------
bool Load(const char *fileName, MeshList &meshList, BoneListInfo &boneListInfo, MaterialList &materialList, AnimationInfo &anime)
{
	LogOutput("[FBXLoad]\n");
	LogOutput("FileName: %s\n", fileName);

	// �ǂݍ��ݏ�����
	FbxImporter *pImporter = FbxImporter::Create(g_pManager, "");
	if (!pImporter->Initialize(fileName, -1, g_pManager->GetIOSettings()))
	{
		LogOutput("\nfailed...\n");
		return false;
	}

	// �ǂݍ���
	FbxScene *pScene = FbxScene::Create(g_pManager, "originalScene");
	pImporter->Import(pScene);
	// �O�p��
	FbxGeometryConverter geometryConverter(g_pManager);
	geometryConverter.Triangulate(pScene, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(pScene);
	// �}�e���A���ŕ���
	geometryConverter.SplitMeshesPerMaterial(pScene, true);

	// ���b�V���f�[�^�擾
	GetMesh(pScene, meshList);

	// �}�e���A���f�[�^�擾
	GetMaterial(pScene, materialList);

	// �{�[���f�[�^�擾
	GetBone(pScene, boneListInfo);

	// �A�j���[�V�����f�[�^�擾
	GetAnimation(pScene, anime);

	pImporter->Destroy();
	LogOutput("\nsucsess!\n");
	return true;
}

//-----------------------------------------------------
//
/// @brief ���O�����֐��̐ݒ�
/// @param [in] func �R�[���o�b�N�֐�
//
//-----------------------------------------------------
void SetLogCallbackFunc(LogCallbackFunc func)
{
	g_LogFunc = func;
}

//-----------------------------------------------------
//
/// @brief ���O�o��
/// @param[in] format �o�̓t�H�[�}�b�g
/// @param[in] ... �ό���
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