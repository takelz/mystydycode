#include "Texture.h"
#include <DirectX/DirectXTex/DirectXTex.h>
#include <DirectX/DirectXTex/WICTextureLoader.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


TextureManager* TextureManager::pInstance = NULL;

TextureManager* TextureManager::Get(){
	if (pInstance == nullptr) Create();
	return pInstance;
}


void TextureManager::Create() {
	if (pInstance == nullptr) {
		pInstance = new TextureManager();
	}
}

void TextureManager::Delete()
{
	//SAFE_DELETE(m_pInstance);
}

ID3D11ShaderResourceView* TextureManager::Load(const char *file) {
	DataMap::iterator itr = textureMap.find(file);	// �����w��̂��̂�������Ȃ��Ƃ�end()�𗬂�
	if (itr != textureMap.end()) {
		// �ǂݍ��ݍς݃f�[�^��Ԃ�
		++itr->second.refCount;
		return itr->second.pTexture;
	}

	// �w�肳�ꂽ�p�X�Ƀt�@�C�������邩�m�F(��݂��ݎ��s)
	if (!PathFileExistsA(file)) {
		MessageBox(NULL, "�e�N�X�`���G���[�F�w�肳�ꂽ�t�@�C��������܂���", file, MB_OK);
		return nullptr;
	}

	// �ǂݍ��ݍς݃t�@�C���łȂ��Ƃ��ȉ��̓���œǂݍ���
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, file, -1, wPath, MAX_PATH);
	DirectX::TexMetadata mdata;
	GetMetadataFromTGAFile(wPath, mdata);
	DirectX::ScratchImage image;
	LoadFromTGAFile(wPath, &mdata, image);

	ID3D11Device* pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();
	ID3D11ShaderResourceView *pTexture;

	HRESULT hr = CreateShaderResourceView(
		pDevice, image.GetImages(), image.GetImageCount(), mdata, &pTexture);
	if (FAILED(hr)) {
		ID3D11Resource *tex = NULL;
		hr = DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wPath, &tex, &pTexture);
		if (FAILED(hr)) {
			return NULL;
		}
	}

	// ���[�h��������dataMap�ɕۑ�
	textureMap.insert(MapKey(file, { pTexture, 1 }));	
	return pTexture;
}

void TextureManager::Release(ID3D11ShaderResourceView* pTexture)
{
	// �o�^�ς݃f�[�^��T��
	DataMap::iterator it = textureMap.begin();
	while (it != textureMap.end())
	{
		if (it->second.pTexture == pTexture)
		{
			// ��������
			--it->second.refCount;
			if (it->second.refCount <= 0)
			{
				// �Q�Ƃ��Ȃ��Ȃ���
				//delete it->second.pTexture;
				pTexture->Release();
				textureMap.erase(it);
			}
			break;
		}
		++it;
	}
}