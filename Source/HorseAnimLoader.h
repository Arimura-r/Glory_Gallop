#pragma once
#include "AnimDefine.h"
#include "HorseAnimDefine.h"
#include "../Library/GameObject.h"

class Animator;

class HorseAnimLoader :public GameObject
{
public:
	HorseAnimLoader();
	~HorseAnimLoader();

	//アニメーションデータをコピーしたものを渡す
	Animator* GetAnimDupli()const;
	
private:
	//ToDo　外部ファイルから指定できるようにする
	std::vector<AnimSetting> horseAnims
	{
		AnimSetting(HorseAnim::WAIT,"data\\model\\character\\animation\\HorseAnim.mv1",true,1.0f,3),
		AnimSetting(HorseAnim::RUN,"data\\model\\character\\animation\\HorseAnim.mv1",true,1.0f,2),
	};

	Animator* horseAnim;
};