#pragma once
#include "../Library/GameObject.h"
#include "SoundResourseList.h"
#include <vector>

class SoundManager :public GameObject
{
public :
	SoundManager();
	~SoundManager();
	void Update()override;

	/// <summary>
	/// 音声を再生する
	/// </summary>
	/// <param name="_soundName">サウンド名</param>
	/// <param name="_volume">ボリューム　初期化時に指定したの音量が50　最大が100、最小が0</param>
	/// <param name="_loop">ループするかどうか</param>
	/// <param name="_dupli">音声ハンドルを複製したものを再生するか(複数同時に再生するならtrue)</param>
	/// <returns></returns>
	int Play(std::string _soundName, int _volume = 50, bool _loop = false, bool _dupli = false);
	//現在その音声が再生されていないなら再生する　再生されたならハンドル　既に再生されてたら-1を返す
	int PlayOnce(std::string _soundName, int _volume = 50, bool _loop = false, bool _dupli = false);

	void Stop(std::string _soundName);
	void Stop(int _handle);

	bool CheckPlay(std::string _soundName);
	bool CheckPlay(int _handle);

	/// <summary>
	/// 音量を変更する
	/// </summary>
	/// <param name="_volume">ボリューム　0~255</param>
	/// <param name="_handle">変更したいハンドル</param>
	void ChangeVolume(int _volume,int _handle);
	/// <summary>
	/// 音量を変更する
	/// </summary>
	/// <param name="_volume">ボリューム　初期化時に指定した音量が50　最大が100、最小が0</param>
	/// <param name="_soundName">変更したいサウンドの名前(全ての同名のサウンドの再生音量に影響する)</param>
	void ChangeVolume(int _volume, std::string _soundName);

	//各シーンの始めに必ず呼ぶ　将来的にはSceneFactory内から呼ぶ予定
	void LoadResource(std::string _sceneName);
	//各シーンの最後に必ず呼ぶ　将来的にはSceneFactory内から呼ぶ予定
	void DeleteResource(std::string _sceneName);

private:
	//ロードしているサウンドのデータ
	struct LoadedSoundData
	{
		SoundResource::SoundData soundData;
		//std::string soundName;
		int handle;

		LoadedSoundData()
		{
			soundData = SoundResource::SoundData();
			handle = -1;
		}

		LoadedSoundData(SoundResource::SoundData _soundData, int _handle)
			:soundData(_soundData), handle(_handle)
		{}
	};
	//再生中のサウンドデータ
	struct PlaySoundData
	{
		std::string soundName;
		int handle;
		bool dupli;
		std::string useScene;

		PlaySoundData(std::string _soundName, int _handle, bool _dupli,std::string _useScene)
			:soundName(_soundName), handle(_handle),dupli(_dupli),useScene(_useScene)
		{}
	};

	//呼ばれたサウンドデータをロードする
	void LoadSound(std::string _name, const SoundResource::SoundData& _data);
	//呼ばれたサウンドデータを消す
	void DeleteSound(int _handle);

	//サウンドの初期値と指定された音量(0~100)から適切な音量で再生する
	int CalcSoundVolume(LoadedSoundData _data, int _volume);

	std::map<std::string,LoadedSoundData> loadedSounds;//ロードしているサウンド
	std::vector<PlaySoundData> playingSounds;//再生中のサウンドのリスト
};