#include "PressNextUI.h"
#include "GameManager.h"
#include "ResultState.h"

namespace
{
	const float ALPHA_MAX = 255.0f;
	const float DEFAULT_ALPHA_CHANGE_SPEED = 1.0f;
	const float TEXT_SCALE = 1.0f;
}

PressNextUI::PressNextUI(VECTOR2 _position, GameManager* _gameManager):UIBase(_position),gameManager(_gameManager)
{
	alpha = ALPHA_MAX;
	alphaChangeSpeed = -DEFAULT_ALPHA_CHANGE_SPEED;
	hImage = LoadGraph("data/texture/pressNextText.png");
}

PressNextUI::~PressNextUI()
{
	DeleteGraph(hImage);
}

void PressNextUI::Update()
{
	//タイトルに戻れる描画ステートの場合のみ描画、α値変更の処理を行う
	active =	gameManager->GetResultState()->GetCurrentDrawState() == ResultState::RACE_POSITION ||
				gameManager->GetResultState()->GetCurrentDrawState() == ResultState::PAY_OUT_RESULT;

	if (not active)
		return;

	if (alpha > ALPHA_MAX || alpha < 0)
	{
		alphaChangeSpeed *= -1;
	}

	alpha += alphaChangeSpeed;
}

void PressNextUI::DrawUI()
{
	if (not active)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
	DrawRotaGraphF(position.x, position.y, TEXT_SCALE, 0.0f, hImage, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)ALPHA_MAX);
}
