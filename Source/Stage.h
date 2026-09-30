#pragma once
#include "../Library/Object3D.h"

class Stage :public Object3D
{
public:
	Stage();
	~Stage();

	/// <summary>
	/// 線とステージが当たっているかどうかを判定する(接地判定に使用)　当たっているなら当たった座標を送る
	/// </summary>
	/// <param name="_pos1">線の始点</param>
	/// <param name="_pos2">線の終点</param>
	/// <param name="_hitPos">線と当たった座標があるなら返す</param>
	/// <returns></returns>
	bool CollideLine(VECTOR3 _pos1, VECTOR3 _pos2, VECTOR3& _hitPos);
};

class StageLine :public Object3D
{
public:
	StageLine();
	~StageLine();
};

class StageSeat :public Object3D
{
public:
	StageSeat();
	~StageSeat();
};

class StageWall :public Object3D
{
public:
	StageWall();
	~StageWall();
};