#pragma once

#include <DirectX/DX11Util.h>
#include <string>
#include <map>

class TextureManager{
private:
	// @brief �ǂݍ��ݏ��
	struct LoadInfo{
		ID3D11ShaderResourceView *pTexture;	//< �ǂݍ��݃f�[�^
		int refCount;						//< �Q�ƃJ�E���g
	};

private:
	typedef std::pair<std::string, LoadInfo> MapKey;
	typedef std::map<std::string, LoadInfo> DataMap;

public:
	static TextureManager* Get();
	static void Create();
	static void Delete();
public:
	TextureManager() {};
	~TextureManager() {};

	void Entry(ID3D11ShaderResourceView *pTexture);
	ID3D11ShaderResourceView* Load(const char *file);
	void Release(ID3D11ShaderResourceView *pTexture);

private:
	static TextureManager* pInstance;
private:
	DataMap textureMap;
};