#include "FBXLoadMesh.h"
#include <algorithm>

namespace fbx {

void GetIndex(MeshInfo::IndexList &list, FbxMesh *pMesh);
void GetVertex(MeshInfo::VertexList &list, FbxMesh *pMesh);
void GetNormal(MeshInfo::NormalInfo &info, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList);
void GetUV(MeshInfo::UVList &list, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList);
void GetMaterial(MeshInfo::MaterialName &name, FbxMesh *pMesh, FbxNode *pNode);
void GetSkin(MeshInfo::SkinInfo &info, FbxMesh *pMesh, MeshInfo::VertexList &vtxList);

//-----------------------------------------------------
//
/// @brief データクリア
//
//-----------------------------------------------------
void CleanupMesh(MeshList &meshList)
{
	MeshList::iterator it = meshList.begin();
	while (it != meshList.end()) {
		it->indexList.clear();
		it->vertexList.clear();
		it->normalInfo.list.clear();
		MeshInfo::UVList::iterator itUV = it->uvList.begin();
		while (itUV != it->uvList.end()) {
			itUV->list.clear();
			++itUV;
		}
		it->materialName.clear();
		it->skinInfo.weightList.clear();
		it->skinInfo.boneList.clear();
		++it;
	}
	meshList.clear();
}

//-----------------------------------------------------
//
/// @brief データ取得
//
//-----------------------------------------------------
void GetMesh(FbxScene *pScene, MeshList &meshList)
{
	CleanupMesh(meshList);
	meshList.resize(pScene->GetMemberCount<FbxMesh>());
	MeshList::iterator it = meshList.begin();
	LogOutput("MeshNum : %d\n", meshList.size());
	while (it != meshList.end()) {
		FbxMesh *pMesh = pScene->GetMember<FbxMesh>(static_cast<int>(it - meshList.begin()));
		MeshInfo &info = *it;
		LogOutput("========== MeshData[No.%d/Name:%s] ==========\n", it - meshList.begin(), pMesh->GetNode()->GetInitialName());

		GetIndex(info.indexList, pMesh);
		GetVertex(info.vertexList, pMesh);
		GetNormal(info.normalInfo, pMesh, info.indexList, info.vertexList);
		GetUV(info.uvList, pMesh, info.indexList, info.vertexList);
		GetMaterial(info.materialName, pMesh, pScene->GetRootNode());
		GetSkin(info.skinInfo, pMesh, info.vertexList);

		LogOutput("\n");
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief インデックスを取得
//
//-----------------------------------------------------
void GetIndex(MeshInfo::IndexList &list, FbxMesh *pMesh)
{
	list.resize(pMesh->GetPolygonCount() * 3);
	LogOutput("Indices(polygon) : %d(%d)\n", list.size(), pMesh->GetPolygonCount());
	for (int i = 0; i < pMesh->GetPolygonCount(); ++i) {
		LogOutput("[(%d)", i);
		for (int j = 0; j < 3; ++j) {
			list[i * 3 + j] = pMesh->GetPolygonVertex(i, j);
			LogOutput("%d", list[i * 3 + j]);
			if (j != 2) { LogOutput(","); }
		}
		LogOutput("]");
	}
	LogOutput("\n");
}


//-----------------------------------------------------
//
// 頂点情報を取得
//
//-----------------------------------------------------
void GetVertex(MeshInfo::VertexList &list, FbxMesh *pMesh)
{
	list.resize(pMesh->GetControlPointsCount());
	LogOutput("Vertices : %d\n", list.size());
	FbxVector4 *pVertices = pMesh->GetControlPoints();
	MeshInfo::VertexList::iterator it = list.begin();
	while (it != list.end()) {
		int index = static_cast<int>(it - list.begin());
		it->x = static_cast<float>(pVertices[index][0]);
		it->y = static_cast<float>(pVertices[index][1]);
		it->z = static_cast<float>(pVertices[index][2]);
		LogOutput("[(%d)%.3f,%.3f,%.3f]", index, it->x, it->y, it->z);
		++it;
	}
	LogOutput("\n");
}

//-----------------------------------------------------
//
/// @brief 法線情報を取得
//
//-----------------------------------------------------
void GetNormal(MeshInfo::NormalInfo &info, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList)
{
	FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal();
	if (pNormal == NULL) {
		return;
	}

	FbxLayerElement::EMappingMode mapping = pNormal->GetMappingMode();
	FbxLayerElement::EReferenceMode reference = pNormal->GetReferenceMode();

	// ログ出力
	LogOutput("Normals(mapping/reference) : ");
	if (mapping == FbxGeometryElement::eByControlPoint) {
		LogOutput("%d(ByVertex/", vtxList.size());
	} else if (mapping == FbxGeometryElement::eByPolygonVertex) {
		LogOutput("%d(ByIndex/", idxList.size());
	} else {
		LogOutput("0(undefined/");
	}
	if (reference == FbxGeometryElement::eDirect) {
		LogOutput("Direct)");
	} else if (reference == FbxGeometryElement::eIndexToDirect) {
		LogOutput("Index)");
	} else{
		LogOutput("undefined)");
	}
	LogOutput("\n");

	// マッピング別に初期化
	if (mapping == FbxGeometryElement::eByControlPoint) {
		info.mapping = MeshInfo::MAPPING_VERTEX;
		info.list.resize(vtxList.size());
	}
	else if (mapping == FbxGeometryElement::eByPolygonVertex) {
		info.mapping = MeshInfo::MAPPING_INDEX;
		info.list.resize(idxList.size());
	}
	MeshInfo::NormalInfo::List::iterator it = info.list.begin();
	while (it != info.list.end()) {
		int index = static_cast<int>(it - info.list.begin());
		if (reference == FbxGeometryElement::eIndexToDirect) {
			index = pNormal->GetIndexArray().GetAt(index); // インデックス対応
		}
		FbxVector4 normal = pNormal->GetDirectArray().GetAt(index);
		it->x = static_cast<float>(normal[0]);
		it->y = static_cast<float>(normal[1]);
		it->z = static_cast<float>(normal[2]);
		LogOutput("[(%d)%.3f,%.3f,%.3f]", it - info.list.begin(), it->x, it->y, it->z);
		++ it;
	}
	LogOutput("\n");
}

//-----------------------------------------------------
//
/// @brief UV情報を取得
//
//-----------------------------------------------------
void GetUV(MeshInfo::UVList &list, FbxMesh *pMesh, MeshInfo::IndexList &idxList, MeshInfo::VertexList &vtxList)
{
	int uvCount = pMesh->GetElementUVCount();
	if (uvCount <= 0) {
		return;
	}

	list.resize(uvCount);
	LogOutput("UV : %d\n", list.size());
	MeshInfo::UVList::iterator it = list.begin();
	while (it != list.end()) {
		int index = static_cast<int>(it - list.begin());
		FbxGeometryElementUV* pUV = pMesh->GetElementUV(index);
		if (pUV) {
			FbxLayerElement::EMappingMode mapping = pUV->GetMappingMode();
			FbxLayerElement::EReferenceMode reference = pUV->GetReferenceMode();

			// ログ出力
			LogOutput("UVSet[No.%d](mapping/reference) :", index);
			if (mapping == FbxGeometryElement::eByControlPoint) {
				LogOutput("%d(ByVertex/", vtxList.size());
			} else if (mapping == FbxGeometryElement::eByPolygonVertex) {
				LogOutput("%d(ByIndex/", idxList.size());
			} else{
				LogOutput("0(undefined/");
			}
			if (reference == FbxGeometryElement::eDirect) {
				LogOutput("Direct)");
			} else if (reference == FbxGeometryElement::eIndexToDirect) {
				LogOutput("Index)");
			} else{
				LogOutput("undefined)");
			}
			LogOutput("\n");

			// マッピング別に初期化
			if (mapping == FbxGeometryElement::eByControlPoint) {
				it->mapping = MeshInfo::MAPPING_VERTEX;
				it->list.resize(vtxList.size());
			} else if (mapping == FbxGeometryElement::eByPolygonVertex) {
				it->mapping = MeshInfo::MAPPING_INDEX;
				it->list.resize(idxList.size());
			}
			MeshInfo::UVInfo::List::iterator itUV = it->list.begin();
			while (itUV != it->list.end()) {
				int uvIndex = static_cast<int>(itUV - it->list.begin());
				if (reference == FbxGeometryElement::eIndexToDirect) {
					uvIndex = pUV->GetIndexArray().GetAt(uvIndex); // インデックス対応
				}
				FbxVector2 uv = pUV->GetDirectArray().GetAt(uvIndex);
				itUV->x = static_cast<float>(uv[0]);
				itUV->y = static_cast<float>(uv[1]);
				LogOutput("[(%d)%.3f,%.3f]", itUV - it->list.begin(), itUV->x, itUV->y);
				++itUV;
			}
			LogOutput("\n");
		}
		++it;
	}
}

//-----------------------------------------------------
//
/// @brief マテリアル名を取得
//
//-----------------------------------------------------
void GetMaterial(MeshInfo::MaterialName &name, FbxMesh *pMesh, FbxNode *pNode)
{
	int attrCnt = pNode->GetNodeAttributeCount();
	for (int i = 0; i < attrCnt; ++i) {
		FbxNodeAttribute* attr = pNode->GetNodeAttributeByIndex(i);
		if (attr == pMesh) {
			if (attr->GetAttributeType() == FbxNodeAttribute::eMesh) {
				name = pNode->GetMaterial(i)->GetInitialName();
				return;
			}
		}
	}

	for (int i = 0; i < pNode->GetChildCount(); ++i) {
		GetMaterial(name, pMesh, pNode->GetChild(i));
	}
}

//-----------------------------------------------------
//
/// @brief 頂点ブレンドを取得
//
//-----------------------------------------------------
void GetSkin(MeshInfo::SkinInfo &info, FbxMesh *pMesh, MeshInfo::VertexList &vtxList)
{
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount > 0) {
		LogOutput("Skin : %d\n", skinCount);

		// メッシュに紐づいたボーンの情報を取り出す
		typedef std::vector<MeshInfo::SkinInfo::WeightSet::WeightInfo> WeightInfoWork;
		typedef std::vector<WeightInfoWork> WeightSetWork;
		WeightSetWork weightSetWork(vtxList.size());
		FbxSkin *pSkin = static_cast<FbxSkin*>(pMesh->GetDeformer(0, FbxDeformer::eSkin)); // 単一のスキンのみ
		info.boneList.resize(pSkin->GetClusterCount());
		MeshInfo::SkinInfo::BoneList::iterator itBone = info.boneList.begin();
		while (itBone != info.boneList.end()) {
			int index = static_cast<int>(itBone - info.boneList.begin());
			// ボーン情報を取得
			FbxCluster *pCluster = pSkin->GetCluster(index);
			FbxAMatrix mat;
			pCluster->GetTransformLinkMatrix(mat);
			itBone->name = pCluster->GetLink()->GetInitialName();
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					itBone->offset[i][j] = static_cast<float>(mat[i][j]);
				}
			}

			// 対応する頂点ブレンドを取得
			int *pIndices = pCluster->GetControlPointIndices();
			double *pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < pCluster->GetControlPointIndicesCount(); ++i) {
				fbx::MeshInfo::SkinInfo::WeightSet::WeightInfo weightInfo = {index, static_cast<float>(pWeights[i])};
				weightSetWork[pIndices[i]].push_back(weightInfo);
			}
			++itBone;
		}

