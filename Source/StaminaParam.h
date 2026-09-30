#pragma once
#include "../Library/myDxLib.h"
#include "../ImGui/imgui.h"
#include "Easing.h"

struct StaminaParam
{
	float currentStamina;
	float maxStamina;


	StaminaParam()
	{
		maxStamina = 100.0f;
		currentStamina = maxStamina;
	}

	void Update(float _lossNum)
	{
		currentStamina -= _lossNum;
		if (currentStamina < 0)
			currentStamina = 0.0f;

		//鞭を入れている最中はスピードの減少を無視する
		//if (_whipping)
			//return;

		//スタミナが残り少なくなったら減速する
		//if (GetCurrentStaminaRate() < 0.1f)
		//{
		//	_speed.OutEnergy(max(GetCurrentStaminaRate() / 0.1f * _speed.baseSpeed, _speed.minSpeed));
		//}
	}
	void ImGuiUpdate()
	{
		//ImGui::InputFloat("CurrentStamina", &currentStamina);
		//ImGui::InputFloat("MaxStamina", &maxStamina);
	}

	float GetMaxStamina()const { return maxStamina; }
	float GetCurrentStamina()const { return currentStamina; }
	//現在のスタミナの割合を表示
	float GetCurrentStaminaRate()const
	{
		return currentStamina / maxStamina;
	}
	

	//現在のスタミナ消費量からどれだけのスピードを出せるかを計算する
	//float GetSpeedFromStaminaCost(float _staminaCost,const SpeedParam& _speed)
	//{
	//	float ret = 0.0f;
	//	
	//	//スタミナ消費量が通常速度よりも遅いとき
	//	if (_staminaCost < baseSpeedCost)
	//	{
	//		ret = EasingFunc::InOutCubic(_staminaCost, baseSpeedCost,_speed.baseSpeed,_speed.minSpeed );
	//	}
	//	else//スタミナ消費量が通常速度よりも多いとき
	//	{
	//		ret = EasingFunc::OutQuart(_staminaCost - baseSpeedCost, maxSpeedCost - _speed.baseSpeed, _speed.topSpeed, _speed.baseSpeed);
	//	}

	//	return ret;
	//}

private:
};