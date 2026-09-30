#pragma once
#include "../Library/GameObject.h"
#include <vector>

class GameManager;
class Camera;
class CourseDataManager;
class HorseManager;
class Horse;
class UIBase;

//UIをまとめて管理するクラス
class PlayUIManager :public GameObject
{
public:
	PlayUIManager(GameManager* _gameManager, CourseDataManager* _course,HorseManager* _horse,Camera* _camera);
	~PlayUIManager();

	//UIを描画する
	void DrawUI();

	//UIを表示する対象を変更する
	void ChangeUITarget();

private:
	std::vector<UIBase*> uiList;
	Camera* camera;
};