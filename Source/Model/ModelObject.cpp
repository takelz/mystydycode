/**
 * @file ModelObject.cpp
 * @brief ���f���`��
 * @author ryota.ssk
 */
#include "ModelObject.h"
#include "ModelAnimator.h"

#include <FBXLoader/FBXLoadMesh.h>
#include <FBXLoader/FBXLoadMaterial.h>
#include <FBXLoader/FBXLoadBone.h>
#include <FBXLoader/FBXLoadAnimation.h>

#include <DirectX/DirectXTex/DirectXTex.h>
#include <DirectX/DirectXTex/WICTextureLoader.h>
#include <DirectX/DX11Light.h>

#include <Shader/Shader.h>




using namespace DirectX;


ID3D11InputLayout *ModelObject::m_format = NULL;
ID3D11VertexShader *ModelObject::m_pVertexShader = NULL;
ID3D11PixelShader *ModelObject::m_pPixelShader = NULL;
ID3D11SamplerState *ModelObject::m_pSamplerState = NULL;
ID3D11SamplerState *ModelObject::m_pSamplerStateWRAP = NULL;

FLOAT shaderTime = 0;;

struct MatrixBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 bone[120];
	XMFLOAT4 time;
	XMFLOAT4X4 shadowViewProj;
};
struct MaterialBuffer
{
	D3DCOLORVALUE diffuse;
	D3DCOLORVALUE ambient;
	D3DCOLORVALUE specular;
	XMVECTOR texture;
};

/**
 * @brief �R���X�g���N�^
 */
ModelObject::ModelObject()
{
	SetColor({ 1.f, 1.f, 1.f, 1.f });
}
/**
 * @brief �f�X�g���N�^
 */
ModelObject::~ModelObject()
{
	MeshList::iterator meshIt = m_meshList.begin();
	while (meshIt != m_meshList.end()) {
		if (meshIt->indexBuffer != NULL) {
			meshIt->indexBuffer->Release();
		}
		if (meshIt->vertexBuffer != NULL) {
			meshIt->vertexBuffer->Release();
		}
		++meshIt;
	}

	MaterialList::iterator matIt = m_materialList.begin();
	while (matIt != m_materialList.end()) {
		if (matIt->pTexture != NULL) {
			matIt->pTexture->Release();
		}
		if (matIt->pResource != NULL) {
			matIt->pResource->Release();
		}
		++ matIt;
	}


	if (m_matrixBuffer != NULL) {
		m_matrixBuffer->Release();
	}
	if (m_materialBuffer != NULL) {
		m_materialBuffer->Release();
	}
}

/**
 * @brief FBX�t�@�C���ǂݍ���
 * @param [in] file �t�@�C����
 * @return ��������
 */
