#pragma once
#include "../Library/GameObject.h"

class HorseManager;
class InputManager;
class SoundManager;

//レース出走前に各馬のデータ表示する箇所を管理するクラス
class HorseDataDrawManager :public GameObject
{
public:
	HorseDataDrawManager(HorseManager* _horse, InputManager* _input, SoundManager* _sound);
	~HorseDataDrawManager();

	void HorseDataUpdate();
	int GetCursorHorseNum()const { return cursorHorseNum; }

private:
	int cursorHorseNum;
	HorseManager* horseManager;
	InputManager* input;
	SoundManager* sound;
};