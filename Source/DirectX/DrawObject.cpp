/**
 * @file DrawObject.cpp
 * @brief デバッグ描画
 * @author ryota.ssk
 */
#include "DrawObject.h"
#include "DX11Util.h"
#include <vector>
#include <Shader/Shader.h>

#if ENABLE_DRAW_OBJ

namespace DirectX {
namespace debug {
	
//----- 定数
const UINT MAX_LINE_NUM = 10000;
const UINT CIRCLE_DETAIL = 4;
const UINT CIRCLE_SPLIT_NUM = 4 * CIRCLE_DETAIL;


//----- 構造体
// 定数バッファ
struct ConstantBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 viewProj;
	XMFLOAT4 color;
};
// 頂点情報
struct DrawObjectVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};
// ライン情報
struct DrawLineInfo
{
	XMFLOAT3 start;
	XMFLOAT3 end;
	XMFLOAT4 color;
};
typedef std::vector<DrawLineInfo> DrawLineList;
// 図形情報
struct DrawShapeInfo
{
	XMMATRIX mat;
	XMFLOAT4 color;
};
typedef std::vector<DrawShapeInfo> DrawShapeList;
// 描画図形種別
enum DrawShapeType
{
	DST_WIRE_BOX,
	DST_BOX,
	DST_WIRE_CIRCLE,
	DST_CIRCLE,
	DST_WIRE_SPHERE,
	DST_SPHERE,
	DST_MAX
};
// 描画情報
struct DrawBufferInfo
{
	ID3D11Buffer **ppVtxBuf;
	ID3D11Buffer **ppIdxBuf;
	int drawNum;
};

//----- グローバル変数
ID3D11InputLayout *g_pDrawObjectFormat = NULL;
ID3D11VertexShader *g_pDrawObjectVS = NULL;
ID3D11PixelShader *g_pDrawObjectPS = NULL;
ID3D11Buffer *g_pConstantBuffer = NULL;
ID3D11Buffer *g_pLineVertexBuffer = NULL;
ID3D11Buffer *g_pBoxVertexBuffer = NULL;
ID3D11Buffer *g_pWireBoxIndexBuffer = NULL;
ID3D11Buffer *g_pBoxIndexBuffer = NULL;
ID3D11Buffer *g_pSphereVertexBuffer = NULL;
ID3D11Buffer *g_pWireCircleIndexBuffer = NULL;
ID3D11Buffer *g_pCircleIndexBuffer = NULL;
ID3D11Buffer *g_pWireSphereIndexBuffer = NULL;
ID3D11Buffer *g_pSphereIndexBuffer = NULL;
DrawLineList g_drawLineList;
DrawShapeList g_drawShapeList[DST_MAX];
DrawBufferInfo g_drawBufferList[DST_MAX] = {
	{ &g_pBoxVertexBuffer, &g_pWireBoxIndexBuffer, 0 },
	{ &g_pBoxVertexBuffer, &g_pBoxIndexBuffer, 0 },
	{ &g_pSphereVertexBuffer, &g_pWireCircleIndexBuffer, 0 },
	{ &g_pSphereVertexBuffer, &g_pCircleIndexBuffer, 0 },
	{ &g_pSphereVertexBuffer, &g_pWireSphereIndexBuffer, 0 },
	{ &g_pSphereVertexBuffer, &g_pSphereIndexBuffer, 0 },
};


/**
 * @brief 初期化
 * @return 処理結果
 */
