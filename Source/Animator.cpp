#include "Animator.h"
#include <assert.h>
#include "../Library/myDxLib.h"
#include "../Library/Time.h"
//#include "../YLibrary/EmptyObject.h"
//#include "../YLibrary/MeshRender.h"

Animator::Animator()
{
	infos.clear();
	hModel = -1;
}

Animator::~Animator()
{
	// アタッチされているアニメを全部外す
	for (auto& info : infos)
	{
		if (info.second.attachID >= 0 && hModel >= 0)
		{
			MV1DetachAnim(hModel, info.second.attachID);
			info.second.attachID = -1;
		}
	}

	// アニメ素材を削除
	for (auto& info : infos)
	{
		if (info.second.handle > 0)
		{
			MV1DeleteModel(info.second.handle);
			info.second.handle = -1;
		}
	}

	infos.clear();
}

void Animator::Update()
{
	if (GetCurrentID() < 0)
		return;

	float sum = 0; // 全ブレンド倍率の合計(比重を求めるために必要)
	for (int i = 0; i < (int)playingIDs.size(); i++)
	{
		int id = playingIDs[i];
		// 再生フレームを進める
		PlayingFrame(id);
		AnimInfo& info = infos[id];
		MV1SetAttachAnimTime(hModel, info.attachID, info.currentFrame);

		// ブレンドを進める
		PlayingBlendRate(info, (i == 0));
		sum += info.blendRate;
	}
	// ０除算防止
	if (sum <= 0)
		sum = 1.0f;

	int i = 0;
	while (i < (int)playingIDs.size())
	{
		int id = playingIDs[i];
		AnimInfo& info = infos[id];
		// 過去のもので、ブレンド倍率が０に近いものは無効化して次の要素へ
		if (i != 0 && info.blendRate <= DETACH_BLEND_WEIGHT)
		{
			MV1DetachAnim(hModel, info.attachID);
			info.attachID = -1;
			playingIDs.erase(playingIDs.begin() + i);
			continue;
		}
		// 各ブレンド倍率の比重
		float weight = info.blendRate / sum;
		// 最終的なブレンド倍率(比重)を反映
		MV1SetAttachAnimBlendRate(hModel, info.attachID, weight);
		i++;
	}
}

void Animator::Copy(Animator* _anim) const
{
	_anim->infos = this->infos;
	_anim->hModel = this->hModel;
	for (auto& info : _anim->infos)
	{
		if (info.second.handle > 0)
			info.second.handle = MV1DuplicateModel(info.second.handle);
		info.second.attachID = -1;
	}
	_anim->playingIDs.clear();
}

int Animator::GetCurrentID() const
{
	if ((int)playingIDs.size() <= 0)
		return -1;
	return playingIDs[0];
}

float Animator::GetCurrentFrame(const int& _id)
{
	return infos[_id].currentFrame;
}

float Animator::GetCurrentAnimCurrentFrame()
{
	return infos[GetCurrentID()].currentFrame;
}

float Animator::GetCurrentAnimFrameSpeed()
{
	return infos[GetCurrentID()].speedRate.Get() * 30 * Time::DeltaTime();
}

float Animator::GetCurrentDefAnimSpeedRate()
{
	return infos[GetCurrentID()].defSpeedRate;
}

bool Animator::GetDidLoop()
{
	return infos[GetCurrentID()].didLoop;
}

void Animator::SetCurrentAnimSpeedRate(const float& _rate)
{
	infos[GetCurrentID()].speedRate.value = _rate;
}

void Animator::AddFile(const AnimSetting& _animSetting)
{
	AnimInfo info;
	// 素材ファイル読み込み
	info.handle = MV1LoadModel(_animSetting.filePath.c_str());
	assert(info.handle >= 0);

	// 設定情報を反映
	info.set = _animSetting;
	info.speedRate = _animSetting.speedRate;
	float s = info.speedRate.value;
	// 単体の値しか入っていないならそのまま、乱数範囲が入っているならその最小値を代入
	info.defSpeedRate = (s != 0) ? s : info.speedRate.rand.range.mini;
	info.set.blendFrame = _animSetting.blendFrame;
	info.blendFrame = _animSetting.blendFrame;

	infos.insert({ _animSetting.id, info });
}

void Animator::AddFile(const std::vector<AnimSetting>& _animSetting)
{
	for (const auto& set : _animSetting)
		AddFile(set);
}

