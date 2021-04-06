#pragma once
#include"GameObjectBase.h"
#include<map>
#include<functional>



#define RENDER_PATH (RenderTargetManager::Get()->GetCurrentPath())

// ���X�g��unity�̃q�G�����L�[�r���[�̃C���[�W��

class GameObjectList {
	friend class ColliderCircle;
	friend class ColliderAABB;
	friend class ColliderOBB;
public:

	static GameObjectList *Get() {
		static GameObjectList gameObjectList;
		return &gameObjectList;
	}

	GameObject* Generate(GameObject *obj);
	
	const std::list<GameObject*> *GetTagList(TAG tag);

	// �Q�[���I�u�W�F�N�g���X�g�ɓ����O��TAG���ݒ肳��Ă���Ȃ玩���I�Ɋ��蓖�Ă���
	// ���X�g�ɂ��ꂽ��Ȃ�΂��̊֐��Ń^�O��ݒ肷��
	void RegisterTagObject(GameObject* obj, TAG setTag);

	// ���s���͏ォ��
	void Setup();

	void Start();
	void Collide();			// 
	void Update();
	void Draw();	
	void LateUpdate();		// Draw�̒���ɍX�V


	void Uninit();

	void Reset();			// ���X�g�������ׂă��Z�b�g(��j��I�u�W�F�N�g�͏���)
	void CompletelyReset();	// ��j��I�u�W�F�N�g���܂ފ��S�Ƀ��Z�b�g�A�͎������Ȃ�

	void AddDrawList(GameObject *obj) { addDrawList.emplace_back(obj); };
	// �C�e���[�^�\�̏������Ŏg��
	std::list<std::function<void()> >::iterator* GetItrEnd() { return &itrEnd; }

	// �S�I�u�W�F�N�g�̃��X�g
	std::list<GameObject*> objectList;	
	// �^�O�������ꂽ���X�g
	std::map<TAG,std::list<GameObject*>> tagList;
	// Start()�֐�
	std::list<std::function<void()> > startObjectList;

	// �c�ݗp�֐�
	std::list<GameObject*> addDrawList;

	std::list<std::function<void()> >::iterator itrEnd;

	// �A�b�v�f�[�g�֐����~�߂�p�ALateUpdate()�͎~�܂�Ȃ�
	bool stopUpdate;

	GameObjectList();
	~GameObjectList() {};

};