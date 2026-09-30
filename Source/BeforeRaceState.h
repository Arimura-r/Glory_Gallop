#pragma once
#include"../Library/GameObject.h"

class InputManager;
class SoundManager;
class TicketManager;
class HorseManager;

class BetManager;
class HorseDataDrawManager;

//レース前の状態を管理するクラス
class BeforeRaceState :public GameObject
{
public:
	//レース前の情報選択画面
	enum BeforeRaceSelected
	{
		RACE_DATA = 0,
		ALL_HORSE_DATA,
		TICKET,
		START,

		SELECT_NUM,
	};

	BeforeRaceState(InputManager* _input, SoundManager* _sound, TicketManager* _ticket, HorseManager* _horse);
	~BeforeRaceState();
	void UpdateState();

	BeforeRaceSelected GetCurrentBeforeRaceSelect()const { return currentSelect; }
	//ステート変更を要望する
	bool GetRequestStateChange()const { return requestStateChange; }

	BetManager* GetBetManager()const { return bet; }
	HorseDataDrawManager* GetHorseDataDrawManager()const { return horseData; }

private:
	//選択項目の移動処理
	void SelectorControll();

	InputManager* input;
	SoundManager* sound;
	HorseManager* horse;

	HorseDataDrawManager* horseData;
	BetManager* bet;

	bool requestStateChange;
	BeforeRaceSelected currentSelect;
};