#pragma once
#include "../Library/myDxLib.h"
#include "../ImGui/imgui.h"

struct SpeedParam
{
	float currentSpeed;//現在速度
	float currentAccelaration;//現在の加速度
	float currentTargetSpeed;//現在の目標速度

	float minSpeed;//最低速度
	float baseSpeed;//基礎速度
	float topSpeed;//最高速度

	float defaultDecel;//通常時に徐々に減速する際の速度
	float defaultAccelaration;//鞭を使った時の1秒あたりの加速

	float outEnergyDeceleration;//スタミナ切れ時の減速度

	SpeedParam()
	{
		currentSpeed = 0.0f;
		currentAccelaration = 0.0f;
		currentTargetSpeed = 0.0f;
		minSpeed = 0.0f;
		baseSpeed = 0.0f;
		topSpeed = 0.0f;
		defaultDecel = 0.0f;
		defaultAccelaration = 0.0f;
		outEnergyDeceleration = 0.0f;
	}
	SpeedParam(float _minSpeed,float _baseSpeed, float _topSpeed, float _defDec, float _defAc)
	{
		minSpeed = _minSpeed;
		currentSpeed = 0.0f;
		currentAccelaration = 0.0f;
		currentTargetSpeed = 0.0f;
		baseSpeed = _baseSpeed;
		topSpeed = _topSpeed;
		defaultDecel = _defDec;
		defaultAccelaration = _defAc;
		outEnergyDeceleration = 0.0f;
	}

	//瞬間的にスピードを増加させる
	void SpeedUp(float _upSpeed)
	{
		currentSpeed += _upSpeed;
		currentTargetSpeed += _upSpeed;
		if (currentSpeed > topSpeed)
			currentSpeed = topSpeed;
		if(currentTargetSpeed>topSpeed)
			currentTargetSpeed = topSpeed;
	}
	//ブレーキをかけ、瞬間的にスピードを落す
	void SpeedDown(float _downSpeed)
	{
		currentSpeed -= _downSpeed;
		currentTargetSpeed -= _downSpeed;

		if (currentSpeed < minSpeed)
			currentSpeed = minSpeed;
		if (currentTargetSpeed < minSpeed)
			currentTargetSpeed = minSpeed;
	}

	//スタート直後に標準速度になるまでスピードを上げる
	void Start()
	{
		currentAccelaration = defaultAccelaration;
		//currentAccelaration = startAccelaration;
		currentTargetSpeed = baseSpeed;
	}
	//鞭を打つ
	void Whip()
	{
		currentAccelaration = 0.0f;
	}
	//鞭の速度アップの終了
	void WhipEnd()
	{
		//currentAccelaration = -whipAccelaration;
		//currentTargetSpeed = baseSpeed;
	}
	//スタミナ切れ　以降徐々に減速を行う
	void OutEnergy()
	{
		currentTargetSpeed = minSpeed;
		currentAccelaration = -outEnergyDeceleration;
	}

	//目標速度に達しているか
	bool IsAccelEnd()const
	{
		return currentSpeed == currentTargetSpeed || currentSpeed>=topSpeed;
	}

	//加速を開始する
	void Accelaration(float _targetSpeed)
	{
		currentAccelaration = defaultAccelaration;
		currentTargetSpeed = _targetSpeed;
	}

	//減速を開始する
	void Decelaration(float _targetSpeed)
	{
		currentAccelaration = -defaultDecel;
		currentTargetSpeed = _targetSpeed;
	}

	void ImGuiUpdate()
	{
		ImGui::InputFloat("CurrentSpeed", &currentSpeed);
		ImGui::InputFloat("CurrentAccelaration", &currentAccelaration);
		ImGui::InputFloat("CurrentTargetSpeed", &currentTargetSpeed);
		ImGui::InputFloat("BaseSpeed", &baseSpeed);
		ImGui::InputFloat("TopSpeed", &topSpeed);
		ImGui::InputFloat("DefaultDecelaration", &defaultDecel);
		ImGui::InputFloat("DefaultAccelaration", &defaultAccelaration);
	}

	void Update()
	{
		currentTargetSpeed = min(currentTargetSpeed, topSpeed);
		currentTargetSpeed = max(currentTargetSpeed, minSpeed);
		currentSpeed = min(currentSpeed, topSpeed);
		//currentSpeed = max(currentSpeed, minSpeed);

		//現在の目標速度に現在速度が達していない場合、現在の加速度に応じて速度を上げる
		if (currentTargetSpeed > currentSpeed)
		{
			currentSpeed += Time::DeltaTime() * currentAccelaration;

			if (currentTargetSpeed < currentSpeed)
				currentSpeed = currentTargetSpeed;
		}
		//減速の処理
		else if (currentTargetSpeed < currentSpeed)
		{
			//if (currentAccelaration <= 0)
			//{
				currentSpeed += Time::DeltaTime() * currentAccelaration;
				if (currentTargetSpeed > currentSpeed)
					currentSpeed = currentTargetSpeed;
			//}
			/*else
			{
				currentSpeed -= Time::DeltaTime() * outEnergyDeceleration;
				if (currentTargetSpeed > currentSpeed)
					currentSpeed = currentTargetSpeed;
			}*/
		}
	}
};