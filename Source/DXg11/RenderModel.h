#pragma once
#include <DirectX/DX11Graphics.h>
#include <DirectX/DX11Light.h>
#include <DirectX/DrawObject.h>
#include "Transform.h"
#include <Model/ModelObject.h>
#include <unordered_map>


// モデルの形に合わせたコライダを作るため
class ModelObjEx : public ModelObject
{
private:
	DirectX::XMFLOAT3 vCenter;	// 境界ボックス/境界球中心座標
	DirectX::XMFLOAT3 vBBox;		// 境界ボックス サイズ
	float radius;				// 境界球半径

public:
	ModelObjEx();
	virtual ~ModelObjEx();

	bool Create(const char* file, bool isDynamic = false);

	const DirectX::XMFLOAT3& GetCenter(){ return vCenter; }
	const DirectX::XMFLOAT3& GetBBox(){ return vBBox; }
	float GetRadius() { return radius; }
	bool Intersect(const DirectX::XMFLOAT3* pRayPos,const DirectX::XMFLOAT3* pRayDir,float* pfDist,DirectX::XMFLOAT3* pNormal = nullptr);
};

class ModelStorage {
public:


	static ModelObjEx* ReadModel(const char *name);

private:
	ModelStorage();
	~ModelStorage();
	static std::unordered_map<std::string, ModelObjEx*> storageList;

};


class RenderModel {
public:
	RenderModel();
	~RenderModel() {};
	void Load(const char *name);
	void Draw(DirectX::Transform& trans);
	void Draw(DirectX::Transform& trans, ModelAnimator *animator);
	
	void SetColor(float r, float g, float b, float a = 1.0f) {
		D3DCOLORVALUE col;
		col.r = r; col.g = g; col.b = b; 
		col.a = a;
		model->SetColor(col);
	}

	DirectX::Vector3 GetCenter();	// モデルの中心
	DirectX::Vector3 GetBBox();		// モデルを箱とした大きさ
	float GetRadius();		// モデルの半径
	void SetTextureWrap(bool on) { model->m_isWrapTex = on; };
private:
	ModelObjEx *model;
	DirectX::XMFLOAT4X4 mtxWorld;
};

