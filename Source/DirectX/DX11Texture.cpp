#include <DirectX/DX11Texture.h>
#include <DirectX/DirectXTex/DirectXTex.h>
#include <DirectX/DirectXTex/WICTextureLoader.h>
#include <Debug/DebugUtility.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

DX11TextureManager* DX11TextureManager::m_pInstance = NULL;

DX11TextureManager* DX11TextureManager::Instance()
{
	return m_pInstance;
}
void DX11TextureManager::Create()
{
	if (m_pInstance == NULL) {
		m_pInstance = NEW DX11TextureManager;
	}
}
void DX11TextureManager::Delete()
{
	SAFE_DELETE(m_pInstance);
}

DX11TextureManager::DX11TextureManager()
{
}
DX11TextureManager::~DX11TextureManager()
{
}

void DX11TextureManager::Entry(Texture *pTexture)
{
	// �A�h���X�𕶎���ɕϊ�
	char buf[20];
	sprintf_s(buf, sizeof(buf), "0x%IX", reinterpret_cast<__int64>(pTexture));

	// ����L�[���Ȃ���Γo�^
	if (m_textureMap.find(buf) == m_textureMap.end()) {
		m_textureMap.insert(MapKey(buf, { pTexture, 1 }));
	}
}
Texture* DX11TextureManager::Load(const char *file)
{
	// �ǂݍ��ݍς݃e�N�X�`�����`�F�b�N
	LoadMap::iterator it = m_textureMap.find(file);
	if (it != m_textureMap.end())
	{
		// �ǂݍ��ݍς݃f�[�^��Ԃ�
		++it->second.refCount;
		return it->second.pTexture;
	}

	// �w�肳�ꂽ�p�X�Ƀt�@�C�������邩�m�F
	if (!PathFileExistsA(file)) {
		return NULL;
	}

	// �ǂݍ���
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, file, -1, wPath, MAX_PATH);
	DirectX::TexMetadata mdata;
	GetMetadataFromTGAFile(wPath, mdata);
	DirectX::ScratchImage image;
	LoadFromTGAFile(wPath, &mdata, image);

	ID3D11Device* pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();
	Texture *pTexture;

	HRESULT hr = CreateShaderResourceView(
		pDevice, image.GetImages(), image.GetImageCount(), mdata, &pTexture);
	if (FAILED(hr)) {
		ID3D11Resource *tex = NULL;
		hr = DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wPath, &tex, &pTexture);
		if (FAILED(hr)) {
			return NULL;
		}
	}

	m_textureMap.insert(MapKey(file, { pTexture, 1 }));
	return pTexture;
}

void DX11TextureManager::Release(Texture* pTexture)
{
	// �o�^�ς݃f�[�^��T��
	LoadMap::iterator it = m_textureMap.begin();
	while (it != m_textureMap.end())
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
				m_textureMap.erase(it);
			}
			break;
		}
		++it;
	}
}

// EOF