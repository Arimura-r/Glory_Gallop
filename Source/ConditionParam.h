#pragma once
#include "../Library/myDxLib.h"
#include "../ImGui/imgui.h"

//馬の調子を管理するクラス
struct ConditionParam
{
	enum ConditionType
	{
		NONE=-1,
		VERY_BAD=0,//かなり悪い
		BAD,//少し悪い
		NORMAL,//普通
		GOOD,//良い

		CONDITION_NUM,
	};

	ConditionParam();
	~ConditionParam();
	void ImGuiUpdate()const
	{
		ImGui::Text("Condition%d", (int)condition);
	}

	//現在のコンディションをパラメータに適用する
	void ApplyConditionParam(float& _topSpeed,float& _baseSpeedCost,float& _maxSpeedCost);

	ConditionType condition;

private:
};