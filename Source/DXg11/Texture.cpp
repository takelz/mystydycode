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
	DataMap::iterator itr = textureMap.find(file);	// もし指定のものが見つからないときend()を流す
	if (itr != textureMap.end()) {
		// 読み込み済みデータを返す
		++itr->second.refCount;
		return itr->second.pTexture;
	}

	// 指定されたパスにファイルがあるか確認(よみこみ失敗)
	if (!PathFileExistsA(file)) {
		MessageBox(NULL, "テクスチャエラー：指定されたファイルがありません", file, MB_OK);
		return nullptr;
	}

	// 読み込み済みファイルでないとき以下の動作で読み込む
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

	// ロードした情報をdataMapに保存
	textureMap.insert(MapKey(file, { pTexture, 1 }));	
	return pTexture;
}

void TextureManager::Release(ID3D11ShaderResourceView* pTexture)
{
	// 登録済みデータを探索
	DataMap::iterator it = textureMap.begin();
	while (it != textureMap.end())
	{
		if (it->second.pTexture == pTexture)
		{
			// 見つかった
			--it->second.refCount;
			if (it->second.refCount <= 0)
			{
				// 参照がなくなった
				//delete it->second.pTexture;
				pTexture->Release();
				textureMap.erase(it);
			}
			break;
		}
		++it;
	}
}