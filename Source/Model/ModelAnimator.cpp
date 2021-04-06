/**
 * @file ModelAnimator.cpp
 * @brief アニメーション処理
 * @author ryota.ssk
 */
#include "ModelAnimator.h"

#include <FBXLoader/FBXLoadMesh.h>
#include <FBXLoader/FBXLoadMaterial.h>
#include <FBXLoader/FBXLoadBone.h>
#include <FBXLoader/FBXLoadAnimation.h>

#include <DirectX/DrawObject.h>

using namespace DirectX;

ModelAnimator::ModelAnimator()
{
}
ModelAnimator::~ModelAnimator()
{
}

/**
 * @brief アニメーションデータ読み込み
 * @param[in] file ファイル名
 * @return アニメインデックス
 */
int ModelAnimator::AddLoad(const char *file)
{
	char directory[MAX_PATH];
	PushDirectory(file, directory);

	// 読み込み準備
	fbx::LoadInitialize();
	fbx::MeshList meshList;
	fbx::BoneListInfo boneListInfo;
	fbx::MaterialList materialList;
	fbx::AnimationInfo animeInfo;

	// 読み込み
	bool ret = fbx::Load(PathFindFileNameA(file), meshList, boneListInfo, materialList, animeInfo);
	if (ret) {
		if (m_animeList.empty()) {
			InitBone(boneListInfo);
		}
		else if (boneListInfo.list.size() != m_boneList.size()) {
			return -1;
		}
		InitAnimation(animeInfo, boneListInfo);
	}

	// 終了
	fbx::CleanupAnimation(animeInfo);
	fbx::CleanupBone(boneListInfo);
	fbx::CleanupMaterial(materialList);
	fbx::CleanupMesh(meshList);
	fbx::LoadTerminate();

	PopDirectory(directory);

	return ret ? static_cast<int>(m_animeList.size() - 1) : -1;
}

/**
 * @brief 再生
 * @param[in] rate 再生速度
 */
bool ModelAnimator::Step(float rate)
{
	if (m_animeList.empty()) {
		return false;
	}

	// 初期化
	for (int i = 0; i < m_calcBoneList.size(); ++i) {
		m_calcBoneList[i] = XMMATRIX();
	}

	// パラメトリックがあれば計算
	int paraBlendIndex = -1;
	if (m_parametricSet[0].index >= 0) {
		for (int i = 0; i < MAX_BLEND_NUM; ++i) {
			if (m_blendSet[i].index == -1) {
				paraBlendIndex = i;
				break;
			}
		}
		for (int i = 0; i < MAX_PARAMETRIC_NUM; ++i) {
			UpdatePlayInfo(m_parametricSet[i], rate);
			for (int j = 0; j < m_blendSet[paraBlendIndex].matrix.size(); ++j) {
				// パラメトリックはブレンドに一旦格納
				if (i == 0) {
					m_blendSet[paraBlendIndex].matrix[j].calc = m_parametricSet[i].matrix[j].calc * m_parametricSet[i].blend;
				}
				else {
					m_blendSet[paraBlendIndex].matrix[j].calc += m_parametricSet[i].matrix[j].calc * m_parametricSet[i].blend;
				}
			}
		}
	}

	// ブレンドの計算
	if (m_blendTime > 0.f) {
		m_blendTime -= rate;
		if (m_blendTime <= 0.f) {
			m_blendSet[0] = m_blendSet[1];
			m_blendSet[0].blend = 1.f;
			InitPlayInfo(m_blendSet[1]);
			InitPlayInfo(m_parametricSet[0]);
			InitPlayInfo(m_parametricSet[1]);
		} else {
			m_blendSet[0].blend = m_blendTime / m_totalBlendTime;
			m_blendSet[1].blend = 1.f - m_blendSet[0].blend;
		}
	}


	for (int i = 0; i < MAX_BLEND_NUM; ++i) {
		if (m_blendSet[i].index >= 0 || paraBlendIndex == i) {
			if (paraBlendIndex != i) {
				UpdatePlayInfo(m_blendSet[i], rate);
			}

			// ブレンドを考慮してボーンを計算
			for (int j = 0; j < m_calcBoneList.size(); ++j) {
				m_calcBoneList[j] += m_blendSet[i].matrix[j].calc * m_blendSet[i].blend;
			}
		}
	}

	if (!m_blendSet[0].loop) {
		// パラメトリックは保留
		if (m_blendSet[0].index >= 0) {
			return m_blendSet[0].frame < m_animeList[m_blendSet[0].index].time;
		}
	}

	return true;
}

