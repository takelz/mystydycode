#pragma once

// �e�����_�[�ɑ΂��Ă̐U�蕪����GetCurrentRenderPath�Ő��l��
// �擾���Ċe�I�u�W�F�N�g��Draw�֐���switch���ŕ�����@
// ���œK�ȃv���O�����ɂ���ɂ̓x�[�X�v���O�����̑�K�͂ȕύX���K�v�ɂȂ�̂Ō���͂����

#include <DirectX/DX11Util.h>
#include <DirectX/DX11Graphics.h>

#define MAX_RENDERTARGET_PATH (3)
#define PATH_SHADOW_NUM (1)			// �V���h�E�e�N�X�`���̕`�����݂̃p�X

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