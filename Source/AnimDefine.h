#pragma once
#include "../Library/myDxLib.h"
#include <string>
#include "Util.h"

/// <summary>
/// アニメーション管理に必要な値　綾部
/// </summary>

// アニメーション設定値
struct AnimSetting
{
	int id = -1;				// アニメーション番号
	std::string filePath = "";	// 素材ファイルのパス
	bool doLoop = false;		// ループ再生するか
	Util::FParam speedRate;		// 再生速度(フレーム単位)
	Util::FParam blendFrame;	// 前アニメーションからブレンドする時間(フレーム単位)
	float startFrame = 0;		// 開始フレーム
	float endFrame = -1;		// 終了フレーム(-1で素材のデフォルト値)
	int animIndex = 0;			// アニメーションの番号

	AnimSetting() {}
	AnimSetting(const int& _id,
		const std::string& _filePath =		"",
		const bool& _doLoop =				false,
		const Util::FParam& _frameSpeed =	1.0f,
		const int& _animIndex = -1,
		const Util::FParam& _blendFrame =	10,
		const float& _startFrame =			0,
		const float& _endFrame =			-1)
	{
		id = _id;
		filePath = _filePath;
		doLoop = _doLoop;
		speedRate = _frameSpeed;
		animIndex = _animIndex;
		blendFrame = _blendFrame;
		startFrame = _startFrame;
		endFrame = _endFrame;
	}
};

// アニメーションが持つ情報
struct AnimInfo
{
	int handle = -1;		 // アニメーション素材のハンドル
	int attachID = -1;		 // アタッチ番号
	AnimSetting set;		 // 設定値
	float currentFrame = 0;	 // 現在の再生位置
	float prevFrame = 0;	 // 前回の再生位置
	Util::FParam speedRate;	 // 再生速度倍率(何フレームずつ進めるか)
	float defSpeedRate = 0;	 // 再生速度倍率の初期値(speedRateのvalueか乱数範囲の最小が入る)
	Util::FParam blendFrame; // 前アニメーションからブレンドする時間(フレーム単位)
	float blendRate = 0;	 // 現在のブレンド倍率 
	bool didLoop = false;	 // 最初のループをおこなったか

	AnimInfo() {}
};