/**
 * @brief ボーン描画
 * @param[in] world ワールド行列
 */
void ModelAnimator::Draw(const XMMATRIX &world)
{
	if (m_boneList.empty()) {
		return;
	}
	struct BoneDrawFunction
	{
		static void Run(BoneList &list, CalcBoneList &calc, int index, XMFLOAT3 parentPos, const XMMATRIX &world)
		{
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, XMVector3TransformCoord(XMVECTOR(), calc[index] * world));
			DirectX::debug::DrawLine(pos, parentPos);

			// 子要素探索
			for (unsigned int i = 0; i < list[index].children.size(); ++i) {
				Run(list, calc, list[index].children[i], pos, world);
			}
		}
	};

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, XMVector3TransformCoord(XMVECTOR(), world));
	BoneDrawFunction::Run(m_boneList, m_calcBoneList, m_rootBone, pos, world);
}


/**
 * @brief 単発再生
 * @param[in] index 読み込みアニメ番号
 * @param[in] loop ループ
 * @param[in] start 再生開始位置
 */
void ModelAnimator::Play(int index, bool loop, float start)
{
	// インデックスのチェック
	if (index < 0 || m_animeList.size() <= index) {
		return;
	}

	InitPlayInfo(m_blendSet[0]);
	InitPlayInfo(m_blendSet[1]);
	InitPlayInfo(m_parametricSet[0]);
	InitPlayInfo(m_parametricSet[1]);
	m_blendSet[0].index = index;
	m_blendSet[0].frame = start;
	m_blendSet[0].loop = loop;
}

/**
 * @brief パラメトリック再生
 * @param[in] anime1 かけ合わせアニメ番号1
 * @param[in] anime2 かけ合わせアニメ番号2
 * @param[in] loop ループ
 */
void ModelAnimator::PlayParametric(int anime1, int anime2, bool loop)
{
	// インデックスのチェック
	if (anime1 == anime2) {
		return;
	}
	if (anime1 < 0 || m_animeList.size() <= anime1) {
		return;
	}
	if (anime2 < 0 || m_animeList.size() <= anime2) {
		return;
	}

	InitPlayInfo(m_blendSet[0]);
	InitPlayInfo(m_blendSet[1]);
	for (int i = 0; i < MAX_PARAMETRIC_NUM; ++i) {
		InitPlayInfo(m_parametricSet[i]);
		m_parametricSet[i].loop = loop;

	}
	m_parametricSet[0].index = anime1;
	m_parametricSet[1].index = anime2;

	SetParametricBlend(0.f);
}

/**
 * @brief ブレンド率変更
 * @param[in] blend 0.f ~ 1.f
 */
void ModelAnimator::SetParametricBlend(float blend)
{
	if (m_parametricSet[0].index < 0) {
		return;
	}
	if (blend < 0.f) { blend = 0.f; }
	if (blend > 1.f) { blend = 1.f; }

	ModelAnimator::PlayInfo &play1 = m_parametricSet[0];
	ModelAnimator::PlayInfo &play2 = m_parametricSet[1];
	play1.blend = 1.f - blend;
	play2.blend = blend;

	float totalTime = m_animeList[play1.index].time * play1.blend + m_animeList[play2.index].time * play2.blend;
	play1.time = m_animeList[play1.index].time / totalTime;
	play2.time = m_animeList[play2.index].time / totalTime;
}

/**
 * @brief ブレンド再生
 * @param[in] index 読み込みアニメ番号
 * @param[in] blend ブレンド時間
 * @param[in] loop ループ
 * @param[in] start 再生開始位置
 */
