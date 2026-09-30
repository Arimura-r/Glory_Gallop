#pragma once
#include "../Library/GameObject.h"
#include <vector>

class UIBase;
class HorseManager;
class GameManager;
class TicketManager;

class ResultUIManager :public GameObject
{
public:
	ResultUIManager(HorseManager* _horse,GameManager* _gameManager,TicketManager* _ticket,bool _canPlayerControll);
	~ResultUIManager();

	void Update()override;
	//UI‚ð•`‰æ‚·‚é
	void DrawUI();

private:
	std::vector<UIBase*> uiList;
};