bool InitializeDrawObject()
{
	// 初期化チェック
	if (g_pDrawObjectFormat != NULL) { return false; }
	if (g_pDrawObjectVS != NULL) { return false; }
	if (g_pDrawObjectPS != NULL) { return false; }
	if (g_pConstantBuffer != NULL) { return false; }

	// シェーダー作成
	if (FAILED(CreateVertexShader("Assets/Shader/object_vs.cso", InputLayoutKind::VTX_COL, &g_pDrawObjectVS, &g_pDrawObjectFormat))) {
		return false;
	}
	if (FAILED(CreatePixelShader("Assets/Shader/object_ps.cso", &g_pDrawObjectPS))) {
		return false;
	}

	ID3D11Device *pDevice = GetDX11Device();

	// 定数バッファ
	D3D11_BUFFER_DESC transformBufDesc;
	transformBufDesc.ByteWidth = sizeof(ConstantBuffer);
	transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
	transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufDesc.CPUAccessFlags = 0;
	transformBufDesc.MiscFlags = 0;
	transformBufDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&transformBufDesc, NULL, &g_pConstantBuffer);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC vtxBufferDesc;
	vtxBufferDesc.ByteWidth = 0;
	vtxBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vtxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vtxBufferDesc.MiscFlags = 0;
	vtxBufferDesc.StructureByteStride = 0;

	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.ByteWidth = 0;
	idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags = 0;
	idxBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = NULL;
	initialData.SysMemPitch = 0;
	initialData.SysMemSlicePitch = 0;
	// ライン
	vtxBufferDesc.ByteWidth = sizeof(DrawObjectVertex) * MAX_LINE_NUM * 2;
	if (FAILED(pDevice->CreateBuffer(&vtxBufferDesc, NULL, &g_pLineVertexBuffer))) {
		return false;
	}
	//--- ボックス
	// 頂点
	DrawObjectVertex boxVtx[] = {
		{{-0.5f,-0.5f,-0.5f}, {1,1,1,1}},
		{{-0.5f,-0.5f, 0.5f}, {1,1,1,1}},
		{{-0.5f, 0.5f, 0.5f}, {1,1,1,1}},
		{{-0.5f, 0.5f,-0.5f}, {1,1,1,1}},
		{{ 0.5f,-0.5f,-0.5f}, {1,1,1,1}},
		{{ 0.5f,-0.5f, 0.5f}, {1,1,1,1}},
		{{ 0.5f, 0.5f, 0.5f}, {1,1,1,1}},
		{{ 0.5f, 0.5f,-0.5f}, {1,1,1,1}},
	};
	vtxBufferDesc.ByteWidth = sizeof(boxVtx);
	vtxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vtxBufferDesc.CPUAccessFlags = 0;
	initialData.pSysMem = boxVtx;
	if (FAILED(pDevice->CreateBuffer(&vtxBufferDesc, &initialData, &g_pBoxVertexBuffer))) {
		return false;
	}
	// ワイヤー
	WORD wireBoxIdx[] = {
		0,1, 1,2, 2,3, 3,0,
		4,5, 5,6, 6,7, 7,4,
		0,4, 1,5, 2,6, 3,7,
	};
	g_drawBufferList[DST_WIRE_BOX].drawNum = _countof(wireBoxIdx);
	idxBufferDesc.ByteWidth = sizeof(wireBoxIdx);
	initialData.pSysMem = wireBoxIdx;
	if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &initialData, &g_pWireBoxIndexBuffer))) {
		return false;
	}
	// ソリッド
	WORD boxIdx[] = {
		0,1,2, 0,2,3,
		6,5,4, 7,6,4,
		2,1,5, 2,5,6,
		4,0,3, 4,3,7,
		7,3,2, 7,2,6,
		1,0,4, 1,4,5,
	};
	g_drawBufferList[DST_BOX].drawNum = _countof(boxIdx);
	idxBufferDesc.ByteWidth = sizeof(boxIdx);
	initialData.pSysMem = boxIdx;
	if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &initialData, &g_pBoxIndexBuffer))) {
		return false;
	}
	//--- 球
	// 頂点
	const int SPHERE_Y_SPLIT = CIRCLE_DETAIL * 2 - 1;
	const int SPHERE_VTX_MAX = CIRCLE_SPLIT_NUM * SPHERE_Y_SPLIT + 2;
	DrawObjectVertex sphereVtx[SPHERE_VTX_MAX];
	int sphereVtxIdx = 1;
	float xzDeg = 360.f / CIRCLE_SPLIT_NUM;
	for (int yi = 0; yi < SPHERE_Y_SPLIT; ++yi) {
		float yRad = XMConvertToRadians(180.f / (SPHERE_Y_SPLIT + 1) * (yi + 1));
		for (int xzi = 0; xzi < CIRCLE_SPLIT_NUM; ++xzi) {
			float xzRad = XMConvertToRadians(xzDeg * xzi);
			sphereVtx[sphereVtxIdx].pos = XMFLOAT3(
				sinf(yRad) * cosf(xzRad),
				cosf(yRad),
				sinf(yRad) * sinf(xzRad)
			);
			sphereVtx[sphereVtxIdx].color = DirectX::XMFLOAT4(1, 1, 1, 1);
			++sphereVtxIdx;
		}
	}
	sphereVtx[0].pos = XMFLOAT3(0, 1, 0);
	sphereVtx[SPHERE_VTX_MAX - 1].pos = XMFLOAT3(0, -1, 0);
	sphereVtx[0].color = sphereVtx[SPHERE_VTX_MAX - 1].color = DirectX::XMFLOAT4(1, 1, 1, 1);
	vtxBufferDesc.ByteWidth = sizeof(sphereVtx);
	initialData.pSysMem = sphereVtx;
	if (FAILED(pDevice->CreateBuffer(&vtxBufferDesc, &initialData, &g_pSphereVertexBuffer))) {
		return false;
	}
	// 円ワイヤー
	const UINT circleStartIdx = CIRCLE_SPLIT_NUM * (CIRCLE_DETAIL - 1) + 1;
	WORD wireCircleIdx[CIRCLE_SPLIT_NUM + 1];
	for (int i = 0; i < CIRCLE_SPLIT_NUM; ++i) {
		wireCircleIdx[i] = circleStartIdx + i;
	}
	wireCircleIdx[CIRCLE_SPLIT_NUM] = circleStartIdx;
	g_drawBufferList[DST_WIRE_CIRCLE].drawNum = _countof(wireCircleIdx);
	idxBufferDesc.ByteWidth = sizeof(wireCircleIdx);
	initialData.pSysMem = wireCircleIdx;
	if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &initialData, &g_pWireCircleIndexBuffer))) {
		return false;
	}
	// 円ソリッド
	const UINT circlePolyNum = CIRCLE_SPLIT_NUM - 2;
	WORD circleIdx[circlePolyNum * 3];
	for (int i = 0; i < circlePolyNum; ++i) {
		int idx = i * 3;
		circleIdx[idx + 0] = wireCircleIdx[0];
		circleIdx[idx + 1] = wireCircleIdx[i + 2];
		circleIdx[idx + 2] = wireCircleIdx[i + 1];
	}
	g_drawBufferList[DST_CIRCLE].drawNum = _countof(circleIdx);
	idxBufferDesc.ByteWidth = sizeof(circleIdx);
	initialData.pSysMem = circleIdx;
	if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &initialData, &g_pCircleIndexBuffer))) {
		return false;
	}
	// 球ワイヤー
	WORD wireSphereIdx[3][CIRCLE_SPLIT_NUM * 2];
	for (int i = 0; i < CIRCLE_SPLIT_NUM - 1; ++i) {
		wireSphereIdx[0][i * 2 + 0] = circleStartIdx + i;
		wireSphereIdx[0][i * 2 + 1] = circleStartIdx + i + 1;
	}
	wireSphereIdx[0][CIRCLE_SPLIT_NUM * 2 - 2] = circleStartIdx + CIRCLE_SPLIT_NUM - 1;
	wireSphereIdx[0][CIRCLE_SPLIT_NUM * 2 - 1] = circleStartIdx;
	wireSphereIdx[1][0] = 0;
	wireSphereIdx[1][1] = 1;
	wireSphereIdx[1][2] = 0;
	wireSphereIdx[1][3] = CIRCLE_SPLIT_NUM / 2 + 1;
	for (int i = 0; i < CIRCLE_DETAIL * 2 - 2; ++i) {
		int idx = (i + 1) * 4;
		wireSphereIdx[1][idx + 0] = wireSphereIdx[1][idx - 3];
		wireSphereIdx[1][idx + 1] = wireSphereIdx[1][idx - 3] + CIRCLE_SPLIT_NUM;
		wireSphereIdx[1][idx + 2] = wireSphereIdx[1][idx - 1];
		wireSphereIdx[1][idx + 3] = wireSphereIdx[1][idx - 1] + CIRCLE_SPLIT_NUM;
	}
	wireSphereIdx[1][CIRCLE_SPLIT_NUM * 2 - 4] = wireSphereIdx[1][CIRCLE_SPLIT_NUM * 2 - 7];
	wireSphereIdx[1][CIRCLE_SPLIT_NUM * 2 - 3] = SPHERE_VTX_MAX - 1;
	wireSphereIdx[1][CIRCLE_SPLIT_NUM * 2 - 2] = wireSphereIdx[1][CIRCLE_SPLIT_NUM * 2 - 5];
	wireSphereIdx[1][CIRCLE_SPLIT_NUM * 2 - 1] = SPHERE_VTX_MAX - 1;
	wireSphereIdx[2][0] = 0;
	wireSphereIdx[2][1] = CIRCLE_DETAIL + 1;
	wireSphereIdx[2][2] = 0;
	wireSphereIdx[2][3] = CIRCLE_SPLIT_NUM / 2 + wireSphereIdx[2][1];
	for (int i = 0; i < CIRCLE_DETAIL * 2 - 2; ++i) {
		int idx = (i + 1) * 4;
		wireSphereIdx[2][idx + 0] = wireSphereIdx[2][idx - 3];
		wireSphereIdx[2][idx + 1] = wireSphereIdx[2][idx - 3] + CIRCLE_SPLIT_NUM;
		wireSphereIdx[2][idx + 2] = wireSphereIdx[2][idx - 1];
		wireSphereIdx[2][idx + 3] = wireSphereIdx[2][idx - 1] + CIRCLE_SPLIT_NUM;
	}
	wireSphereIdx[2][CIRCLE_SPLIT_NUM * 2 - 4] = wireSphereIdx[2][CIRCLE_SPLIT_NUM * 2 - 7];
	wireSphereIdx[2][CIRCLE_SPLIT_NUM * 2 - 3] = SPHERE_VTX_MAX - 1;
	wireSphereIdx[2][CIRCLE_SPLIT_NUM * 2 - 2] = wireSphereIdx[2][CIRCLE_SPLIT_NUM * 2 - 5];
	wireSphereIdx[2][CIRCLE_SPLIT_NUM * 2 - 1] = SPHERE_VTX_MAX - 1;
	g_drawBufferList[DST_WIRE_SPHERE].drawNum = sizeof(wireSphereIdx) / sizeof(WORD);
	idxBufferDesc.ByteWidth = sizeof(wireSphereIdx);
	initialData.pSysMem = wireSphereIdx;
	if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &initialData, &g_pWireSphereIndexBuffer))) {
		return false;
	}
	// 球ソリッド
	const UINT SPHERE_POLYGON_MAX = CIRCLE_SPLIT_NUM * 2 + CIRCLE_SPLIT_NUM * 2 * (CIRCLE_DETAIL - 1) * 2;
	const UINT SPHERE_IDX_MAX = SPHERE_POLYGON_MAX * 3;
	WORD sphereIdx[SPHERE_IDX_MAX];
	for (int i = 0; i < CIRCLE_SPLIT_NUM; ++i) {
		int idx = i * 3;
		sphereIdx[idx + 0] = 0;
		sphereIdx[idx + 1] = ((i + 1) % CIRCLE_SPLIT_NUM) + 1;
		sphereIdx[idx + 2] = i + 1;
	}
	for (int j = 0; j < (CIRCLE_DETAIL - 1) * 2; ++j) {
		int startIdx = j * CIRCLE_SPLIT_NUM + 1;
		for (int i = 0; i < CIRCLE_SPLIT_NUM; ++i) {
			int idx = i * 6 + CIRCLE_SPLIT_NUM * 3 + (CIRCLE_SPLIT_NUM * 6) * j;
			sphereIdx[idx + 0] = startIdx + i;
			sphereIdx[idx + 1] = startIdx + CIRCLE_SPLIT_NUM + ((i + 1) % CIRCLE_SPLIT_NUM);
			sphereIdx[idx + 2] = startIdx + CIRCLE_SPLIT_NUM + i;
			sphereIdx[idx + 3] = startIdx + i;
			sphereIdx[idx + 4] = startIdx + ((i + 1) % CIRCLE_SPLIT_NUM);
			sphereIdx[idx + 5] = startIdx + CIRCLE_SPLIT_NUM + ((i + 1) % CIRCLE_SPLIT_NUM);
		}
	}
	for (int i = 0; i < CIRCLE_SPLIT_NUM; ++i) {
		int idx = SPHERE_IDX_MAX - (i + 1) * 3;
		sphereIdx[idx + 0] = SPHERE_VTX_MAX - 1;
		sphereIdx[idx + 1] = SPHERE_VTX_MAX - (((i + 1) % CIRCLE_SPLIT_NUM) + 2);
		sphereIdx[idx + 2] = SPHERE_VTX_MAX - (i + 2);
	}
	g_drawBufferList[DST_SPHERE].drawNum = SPHERE_IDX_MAX;
	idxBufferDesc.ByteWidth = sizeof(sphereIdx);
	initialData.pSysMem = sphereIdx;
	if (FAILED(pDevice->CreateBuffer(&idxBufferDesc, &initialData, &g_pSphereIndexBuffer))) {
		return false;
	}

	return true;
}