void ModelAnimator::PlayBlendMotion(int index, float blend, bool loop, float frame)
{
	if (m_blendSet[0].index == index) {
		return; // 同じアニメは再生しない
	}
	if (index == -1 && m_parametricSet[0].index < 0) {
		return;
	}


	InitPlayInfo(m_blendSet[1]);
	m_blendSet[1].index = index;
	m_blendSet[1].loop = loop;
	m_blendSet[1].frame = frame;
	m_blendSet[1].blend = 0.f;

	m_blendTime = blend;
	m_totalBlendTime = blend;
}

void ModelAnimator::InitBone(fbx::BoneListInfo &bone)
{
	m_boneList.resize(bone.list.size());
	m_calcBoneList.resize(bone.list.size());

	for (int i = 0; i < MAX_BLEND_NUM; ++i) {
		m_blendSet[i].matrix.resize(bone.list.size());
	}
	for (int i = 0; i < MAX_PARAMETRIC_NUM; ++i) {
		m_parametricSet[i].matrix.resize(bone.list.size());
	}


	BoneList::iterator it = m_boneList.begin();
	while (it != m_boneList.end()) {
		int index = static_cast<int>(it - m_boneList.begin());
		fbx::BoneInfo &info = bone.list[index];

		XMMATRIX preX = XMMatrixRotationX(XMConvertToRadians(info.preRotate.x));
		XMMATRIX preY = XMMatrixRotationY(XMConvertToRadians(info.preRotate.y));
		XMMATRIX preZ = XMMatrixRotationZ(XMConvertToRadians(info.preRotate.z));
		it->order = info.order;
		XMStoreFloat4x4(&it->preMatrix, CalcRotateFromOrder(preX, preY, preZ, it->order));

		it->trans = XMFLOAT3(info.translate.x, info.translate.y, info.translate.z);
		it->rot = XMFLOAT3(info.rotation.x, info.rotation.y, info.rotation.z);
		it->scale = XMFLOAT3(info.scale.x, info.scale.y, info.scale.z);

		// 子要素設定
		for (unsigned int i = 0; i < info.children.size(); ++i) {
			it->children.push_back(info.children[i]);
		}

		++it;
	}

	m_rootBone = bone.root;
}
void ModelAnimator::InitAnimation(fbx::AnimationInfo &info, fbx::BoneListInfo &bone)
{
	if (info.curveList.empty()) {
		return;
	}

	AnimeInfo anime;

	// 再生時間
	anime.time = static_cast<float>(info.totalFrame);

	// アニメーションカーブコピー
	anime.curveList.resize(info.curveList.size());
	AnimeInfo::CurveList::iterator it = anime.curveList.begin();
	while (it != anime.curveList.end()) {
		int index = static_cast<int>(it - anime.curveList.begin());
		fbx::AnimationInfo::CurveInfo &curveInfo = info.curveList[index];

		// ボーンのインデックスを探索
		it->boneIndex = -1;
		fbx::BoneList::iterator itBone = bone.list.begin();
		while (itBone != bone.list.end()) {
			if (itBone->name == curveInfo.name) {
				it->boneIndex = static_cast<int>(itBone - bone.list.begin());
				break;
			}
			++itBone;
		}

		// キーのコピー
		for (int i = 0; i < fbx::AnimationInfo::CurveInfo::CHANNEL_MAX; ++i) {
			fbx::AnimationInfo::CurveInfo::List::iterator itKey = curveInfo.keyList[i].begin();
			while (itKey != curveInfo.keyList[i].end()) {
				AnimeInfo::CurveInfo::KeyInfo key = { itKey->frame, itKey->value };
				it->keyList[i].push_back(key);
				++itKey;
			}
		}

		++it;
	}
	m_animeList.push_back(anime);
}


void ModelAnimator::InitPlayInfo(PlayInfo &info)
{
	info.index = -1;
	info.blend = 1.f;
	info.time = 1.f;
	info.frame = 0;
	info.loop = false;
	for (int i = 0; i < m_boneList.size(); ++i) {
		info.matrix[i].trans = m_boneList[i].trans;
		info.matrix[i].rot = m_boneList[i].rot;
		info.matrix[i].scale = m_boneList[i].scale;
	}
}