		// 取り出したデータをMeshInfo::WeightSet::WEIGHT_NUM_MAX分にそろえる
		info.weightList.resize(vtxList.size());
		WeightSetWork::iterator itWeight = weightSetWork.begin();
		while (itWeight != weightSetWork.end()) {
			// ソート
			std::sort(
				itWeight->begin(), itWeight->end(),
				[](const MeshInfo::SkinInfo::WeightSet::WeightInfo &objA, const MeshInfo::SkinInfo::WeightSet::WeightInfo &objB) {
					return objA.value > objB.value;
				});
			// 頂点ブレンドデータの数をそろえる
			while (itWeight->size() > MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX) {
				itWeight->pop_back();
			}
			while (itWeight->size() < MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX) {
				fbx::MeshInfo::SkinInfo::WeightSet::WeightInfo weightInfo = {0, 0.0f};
				itWeight->push_back(weightInfo);
			}
			// 正規化
			int index = static_cast<int>(itWeight - weightSetWork.begin());
			float total = 0.0f;
			for (int i = 0; i < MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX; ++i) {
				info.weightList[index].weight[i].index = (*itWeight)[i].index;
				total += (*itWeight)[i].value;
			}
			if (total == 0.0f) {
				total = 1.0f;
			}
			for (int i = 0; i < MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX; ++i) {
				info.weightList[index].weight[i].value = (*itWeight)[i].value / total;
			}

			LogOutput("[(%d)%d{%.3f},%d{%.3f},%d{%.3f},%d{%.3f}]", index,
				info.weightList[index].weight[0].index, info.weightList[index].weight[0].value,
				info.weightList[index].weight[1].index, info.weightList[index].weight[1].value,
				info.weightList[index].weight[2].index, info.weightList[index].weight[2].value,
				info.weightList[index].weight[3].index, info.weightList[index].weight[3].value);
			++itWeight;
		}
		LogOutput("\n");
	} else {
		// ほぼほぼユニティちゃんのための対応かも
		// ボーンのノードに紐づいているかチェック
		FbxNode *pMeshNode = pMesh->GetNode();
		FbxNode *parent = pMeshNode->GetParent();
		while (parent != NULL) {
			FbxNodeAttribute *attr = parent->GetNodeAttribute();
			if (attr != NULL) {
				FbxNodeAttribute::EType type = attr->GetAttributeType();
				if (type == FbxNodeAttribute::eSkeleton) {
					break;
				}
				else if (type == FbxNodeAttribute::eMesh) {
					pMeshNode = parent;
				}
			}
			parent = parent->GetParent();
		}
		// 紐づいていた場合、そのボーンに完全依存する頂点ブレンドとして扱う
		if (parent != NULL) {
			// ボーン情報設定
			MeshInfo::SkinInfo::BoneInfo boneInfo;
			FbxAMatrix mat = pMeshNode->EvaluateLocalTransform();
			mat = mat.Inverse();
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					boneInfo.offset[i][j] = static_cast<float>(mat[i][j]);
				}
			}
			boneInfo.name = parent->GetInitialName();
			info.boneList.push_back(boneInfo);

			// 頂点ブレンド設定
			info.weightList.resize(vtxList.size());
			MeshInfo::SkinInfo::WeightList::iterator it = info.weightList.begin();
			while (it != info.weightList.end()) {
				for (int i = 0; i < MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX; ++i) {
					it->weight[i].index = 0;
					it->weight[i].value = 0.0f;
				}
				it->weight[0].value = 1.0f;
				++it;
			}
		}
	}
}

}; // fbx