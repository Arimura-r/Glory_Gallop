#include "UIManager.h"
#include "BeforeRaceUIManager.h"
#include "BeforeStartUIManager.h"
#include "PlayUIManager.h"
#include "ResultUIManager.h"
#include "PlaySceneUIManager.h"
#include "GameManager.h"
#include <assert.h>

UIManager::UIManager(GameManager* _gameManager, CourseDataManager* _course, HorseManager* _horse, TicketManager* _ticket, Camera* _camera, bool _canPlayerControll)
{
	gameManager = _gameManager;
	beforeRaceUI = new BeforeRaceUIManager(_horse,_gameManager,_course,_ticket);
	beforeStartUI = new BeforeStartUIManager(_gameManager);
	playUI = new PlayUIManager(_gameManager,_course,_horse,_camera);
	resultUI = new ResultUIManager(_horse,_gameManager,_ticket,_canPlayerControll);
	playSceneUI = new PlaySceneUIManager(_horse, _gameManager, _camera);
}

UIManager::~UIManager()
{
}

void UIManager::Update()
{
}

void UIManager::Draw()
{
	playSceneUI->DrawUI();
	switch (gameManager->GetGameState())
	{
	case BEFORE_RACE:
		beforeRaceUI->DrawUI();
		break;
	case BEFORE_PLAY:
		beforeStartUI->DrawUI();
		break;
	case IN_PLAY:
		playUI->DrawUI();
		break;
	case RESULT:
		resultUI->DrawUI();
		break;
	default:
		assert(false && "UI•\¦‚ÌState‚ÉˆÙí‚È’l‚ª“ü‚Á‚Ä‚¢‚Ü‚·");
		break;
	}
}
