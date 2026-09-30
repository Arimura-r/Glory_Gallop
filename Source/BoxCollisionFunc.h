#pragma once
#include "../Library/myDxLib.h"

namespace BoxCollisionFunc
{
	//pos1,pos2の中から小さい方の値をそれぞれ入れて返す
	VECTOR3 GetMinPosition(VECTOR3 _pos1, VECTOR3 _pos2);
	//pos1,pos2の中から大きい方の値をそれぞれ入れて返す
	VECTOR3 GetMaxPosition(VECTOR3 _pos1, VECTOR3 _pos2);

	/// <summary>
	/// _posが_boxPos1,_boxPos2の範囲の直方体の中に入っているかを調べる
	/// </summary>
	/// <param name="_pos">直方体に当たっているかを調べる座標</param>
	/// <param name="_boxPos1">直方体の片方の角の座標</param>
	/// <param name="_boxPos2">_boxPos1の反対の角の座標</param>
	/// <returns></returns>
	bool IsInBox(VECTOR3 _pos, VECTOR3 _boxPos1, VECTOR3 _boxPos2);
}