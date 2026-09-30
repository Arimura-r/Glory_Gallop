#pragma once
#include "../Library/GameObject.h"
#include "HorseParam.h"
#include <vector>

class HorseParamReader :public GameObject
{
public:
	HorseParamReader();
	~HorseParamReader();

	const std::vector<HorseParam>& GetParams()const { return params; }

private:
	const std::string PARAM_FILE_PATH = "data\\csv\\horseParam.csv";

	void Load();

	std::vector<HorseParam>params;
};