#include "SoundManager.h"
#include <assert.h>

SoundManager::SoundManager()
{
	DontDestroyOnSceneChange(true);
	SoundManager::LoadResource("COMMON");
}

SoundManager::~SoundManager()
{
	DeleteResource("COMMON");
}

void SoundManager::Update()
{
	//再生中のサウンドがまだ再生しているかを確認する
	for (auto it = playingSounds.begin(); it != playingSounds.end();)
	{
		int handle = (*it).handle;
		if (CheckPlay(handle))
		{
			it++;
			continue;
		}

		if ((*it).dupli)
			DeleteSound(handle);
		else
			Stop(handle);

		it = playingSounds.erase(it);
	}

}

int SoundManager::Play(std::string _soundName, int _volume, bool _loop, bool _dupli)
{
	int handle = loadedSounds[_soundName].handle;
	if (_dupli)
		handle = DuplicateSoundMem(handle);

	int type = DX_PLAYTYPE_BACK;
	if (_loop)
		type = DX_PLAYTYPE_LOOP;

	int ret=PlaySoundMem(handle, type);

	//再生出来たなら再生中リストに入れる
	if (ret == 0)
		playingSounds.push_back(PlaySoundData(_soundName,handle,_dupli,loadedSounds[_soundName].soundData.useScene));
	else
		assert(false && "サウンド名が無効かロードされていません");

	ChangeVolumeSoundMem(CalcSoundVolume(loadedSounds[_soundName], _volume), handle);

	return handle;
}

int SoundManager::PlayOnce(std::string _soundName, int _volume, bool _loop, bool _dupli)
{
	//再生されているなら-1を返す
	if (CheckPlay(_soundName))
		return -1;
	
	int handle=Play(_soundName, _volume, _loop, _dupli);

	return handle;
}

void SoundManager::Stop(std::string _soundName)
{
	for(const auto& s : playingSounds)
	{
		if (s.soundName != _soundName)
			continue;

		StopSoundMem(s.handle);
	}
}

void SoundManager::Stop(int _handle)
{
	StopSoundMem(_handle);
}

bool SoundManager::CheckPlay(std::string _soundName)
{
	for (const auto& s : playingSounds)
	{
		if (s.soundName == _soundName)
			return true;
	}
	return false;
}

bool SoundManager::CheckPlay(int _handle)
{
	return CheckSoundMem(_handle);
}

void SoundManager::ChangeVolume(int _volume, int _handle)
{
	//サウンドを再生していなかったら無視する
	if (not CheckPlay(_handle))
		return;

	ChangeVolumeSoundMem(_volume, _handle);
}

void SoundManager::ChangeVolume(int _volume, std::string _soundName)
{
	for (const auto& s : playingSounds)
	{
		if (s.soundName != _soundName)
			continue;

		ChangeVolumeSoundMem(CalcSoundVolume(loadedSounds[_soundName],_volume), s.handle);
	}
}

void SoundManager::LoadResource(std::string _sceneName)
{
	auto soundList = SoundResource::GetSoundData();

	for (const auto& s : soundList)
	{
		if (s.second.useScene == _sceneName)
			LoadSound(s.first, s.second);
	}
}

void SoundManager::DeleteResource(std::string _sceneName)
{
	for (auto it=loadedSounds.begin();it!=loadedSounds.end();)
	{
		if ((*it).second.soundData.useScene == _sceneName)
		{
			DeleteSound((*it).second.handle);
			it = loadedSounds.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto it = playingSounds.begin(); it != playingSounds.end();)
	{
		if ((*it).useScene != _sceneName)
		{
			DeleteSound((*it).handle);
			it=playingSounds.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void SoundManager::LoadSound(std::string _name, const SoundResource::SoundData& _data)
{
	loadedSounds.emplace(_name, LoadedSoundData(_data, LoadSoundMem((SoundResource::SOUND_FOLDER_PATH + _data.filePath).c_str())));
}

void SoundManager::DeleteSound(int _handle)
{
	Stop(_handle);
	DeleteSoundMem(_handle);
}

int SoundManager::CalcSoundVolume(LoadedSoundData _data, int _volume)
{
	int volume = max(min(_volume, 100),0);//0~100に収める
	int baseVolume = _data.soundData.baseVolume;

	float volRate=(float)volume / 50;
	int playVol = (int)max(min(baseVolume * volRate, 255), 0);

	return playVol;
}
