#pragma once
#include "../Library/GameObject.h"
#include "inputManager.h"

class KeyboardInputManager :public GameObject
{
public:
	KeyboardInputManager();
	~KeyboardInputManager();
	void Update()override;

	///<summary>キーボード用　指定したキーが押されている間trueを返す</summary>
	/// <param name="_keyCode">キーの種類</param>
	bool GetIsKeyboardPushing(int _keyCode);
	///<summary>キーボード用　指定したキーが押され始めた1Fのみ間trueを返す</summary>
	/// <param name="_keyCode">キーの種類</param>
	bool GetIsKeyboardPut(int _keyCode);
	///<summary>キーボード用　指定したキーを押して離した1Fのみ間trueを返す</summary>
	/// <param name="_keyCode">キーの種類</param>
	bool GetIsKeyboardRelease(int _keyCode);
private:
	//キーボードの入力を保存しておく配列の大きさ
	static const int KEYBOARD_BUF_NUM = 256;
	//キーボード入力のリスト用
	struct KeyboardInput
	{
		char kInput[KeyboardInputManager::KEYBOARD_BUF_NUM];
	};
	KeyboardInput keyboardInputNow;
	std::list<KeyboardInput>keyboardInput;

};