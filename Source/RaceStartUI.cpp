#include "RaceStartUI.h"
#include "GameManager.h"
#include "BeforeRaceState.h"

RaceStartUI::RaceStartUI(VECTOR2 _position, GameManager* _gameManager):UIBase(_position),
gameManager(_gameManager)
{
	//ToDo@TitleScene‚Æ“¯‚¶‰æ‘œ‚ð2‰ñƒ[ƒh‚µ‚Ä‚¢‚é‚Ì‚ÅResourceManager‚È‚Ç‚Å‹¤’Ê‰»‚·‚é
	startImage = LoadGraph("data/texture/playText.png");
}

RaceStartUI::~RaceStartUI()
{
	DeleteGraph(startImage);
}

void RaceStartUI::DrawUI()
{
	if (gameManager->GetBeforeRaceState()->GetCurrentBeforeRaceSelect() != BeforeRaceState::START)
		return;

	DrawRotaGraphF(position.x, position.y, 1.0f, 0.0f, startImage, true);
}
