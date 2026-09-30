#pragma once
#include "../Library/myDxLib.h"
#include "../ImGui/imgui.h"

//現在速度に応じて速度を上げる際のコストを管理するクラス(スタミナ・ブーストで使用)
struct SpeedCostParam
{
	float minSpeedCost;//最低速度で走った時のスタミナ消費量
	float baseSpeedCost;//通常速度で走った時のスタミナ消費量
	float maxSpeedCost;//最高速度で走った時のスタミナ消費量
	float whipCost;//鞭を入れている際のスタミナ消費量

	SpeedCostParam()
	{
		minSpeedCost = 0.0f;
		baseSpeedCost = 0.0f;
		maxSpeedCost = 0.0f;
		whipCost = 0.0f;
	}

	//コストを計算する
	float LossCost(bool _whipping, const SpeedParam& _speed)const
	{
		if (_whipping)
			return whipCost * Time::DeltaTime();

		float ret = 0.0f;

		//現在速度が通常速度以下の場合のスタミナ消費量の計算
		if (_speed.currentSpeed < _speed.baseSpeed)
		{
			//ToDo 最高速度との兼ね合いも含めて要調整

			//float rate=_speed.currentSpeed / _speed.baseSpeed;
			ret = EasingFunc::InOutCubic(_speed.currentSpeed, _speed.baseSpeed, baseSpeedCost, minSpeedCost);
		}
		else//現在速度が通常速度以上の場合のスタミナ消費量の計算
		{
			ret = EasingFunc::OutQuart(_speed.currentSpeed - _speed.baseSpeed, _speed.topSpeed - _speed.baseSpeed, maxSpeedCost, baseSpeedCost);
		}

		return ret * Time::DeltaTime();
	}
};