bool ModelObject::Create(const char *file, bool bExplosion)
{
	ID3D11Device *pDevice = GetDX11Device();
	/*
	// �V�F�[�_�쐬
	if (m_format == NULL && m_pVertexShader == NULL) {
		if (FAILED(CreateVertexShader("Assets/Shader/model_vs.cso", InputLayoutKind::SKIN, &m_pVertexShader, &m_format))) {
			return false;
		}
	}
	if (m_pPixelShader == NULL) {
		if (FAILED(CreatePixelShader("Assets/Shader/model_ps.cso", &m_pPixelShader))) {
			return false;
		}
	}
	*/




	if (m_pSamplerState == NULL) {
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	
		sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;	// �J��Ԃ��𖳌��A�J��Ԃ��L����WRAP
		sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.BorderColor[0] = 1;
		sd.BorderColor[1] = 1;
		sd.BorderColor[2] = 1;
		sd.BorderColor[3] = 1;
		if (FAILED(pDevice->CreateSamplerState(&sd, &m_pSamplerState))) {
			return false;
		}
	}

	if (m_pSamplerStateWRAP== NULL) {
		D3D11_SAMPLER_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	// �J��Ԃ��𖳌��A�J��Ԃ��L����WRAP
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		if (FAILED(pDevice->CreateSamplerState(&sd, &m_pSamplerStateWRAP))) {
			return false;
		}
	}

	



	char directory[MAX_PATH];
	PushDirectory(file, directory);

	// �ǂݍ��ݏ���
	fbx::LoadInitialize();
	fbx::MeshList meshList;
	fbx::BoneListInfo boneListInfo;
	fbx::MaterialList materialList;
	fbx::AnimationInfo animeInfo;

	// �ǂݍ���
	bool ret = fbx::Load(PathFindFileNameA(file), meshList, boneListInfo, materialList, animeInfo);
	if (ret) {
		m_isDynamic = bExplosion;
		InitMesh(meshList, boneListInfo, bExplosion);
		InitMaterial(materialList);

		// �}�g���b�N�X�o�b�t�@
		D3D11_BUFFER_DESC matrixBufDesc;
		matrixBufDesc.ByteWidth = sizeof(MatrixBuffer);
		matrixBufDesc.Usage = D3D11_USAGE_DEFAULT;
		matrixBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufDesc.CPUAccessFlags = 0;
		matrixBufDesc.MiscFlags = 0;
		matrixBufDesc.StructureByteStride = 0;
		pDevice->CreateBuffer(&matrixBufDesc, NULL, &m_matrixBuffer);
		// �}�e���A���o�b�t�@
		D3D11_BUFFER_DESC materialBufDesc;
		materialBufDesc.ByteWidth = sizeof(MaterialBuffer);
		materialBufDesc.Usage = D3D11_USAGE_DEFAULT;
		materialBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		materialBufDesc.CPUAccessFlags = 0;
		materialBufDesc.MiscFlags = 0;
		materialBufDesc.StructureByteStride = 0;
		pDevice->CreateBuffer(&materialBufDesc, NULL, &m_materialBuffer);

	}

	// �I��
	fbx::CleanupAnimation(animeInfo);
	fbx::CleanupBone(boneListInfo);
	fbx::CleanupMaterial(materialList);
	fbx::CleanupMesh(meshList);
	fbx::LoadTerminate();

	PopDirectory(directory);

	return ret;
}

//-----------------------------------------------------
//
/// @brief �`��
//
//-----------------------------------------------------
void ModelObject::Draw(const Transform &transform, ModelAnimator *pAnimator, DrawFlag flag)
{
	if (flag == DrawFlag::NONE || (flag & DrawFlag::MODEL) > 0 ) {
		DrawModel(transform, pAnimator);
	}
	if ((flag & DrawFlag::BONE) > 0) {
		if (pAnimator != NULL) {
			pAnimator->Draw(transform.world);
		}
	}
}

/*
 * @brief ���_�o�b�t�@�`������
 */
void ModelObject::Write(VertexWriteCallBack func, void *pArg)
{
	ID3D11Device *pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();
	D3D11_MAPPED_SUBRESOURCE pSubResource;
	HRESULT hr;

	MeshList::iterator meshIt = m_meshList.begin();
	while (meshIt != m_meshList.end()) {

		// �R�s�[�p�̃o�b�t�@�쐬
		ID3D11Buffer *copyBuffer;
		D3D11_BUFFER_DESC copyDesc;
		ZeroMemory(&copyDesc, sizeof(D3D11_BUFFER_DESC));
		meshIt->vertexBuffer->GetDesc(&copyDesc);
		copyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		copyDesc.Usage = D3D11_USAGE_STAGING;
		copyDesc.BindFlags = 0;
		copyDesc.MiscFlags = 0;
		hr = pDevice->CreateBuffer(&copyDesc, NULL, &copyBuffer);
		if (FAILED(hr)) {
			continue;
		}

		// �R�[���o�b�N�f�[�^�쐬
		const int memSize = int(sizeof(VertexInfo) * meshIt->vertexList.size());
		WriteVertexInfo write = {
			int(meshIt->vertexList.size()),
			&meshIt->vertexList.front(),
			new VertexInfo[meshIt->vertexList.size()],
			pArg
		};
		pDeviceContext->CopyResource(copyBuffer, meshIt->vertexBuffer);
		hr = pDeviceContext->Map(copyBuffer, 0, D3D11_MAP_READ, 0, &pSubResource);
		if (SUCCEEDED(hr)) {
			memcpy_s(write.pWrite, memSize, pSubResource.pData, memSize);
		}
		pDeviceContext->Unmap(copyBuffer, 0);
		SAFE_RELEASE(copyBuffer);

		// ���_��������
		func(write);

		// �f�[�^�R�s�[
		hr = pDeviceContext->Map(meshIt->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pSubResource);
		if (SUCCEEDED(hr)) {
			memcpy_s(pSubResource.pData, memSize, write.pWrite, memSize);
		}
		pDeviceContext->Unmap(meshIt->vertexBuffer, 0);
		SAFE_DELETE_ARRAY(write.pWrite);

		++meshIt;
	}
}


