#pragma once
#include "../Library/myDxLib.h"
#include <string>

namespace TimeStringFunc
{
	/// <summary>
	/// タイムを表示する
	/// </summary>
	/// <param name="_time">時間の変数</param>
	/// <param name="_intervalChar">時間と分、分と秒の間のテキスト</param>
	/// <param name="_decimalPlaces">小数以下の桁数　0ならそもそも表示しない</param>
	/// <param name="_drawZero">一番上の桁が0の時に表示するかどうか</param>
	std::string GetTimeString(float _time,char _intervalChar,int _decimalPlaces=1,bool _drawZero=false);
}