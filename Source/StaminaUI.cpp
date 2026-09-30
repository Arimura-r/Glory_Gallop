#include "StaminaUI.h"
#include "Horse.h"
#include "DrawFigureFunc.h"

StaminaUI::StaminaUI(VECTOR2 _position, Horse* _target):UIBase(_position,_target)
{
	target = _target;
}

StaminaUI::~StaminaUI()
{
}

void StaminaUI::DrawUI()
{
	if (target == nullptr)
		return;

	float reamingStamina = target->GetStaminaParam().GetCurrentStaminaRate();
	float chargeBoost = target->GetParam().boost.GetChargeBoostRate();

	DrawFormatStringF(position.x, position.y, 0xffffff, "Stamina：%.1f %%", reamingStamina*100);

	float lossStamina = target->GetLossStamina();//1秒間に失うスタミナ
	float reamingTime = target->GetRemainingDistance()/target->GetSpeedParam().currentSpeed;

	//std::string currentPace;
	////残りスタミナと現在の消費量からスタミナが持つのかを確認
	//if (lossStamina * reamingTime> reamingStamina*100)
	//{
	//	currentPace = "ハイペース";
	//}
	//else if (lossStamina * reamingTime + 20 < reamingStamina*100)
	//{
	//	currentPace = "スローペース";
	//}
	//else
	//{
	//	currentPace = "ミドルペース";
	//}

	//DrawFormatStringF(position.x, position.y + 25, 0xffffff, "現在のペース：%s", currentPace.c_str());
	//DrawFormatStringF(position.x, position.y + 25, 0xffffff, "ゴールまでの時間：%.1f　失うスタミナ：%.3f　現在のペース：%s", reamingTime, lossStamina,currentPace.c_str());

	int gaugeLength = 200;
	DrawFigureFunc::DrawMixGauge(position + VECTOR2(0, 75), gaugeLength, 30, reamingStamina,chargeBoost, 0x00ff00,0x0000ff, 0xff0000);
}