/**
 * @brief �J���[�ݒ�
 * @param[in] color �F���
 */
void ModelObject::SetColor(D3DCOLORVALUE color)
{
	m_modelColor = color;
}

size_t ModelObject::GetMeshCount() const
{
	return m_meshList.size();
}
size_t ModelObject::GetPolygonCount(int mesh) const
{
	if (GetMeshCount() <= mesh) {
		return -1;
	}
	if (m_meshList[mesh].indexList.empty()) {
		return m_meshList[mesh].vertexList.size() / 3;
	}
	else {
		return m_meshList[mesh].indexList.size() / 3;
	}
}
const ModelObject::VertexInfo *ModelObject::GetVertexInfo(int mesh, int polygon, int idx) const
{
	if (GetPolygonCount(mesh) <= polygon) {
		return NULL;
	}
	if (idx < 0 || 3 <= idx) {
		return NULL;
	}

	int vtxIdx = polygon * 3 + idx;
	if (!m_meshList[mesh].indexList.empty()) {
		vtxIdx = m_meshList[mesh].indexList[vtxIdx];
	}
	return &m_meshList[mesh].vertexList[vtxIdx];
}

//-----------------------------------------------------
//
/// @brief ���b�V���f�[�^�̏�����
//
//-----------------------------------------------------
void ModelObject::InitMesh(fbx::MeshList &list, fbx::BoneListInfo &bone, bool bExplosion)
{
	ID3D11Device *pDevice = GetDX11Device();

	m_meshList.resize(list.size());
	fbx::MeshList::iterator it = list.begin();
	while (it != list.end()) {
		int index = static_cast<int>(it - list.begin());

		// �}�b�s���O�`�F�b�N
		bool isMappingIndex = it->normalInfo.mapping == fbx::MeshInfo::MAPPING_INDEX;
		if (it->uvList.size() > 0) {
			// �P��uv�݂̂ɑΉ�
			isMappingIndex |= it->uvList[0].mapping == fbx::MeshInfo::MAPPING_INDEX;
		}
		if (bExplosion) {
			isMappingIndex = true;
		}

		// ���_�ɑΉ������}�b�s���O���Ȃ���΃C���f�b�N�X�쐬
		if (!isMappingIndex) {
			// �f�[�^�R�s�[
			m_meshList[index].indexList.resize(it->indexList.size());
			for (unsigned int i = 0; i < it->indexList.size(); ++i) {
				m_meshList[index].indexList[i] = it->indexList[i];
			}
			// �o�b�t�@�쐬
			int idxNum = static_cast<int>(it->indexList.size());
			D3D11_BUFFER_DESC idxBufferDesc;
			idxBufferDesc.ByteWidth = idxNum * sizeof(MeshInfo::IndexInfo);
			idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			idxBufferDesc.CPUAccessFlags = 0;
			idxBufferDesc.MiscFlags = 0;
			idxBufferDesc.StructureByteStride = 0;

			MeshInfo::IndexInfo *pIdxData = new MeshInfo::IndexInfo[idxNum];
			for (int i = 0; i < idxNum; ++i) {
				memcpy_s(&pIdxData[i], sizeof(MeshInfo::IndexInfo), &m_meshList[index].indexList[i], sizeof(MeshInfo::IndexInfo));
			}

			D3D11_SUBRESOURCE_DATA idxResourceData;
			idxResourceData.pSysMem = pIdxData;
			idxResourceData.SysMemPitch = 0;
			idxResourceData.SysMemSlicePitch = 0;

			if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &idxResourceData, &m_meshList[index].indexBuffer))) {
				assert("index");
			}

			delete[] pIdxData;
		}
		else {
			m_meshList[index].indexBuffer = NULL;
		}

		// ���_�f�[�^�쐬
		m_meshList[index].vertexList.resize(isMappingIndex ? it->indexList.size() : it->vertexList.size());
		for (unsigned int i = 0; i < m_meshList[index].vertexList.size(); ++i) {
			int vtxIdx = i;
			int normIdx = i;
			int uvIdx = i;
			if (isMappingIndex) {
				vtxIdx = it->indexList[i];
				if (it->normalInfo.mapping == fbx::MeshInfo::MAPPING_VERTEX) { normIdx = vtxIdx; }
				if (it->uvList.size() > 0 && it->uvList[0].mapping == fbx::MeshInfo::MAPPING_VERTEX) { uvIdx = vtxIdx; }
			}

			m_meshList[index].vertexList[i].pos = XMFLOAT3(it->vertexList[vtxIdx].x, it->vertexList[vtxIdx].y, it->vertexList[vtxIdx].z);
			if (it->normalInfo.list.size() > 0) {
				m_meshList[index].vertexList[i].normal = XMFLOAT3(it->normalInfo.list[normIdx].x, it->normalInfo.list[normIdx].y, it->normalInfo.list[normIdx].z);
			}
			else {
				m_meshList[index].vertexList[i].normal = XMFLOAT3(0, 0, 0);
			}
			if (it->uvList.size() > 0 && it->uvList[0].list.size()) {
				m_meshList[index].vertexList[i].uv = XMFLOAT2(it->uvList[0].list[uvIdx].x, it->uvList[0].list[uvIdx].y);
				m_meshList[index].vertexList[i].uv.y = 1.0f - m_meshList[index].vertexList[i].uv.y;	// �㉺���]
			}
			else {
				m_meshList[index].vertexList[i].uv = XMFLOAT2(0, 0);
			}
			if (it->skinInfo.weightList.size() > 0) {
				for (int j = 0; j < fbx::MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX; ++j) {
					int boneIndex = it->skinInfo.weightList[vtxIdx].weight[j].index;
					float boneValue = it->skinInfo.weightList[vtxIdx].weight[j].value;
					m_meshList[index].vertexList[i].index[j] = boneIndex;
					m_meshList[index].vertexList[i].weight[j] = boneValue;
				}
			}
			else {
				for (int j = 0; j < fbx::MeshInfo::SkinInfo::WeightSet::WEIGHT_NUM_MAX; ++j) {
					m_meshList[index].vertexList[i].index[j] = 0;
					m_meshList[index].vertexList[i].weight[j] = 0.0f;
				}
				m_meshList[index].vertexList[i].weight[0] = 1.f;
			}
			m_meshList[index].vertexList[i].color = { 1, 1, 1, 1 };
		}

		// ���b�V���ɑΉ�����{�[���ݒ�
		fbx::MeshInfo::SkinInfo::BoneList::iterator meshBoneIt = it->skinInfo.boneList.begin();
		while (meshBoneIt != it->skinInfo.boneList.end()) {
			fbx::BoneList::iterator boneIt = bone.list.begin();
			while (boneIt != bone.list.end()) {
				if (meshBoneIt->name == boneIt->name) {
					MeshInfo::BoneInfo info;
					info.index = static_cast<int>(boneIt - bone.list.begin());
					XMMATRIX mat;
					for (int i = 0; i < 4; ++i) {
						mat.r[i] = XMVectorSet(meshBoneIt->offset[i][0], meshBoneIt->offset[i][1], meshBoneIt->offset[i][2], meshBoneIt->offset[i][3]);
					}
					XMStoreFloat4x4(&info.inverse, XMMatrixInverse(NULL, mat));
					m_meshList[index].boneList.push_back(info);
					break;
				}
				++boneIt;
			}
			++meshBoneIt;
		}

		// ���_�o�b�t�@�쐬
		size_t vtxNum = m_meshList[index].vertexList.size();
		D3D11_BUFFER_DESC vtxBufferDesc;
		vtxBufferDesc.ByteWidth = static_cast<UINT>(sizeof(VertexInfo) * vtxNum);
		vtxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vtxBufferDesc.MiscFlags = 0;
		vtxBufferDesc.StructureByteStride = 0;
		if (m_isDynamic) {
			vtxBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vtxBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			vtxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vtxBufferDesc.CPUAccessFlags = 0;
		}


		VertexInfo *pVtxData = new VertexInfo[vtxNum];
		for (size_t i = 0; i < vtxNum; ++i) {
			memcpy_s(&pVtxData[i], sizeof(VertexInfo), &m_meshList[index].vertexList[i], sizeof(VertexInfo));
		}

		D3D11_SUBRESOURCE_DATA vtxResourceData;
		vtxResourceData.pSysMem = pVtxData;
		vtxResourceData.SysMemPitch = 0;
		vtxResourceData.SysMemSlicePitch = 0;

		if (FAILED(pDevice->CreateBuffer(&vtxBufferDesc, &vtxResourceData, &m_meshList[index].vertexBuffer))) {
			assert("vtx");
		}

		delete[] pVtxData;


		// �}�e���A��
		m_meshList[index].materialName = it->materialName;

		++it;
	}
}
//-----------------------------------------------------
//
/// @brief �}�e���A���f�[�^�̏�����
//
//-----------------------------------------------------
void ModelObject::InitMaterial(fbx::MaterialList &list)
{
	ID3D11Device *pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	m_materialList.resize(list.size());
	MaterialList::iterator it = m_materialList.begin();
	while (it != m_materialList.end()) {
		int index = static_cast<int>(it - m_materialList.begin());
		const fbx::MaterialInfo &info = list[index];
		MaterialInfo::Material &mat = m_materialList[index].material;

		// �}�e���A����
		it->name = list[index].name;

		// �}�e���A��
		if (info.pLambert != NULL) {
			mat.Ambient.r = info.pLambert->ambient.r;
			mat.Ambient.g = info.pLambert->ambient.g;
			mat.Ambient.b = info.pLambert->ambient.b;
			mat.Ambient.a = info.pLambert->ambient.a;
			mat.Diffuse.r = info.pLambert->diffuse.r;
			mat.Diffuse.g = info.pLambert->diffuse.g;
			mat.Diffuse.b = info.pLambert->diffuse.b;
			mat.Diffuse.a = info.pLambert->diffuse.a;
			mat.Emissive.r = info.pLambert->emissive.r;
			mat.Emissive.g = info.pLambert->emissive.g;
			mat.Emissive.b = info.pLambert->emissive.b;
			mat.Emissive.a = info.pLambert->emissive.a;
		}
		if (info.pPhong != NULL) {
			mat.Specular.r = info.pPhong->specular.r;
			mat.Specular.g = info.pPhong->specular.g;
			mat.Specular.b = info.pPhong->specular.b;
			mat.Specular.a = info.pPhong->specular.a;
			mat.Power = info.pPhong->shininess;
			mat.Diffuse.a = 1.0f - info.pPhong->transparency;
		}

		// �e�N�X�`��
		it->pTexture = NULL;
		it->pResource = NULL;
		if (info.texture.length() > 0) {
			const char *filePath = NULL;
			if (PathFileExistsA(info.texture.c_str())) {
				// ���ʂ�̈ʒu�Ƀe�N�X�`����������
				filePath = info.texture.c_str();
			}
			else {
				// ���ʂ�ɂȂ������̂Ń��f���̃p�X�Ɠ����ʒu�Ō���
				filePath = PathFindFileNameA(info.texture.c_str());
				if (!PathFileExistsA(filePath)) {
					filePath = NULL;
				}
			}
			if (filePath != NULL) {
				wchar_t wPath[MAX_PATH];
				size_t wLen = 0;
				MultiByteToWideChar(0, 0, filePath, -1, wPath, MAX_PATH);

				TexMetadata mdata;
				GetMetadataFromTGAFile(wPath, mdata);
				ScratchImage image;
				LoadFromTGAFile(wPath, &mdata, image);

				HRESULT hr = CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), mdata, &it->pResource);

				if (FAILED(hr)) {
					ID3D11Resource *tex = NULL;
					CreateWICTextureFromFile(pDevice, pDeviceContext, wPath, &tex, &it->pResource);
				}
			}
		}

		++it;
	}
}

