#pragma once
#include "../Library/GameObject.h"
#include "Screen.h"

class Horse;

class UIBase :public GameObject
{
public:
	UIBase() {};
	UIBase(VECTOR2 _position);
	UIBase(VECTOR2 _position, Horse* _target,bool _active=true);
	virtual ~UIBase();
	virtual void DrawUI() {};

	bool GetIsActive()const { return active; }
	void SetActive(bool _active) { active = _active; }

	//情報を表示する対象(プレイヤーを変更する)
	void SetTarget(Horse* _target) { target = _target; }

protected:
	VECTOR2 position;
	Horse* target;//情報を表示する対象(基本的にはプレイヤー)
	bool active;//trueの時のみ動かす
};