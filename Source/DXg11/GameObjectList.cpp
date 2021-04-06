#include"GameObjectList.h"
#include"../Camera.h"
#include <Shader/ShadowWrite.h>
#include <Shader/NormalRender.h>
#include <Shader/PostBlur.h>
#include <Shader/PostBloom.h>
#include <Shader/DepthBuffer.h>
#include <Shader/PostFog.h>
#include <Shader/PostNoiseWave.h>
#include <Shader/PostDistortionObject.h>
#include <Shader/PostPerlinGlass.h>

static unsigned int currentUniqueNum = 0;


GameObjectList::GameObjectList(){
	itrEnd = startObjectList.end();

	// �e�X�g
	ShadowWrite::Get()->Create(0,0);
}

void GameObjectList::Setup() {
	// �^�O�̃f�[�^���X�g�̏���
	for (int i = 0; i < TAG_MAX;i++) {
		tagList[(TAG)i];
	}
}

void GameObjectList::Start() {
	if (!startObjectList.size())return;	// �����Ȃ��Ȃ�X���[
	
	auto itr = startObjectList.begin();
	int size = startObjectList.size();
	for (int i = 0; i < size; i++) {
		(*itr)();
		++itr;
	}
	startObjectList.clear();
}


void GameObjectList::Collide() {

}


void GameObjectList::Update() {
	// �V�F�[�_�[��time�p
	ShaderTimeCountUp();

	auto itr = objectList.begin();
	auto endPoint = objectList.end();
	while (itr != endPoint) {
		// �X�^�[�g�Ă΂�ĂȂ��Ƃ��̓X�^�[�g
		if (!(*itr)->endStart) {
			(*itr)->Start();
			(*itr)->endStart = true;
		}
		if(!stopUpdate)
			(*itr)->Update();
		// �폜
		if ((*itr)->isDestroy) {
			// �^�O���X�g����폜
			if((*itr)->tag != TAG_NONE)
				tagList[(*itr)->tag].erase((*itr)->tagListItr);

			// Start���X�g����폜
			//if ((*itr)->startListItr != GetItrEnd()) {
			//
			//}

			// �I�u�W�F�N�g���X�g����폜
			GameObject *obj = (*itr);	// �������Ɉڂ��Ă���C�e���[�^�g���ă��X�g�폜
			itr = objectList.erase(itr);
			delete obj;
		}
		if (itr == endPoint) break;
		++itr;
	}
}

const std::list<GameObject*> *GameObjectList::GetTagList(TAG tag) {
	return &tagList[tag];
}

void GameObjectList::RegisterTagObject(GameObject* obj, TAG setTag) {
	if (obj->tag != TAG_NONE) {
		MessageBox(NULL, "���łɃ^�O���ݒ肳��Ă��܂�", "GameObjectError", MB_OK);
		return;
	}
	if (setTag == TAG_NONE)return;	// ���̃^�O�͂������Ȃ��@

	tagList[obj->tag].emplace_back(obj);
	obj->tagListItr = --tagList[obj->tag].end();	// �Ō��end����O�����X�g�̍Ō�̃f�[�^

}




void GameObjectList::Draw() {



	auto itr = objectList.begin();
	auto endPoint = objectList.end();


	for (int loop = 1; loop <= MAX_RENDERTARGET_PATH; loop++) {
		RenderTargetManager::Get()->OrderRender(loop);

		itr = objectList.begin();
		endPoint = objectList.end();


		// ������g�ݑւ��鎞�Ԃ��Ȃ��̂Ŏ蓮�ŃV�F�[�_�؂�ւ�
		while (itr != endPoint) {
			if (RENDER_PATH == 1) {	// ��x�ڂ�DEPTHWRITE�͌�������̂���
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_WRITE);
			}
			else if (RENDER_PATH == 2) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_BUFFER);
			}
			else if (RENDER_PATH == 3 || RENDER_PATH == 4) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_SHADOW);
			}

			(*itr)->Draw();
			++itr;
		}

		itr = objectList.begin();
		endPoint = objectList.end();
		while (itr != endPoint) {
			if (RENDER_PATH == 1) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_WRITE);
			}
			else if (RENDER_PATH == 2) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_BUFFER);
			}
			else if (RENDER_PATH == 3 || RENDER_PATH == 4) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_SHADOW);
			}
			(*itr)->LateDraw();
			++itr;
		}

		if (RENDER_PATH == 2) {
			RenderTargetManager::Get()->OrderRender(5);
			auto addItr = addDrawList.begin();
			auto  addEndPoint = addDrawList.end();
			while (addItr != addEndPoint) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_NORMAL);
				(*addItr)->LateDraw();
				++addItr;
			}
		}

	}


}




void GameObjectList::LateUpdate() {
	auto itr = objectList.begin();
	auto endPoint = objectList.end();


	while (itr != endPoint) {
		(*itr)->SystemLateUpdate();
		++itr;
	}



	// �����Ƀ����_�[���ʂ̕\��


	// �^�[�Q�b�g���ς���
	GetDX11Graphics()->EnableZBuffer(false);

	//Blur::Get()->Render();
	PostBloom::Get()->Render();
	PostFog::Get()->Render();
	PostNoiseWave::Get()->Render();
	PostPerlinGlass::Get()->Render();


	// �o�b�N�o�b�t�@�A�X�N���[�����W��ɕ`�悷��
	GetDX11Graphics()->EnableZBuffer(false);
	NormalRender::Get()->Render();		
	//ShadowWrite::Get()->Render();
	//DepthBuffer::Get()->Render();
	//PostDistortionObject::Get()->Render();

	itr = objectList.begin();
	endPoint = objectList.end();
	while (itr != endPoint) {
		(*itr)->ScreenDraw();
		++itr;
	}


	GetDX11Graphics()->EnableZBuffer(true);
}





void GameObjectList::Uninit() {

}

GameObject* GameObjectList::Generate(GameObject *obj) {
	obj->transform.instanceID = currentUniqueNum;
	currentUniqueNum++;

	objectList.emplace_back(obj);
	if (obj->tag != TAG_NONE) {
		tagList[obj->tag].emplace_back(obj);
		obj->tagListItr = --tagList[obj->tag].end();	// �Ō��end����O�����X�g�̍Ō�̃f�[�^
	}
	return obj;
}

void GameObjectList::Reset() {
	auto itr = objectList.begin();
	auto endPoint = objectList.end();
	
	// �ǉ��̃��X�g����폜
	addDrawList.clear();


	while (itr != endPoint) {
		// ��j��I�u�W�F�N�g�łȂ���΍폜
		if (!(*itr)->dontDestroy) {
			// �^�O���X�g����폜�B�^�O�̃C�e���[�^�͊e�I�u�W�F�N�g���ێ����Ă�̂ł�����g��
			if ((*itr)->tag != TAG_NONE)
				tagList[(*itr)->tag].erase((*itr)->tagListItr);
			// �I�u�W�F�N�g���X�g����폜
			GameObject *obj = (*itr);	// �������Ɉڂ��Ă���C�e���[�^�g���ă��X�g�폜
			itr = objectList.erase(itr);
			delete obj;
		}
		else {
			++itr;
		}

		if (itr == endPoint)break;
		
	}
}