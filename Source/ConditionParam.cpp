#include "ConditionParam.h"
#include <vector>
#include "../Library/Random.h"

namespace
{
	std::vector<double> CONDITION_WIGHT{ 0.05,0.1,0.5,0.1 };//各調子になる確率

	//調子が悪い時に元のパラメータに何を掛けるか
	float VERY_BAD_TOP_SPEED_RATE = 0.87f;
	float VERY_BAD_BASE_SPEED_COST_RATE = 1.1f;
	float VERY_BAD_TOP_SPEED_COST_RATE = 1.1f;

	//調子が少し悪い時に元のパラメータに何を掛けるか
	float BAD_TOP_SPEED_RATE = 0.96f;
	float BAD_BASE_SPEED_COST_RATE = 1.03f;
	float BAD_TOP_SPEED_COST_RATE = 1.03f;

	//調子が良い時に元のパラメータに何を掛けるか
	float GOOD_TOP_SPEED_RATE = 1.04f;
	float GOOD_BASE_SPEED_COST_RATE = 0.96f;
	float GOOD_TOP_SPEED_COST_RATE = 0.96f;

	float CONDITION_PARAM_RATE_RANGE = 0.05f;//パラメータの変更幅
}

ConditionParam::ConditionParam()
{
	condition = (ConditionType)Random::GetWeightedIndex(CONDITION_WIGHT);
}

ConditionParam::~ConditionParam()
{
}

void ConditionParam::ApplyConditionParam(float& _topSpeed, float& _baseSpeedCost, float& _maxSpeedCost)
{
	switch (condition)
	{
	case ConditionParam::VERY_BAD:
		_topSpeed *= VERY_BAD_TOP_SPEED_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		_baseSpeedCost *= VERY_BAD_BASE_SPEED_COST_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		_maxSpeedCost *= VERY_BAD_TOP_SPEED_COST_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		break;
	case ConditionParam::BAD:
		_topSpeed *= BAD_TOP_SPEED_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		_baseSpeedCost *= BAD_BASE_SPEED_COST_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		_maxSpeedCost *= BAD_TOP_SPEED_COST_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		break;
	case ConditionParam::GOOD:
		_topSpeed *= GOOD_TOP_SPEED_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		_baseSpeedCost *= GOOD_BASE_SPEED_COST_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		_maxSpeedCost *= GOOD_TOP_SPEED_COST_RATE * Random::GetFloat(1.0f, CONDITION_PARAM_RATE_RANGE);
		break;
	}
}
