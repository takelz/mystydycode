#if !defined(__FBX_LOADER_H__)
#define __FBX_LOADER_H__

#include <fbxsdk.h>
#include <vector>
#include <string>


#if (_WIN64)
#if (_MSC_VER >= 1910) // VS2017
#if (_DEBUG)
#pragma comment(lib, "FBX\\libfbxsdk_vs2017d.lib")
#pragma comment(lib, "FBX\\libxml2_vs2017d.lib")
#pragma comment(lib, "FBX\\zlib_vs2017d.lib")
#else
#pragma comment(lib, "FBX\\libfbxsdk_vs2017.lib")
#pragma comment(lib, "FBX\\libxml2_vs2017.lib")
#pragma comment(lib, "FBX\\zlib_vs2017.lib")
#endif // _DEBUG
#elif(_MSC_VER >= 1900) // VS2015
#if (_DEBUG)
#pragma comment(lib, "FBX\\libfbxsdk_vs2015d.lib")
#pragma comment(lib, "FBX\\libxml2_vs2015d.lib")
#pragma comment(lib, "FBX\\zlib_vs2015d.lib")
#else
#pragma comment(lib, "FBX\\libfbxsdk_vs2015.lib")
#pragma comment(lib, "FBX\\libxml2_vs2015.lib")
#pragma comment(lib, "FBX\\zlib_vs2015.lib")
#endif // _DEBUG
#endif // _MSC_VER
#else
#if (_DEBUG)
#pragma comment(lib, "FBX\\libfbxsdk_vs2017d.lib")
#pragma comment(lib, "FBX\\libxml2_vs2017d.lib")
#pragma comment(lib, "FBX\\zlib_vs2017d.lib")
#else
#pragma comment(lib, "FBX\\libfbxsdk_vs2017.lib")
#pragma comment(lib, "FBX\\libxml2_vs2017.lib")
#pragma comment(lib, "FBX\\zlib_vs2017.lib")
#endif // _DEBUG
//#error "x86のlibfbxsdk.libを用意してください"
#if (_DEBUG)
#pragma comment(lib, "FBX\\libfbxsdk_vs2017d_86.lib")
#pragma comment(lib, "FBX\\libxml2_vs2017d_86.lib")
#pragma comment(lib, "FBX\\zlib_vs2017d_86.lib")
#else
#pragma comment(lib, "FBX\\libfbxsdk_vs2017_86.lib")
#pragma comment(lib, "FBX\\libxml2_vs2017_86.lib")
#pragma comment(lib, "FBX\\zlib_vs2017_86.lib")
#endif // _DEBUG
#endif // _WIN64


namespace fbx {

// 回転順序
enum RotateOrder {
	XYZ,
	ZXY,
};

// メッシュ情報
struct MeshInfo
{
	enum Mapping {
		MAPPING_VERTEX,
		MAPPING_INDEX,
	};
	struct Vector2 {
		float x, y;
	};
	struct Vector3 {
		float x, y, z;
	};
	struct NormalInfo {
		typedef std::vector<Vector3> List;
		Mapping		mapping;
		List		list;
	};
	struct UVInfo {
		typedef std::vector<Vector2> List;
		Mapping		mapping;
		List		list;
	};
	typedef std::vector<UVInfo> UVList;
	struct SkinInfo {
		struct WeightSet {
			struct WeightInfo {
				long	index;
				float	value;
			};
			static const long WEIGHT_NUM_MAX = 4;
			WeightInfo weight[WEIGHT_NUM_MAX];
		};
		struct BoneInfo {
			std::string name;
			float offset[4][4];
		};
		typedef std::vector<WeightSet> WeightList;
		typedef std::vector<BoneInfo> BoneList;

		WeightList		weightList;
		BoneList		boneList;
	};
	typedef std::vector<long> IndexList;
	typedef std::vector<Vector3> VertexList;
	typedef std::string MaterialName;

	IndexList		indexList;		///< インデックス情報
	VertexList		vertexList;		///< 頂点情報
	NormalInfo		normalInfo;		///< 法線情報
	UVList			uvList;			///< UV情報
	MaterialName	materialName;	///< マテリアル名(マテリアルで分割してるので一つ
	SkinInfo		skinInfo;		///< スキン情報
};
typedef std::vector<MeshInfo> MeshList;

// ボーン情報
struct BoneInfo {
	struct Vector3 {
		float x, y, z;
	};
	typedef std::vector<long> ChildList;

	std::string		name;		///< ボーン名
	Vector3			translate;	///< 移動値
	Vector3			rotation;	///< 回転値
	Vector3			scale;		///< 拡大縮小
	Vector3			preRotate;	///< 事前回転
	RotateOrder		order;		///< 回転順序
	ChildList		children;	///< 子要素
};
typedef std::vector<BoneInfo> BoneList;
struct BoneListInfo {
	int root;
	BoneList list;
};

// マテリアル情報
struct MaterialInfo
{
	struct Color {
		float r, g, b, a;
	};
	struct Vector3 {
		float x, y, z;
	};
	struct Lambert {
		Color	ambient;
		Color	diffuse;
		Color	emissive;
		Vector3	bump;
		float	transparency;
	};
	struct Phong {
		Color	specular;
		float	shininess;
		float	reflectivity;
		float	transparency;
	};
	std::string name;			///< マテリアル名
	std::string texture;		///< テクスチャ名
	Lambert		*pLambert;		///< ランバート情報
	Phong		*pPhong;		///< フォン情報
};
typedef std::vector<MaterialInfo> MaterialList;

// アニメーション情報
struct AnimationInfo
{
	struct CurveInfo
	{
		enum Channels {
			CHANNEL_TRANSLATION_X,
			CHANNEL_TRANSLATION_Y,
			CHANNEL_TRANSLATION_Z,
			CHANNEL_ROTATION_X,
			CHANNEL_ROTATION_Y,
			CHANNEL_ROTATION_Z,
			CHANNEL_SCALING_X,
			CHANNEL_SCALING_Y,
			CHANNEL_SCALING_Z,
			CHANNEL_MAX
		};
		struct KeyInfo {
			long	frame;
			float	value;
		};
		typedef std::vector<KeyInfo> List;

		std::string		name;					///< 対応するボーン名
		List			keyList[CHANNEL_MAX];	///< チャンネルごとのキー情報
	};
	typedef std::vector<CurveInfo> List;

	long		totalFrame;		///< アニメーション時間
	List		curveList;		///< アニメ情報
};

/// ログ出力関数型定義
typedef void(*LogCallbackFunc)(const char *);


/// @brief 初期化
void LoadInitialize();

/// @brief 終了処理
void LoadTerminate();

/// @brief FBX読み込み
/// @param [in] fileName ファイル名
/// @param [out] meshList メッシュ読み込み先
/// @param [out] boneList ボーン読み込み先
/// @param [out] materiaList マテリアル読み込み先
/// @param [out] anime アニメーション読み込み先
/// @return 読み込み結果
bool Load(const char *fileName, MeshList &meshList, BoneListInfo &boneListInfo, MaterialList &materialList, AnimationInfo &anime);

/// @brief ログ処理関数の設定
/// @param [in] func コールバック関数
void SetLogCallbackFunc(LogCallbackFunc func);

/// @brief ログ出力
/// @param[in] format 出力フォーマット
/// @param[in] ... 可変個引数
void LogOutput(const char *format, ...);

}; // fbx

#endif // __FBX_LOADER_H__