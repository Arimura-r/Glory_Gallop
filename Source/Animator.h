#pragma once
#include <unordered_map>
#include <string>
//#include "../YLibrary/Component.h"
#include "AnimDefine.h"
#include <vector>
#include "Util.h"

/// <summary>
/// モデルごとにアニメーションの管理をするためのクラス　綾部
/// </summary>
class Animator
{
public:
	Animator();
	~Animator();
	void Update();

	// 自身のコピーをセットする(複製用)
	void Copy(Animator* _anim) const;

	int GetCurrentID()const;
	float GetCurrentFrame(const int& _id);
	float GetCurrentAnimCurrentFrame();
	float GetCurrentAnimFrameSpeed();
	float GetCurrentDefAnimSpeedRate();
	bool GetDidLoop();

	void SetModel(const int& _hModel) { hModel = _hModel; }
	void SetBlendFrame(const int& _id, const float& _blendFrame) { infos[_id].blendFrame = _blendFrame; }
	void SetCurrentAnimSpeedRate(const float& _rate);

	// アニメーションを読み込んで登録する(単体)
	void AddFile(const AnimSetting& _animSetting);
	// アニメーションを読み込んで登録する(複数)
	void AddFile(const std::vector<AnimSetting>& _animSetting);

	// 再生
	void Play(const int& _id);
	// 指定と再生中のIDが違うなら再生
	void PlayOnce(const int& _id);
	// 現在再生中のアニメーションが終了したか
	bool IsFinish();

	// 再生中のアニメーションを最初のフレームにする
	void ResetCurrentFrame() { infos[GetCurrentID()].currentFrame = 0; }
	// ブレンドフレームを初期設定値に戻す
	void ResetBlendFrame(const int& _id) { infos[_id].blendFrame = infos[_id].set.blendFrame; }

	// 再生位置があるフレームを過ぎたか(指定の値と同じ時もtrue)
	bool JudgeCurrentFramePassed_Included(const float& _frame);
	// 再生位置があるフレームを過ぎたか(指定の値より大きい時true)
	bool JudgeCurrentFramePassed_After(const float& _frame);

	// 再生位置があるフレームを過ぎたか一度だけ判定する(指定の値と同じ時もtrue)
	bool JudgeCurrentFramePassedOnce_Included(const float& _frame);
	// 再生位置があるフレームを過ぎたか一度だけ判定する(指定の値より大きい時true)
	bool JudgeCurrentFramePassedOnce_After(const float& _frame);

private:
	// 再生時の初期化処理
	void PlayInit(const int& _id);
	// 再生フレームを進める
	void PlayingFrame(const int& _id);
	// ブレンドを進める
	void PlayingBlendRate(AnimInfo& _info, const bool& _isCurrent);
	// 指定の再生位置を現在の再生範囲に収めて返す
	float GetCurrentFrameClamp(const float& _frame);
	float GetCurrentAnimPrevFrame();

	// アニメーションをデタッチするかの基準となるブレンドの比重
	const float DETACH_BLEND_WEIGHT = 0.001f;

	// アニメーション情報コンテナ(アニメーション番号と情報のセット)
	std::unordered_map<int, AnimInfo>infos;
	int hModel; // アニメーションをセットするモデルハンドル
	std::vector<int> playingIDs; // 再生中アニメーションのID
};
