#include "ShaderSelector.h"


void ShaderSelector::Setup() {
	ShaderData *input;
	// SHADER_MODEL
	input = new ShaderData();
	CreateVertexShader("Assets/Shader/model_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/model_ps.cso", &input->pPixelShader);
	dataList[SHADER_MODEL] = input;
	
	// SHADER_BLUE
	input = new ShaderData();
	CreateVertexShader("Assets/Shader/blue_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/blue_ps.cso", &input->pPixelShader);
	dataList[SHADER_BLUE] = input;


	input = new ShaderData();
	CreateVertexShader("Assets/Shader/crystal_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/crystal_ps.cso", &input->pPixelShader);
	dataList[SHADER_CRYSTAL] = input;


	input = new ShaderData();
	CreateVertexShader("Assets/Shader/barrier_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/barrier_ps.cso", &input->pPixelShader);
	dataList[SHADER_BARRIER] = input;


	input = new ShaderData();
	CreateVertexShader("Assets/Shader/shadow_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/shadow_ps.cso", &input->pPixelShader);
	dataList[SHADER_SHADOW] = input;


	input = new ShaderData();
	CreateVertexShader("Assets/Shader/shadow_depthwrite_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/shadow_depthwrite_ps.cso", &input->pPixelShader);
	dataList[SHADER_DEPTH_WRITE] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/depth_buffer_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/depth_buffer_ps.cso", &input->pPixelShader);
	dataList[SHADER_DEPTH_BUFFER] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/normal_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/normal_ps.cso", &input->pPixelShader);
	dataList[SHADER_NORMAL] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/damage_emission_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/damage_emission_ps.cso", &input->pPixelShader);
	dataList[SHADER_DAMAGE_EMISSION] = input;


	input = new ShaderData();
	CreateVertexShader("Assets/Shader/coin_vs.cso", InputLayoutKind::SKIN, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/coin_ps.cso", &input->pPixelShader);
	dataList[SHADER_COIN] = input;


	currentData = dataList[SHADER_MODEL];

}


ShaderSelector::ShaderSelector(){
	currentData = nullptr;

};

// シェーダータイプがモデルかシャドウでない場合
void ShaderSelector::SelectModelShader(SHADER shader) {
	if (!(currentShaderName == SHADER_SHADOW || currentShaderName == SHADER_MODEL)) return;
	if (currentShaderName == shader) return;
	
	currentData = dataList[shader];
	currentShaderName = shader;


}


void ShaderSelector::SelectModelShaderRender(SHADER shader) {
	if (currentShaderName == shader) return;

	currentData = dataList[shader];
	currentShaderName = shader;


}



void ShaderSelector::SetFormat() {

	GetDX11DeviceContext()->IASetInputLayout(currentData->format);
	// もし基本のシェーダーなら何も操作しないように
	GetDX11DeviceContext()->PSSetConstantBuffers(1, 1, GetDX11Light()->GetBuffer());
}

void ShaderSelector::SetShader() {

	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	pDeviceContext->VSSetShader(currentData->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(currentData->pPixelShader, NULL, 0);


}



////////////////////////////////////////////Tex


void TextureShaderSelector::Setup() {
	ShaderData *input;
	// SHADER_TEX_NORMAL
	input = new ShaderData();
	CreateVertexShader("Assets/Shader/polygon_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/polygon_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_DEFAULT] = input;


	input = new ShaderData();
	CreateVertexShader("Assets/Shader/post_blur_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/post_blur_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_BLUR] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/post_getbright_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/post_getbright_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_GETBRIGHT] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/post_bloom_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/post_bloom_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_BLOOM] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/post_fog_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/post_fog_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_FOG] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/block_noise_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/block_noise_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_BLOCKNOISE] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/post_distortion_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/post_distortion_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_DISTORTION] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/perlin_noise_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/perlin_noise_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_PERLIN_NOISE] = input;

	input = new ShaderData();
	CreateVertexShader("Assets/Shader/post_glass_filter_vs.cso", InputLayoutKind::VTX_POLYGON, &input->pVertexShader, &input->format);
	CreatePixelShader("Assets/Shader/post_glass_filter_ps.cso", &input->pPixelShader);
	dataList[SHADER_TEX_GLASS_FILTER] = input;

	currentData = dataList[SHADER_TEX_DEFAULT];

}


TextureShaderSelector::TextureShaderSelector() {
	currentData = nullptr;

};


void TextureShaderSelector::SelectShader(SHADER shader) {
	if (currentShaderName == shader) return;

	currentData = dataList[shader];
	currentShaderName = shader;


}





void TextureShaderSelector::SetFormat() {

	GetDX11DeviceContext()->IASetInputLayout(currentData->format);

}

void TextureShaderSelector::SetShader() {

	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	pDeviceContext->VSSetShader(currentData->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(currentData->pPixelShader, NULL, 0);

	SelectShader(SHADER_TEX_DEFAULT);

}

