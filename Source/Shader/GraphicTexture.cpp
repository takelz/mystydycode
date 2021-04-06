#include "GraphicTexture.h"



bool GraphicTexture::Load(const char* file)
{
	SAFE_RELEASE(pTexture);

	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, file, -1, wPath, MAX_PATH);
	DirectX::TexMetadata mdata;
	GetMetadataFromTGAFile(wPath, mdata);
	DirectX::ScratchImage image;
	LoadFromTGAFile(wPath, &mdata, image);

	ID3D11Device* pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	HRESULT hr = CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), mdata, &pTexture);
	if (FAILED(hr)) {
		ID3D11Resource *tex = NULL;
		hr = DirectX::CreateWICTextureFromFile(pDevice, pDeviceContext, wPath, &tex, &pTexture);
		if (FAILED(hr)) {
			return false;
		}
	}
	return true;
}


void GraphicTexture::MakeTexDesc(D3D11_TEXTURE2D_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pData, UINT width, UINT height)
{
	// テクスチャ情報
	ZeroMemory(pDesc, sizeof(D3D11_TEXTURE2D_DESC));
	pDesc->Width = width;
	pDesc->Height = height;
	pDesc->MipLevels = 1;
	pDesc->ArraySize = 1;
	pDesc->Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pDesc->SampleDesc.Count = 1;
	pDesc->Usage = D3D11_USAGE_DEFAULT;
	pDesc->BindFlags = D3D11_BIND_SHADER_RESOURCE;
	// 初期化データ
	ZeroMemory(pData, sizeof(D3D11_SUBRESOURCE_DATA));
	pData->SysMemPitch = 4 * width;
}
HRESULT GraphicTexture::CreateResource(ID3D11Texture2D *pTex, const D3D11_TEXTURE2D_DESC &desc)
{
	ID3D11Device* pDevice = GetDX11Device();

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	ZeroMemory(&shaderDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderDesc.Format = desc.Format;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MostDetailedMip = 0;
	shaderDesc.Texture2D.MipLevels = desc.MipLevels;

	return pDevice->CreateShaderResourceView(pTex, &shaderDesc, &pTexture);
}


bool GraphicTexture::CreateRenderTarget(UINT width, UINT height, const float inClearColor[4])
{
	ID3D11Device* pDevice = GetDX11Device();

	// テクスチャ情報
	D3D11_TEXTURE2D_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	MakeTexDesc(&desc, &data, width, height);
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET ;
	// テクスチャデータ
	HRESULT hr = E_FAIL;
	ID3D11Texture2D *pTex = NULL;
	hr = pDevice->CreateTexture2D(&desc, NULL, &pTex);
	// リソース生成
	if (SUCCEEDED(hr))
	{
		hr = CreateResource(pTex, desc);
	}
	// レンダーターゲット作成
	if (SUCCEEDED(hr))
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset(&rtvDesc, 0, sizeof(rtvDesc));
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = pDevice->CreateRenderTargetView(pTex, &rtvDesc, &pRenderTarget);
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = (FLOAT)width;
		viewPort.Height = (FLOAT)height;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		memcpy(clearColor, inClearColor, sizeof(inClearColor));
		GetDX11DeviceContext()->ClearRenderTargetView(pRenderTarget, clearColor);
	}
	CreateResource(pTex, desc);
	pDesc = desc;
	SAFE_RELEASE(pTex);
	return hr == S_OK;
}



bool GraphicTexture::CreateDepthStencil(UINT width, UINT height, bool enableStencil)
{
	ID3D11Device* pDevice = GetDX11Device();

	// テクスチャ情報
	D3D11_TEXTURE2D_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	MakeTexDesc(&desc, &data, width, height);
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.Format = enableStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	// テクスチャデータ
	HRESULT hr = E_FAIL;
	ID3D11Texture2D *pTex = NULL;
	hr = pDevice->CreateTexture2D(&desc, NULL, &pTex);
	// リソース作成
	if (SUCCEEDED(hr))
	{
		// DXGI_FORMAT_D24_UNORM_S8_UINT が ShaderResourceView に対応してない
		//hr = CreateResource(pTex, desc);
	}
	// 深度バッファ作成
	if (SUCCEEDED(hr))
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		dsDesc.Format = desc.Format;
		dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		hr = pDevice->CreateDepthStencilView(pTex, &dsDesc, &pDepthStencil);
		GetDX11DeviceContext()->ClearDepthStencilView(pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	SAFE_RELEASE(pTex);
	return hr == S_OK;
}