void ModelAnimator::UpdatePlayInfo(PlayInfo &info, float rate)
{
	// アニメ位置計算関数
	struct CalcAnimeFrameFunction
	{
		static float Run(AnimeInfo::CurveInfo::KeyList &key, float frame, float defVal)
		{
			float value = defVal;
			AnimeInfo::CurveInfo::KeyList::iterator it = key.begin();
			while (it != key.end()) {
				// 指定フレームを超えるキーを探索
				if (frame < it->frame) {
					int prevIndex = static_cast<int>(it - key.begin()) - 1;
					if (prevIndex < 0) {
						// 先頭のキーよりも前のフレームなので、先頭の値を使用
						value = it->value;
					}
					else {
						// ひとつ前のフレームとの線形補間値を使用
						const AnimeInfo::CurveInfo::KeyInfo &prevKey = key[prevIndex];
						value = (it->value - prevKey.value) * (frame - prevKey.frame) / (it->frame - prevKey.frame) + prevKey.value;
					}
					break;
				}
				// 指定フレームと一致したのでそのまま使用
				else if (frame == it->frame) {
					value = it->value;
					break;
				}

				++it;
			}

			// 最終キーよりも後のフレームなので、一番後ろの値を使用
			if (it == key.end() && key.size() > 0) {
				value = key.back().value;
			}

			return value;
		}
	};

	// 親の影響を考慮した行列を計算
	struct CalcBoneMatrixFunction
	{
		static void Run(PlayInfo::AnimeMatrix &anime, BoneList &bone, int index, XMMATRIX mat)
		{
			PlayInfo::AnimeMatrixInfo &animeInfo = anime[index];
			BoneInfo &boneInfo = bone[index];
			XMMATRIX trans = XMMatrixTranslation(animeInfo.trans.x, animeInfo.trans.y, animeInfo.trans.z);
			XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(animeInfo.rot.x));
			XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(animeInfo.rot.y));
			XMMATRIX rotZ = XMMatrixRotationZ(XMConvertToRadians(animeInfo.rot.z));
			XMMATRIX scale = XMMatrixScaling(animeInfo.scale.x, animeInfo.scale.y, animeInfo.scale.z);

			XMMATRIX calc = scale * CalcRotateFromOrder(rotX, rotY, rotZ, boneInfo.order) * XMLoadFloat4x4(&boneInfo.preMatrix) * trans;
			animeInfo.calc = calc * mat;

			for (unsigned int i = 0; i < boneInfo.children.size(); ++i) {
				Run(anime, bone, boneInfo.children[i], animeInfo.calc);
			}
		}
	};

	// モーション時間の更新
	AnimeInfo &anime = m_animeList[info.index];
	info.frame += info.time * rate;
	if (anime.time <= info.frame) {
		if (info.loop) {
			info.frame -= anime.time;
		}
		else {
			info.frame = anime.time;
		}
	}
	else if (info.frame < 0.f) {
		info.frame += anime.time;
	}

	// 現在のフレームからボーンのアニメーション値を計算
	AnimeInfo::CurveList::iterator curveIt = anime.curveList.begin();
	while (curveIt != anime.curveList.end()) {
		if (curveIt->boneIndex >= 0) {
			PlayInfo::AnimeMatrixInfo &mat = info.matrix[curveIt->boneIndex];
			float *value[fbx::AnimationInfo::CurveInfo::CHANNEL_MAX] = {
				&mat.trans.x, &mat.trans.y, &mat.trans.z,
				&mat.rot.x, &mat.rot.y, &mat.rot.z,
				&mat.scale.x, &mat.scale.y, &mat.scale.z
			};
			for (int i = 0; i < fbx::AnimationInfo::CurveInfo::CHANNEL_MAX; ++i) {
				*(value[i]) = CalcAnimeFrameFunction::Run(curveIt->keyList[i], info.frame, *(value[i]));
			}
		}
		++curveIt;
	}
	CalcBoneMatrixFunction::Run(info.matrix, m_boneList, m_rootBone, XMMatrixIdentity());
}

// EOF