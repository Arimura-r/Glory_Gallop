#pragma once
#include "../Library/GameObject.h"
#include <vector>

class UIBase;
class HorseManager;
class GameManager;
class CourseDataManager;
class TicketManager;

class BeforeRaceUIManager :public GameObject
{
public:
	BeforeRaceUIManager(HorseManager* _horse, GameManager* _gameManager,CourseDataManager* _course, TicketManager* _ticket);
	~BeforeRaceUIManager();

	void Update()override;
	//UI‚ð•`‰æ‚·‚é
	void DrawUI();

private:
	std::vector<UIBase*> uiList;
};