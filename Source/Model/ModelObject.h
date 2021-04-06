/**
 * @file ModelObject.h
 * @brief モデル描画
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
		int					num;		///< 頂点数
		const VertexInfo	*pInit;		///< 初期データ
		VertexInfo			*pWrite;	///< 書き込み先
		void				*pArg;		///< 引数
	};

private:
	static ID3D11InputLayout *m_format;
	static ID3D11VertexShader *m_pVertexShader;
	static ID3D11PixelShader *m_pPixelShader;
	static ID3D11SamplerState *m_pSamplerState;
	static ID3D11SamplerState *m_pSamplerStateWRAP;


	// メッシュ情報
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

	// マテリアル情報
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

public:
	// トランスフォーム
	struct Transform {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	// 描画オプション
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
	 * @brief FBXファイル読み込み
	 * @param [in] file ファイル名
	 * @return 処理結果
	 */
	bool Create(const char* file, bool bExplosion = false);

	/**
	 * @brief 描画
	 * @param [in] transform 変換行列
	 * @param [in] flag 描画オプション
	 */
	void Draw(const Transform &transform, ModelAnimator *pAnimator = NULL, DrawFlag flag = DrawFlag::NONE);


	//! @brief 頂点書き込み関数ポインタ
	typedef void(*VertexWriteCallBack)(WriteVertexInfo &vtx);
	/**
	 * @brief 頂点書き込み
	 * @param[in] func 呼び出し関数
	 * @param[in] pArg 割り当てデータ
	 */
	void Write(VertexWriteCallBack func, void *pArg = NULL);



	//! @name SetFunction
	/// @{
	/**
	 * @brief カラー設定
	 * @param[in] color 色情報
	 */
	void SetColor(D3DCOLORVALUE color);
	/// @}

	//! @name GetFunction
	/// @{
	size_t GetMeshCount() const;
	size_t GetPolygonCount(int mesh) const;
	const VertexInfo *GetVertexInfo(int mesh, int polygon, int idx) const;
	/// @}

	bool m_isWrapTex;		// テクスチャの繰り返しをONにするか(主にシェーダ用)

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


// アップデートでカウント上げる
void ShaderTimeCountUp();

#endif // __MODEL_OBJECT_H__