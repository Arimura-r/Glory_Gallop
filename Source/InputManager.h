#pragma once
#include <list>
#include <unordered_map>
#include "../Library/gameObject.h"
#include "controllerInputManager.h"
#include "keyboardInputManager.h"
#include "mouseInputManager.h"

class ControllerInputManager;
class KeyboardInputManager;
class MouseInputManager;

struct InputConfigData
{
	PadInputType padInput;
	int keyboardInput;
	MouseInputType mouseInput;

	InputConfigData() { padInput = P_NONE; keyboardInput = -1; mouseInput = M_NO_CONTROL; }
	InputConfigData(PadInputType _padInput, int _keyInput, MouseInputType _mouseInput) { padInput = _padInput; keyboardInput = _keyInput; mouseInput = _mouseInput; }
};

struct KeyConfigData
{
	std::string keyName;
	InputConfigData inputData;

	KeyConfigData() {};
	KeyConfigData(std::string _name, PadInputType _padInput, int _keyInput, MouseInputType _mouseInput)
	{
		keyName = _name;
		InputConfigData data(_padInput, _keyInput, _mouseInput);
		inputData = data;
	}
};

/// <summary>
/// プレイヤーの操作の入力を管理するクラス
///	シーンを跨いで存在できるようにしているためBootSceneで生成推奨
/// </summary>
class InputManager :public GameObject
{
public:
	/// <summary>
	/// 使用したいデバイスのみ生成推奨
	/// 生成したデバイスはGet関数で取得可能
	/// </summary>
	/// <param name="_useController">コントローラー入力を取るか</param>
	/// <param name="_useKeyboard">キーボード入力を取るか</param>
	/// <param name="_useMouse">マウス入力を取るか</param>
	InputManager(bool _useController,bool _useKeyboard,bool _useMouse);
	~InputManager();
	void Update()override;

	/// <summary>
	/// keyconfigを使用して入力を受け取る
	/// </summary>
	/// <param name="_keyName">keyconfigの名前</param>
	/// <returns>そのキーが押されているかどうか</returns>
	bool GetKeyConfigPush(std::string _keyName)const;
	/// <summary>
	/// keyconfigを使用して入力を受け取る
	/// </summary>
	/// <param name="_keyName">keyconfigの名前</param>
	/// <returns>そのキーが押され始めた1フレーム目のみTrue</returns>
	bool GetKeyConfigPut(std::string _keyName)const;

	/// <summary>
	/// キーコンフィグを新たに生成するか、既に指定したキーの要素があるならその要素を書き換える
	/// </summary>
	/// <param name="_ConfigData">キーコンフィグのデータ</param>
	/// <returns>新しく生成したなら1、既にあるものを変えたなら0を返す</returns>
	int CreateOrChangekeyConfig(KeyConfigData _ConfigData);

	///<summary>指定したキーボードのキーかコントローラーのボタンが押されている間Trueを返す</summary>
	//bool GetIsKeyOrButtonPushingNow(int _key, int _button);
	///<summary>指定したキーボードのキーかコントローラーのボタンの押し始め1F目のみTrueを返す</summary>
	//bool GetIsKeyOrButtonPutNow(int _key, int _button);
	///<summary>指定したキーボードのキーかコントローラーのボタンを押して離し始めた1F目のみTrueを返す</summary>
	//bool GetIsKeyOrButtonReleaseNow(int _key, int _button);

	ControllerInputManager* GetControllerInput();
	KeyboardInputManager* GetKeyboardInput();
	MouseInputManager* GetMouseInput();

	//何フレーム前の入力まで覚えておくか
	static const int SAVE_FRAME_MAX;

private:
	ControllerInputManager* controller;
	KeyboardInputManager* keyboard;
	MouseInputManager* mouse;
	std::unordered_map<std::string, InputConfigData>keyConfig;
};