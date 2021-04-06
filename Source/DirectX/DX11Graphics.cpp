/**
 * @file DX11Graphics.cpp
 * @brief DX11�O���t�B�b�N�X
 */
#include <DirectX/DX11Graphics.h>
#include <Debug/DebugMemory.h>

/**
 * @brief �R���X�g���N�^
 */
DX11Graphics::DX11Graphics()
	: m_pDevice(NULL)
	, m_pDeviceContext(NULL)
	, m_pSwapChain(NULL)
	, m_pRenderTargetView(NULL)
	, m_pBlendState(NULL)
	, m_width(0)
	, m_height(0)
{
}

/**
 * @brief �f�X�g���N�^
 */
DX11Graphics::~DX11Graphics()
{
	Exit();
}

/**
 * @brief DirectX Grpaphics �̏���������
 * @param[in] hWnd �E�B���h�E�n���h��
 * @param[in] Width �E�C���h�E�T�C�Y�i���j
 * @param[in] Height �E�C���h�E�T�C�Y�i�����j
 * @param[in] fullscreen �t���X�N���[���ifalse window  true fullscreen )
 * @return �����������������true
 */
HRESULT DX11Graphics::Init(HWND hWnd, UINT Width, UINT Height, bool fullscreen)
{
	HRESULT	hr = E_FAIL;

	// �f�B�X�v���C�̃��t���b�V�����[�g�擾
	IDXGIFactory* factory = NULL;				// factory
	IDXGIAdapter* adapter = NULL;				// videocard
	IDXGIOutput* adapterOutput = NULL;			// monitor
	DXGI_MODE_DESC* displayModeList = NULL;
	UINT numModes = 0;
	UINT numerator = 60;	// ���q
	UINT denominator = 1;	// ����

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr)) {
		return hr;
	}
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr)) {
		return hr;
	}
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr)) {
		return hr;
	}
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr)) {
		return hr;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr)) {
		return hr;
	}
	for (unsigned int i = 0; i < numModes; ++i) {
		if (displayModeList[i].Width == Width && displayModeList[i].Height == Height) {
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}
	SAFE_DELETE_ARRAY(displayModeList);

	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);


	// �X���b�v�`�F�C���̐ݒ�
	// �X���b�v�`�F�C���Ƃ́A�E�C���h�E�ւ̕\���_�u���o�b�t�@���Ǘ�����
	// �N���X�@�}���`�T���v�����O�A���t���b�V�����[�g���ݒ�ł���
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));					// �[���N���A
	sd.BufferDesc.Width = Width;						// �o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = Height;						// �o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B �͈͂O�D�O����P�D�O)
	sd.BufferDesc.RefreshRate.Numerator = numerator;			// ���t���b�V�����[�g�i����j
	sd.BufferDesc.RefreshRate.Denominator = denominator;			// ���t���b�V�����[�g�i���q�j
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;		// �}���`�T���v���̐�
	sd.SampleDesc.Quality = 0;		// �}���`�T���v���̃N�I���e�B
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
	sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	sd.OutputWindow = hWnd;			// �֘A�t����E�C���h�E
	sd.Windowed = fullscreen ? FALSE : TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ���[�h�̎����؂�ւ�

	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,					// Windows Advanced Rasterizer
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;
	// createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,		// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,		// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1		// Direct9.1�Ή�GPU���x��
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex) {
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,					// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�i�m�t�k�k�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			m_driverType,			// �f�o�C�X�h���C�o�̃^�C�v
			NULL,					// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			createDeviceFlags,		// �f�o�C�X�t���O
			featureLevels,			// �@�\���x��
			numFeatureLevels,		// �@�\���x����
			D3D11_SDK_VERSION,		// 
			&sd,					// �X���b�v�`�F�C���̐ݒ�
			&m_pSwapChain,			// IDXGIDwapChain�C���^�t�F�[�X	
			&m_pDevice,				// ID3D11Device�C���^�t�F�[�X
			&m_featureLevel,		// �T�|�[�g����Ă���@�\���x��
			&m_pDeviceContext);		// �f�o�C�X�R���e�L�X�g
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return hr;
	}


	// �����_�����O�^�[�Q�b�g���쐬
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		return hr;
	}
	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	// Z�o�b�t�@
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if (FAILED(hr)) {
		return hr;
	}


	// �X�e���V���X�e�[�g�쐬
	// �X�e���V���ݒ�\���̏�����
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// set up the description of the stencl state
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// stencil operation if pixel is�@back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// create the depth stencil state
	hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hr)) {
		return hr;
	}
	// �f�o�C�X�R���e�L�X�g�փZ�b�g
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);


	// depthstencilview ������
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// create the depth stencil view
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}
	// bind the render target view and depth stencil buffer to the output render pipeline
    m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );


	// setup the raster description which will determine how and what polygons will be drawn
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	// create the rasterrizer state from the description we just filled out 
	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	if (FAILED(hr)) {
		return hr;
	}
	//  set the rasterizer state
	m_pDeviceContext->RSSetState(m_pRasterState);


    // �r���[�|�[�g��ݒ�
    D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
    vp.Width = (FLOAT)Width;
    vp.Height = (FLOAT)Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports( 1, &vp );

	// �u�����h�X�e�[�g
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	float blendFactor[4] = { 1,1,1,1 };
	m_pDevice->CreateBlendState(&bd, &m_pBlendState);
	m_pDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, 0xffffffff);

	// �T���v���\
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);


	m_height = Height;
	m_width  = Width;

	return S_OK;
}

/**
 * @brief DirectX Grpaphics �̏I������
 */
void DX11Graphics::Exit()
{
	if (m_pDeviceContext) {
		m_pDeviceContext->ClearState();
	}
	if (m_pSwapChain) {
		m_pSwapChain->SetFullscreenState(false, NULL);
	}
	SAFE_RELEASE(m_pSamplerState);
	SAFE_RELEASE(m_pRasterState);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pSwapChain);
}

/**
 * @brief �`��N���A
 * @param[in] color �`��F
 */
void DX11Graphics::RenderCleanup(const float color[])
{
	// �^�[�Q�b�g�o�b�t�@�N���A
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	// Z�o�b�t�@�N���A
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/**
 * @brief Z�o�b�t�@�؂�ւ�
 * @param[in] enable �؂�ւ��t���O
 */
void DX11Graphics::EnableZBuffer(bool enable)
{
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, enable ? m_pDepthStencilView : NULL);
}


//! @name GetFunction
/// @{
ID3D11Device* DX11Graphics::GetDevice() const
{
	return m_pDevice;
}
ID3D11DeviceContext* DX11Graphics::GetDeviceContext() const
{
	return m_pDeviceContext;
}
IDXGISwapChain* DX11Graphics::GetSwapChain() const
{
	return m_pSwapChain;
}
ID3D11RenderTargetView* DX11Graphics::GetRenderTargetView() const
{
	return m_pRenderTargetView;
}
ID3D11DepthStencilView* DX11Graphics::GetDepthStencilView() const
{
	return m_pDepthStencilView;
}
/// @}