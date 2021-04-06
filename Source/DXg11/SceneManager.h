#pragma once



// ��ԏ�̃V�[�����ŏ��ɓǂݍ��܂��B0�ł͂Ȃ�1�Ȃ͎̂d�g�ݏ�ύX�����m�ł��Ȃ�����
enum SceneNum {	
	SCENE_TITLE= 1,
	SCENE_MAIN = 2,

	SCENE_GAMEOVER = 3,
	SCENE_MAX,
};

// �V �[���̊�{
class SceneBase {
public:
	virtual void LoadObject() = 0;
	virtual ~SceneBase() {};
private:

};


class SceneManager {
public:
	static SceneManager *Get() {
		static SceneManager sceneManager;
		return &sceneManager;
	}


	void LoadScene(SceneNum sceneNum);	// �V�[���ؑւ𖽗�
	void Repeat();							// �V�[���̍ēǂݍ��݂𖽗�


	void SceneTrance();						// ���ۂɃV�[���ؑցB�A�b�v�f�[�g���S���I���������ɐ؂�ւ�Update�̏I����
	SceneNum GetCurrentScene() { return currentScene; }

private:

	SceneNum setScene;						// �ǂ̃V�[���𖽗߂�����
	SceneNum currentScene;

	SceneBase *pScene;


};




