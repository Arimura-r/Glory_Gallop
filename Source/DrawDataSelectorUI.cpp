#include "DrawDataSelectorUI.h"
#include "GameManager.h"
#include "BeforeRaceState.h"
#include "HorseManager.h"

namespace
{
	const VECTOR2 IMAGE_SIZE = VECTOR2(300, 200);
}

DrawDataSelectorUI::DrawDataSelectorUI(VECTOR2 _position, GameManager* _gameManager, HorseManager* _horseManager):UIBase(_position),
gameManager(_gameManager),horseManager(_horseManager)
{
	std::string texPath = "data/texture/";
	selectImageHandle.emplace(BeforeRaceState::RACE_DATA, LoadGraph((texPath + "raceInfoText.png").c_str()));
	selectImageHandle.emplace(BeforeRaceState::ALL_HORSE_DATA, LoadGraph((texPath + "raceHorseText.png").c_str()));
	selectImageHandle.emplace(BeforeRaceState::TICKET, LoadGraph((texPath + "ticketText.png").c_str()));
	selectImageHandle.emplace(BeforeRaceState::START, LoadGraph((texPath + "startText.png").c_str()));
}

DrawDataSelectorUI::~DrawDataSelectorUI()
{
	for (auto& h : selectImageHandle)
	{
		DeleteGraph(h.second);
	}
}

void DrawDataSelectorUI::Update()
{
}

void DrawDataSelectorUI::DrawUI()
{
	for(int i=0;i< BeforeRaceState::SELECT_NUM;i++)
	{
		auto it = selectImageHandle.find(i);
		//要素が見つからなかったらスキップ
		if (it == selectImageHandle.end())
			continue;

		int selectOrder = i;

		if (horseManager->GetPlayer() != nullptr)
		{
			//プレイヤー操作モードなら馬券は表示しない
			if (i == BeforeRaceState::TICKET)
				continue;
			//馬券以降の項目を1つ上側に寄せる
			else if (i > BeforeRaceState::TICKET)
				selectOrder--;
		}

		float addX = 0.0f;
		//選択されている項目だけ右に出す
		if (gameManager->GetBeforeRaceState()->GetCurrentBeforeRaceSelect() == i)
			addX += 100.0f;

		DrawGraphF(position.x+addX, position.y + selectOrder * IMAGE_SIZE.y, (*it).second, true);
	}
}
