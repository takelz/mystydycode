#include "FBXLoadAnimation.h"

namespace fbx {

void GetKeyInfos(FbxAnimCurve *pCurve, AnimationInfo::CurveInfo::List &list);
void CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime);
void Display(const char *pChannel, AnimationInfo::CurveInfo::List &list);

//-----------------------------------------------------
//
/// @brief �f�[�^�N���A
//
//-----------------------------------------------------
void CleanupAnimation(AnimationInfo &anime)
{
	AnimationInfo::List::iterator it = anime.curveList.begin();
	while (it != anime.curveList.end()) {
		for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i) {
			it->keyList[i].clear();
		}
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief �f�[�^�擾
//
//-----------------------------------------------------
void GetAnimation(FbxScene *pScene, AnimationInfo &anime)
{
	CleanupAnimation(anime);
	int animeStackCount = pScene->GetSrcObjectCount<FbxAnimStack>();
	if (animeStackCount == 0) {
		return;
	}
	FbxAnimStack* pStack = pScene->GetSrcObject<FbxAnimStack>(0);
	int animeLayerCount = pStack->GetMemberCount<FbxAnimLayer>();
	if (animeLayerCount == 0){
		return;
	}
	LogOutput("Animation[stack/layer] : %d/%d\n", animeStackCount, animeLayerCount);
	FbxTime time;
	time.SetTime(0, 0, 0, 1, 0, 0, FbxTime::eFrames60);
	anime.totalFrame = static_cast<long>((pStack->GetLocalTimeSpan().GetDuration().Get() + (time.Get() >> 1)) / time.Get());
	CalcAnimation(pStack->GetMember<FbxAnimLayer>(0), pScene->GetRootNode(), anime);
}

//-----------------------------------------------------
//
/// @brief �m�[�h���̃A�j���[�V���������o��
//
//-----------------------------------------------------
void CalcAnimation(FbxAnimLayer* pLayer, FbxNode* pNode, AnimationInfo &anime)
{
	// �e�v�f�̃L�[�𒊏o
	FbxAnimCurve *pCurves[] = {
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclTranslation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclRotation.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y),
		pNode->LclScaling.GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z),
	};
	AnimationInfo::CurveInfo info;
	for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i) {
		GetKeyInfos(pCurves[i], info.keyList[i]);
	}
	for (int i = 0; i < AnimationInfo::CurveInfo::CHANNEL_MAX; ++i) {
		if (info.keyList[i].size() > 0) {
			info.name = pNode->GetInitialName();
			anime.curveList.push_back(info);

			LogOutput("========== AnimationData[No.%d/Name:%s] ==========\n", anime.curveList.size() - 1, info.name.c_str());
			Display("TransX ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_TRANSLATION_X]);
			Display("TransY ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_TRANSLATION_Y]);
			Display("TransZ ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_TRANSLATION_Z]);
			Display("RotX   ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_ROTATION_X]);
			Display("RotY   ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_ROTATION_Y]);
			Display("RotZ   ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_ROTATION_Z]);
			Display("ScaleX ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_SCALING_X]);
			Display("ScaleY ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_SCALING_Y]);
			Display("ScaleZ ", info.keyList[AnimationInfo::CurveInfo::CHANNEL_SCALING_Z]);
			break;
		}
	}

	// �q�v�f����
	for (int i = 0; i < pNode->GetChildCount(); ++i) {
		CalcAnimation(pLayer, pNode->GetChild(i), anime);
	}
}

//-----------------------------------------------------
//
/// @brief �L�[���擾
//
//-----------------------------------------------------
void GetKeyInfos(FbxAnimCurve *pCurve, AnimationInfo::CurveInfo::List &list)
{
	if (pCurve != NULL) {
		FbxTime time;
		time.SetTime(0, 0, 0, 1, 0, 0, FbxTime::eFrames60);

		list.resize(pCurve->KeyGetCount());
		AnimationInfo::CurveInfo::List::iterator it = list.begin();
		while (it != list.end()) {
			int index = static_cast<int>(it - list.begin());
			it->frame = static_cast<int>((pCurve->KeyGetTime(index).Get() + (time.Get() >> 1)) / time.Get()); // 0.5�����Đ؂�グ
			it->value = pCurve->KeyGetValue(index);
			++it;
		}
	}
}

//-----------------------------------------------------
//
/// @brief ���O�o��
//
//-----------------------------------------------------
void Display(const char *pChannel, AnimationInfo::CurveInfo::List &list)
{
	LogOutput("%s(KeyCount:%d)[frame,key] : ", pChannel, list.size());
	AnimationInfo::CurveInfo::List::iterator it = list.begin();
	while (it != list.end()) {
		LogOutput("[%d,%.3f]", it->frame, it->value);
		++it;
	}
	LogOutput("\n");
}

}; // fbx