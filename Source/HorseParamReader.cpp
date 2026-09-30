#include "HorseParamReader.h"
#include "CsvReader.h"

HorseParamReader::HorseParamReader()
{
	Load();
}

HorseParamReader::~HorseParamReader()
{
	params.clear();
}

void HorseParamReader::Load()
{
	CsvReader* csv = new CsvReader();
	csv->Load(PARAM_FILE_PATH);
	//先頭の行は飛ばす
	for (int l = 1; l < csv->GetLines(); l++)
	{
		HorseParam param;
		param.name = csv->GetString(l, 0);
		param.speed.minSpeed = csv->GetFloat(l, 1);
		param.speed.baseSpeed = csv->GetFloat(l, 2);
		param.speed.topSpeed = csv->GetFloat(l, 3);
		param.speed.defaultDecel = csv->GetFloat(l, 4);
		param.speed.defaultAccelaration = csv->GetFloat(l, 5);
		param.speed.outEnergyDeceleration = csv->GetFloat(l, 6);

		param.whipNum = csv->GetInt(l,7);
		param.whipSpeedUpTime = csv->GetFloat(l, 8);
		
		param.cost.minSpeedCost = csv->GetFloat(l, 9);
		param.cost.baseSpeedCost = csv->GetFloat(l, 10);
		param.cost.maxSpeedCost = csv->GetFloat(l, 11);
		param.cost.whipCost = csv->GetFloat(l, 12);

		param.startSpeed= csv->GetFloat(l, 13);

		param.boost.frontChargeRange = csv->GetFloat(l, 14);
		param.boost.backChargeRange = csv->GetFloat(l, 15);
		param.boost.chargeSpeed = csv->GetFloat(l, 16);

		param.bodyTextureIndex = csv->GetInt(l, 17);

		param.favoriteRate = csv->GetFloat(l, 18);
		
		//調子を適用する
		param.condition.ApplyConditionParam(param.speed.topSpeed, param.cost.baseSpeedCost, param.cost.maxSpeedCost);

		params.push_back(param);
	}

	delete csv;
}
