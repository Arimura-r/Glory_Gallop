#pragma once
#include "../Library/myDxLib.h"
#include "../ImGui/imgui.h"

struct BoostParam
{
	float frontChargeRange;//脚が溜まる最高順位
	float backChargeRange;//脚が溜まる最低順位
	float chargeSpeed;//脚が溜まる速度
	float chargeBoost;//脚の溜まっている量
	float maxBoost;//最大値


	BoostParam()
	{
		frontChargeRange = 0.0f;
		backChargeRange = 0.0f;
		chargeSpeed = 0.0f;
		chargeBoost = 0.0f;
		maxBoost = 100.0f;
	}

	//チャージ可能な順位を0~頭数-1位で返す
	int GetFrontChargeRange(const int _horseMax)const
	{
		int ret = (int)(frontChargeRange * (_horseMax - 1));
		if (ret != 0 && ret != _horseMax)
			ret += 1;

		return ret;
	}
	//チャージ可能な順位を0~頭数-1位で返す
	int GetBackChargeRange(const int _horseMax)const
	{
		int ret = (int)(backChargeRange * (_horseMax - 1));
	/*	if (ret != 0 && ret != _horseMax-1)
			ret += 1;*/

		return ret;
	}

	//ゲージを溜める(溜められているならtrueを返す)
	bool Charge(float _currentPositionRate)
	{
		bool ret = false;
		if (frontChargeRange <= _currentPositionRate && _currentPositionRate <= backChargeRange)
		{
			chargeBoost += chargeSpeed * Time::DeltaTime();
			ret = true;
		}

		return ret;
	}
	void ImGuiUpdate()
	{
		ImGui::SliderFloat("Boost", &chargeBoost, 0.0f, maxBoost);
	}
	//ゲージを使用する
	void UseBoost(float _loss)
	{
		chargeBoost -= _loss;
		if (chargeBoost < 0)
			chargeBoost = 0.0f;
	}
	float GetChargeBoostRate()const { return chargeBoost / maxBoost; }
};