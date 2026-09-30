#pragma once
#include "../Library/GameObject.h"

class GameManager;
class CourseDataManager;
class HorseManager;
class Horse;
class UIBase;
class TicketManager;
class Camera;

class BeforeRaceUIManager;
class BeforeStartUIManager;
class PlayUIManager;
class ResultUIManager;
class PlaySceneUIManager;

class UIManager :public GameObject
{
public:
	UIManager(GameManager* _gameManager, CourseDataManager* _course, HorseManager* _horse, TicketManager* _ticket,Camera* _camera,bool _canPlayerControll);
	~UIManager();
	void Update()override;
	void Draw()override;

	PlayUIManager* GetPlayUI()const { return playUI; }
	ResultUIManager* GetResultUI()const { return resultUI; }
	
private:
	GameManager* gameManager;
	BeforeRaceUIManager* beforeRaceUI;
	BeforeStartUIManager* beforeStartUI;
	PlayUIManager* playUI;
	ResultUIManager* resultUI;
	PlaySceneUIManager* playSceneUI;
};