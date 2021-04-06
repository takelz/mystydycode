/**
 * @file ModelObject.h
 * @brief ���f���`��
 * @author ryota.ssk
 */
#ifndef __MODEL_OBJECT_H__
#define __MODEL_OBJECT_H__

#include "ModelCommon.h"
#include "../Shader/ShaderSelector.h"


class ModelAnimator;

class ModelObject
{
public:
	const static int VERTEX_WEIGHT_NUM = 4;

	struct VertexInfo {
		DirectX::XMFLOAT3	pos;
		DirectX::XMFLOAT3	normal;
		DirectX::XMFLOAT2	uv;
		DirectX::XMFLOAT4	color;
		float				weight[VERTEX_WEIGHT_NUM];
		int					index[VERTEX_WEIGHT_NUM];
	};
	struct WriteVertexInfo {
		int					num;		///< ���_��
		const VertexInfo	*pInit;		///< �����f�[�^
		VertexInfo			*pWrite;	///< �������ݐ�
		void				*pArg;		///< ����
	};

private:
	static ID3D11InputLayout *m_format;
	static ID3D11VertexShader *m_pVertexShader;
	static ID3D11PixelShader *m_pPixelShader;
	static ID3D11SamplerState *m_pSamplerState;
	static ID3D11SamplerState *m_pSamplerStateWRAP;


	// ���b�V�����
	struct MeshInfo {
		struct BoneInfo {
			int index;
			DirectX::XMFLOAT4X4 inverse;
		};
		typedef long IndexInfo;
		typedef std::vector<VertexInfo> VertexList;
		typedef std::vector<IndexInfo> IndexList;
		typedef std::vector<BoneInfo> BoneList;

		VertexList vertexList;
		IndexList indexList;
		ID3D11Buffer *vertexBuffer;
		ID3D11Buffer *indexBuffer;
		BoneList boneList;
		std::string materialName;
	};
	typedef std::vector<MeshInfo> MeshList;

	// �}�e���A�����
	struct MaterialInfo {
		std::string name;
		struct Material {
			D3DCOLORVALUE   Diffuse;
			D3DCOLORVALUE   Ambient;
			D3DCOLORVALUE   Specular;
			D3DCOLORVALUE   Emissive;
			float           Power;
		};
		Material material;
		ID3D11Texture2D *pTexture;
		ID3D11ShaderResourceView *pResource;
	};
	typedef std::vector<MaterialInfo> MaterialList;

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

public:
	// �g�����X�t�H�[��
	struct Transform {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	// �`��I�v�V����
	enum DrawFlag {
		NONE = 0x00,
		MODEL = 0x01,
		BONE = 0x02,
		ALL = 0x03,
	};


public:
	ModelObject();
	~ModelObject();

	/**
	 * @brief FBX�t�@�C���ǂݍ���
	 * @param [in] file �t�@�C����
	 * @return ��������
	 */
	bool Create(const char* file, bool bExplosion = false);

	/**
	 * @brief �`��
	 * @param [in] transform �ϊ��s��
	 * @param [in] flag �`��I�v�V����
	 */
	void Draw(const Transform &transform, ModelAnimator *pAnimator = NULL, DrawFlag flag = DrawFlag::NONE);


	//! @brief ���_�������݊֐��|�C���^
	typedef void(*VertexWriteCallBack)(WriteVertexInfo &vtx);
	/**
	 * @brief ���_��������
	 * @param[in] func �Ăяo���֐�
	 * @param[in] pArg ���蓖�ăf�[�^
	 */
	void Write(VertexWriteCallBack func, void *pArg = NULL);



	//! @name SetFunction
	/// @{
	/**
	 * @brief �J���[�ݒ�
	 * @param[in] color �F���
	 */
	void SetColor(D3DCOLORVALUE color);
	/// @}

	//! @name GetFunction
	/// @{
	size_t GetMeshCount() const;
	size_t GetPolygonCount(int mesh) const;
	const VertexInfo *GetVertexInfo(int mesh, int polygon, int idx) const;
	/// @}

	bool m_isWrapTex;		// �e�N�X�`���̌J��Ԃ���ON�ɂ��邩(��ɃV�F�[�_�p)

private:
	void InitMesh(fbx::MeshList &list, fbx::BoneListInfo &bone, bool bExplosion);
	void InitMaterial(fbx::MaterialList &list);

	void DrawModel(const Transform &transform, const ModelAnimator *pAnimator);

private:
	MeshList		m_meshList;
	MaterialList	m_materialList;

	ID3D11Buffer	*m_matrixBuffer;
	ID3D11Buffer	*m_materialBuffer;

	bool m_isDynamic;

	D3DCOLORVALUE	m_modelColor;
};


// �A�b�v�f�[�g�ŃJ�E���g�グ��
void ShaderTimeCountUp();

#endif // __MODEL_OBJECT_H__