void ModelObject::DrawModel(const Transform &transform, const ModelAnimator *pAnimator)
{
	ID3D11Device *pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	// �`��
	if (pDevice != NULL) {
		//pDeviceContext->IASetInputLayout(m_format);
		//ShaderSelector::Get()->SetFormat();

		MatrixBuffer data;
		XMStoreFloat4x4(&data.world, XMMatrixTranspose(transform.world));
		XMStoreFloat4x4(&data.view, XMMatrixTranspose(transform.view));
		XMStoreFloat4x4(&data.proj, XMMatrixTranspose(transform.proj));

		for (int i = 0; i < 120; ++i) {
			XMStoreFloat4x4(&data.bone[i], XMMatrixIdentity());
		}

		// �`��Ăяo���O�ɂ��낢��Z�b�g����
		//pDeviceContext->PSSetConstantBuffers(1, 1, GetDX11Light()->GetBuffer());

		const ModelAnimator::CalcBoneList *animeBone = NULL;
		if (pAnimator != NULL) {
			animeBone = pAnimator->GetBoneList();
		}

		//data.time = static_cast<UINT>(shaderTime);

		
		MeshList::iterator meshIt = m_meshList.begin();
		while (meshIt != m_meshList.end()) {
			if (meshIt->boneList.size() > 0 && animeBone) {
				MeshInfo::BoneList::iterator boneIt = meshIt->boneList.begin();
				while (boneIt != meshIt->boneList.end()) {
					XMMATRIX inv = XMLoadFloat4x4(&boneIt->inverse);
					XMMATRIX calc = XMMatrixTranspose(inv * (*animeBone)[boneIt->index]);
					int matrixIndex = static_cast<int>(boneIt - meshIt->boneList.begin());
					XMStoreFloat4x4(&data.bone[matrixIndex], calc);
					++boneIt;
				}
			}

			data.time.x = shaderTime * 0.01f;

			XMMATRIX shadowCalc;
			XMMATRIX shadowProj = XMLoadFloat4x4(&GetDX11Light()->GetProjectionMatrix());
			shadowCalc = XMMatrixMultiply(XMLoadFloat4x4(&GetDX11Light()->GetMatrix()),shadowProj);

			// view��proj���|�������̂��킽��
			XMStoreFloat4x4(&data.shadowViewProj, XMMatrixTranspose(shadowCalc));

			pDeviceContext->UpdateSubresource(m_matrixBuffer, 0, NULL, &data, 0, 0);
			pDeviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

			// �}�e���A���̐ݒ�
			bool isSetTex = false;
			MaterialList::iterator matIt = m_materialList.begin();
			while (matIt != m_materialList.end()) {
				if (matIt->name == meshIt->materialName) {
					MaterialBuffer matBuf = {
						matIt->material.Diffuse,
						matIt->material.Ambient,
						matIt->material.Specular,
						XMVectorSet(matIt->pResource ? 1.0f : 0.0f, 0.0f, 0.0f, 0.0f),
					};
					matBuf.diffuse.r *= m_modelColor.r;
					matBuf.diffuse.g *= m_modelColor.g;
					matBuf.diffuse.b *= m_modelColor.b;
					matBuf.diffuse.a *= m_modelColor.a;
					pDeviceContext->UpdateSubresource(m_materialBuffer, 0, NULL, &matBuf, 0, 0);
					pDeviceContext->PSSetConstantBuffers(0, 1, &m_materialBuffer);
					// �e�N�X�`���̌J��Ԃ���n����
					if (m_isWrapTex) {
						pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerStateWRAP);
					}
					else {
						pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
					}
					if (matIt->pResource != NULL) {
						pDeviceContext->PSSetShaderResources(0, 1, &matIt->pResource);

						isSetTex = true;
					}
					break;
				}
				++matIt;
			}

			// �`��
			UINT strides = sizeof(VertexInfo);
			UINT offsets = 0;
			pDeviceContext->IASetVertexBuffers(0, 1, &meshIt->vertexBuffer, &strides, &offsets);
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
			//pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);


			if (meshIt->indexBuffer != NULL) {
				pDeviceContext->IASetIndexBuffer(meshIt->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
				pDeviceContext->DrawIndexed(static_cast<UINT>(meshIt->indexList.size()), 0, 0);
			}
			else {
				pDeviceContext->Draw(static_cast<UINT>(meshIt->vertexList.size()), 0);
			}

			++meshIt;
		}
	}
	m_modelColor.r = 1.0f;
	m_modelColor.g = 1.0f;
	m_modelColor.b = 1.0f;
	m_modelColor.a = 1.0f;
}

void ShaderTimeCountUp() {
	shaderTime++;
}

// EOF