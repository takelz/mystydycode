#ifndef __DX11_TEXTURE_H__
#define __DX11_TEXTURE_H__

#include <DirectX/DX11Util.h>
#include <string>
#include <map>

#define TEXTURE DX11TextureManager::Instance()


typedef ID3D11ShaderResourceView Texture;

/**
 * @brief �e�N�X�`���Ǘ�
 */
class DX11TextureManager
{
private:
	/// @brief �ǂݍ��ݏ��
	struct LoadInfo
	{
		Texture *pTexture;	///< �ǂݍ��݃f�[�^
		int refCount;		///< �Q�ƃJ�E���g
	};

private:
	typedef std::pair<std::string, LoadInfo> MapKey;
	typedef std::map<std::string, LoadInfo> LoadMap;

public:
	static DX11TextureManager* Instance();
	static void Create();
	static void Delete();
public:
	DX11TextureManager();
	~DX11TextureManager();

	void Entry(Texture *pTexture);
	Texture* Load(const char *file);
	void Release(Texture *pTexture);

private:
	static DX11TextureManager* m_pInstance;
private:
	LoadMap m_textureMap;
};

#endif // __DX11_TEXTURE_H__