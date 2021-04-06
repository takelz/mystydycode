#pragma once

// 各レンダーに対しての振り分けはGetCurrentRenderPathで数値を
// 取得して各オブジェクトのDraw関数でswitch文で分ける　
// ※最適なプログラムにするにはベースプログラムの大規模な変更が必要になるので現状はこれで

#include <DirectX/DX11Util.h>
#include <DirectX/DX11Graphics.h>

#define MAX_RENDERTARGET_PATH (3)
#define PATH_SHADOW_NUM (1)			// シャドウテクスチャの描き込みのパス

class RenderTargetManager {
public:
	int GetCurrentPath() { return currentRenderPath; };
	void OrderRender(int path);


	static RenderTargetManager* Get() {
		static RenderTargetManager instance;
		return &instance;
	}

private:

	int currentRenderPath;

	RenderTargetManager();
	RenderTargetManager(const RenderTargetManager&) = delete;
	RenderTargetManager& operator=(const RenderTargetManager&) = delete;
	RenderTargetManager(RenderTargetManager&&) = delete;
	RenderTargetManager& operator=(RenderTargetManager&&) = delete;



};