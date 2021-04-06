/**
 * @file ModelAnimator.h
 * @brief アニメーション処理
 * @author ryota.ssk
 */
#ifndef __MODEL_ANIMATOR_H__
#define __MODEL_ANIMATOR_H__

#include "ModelCommon.h"

class ModelAnimator
{
private:
	// ボーン情報
	struct BoneInfo {
		typedef std::vector<int> ChildList;
		DirectX::XMFLOAT4X4		preMatrix;
		DirectX::XMFLOAT3		trans;
		DirectX::XMFLOAT3		rot;
		DirectX::XMFLOAT3		scale;
		fbx::RotateOrder		order;
		ChildList		children;
	};
	typedef std::vector<BoneInfo> BoneList;

	// アニメーション情報
	struct AnimeInfo {
		struct CurveInfo {
			struct KeyInfo {
				int frame;
				float value;
			};
			typedef std::vector<KeyInfo> KeyList;

			int boneIndex;
			KeyList keyList[fbx::AnimationInfo::CurveInfo::CHANNEL_MAX];
		};
		typedef std::vector<CurveInfo> CurveList;

		CurveList curveList;
		float time;
	};
	typedef std::vector<AnimeInfo> AnimeList;

	// 再生情報
	struct PlayInfo {
		struct AnimeMatrixInfo {
			DirectX::XMFLOAT3 trans;
			DirectX::XMFLOAT3 rot;
			DirectX::XMFLOAT3 scale;
			DirectX::XMMATRIX calc;
		};
		typedef std::vector<AnimeMatrixInfo> AnimeMatrix;
		AnimeMatrix matrix;

		int index;		// 再生アニメ
		float frame;	// 現在フレーム
		float time;		// 内部計算速度
		float blend;
		bool loop;
	};

public:
	typedef std::vector<DirectX::XMMATRIX> CalcBoneList;

public:
	ModelAnimator();
	~ModelAnimator();

	int AddLoad(const char *file);
	bool Step(float rate = 1.f);
	void Draw(const DirectX::XMMATRIX &world);

	//! @name 再生処理
	/// @{
	// 単発再生
	void Play(int index, bool loop = false, float start = 0.f);
	// パラメトリック
	void PlayParametric(int anime1, int anime2, bool loop = false);
	void SetParametricBlend(float blend);
	// ブレンド再生
	void PlayBlendMotion(int index, float blend, bool loop = false, float frame = 0.f);
	/// @}

	const CalcBoneList *GetBoneList() const { return &m_calcBoneList; }

private:
	void InitBone(fbx::BoneListInfo &bone);
	void InitAnimation(fbx::AnimationInfo &info, fbx::BoneListInfo &bone);
	void InitPlayInfo(PlayInfo &info);
	void UpdatePlayInfo(PlayInfo &info, float rate);

private:
	AnimeList			m_animeList;
	BoneList			m_boneList;
	int					m_rootBone;
	CalcBoneList		m_calcBoneList;


	static const int MAX_BLEND_NUM = 2;
	static const int MAX_PARAMETRIC_NUM = 2;

	float				m_blendTime;
	float				m_totalBlendTime;
	PlayInfo			m_blendSet[MAX_BLEND_NUM];
	PlayInfo			m_parametricSet[MAX_PARAMETRIC_NUM];
};


#endif // __MODEL_ANIMATOR_H__