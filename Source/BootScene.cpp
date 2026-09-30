#include "BootScene.h"
#include "inputManager.h"
#include "SoundManager.h"
#include "GameStartData.h"
#include "CommonManager.h"

BootScene::BootScene()
{
	//CommonManagerの生成とCommonManagerに常に生成しておきたいオブジェクトを挿入
	CommonManager*common = new CommonManager();
	common->AddCommonObject<InputManager>(new InputManager(true, true, true));
	common->AddCommonObject<SoundManager>(new SoundManager());
	common->AddCommonObject<GameStartData>(new GameStartData());
	//common->AddCommonObject<FadeManager>(new FadeManager());
	//common->AddCommonObject<Font>(new Font());
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TITLE"); // 起動が終わったらTitleを表示
}

void BootScene::Draw()
{
}
