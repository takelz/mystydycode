#ifndef __DX11_TEXTURE_H__
#define __DX11_TEXTURE_H__

#include <DirectX/DX11Util.h>
#include <string>
#include <map>

#define TEXTURE DX11TextureManager::Instance()


typedef ID3D11ShaderResourceView Texture;

/**
 * @brief テクスチャ管理
 */
class DX11TextureManager
{
private:
	/// @brief 読み込み情報
	struct LoadInfo
	{
		Texture *pTexture;	///< 読み込みデータ
		int refCount;		///< 参照カウント
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