/**
 * @brief 終了処理
 */
void TerminateDrawObject()
{
	SAFE_RELEASE(g_pSphereIndexBuffer);
	SAFE_RELEASE(g_pWireSphereIndexBuffer);
	SAFE_RELEASE(g_pCircleIndexBuffer);
	SAFE_RELEASE(g_pWireCircleIndexBuffer);
	SAFE_RELEASE(g_pSphereVertexBuffer);
	SAFE_RELEASE(g_pBoxIndexBuffer);
	SAFE_RELEASE(g_pWireBoxIndexBuffer);
	SAFE_RELEASE(g_pBoxVertexBuffer);
	SAFE_RELEASE(g_pLineVertexBuffer);
	SAFE_RELEASE(g_pConstantBuffer);
	SAFE_RELEASE(g_pDrawObjectPS);
	SAFE_RELEASE(g_pDrawObjectVS);
	SAFE_RELEASE(g_pDrawObjectFormat);
}

/**
 * @brief 描画処理
 * @param[in] viewproj 変換行列
 */
void FlushDrawObject(const XMFLOAT4X4 &viewproj)
{
	// 初期設定
	ID3D11Device *pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();
	UINT stride = sizeof(DrawObjectVertex);
	UINT offset = 0;
	pDeviceContext->IASetInputLayout(g_pDrawObjectFormat);
	pDeviceContext->VSSetShader(g_pDrawObjectVS, NULL, 0);
	pDeviceContext->PSSetShader(g_pDrawObjectPS, NULL, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// 変換行列
	ConstantBuffer constantBuf;
	constantBuf.color = XMFLOAT4(1, 1, 1, 1);
	constantBuf.world = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	XMStoreFloat4x4(&constantBuf.viewProj, XMMatrixTranspose(XMLoadFloat4x4(&viewproj)));

	// ライン
	if (!g_drawLineList.empty()) {
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &constantBuf, 0, 0);
		DrawObjectVertex lineData[MAX_LINE_NUM * 2];
		for (int i = 0; i < g_drawLineList.size() && i < MAX_LINE_NUM; ++i) {
			lineData[i * 2 + 0].pos = g_drawLineList[i].start;
			lineData[i * 2 + 1].pos = g_drawLineList[i].end;
			lineData[i * 2 + 0].color = g_drawLineList[i].color;
			lineData[i * 2 + 1].color = g_drawLineList[i].color;
		}
		D3D11_MAPPED_SUBRESOURCE pData;
		pDeviceContext->Map(g_pLineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, reinterpret_cast<void*>(lineData), sizeof(lineData));
		pDeviceContext->Unmap(g_pLineVertexBuffer, 0);
		pDeviceContext->IASetVertexBuffers(0, 1, &g_pLineVertexBuffer, &stride, &offset);
		pDeviceContext->Draw(static_cast<UINT>(g_drawLineList.size()) * 2, 0);

		g_drawLineList.clear();
	}
	// 各種図形
	for (int i = 0; i < DST_MAX; ++i) {
		// ワイヤー設定
		switch (i) {
		case DST_WIRE_BOX:
		case DST_WIRE_SPHERE:
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case DST_WIRE_CIRCLE:
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			break;
		case DST_BOX:
		case DST_CIRCLE:
		case DST_SPHERE:
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		}
		if (g_drawBufferList[i].ppIdxBuf != NULL) {
			// 描画設定
			pDeviceContext->IASetVertexBuffers(0, 1, g_drawBufferList[i].ppVtxBuf, &stride, &offset);
			pDeviceContext->IASetIndexBuffer(*g_drawBufferList[i].ppIdxBuf, DXGI_FORMAT_R16_UINT, 0);

			DrawShapeList::iterator it = g_drawShapeList[i].begin();
			while (it != g_drawShapeList[i].end()) {
				// パラメータ更新
				XMMATRIX mat = it->mat;
				XMStoreFloat4x4(&constantBuf.world, XMMatrixTranspose(mat));
				constantBuf.color = it->color;
				pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &constantBuf, 0, 0);

				// 描画
				pDeviceContext->DrawIndexed(g_drawBufferList[i].drawNum, 0, 0);

				++it;
			}
		}
		g_drawShapeList[i].clear();
	}
}

