#include "TitleScene.h"
#include "inputManager.h"
#include "CommonManager.h"
#include "SoundManager.h"
#include "GameStartData.h"
#include "Screen.h"
#include "ModeSelector.h"

namespace
{
	const float TITLE_LOGO_HEIGHT = 300.0f;
	const float TITLE_LOGO_EXRATE = 1.5f;
	const float PLAY_TEXT_HEIGHT = 900.0f;
	const float PLAY_TEXT_EXRATE = 1.0f;

}

TitleScene::TitleScene()
{
	titleBackImage = LoadGraph("data\\texture\\titleBack.png");
	titleLogoImage = LoadGraph("data\\texture\\titleLogo.png");
	playTextImage = LoadGraph("data\\texture\\playText.png");
	//fade = FindGameObject<CommonManager>()->FindCommonObject<FadeManager>();
	//fade->FadeInPlay();
	auto common = FindGameObject<CommonManager>();
	input = common->FindCommonObject<InputManager>();
	sound = common->FindCommonObject<SoundManager>();
	startData = common->FindCommonObject<GameStartData>();
	sound->LoadResource("TITLE");
	sound->Play("Title_BGM", 50, true);

	pushedSelect = false;
	playTextAlpha = 255.0f;
	playTextAlphaChangeSpeed = -1.0f;

	modeSelector = new ModeSelector(playTextImage,input,sound);;
	//canPlayerControll = false;
	//selectMode = false;
}

TitleScene::~TitleScene()
{
	DeleteGraph(titleBackImage);
	DeleteGraph(titleLogoImage);
	DeleteGraph(playTextImage);
	sound->Stop("Title_BGM");
	sound->DeleteResource("TITLE");
}

void TitleScene::Update()
{
	//ボタンを押したらモード選択画面に入る
	if (not modeSelector->GetIsActive() &&
		input->GetKeyboardInput()->GetIsKeyboardPut(KEY_INPUT_Z) ||
		input->GetControllerInput()->GetIsButtonPutNow(P_BUTTON_B))
	{
		sound->PlayOnce("Button_Select");
		modeSelector->Activate();
	}
	else
	{
		//決定が押されたらプレイシーンに向かう
		if (modeSelector->GetIsPlayReady()&&not sound->CheckPlay("Button_Select"))
		{
			GameStartData::Data data = startData->GetData();
			data.canPlayerControll=(modeSelector->GetCanPlayerControll());
			
			startData->SetData(data);
			SceneManager::ChangeScene("PLAY");
		}
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		SceneManager::Exit();
	}

	if (not pushedSelect)
	{
		if (playTextAlpha > 255 || playTextAlpha < 0)
		{
			playTextAlphaChangeSpeed *= -1;
		}

		playTextAlpha += playTextAlphaChangeSpeed;
	}
}

void TitleScene::Draw()
{
	DrawGraph(0, 0, titleBackImage, true);

	if (modeSelector->GetIsActive())
		modeSelector->SelectorDraw();
	else
	{
		DrawRotaGraphF(Screen::WIDTH_CENTER, TITLE_LOGO_HEIGHT, TITLE_LOGO_EXRATE, 0.0f, titleLogoImage, true);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)playTextAlpha);
		DrawRotaGraphF(Screen::WIDTH_CENTER, PLAY_TEXT_HEIGHT, PLAY_TEXT_EXRATE, 0.0f, playTextImage, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	
	//DrawBox(0, 0, 50, 50, ColorFunc::ColorMix(0xff0000, 0x00ff77),true);
}
