#pragma once
#include "../Library/GameObject.h"

class InputManager;
class SoundManager;

class ModeSelector :public GameObject
{
public:
	ModeSelector(int _playTextHandle,InputManager* _input,SoundManager* _sound);
	~ModeSelector();
	void Update()override;
	void SelectorDraw();//描画順を管理しやすくするため継承をせず直接TitleSceneで呼ぶ

	bool GetCanPlayerControll()const { return canPlayerControll; }
	//アクティブにしてUpdate,Drawを回す
	void Activate() { active = true; }

	bool GetIsActive()const { return active; }
	bool GetIsPlayReady()const { return playReady; }

private:
	int playTextHandle;
	int leftArrowHandle;
	int rightArrowHandle;
	int selectModeTextHandle;
	int rideModeTextHandle;
	int watchModeTextHandle;

	bool canPlayerControll;//プレイヤーキャラを操作するかどうかの設定(オフならAIに動かしてもらう)

	bool active;
	bool playReady;//プレイシーンへ遷移する準備が完了したか

	InputManager* input;
	SoundManager* sound;
};