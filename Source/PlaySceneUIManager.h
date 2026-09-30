#pragma once
#include "../Library/GameObject.h"
#include <vector>

class UIBase;
class HorseManager;
class GameManager;
class Camera;

//プレイシーン全体で利用するUIを管理するクラス
class PlaySceneUIManager :public GameObject
{
public:
	PlaySceneUIManager(HorseManager* _horse, GameManager* _gameManager, Camera* _camera);
	~PlaySceneUIManager();

	void Update()override;
	//UIを描画する
	void DrawUI();

private:
	std::vector<UIBase*> uiList;
};