void Animator::Play(const int& _id)
{
	// 再生中のものなら開始フレームに戻して終了
	if (GetCurrentID() == _id)
	{
		AnimInfo& cur = infos[GetCurrentID()];
		cur.currentFrame = cur.set.startFrame;
		cur.prevFrame = cur.currentFrame;
		MV1SetAttachAnimTime(hModel, cur.attachID, cur.currentFrame);
		return;
	}

	// 既に再生中の同じID
	int same = -1;
	for (int i = 0; i < (int)playingIDs.size(); i++)
	{
		if (playingIDs[i] == _id)
		{
			same = i;
			break;
		}
	}
	// 既に再生中の過去アニメがあったら
	if (same > 0 && same < (int)playingIDs.size())
	{
		// そのIDを先頭(現在)に移動
		int id = playingIDs[same];
		playingIDs.erase(playingIDs.begin() + same);
		playingIDs.insert(playingIDs.begin(), id);

		AnimInfo& cur = infos[id];
		// 現在はフェードイン開始
		cur.blendRate = 0.0f;
		// 過去はフェードアウト開始にセット
		// playingIDs[1]で現在より前から回す
		for (int i = 1; i < (int)playingIDs.size(); ++i)
		{
			int pID = playingIDs[i];
			infos[pID].blendRate = 1.0f;
		}
		// 初期化
		PlayInit(id);
		return;
	}

	// ----ここからは新規アニメの場合------

	AnimInfo& info = infos[_id];
	// １個目ならブレンド倍率を１にしてフェードなし
	bool isFirst = playingIDs.empty();
	info.blendRate = isFirst ? 1.0f : 0;

	// 頭に最新IDを追加
	playingIDs.insert(playingIDs.begin(), _id);

	// 設定を反映する
	info.attachID = MV1AttachAnim(hModel, info.set.animIndex, info.handle);
	// 設定の終了フレームが０未満なら素材元の再生時間にする
	if (info.set.endFrame < 0)
		info.set.endFrame = MV1GetAttachAnimTotalTime(hModel, info.attachID);
	// 初期化
	PlayInit(_id);

	// 再生フレームとブレンド倍率を反映
	MV1SetAttachAnimTime(hModel, info.attachID, info.currentFrame);
	MV1SetAttachAnimBlendRate(hModel, info.attachID, info.blendRate);
}

void Animator::PlayOnce(const int& _id)
{
	if (GetCurrentID() == _id)
		return;
	Play(_id);
}

bool Animator::IsFinish()
{
	const AnimInfo& cur = infos[GetCurrentID()];
	return (cur.currentFrame >= cur.set.endFrame);
}

bool Animator::JudgeCurrentFramePassed_Included(const float& _frame)
{
	float f = GetCurrentFrameClamp(_frame);
	// ループした瞬間で、前フレームが指定値より前なら通過しているためtrue
	if (GetDidLoop() && GetCurrentAnimPrevFrame() <= f)
		return true;
	return f >= _frame;
}

bool Animator::JudgeCurrentFramePassed_After(const float& _frame)
{
	float f = GetCurrentFrameClamp(_frame);
	// ループした瞬間で、前フレームが指定値より前なら通過しているためtrue
	if (GetDidLoop() && GetCurrentAnimPrevFrame() <= f)
		return true;
	return f > _frame;
}

bool Animator::JudgeCurrentFramePassedOnce_Included(const float& _frame)
{
	float f = GetCurrentFrameClamp(_frame);
	// ループした瞬間で、前フレームが指定値より前なら通過しているためtrue
	if (GetDidLoop() && GetCurrentAnimPrevFrame() < f)
		return true;
	return (GetCurrentAnimPrevFrame() < f && GetCurrentAnimCurrentFrame() >= f);
}

bool Animator::JudgeCurrentFramePassedOnce_After(const float& _frame)
{
	float f = GetCurrentFrameClamp(_frame);
	// ループした瞬間で、前フレームが指定値より前なら通過しているためtrue
	if (GetDidLoop() && GetCurrentAnimPrevFrame() <= f)
		return true;
	return (GetCurrentAnimPrevFrame() <= f && GetCurrentAnimCurrentFrame() > f);
}

void Animator::PlayInit(const int& _id)
{
	AnimInfo& info = infos[_id];
	info.currentFrame = info.set.startFrame;
	info.prevFrame = info.currentFrame;
	info.speedRate.DoRand();
	info.blendFrame.DoRand();
}

void Animator::PlayingFrame(const int& _id)
{
	AnimInfo& cur = infos[_id];
	cur.prevFrame = cur.currentFrame;
	cur.didLoop = false; // ループする瞬間を確認するため
	// 終了フレームに達していなければ終了
	if (cur.currentFrame < cur.set.endFrame)
	{
		cur.currentFrame += cur.speedRate.Get() * 30 * Time::DeltaTime();
		return;
	}

	// ループする場合
	if (cur.set.doLoop)
	{
		cur.currentFrame -= cur.set.endFrame - cur.set.startFrame;
		// ループした瞬間を知らせる
		cur.didLoop = true;
	}
	// ループしない場合
	else
	{
		cur.currentFrame = cur.set.endFrame;
	}
}

void Animator::PlayingBlendRate(AnimInfo& _info, const bool& _isCurrent)
{
	float bFrame = _info.blendFrame.Get();
	// 過去アニメは現在アニメのブレンドフレームでフェードアウトする
	if (not _isCurrent && GetCurrentID() >= 0)
		bFrame = infos[GetCurrentID()].blendFrame.Get();

	// １フレームあたりのブレンド増加量
	float delta = (30 * Time::DeltaTime()) / bFrame;
	if (_isCurrent)
		_info.blendRate += delta; // フェードイン
	else
		_info.blendRate -= delta; // フェードアウト
	Util::Clamp(_info.blendRate, Util::Range(0.0f, 1.0f));
}

float Animator::GetCurrentFrameClamp(const float& _frame)
{
	const AnimInfo& cur = infos[GetCurrentID()];
	float f = _frame;
	if (f < cur.set.startFrame)
		f = cur.set.startFrame;
	return f;
}

float Animator::GetCurrentAnimPrevFrame()
{
	return infos[GetCurrentID()].prevFrame;
}