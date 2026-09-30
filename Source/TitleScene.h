#pragma once
#include "../Library/SceneBase.h"

class FadeManager;
class InputManager;
class SoundManager;
class GameStartData;
class ModeSelector;

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;

private:
	GameStartData* startData;
	InputManager* input;
	SoundManager* sound;

	ModeSelector* modeSelector;

	int titleBackImage;
	int titleLogoImage;

	int playTextImage;
	float playTextAlpha;
	float playTextAlphaChangeSpeed;
	bool pushedSelect;
	//bool canPlayerControll;//プレイヤーキャラを操作するかどうかの設定(オフならAIに動かしてもらう)
	//bool selectMode;
	//std::string changeScene;//シーン遷移の行先
};
