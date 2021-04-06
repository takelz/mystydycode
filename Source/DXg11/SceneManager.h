#pragma once



// 一番上のシーンが最初に読み込まれる。0ではなく1なのは仕組み上変更を検知できないため
enum SceneNum {	
	SCENE_TITLE= 1,
	SCENE_MAIN = 2,

	SCENE_GAMEOVER = 3,
	SCENE_MAX,
};

// シ ーンの基本
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


	void LoadScene(SceneNum sceneNum);	// シーン切替を命令
	void Repeat();							// シーンの再読み込みを命令


	void SceneTrance();						// 実際にシーン切替。アップデートが全部終わった直後に切り替えUpdateの終わりに
	SceneNum GetCurrentScene() { return currentScene; }

private:

	SceneNum setScene;						// どのシーンを命令したか
	SceneNum currentScene;

	SceneBase *pScene;


};




