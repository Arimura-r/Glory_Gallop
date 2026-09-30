#pragma once
#include "../Library/SceneBase.h"

class SoundManager;

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

private:
	SoundManager* sound;
	float gameSpeedRate;
	//int hModel;
};
