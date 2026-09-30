#include "GameStartData.h"

GameStartData::GameStartData()
{
	DontDestroyOnSceneChange(true);
}

GameStartData::~GameStartData()
{
}

void GameStartData::SetData(Data& _data)
{
	data = _data;
}

