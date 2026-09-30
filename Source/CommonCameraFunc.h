#pragma once
#include "../Library/myDxLib.h"

namespace CommonCameraFunc
{

	//ToDo Lerpにも対応させる
	//現在地点と目標地点の補間し、次フレームの座標を得る
	VECTOR3 MoveBitweenPosition(VECTOR3 _currentPos,VECTOR3 _goalPos,float _moveSpeed);
	//ズームする
	VECTOR3 ZoomIn(VECTOR3 _cameraPos,VECTOR3 _cameraTarget,float _distance);
}