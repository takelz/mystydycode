/**
 * @file ModelAnimator.h
 * @brief �A�j���[�V��������
 * @author ryota.ssk
 */
#ifndef __MODEL_ANIMATOR_H__
#define __MODEL_ANIMATOR_H__

#include "ModelCommon.h"

class ModelAnimator
{
private:
	// �{�[�����
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

	// �A�j���[�V�������
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

	// �Đ����
	struct PlayInfo {
		struct AnimeMatrixInfo {
			DirectX::XMFLOAT3 trans;
			DirectX::XMFLOAT3 rot;
			DirectX::XMFLOAT3 scale;
			DirectX::XMMATRIX calc;
		};
		typedef std::vector<AnimeMatrixInfo> AnimeMatrix;
		AnimeMatrix matrix;

		int index;		// �Đ��A�j��
		float frame;	// ���݃t���[��
		float time;		// �����v�Z���x
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

	//! @name �Đ�����
	/// @{
	// �P���Đ�
	void Play(int index, bool loop = false, float start = 0.f);
	// �p�����g���b�N
	void PlayParametric(int anime1, int anime2, bool loop = false);
	void SetParametricBlend(float blend);
	// �u�����h�Đ�
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