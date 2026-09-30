#pragma once
#include "SpeedParam.h"
#include "StaminaParam.h"
#include "BoostParam.h"
#include "SpeedCostParam.h"
#include "ConditionParam.h"

//全馬で使用するパラメータをまとめる構造体　ToDo処理がこれ以上増えるならクラス化して整理する
struct HorseParam
{
	std::string name;
	SpeedParam speed;
	StaminaParam stamina;
	BoostParam boost;
	SpeedCostParam cost;
	ConditionParam condition;
	int whipNum;
	float whipSpeedUpTime;
	float startSpeed;//スタートの合図から走り出すまでの反応速度
	bool chargingBoost;//ブーストを溜めているかどうか
	bool startOutEnergy;//体力切れで減速を始めたかどうか
	int bodyTextureIndex;//馬体のテクスチャの種類
	float favoriteRate;//人気のしやすさ

	HorseParam()
	{
		whipNum = 0;
		whipSpeedUpTime = 0.0f;
		startSpeed = 0.0f;
		chargingBoost = false;
		startOutEnergy = false;
		bodyTextureIndex = 0;
		favoriteRate = 1.0f;
		//targetPosition = 0;
	}

	void Update(bool _whipping,float _currentPositionRate)
	{
		chargingBoost = false;

		float lossCost=cost.LossCost(_whipping, speed);//現在速度に応じて使用するコストを計算する
		stamina.Update(lossCost);//コスト分スタミナを減らす

		if (stamina.GetCurrentStaminaRate() <= 0.0f)//スタミナがないならブーストを利用する
		{
			boost.UseBoost(lossCost);
			if (boost.chargeBoost <= 0.0f&&not _whipping)//ブーストもないなら減速し始める
			{
				speed.OutEnergy();
				startOutEnergy = true;
			}
		}
		else
		{//スタミナがあるときのみブーストを溜める
			chargingBoost=boost.Charge(_currentPositionRate);//ブーストを溜める条件を満たしているなら溜める
		}
		speed.Update();
	}
};