/**
 * @brief ライン描画情報セット
 * @param[in] start 始点
 * @param[in] end 終点
 * @param[in] color 色
 */
void DrawLine(const XMFLOAT3& start, const XMFLOAT3& end, const XMFLOAT4& color)
{
	g_drawLineList.push_back({ start, end, color });
}
void DrawShape(const DrawShapeArg &arg, const XMFLOAT4 &color, bool wire)
{
	DrawShapeType type = DST_MAX;
	XMMATRIX mat;
	XMMATRIX localMat;

	// 種類
	switch (arg.shape) {
	case DrawShapeArg::ST_BOX:
		type = wire ? DST_WIRE_BOX : DST_BOX;
		break;
	case DrawShapeArg::ST_CIRCLE:
		type = wire ? DST_WIRE_CIRCLE : DST_CIRCLE;
		break;
	case DrawShapeArg::ST_SPHERE:
		type = wire ? DST_WIRE_SPHERE : DST_SPHERE;
		break;
	}
	// 姿勢
	switch (arg.pose) {
	case DrawShapeArg::PT_FIX:
		mat = XMMatrixSet(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			arg.uPose.pos.x, arg.uPose.pos.y, arg.uPose.pos.z, 1);
		break;
	case DrawShapeArg::PT_ROTATION:
		if (arg.shape == DrawShapeArg::ST_SPHERE &&
			arg.center == DrawShapeArg::CT_ORIGIN)
		{
			// 球体は回転しても同じ見た目なので
			XMFLOAT4X4 transMat;
			XMStoreFloat4x4(&transMat, arg.uPose.mat);
			mat = XMMatrixSet(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				transMat._41, transMat._42, transMat._43, 1);
		}
		else {
			mat = arg.uPose.mat;
		}
		break;
	}
	// サイズ
	switch (arg.center) {
	case DrawShapeArg::CT_ORIGIN:
		if (arg.shape == DrawShapeArg::ST_BOX) {
			localMat = XMMatrixSet(
				arg.uSize.vec.x, 0, 0, 0,
				0, arg.uSize.vec.y, 0, 0,
				0, 0, arg.uSize.vec.z, 0,
				0, 0, 0, 1
			);
		}
		else {
			localMat = XMMatrixSet(
				arg.uSize.radius, 0, 0, 0,
				0, arg.uSize.radius, 0, 0,
				0, 0, arg.uSize.radius, 0,
				0, 0, 0, 1
			);			
		}
		break;
	case DrawShapeArg::CT_SHIFT:
		{
			XMFLOAT3 size(
				arg.uSize.shift.max.x - arg.uSize.shift.min.x,
				arg.uSize.shift.max.y - arg.uSize.shift.min.y,
				arg.uSize.shift.max.z - arg.uSize.shift.min.z
			);
			XMFLOAT3 center(
				(arg.uSize.shift.max.x + arg.uSize.shift.min.x) * 0.5f,
				(arg.uSize.shift.max.y + arg.uSize.shift.min.y) * 0.5f,
				(arg.uSize.shift.max.z + arg.uSize.shift.min.z) * 0.5f
			);
			if (arg.shape == DrawShapeArg::ST_BOX) {
				localMat = XMMatrixSet(
					size.x, 0, 0, 0,
					0, size.y, 0, 0,
					0, 0, size.z, 0,
					center.x, center.y, center.z, 1
				);
			}
			else {
				float radius = max(size.x, max(size.y, size.z)) * 0.5f;
				localMat = XMMatrixSet(
					radius, 0, 0, 0,
					0, radius, 0, 0,
					0, 0, radius, 0,
					center.x, center.y, center.z, 1
				);
			}
		}
		break;
	}

	g_drawShapeList[type].push_back({ localMat * mat, color });
}

} // debug
} // DirectX

#endif // ENABLE_DRAW_OBJ
// EOF