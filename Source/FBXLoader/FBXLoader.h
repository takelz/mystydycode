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
//#error "x86��libfbxsdk.lib��p�ӂ��Ă�������"
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

// ��]����
enum RotateOrder {
	XYZ,
	ZXY,
};

// ���b�V�����
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

	IndexList		indexList;		///< �C���f�b�N�X���
	VertexList		vertexList;		///< ���_���
	NormalInfo		normalInfo;		///< �@�����
	UVList			uvList;			///< UV���
	MaterialName	materialName;	///< �}�e���A����(�}�e���A���ŕ������Ă�̂ň��
	SkinInfo		skinInfo;		///< �X�L�����
};
typedef std::vector<MeshInfo> MeshList;

// �{�[�����
struct BoneInfo {
	struct Vector3 {
		float x, y, z;
	};
	typedef std::vector<long> ChildList;

	std::string		name;		///< �{�[����
	Vector3			translate;	///< �ړ��l
	Vector3			rotation;	///< ��]�l
	Vector3			scale;		///< �g��k��
	Vector3			preRotate;	///< ���O��]
	RotateOrder		order;		///< ��]����
	ChildList		children;	///< �q�v�f
};
typedef std::vector<BoneInfo> BoneList;
struct BoneListInfo {
	int root;
	BoneList list;
};

// �}�e���A�����
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
	std::string name;			///< �}�e���A����
	std::string texture;		///< �e�N�X�`����
	Lambert		*pLambert;		///< �����o�[�g���
	Phong		*pPhong;		///< �t�H�����
};
typedef std::vector<MaterialInfo> MaterialList;

// �A�j���[�V�������
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

		std::string		name;					///< �Ή�����{�[����
		List			keyList[CHANNEL_MAX];	///< �`�����l�����Ƃ̃L�[���
	};
	typedef std::vector<CurveInfo> List;

	long		totalFrame;		///< �A�j���[�V��������
	List		curveList;		///< �A�j�����
};

/// ���O�o�͊֐��^��`
typedef void(*LogCallbackFunc)(const char *);


/// @brief ������
void LoadInitialize();

/// @brief �I������
void LoadTerminate();

/// @brief FBX�ǂݍ���
/// @param [in] fileName �t�@�C����
/// @param [out] meshList ���b�V���ǂݍ��ݐ�
/// @param [out] boneList �{�[���ǂݍ��ݐ�
/// @param [out] materiaList �}�e���A���ǂݍ��ݐ�
/// @param [out] anime �A�j���[�V�����ǂݍ��ݐ�
/// @return �ǂݍ��݌���
bool Load(const char *fileName, MeshList &meshList, BoneListInfo &boneListInfo, MaterialList &materialList, AnimationInfo &anime);

/// @brief ���O�����֐��̐ݒ�
/// @param [in] func �R�[���o�b�N�֐�
void SetLogCallbackFunc(LogCallbackFunc func);

/// @brief ���O�o��
/// @param[in] format �o�̓t�H�[�}�b�g
/// @param[in] ... �ό���
void LogOutput(const char *format, ...);

}; // fbx

#endif // __FBX_LOADER_H__