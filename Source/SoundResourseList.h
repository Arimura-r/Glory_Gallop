#pragma once
#include <string>
#include <map>
//将来的には外部化を予定

namespace SoundResource
{
	inline const std::string SOUND_FOLDER_PATH = "data\\sound\\";

	struct SoundData
	{
		SoundData()
		{
			filePath = "";
			useScene = "";
			baseVolume = -1;
		}
		SoundData(std::string _filePath, std::string _useScene, int _baseVolume)
			:filePath(_filePath), useScene(_useScene), baseVolume(_baseVolume)
		{}

		std::string filePath;//サウンドフォルダからのファイルパス
		std::string useScene;//使うシーン　Commonなら全シーン共通で使う
		int baseVolume;//基礎ボリューム(0~255)
	};

	inline const std::map<std::string,SoundData> SOUND_LIST
	{
		{"Title_BGM",SoundData("bgm_title.wav","TITLE",128),},
		{"Button_Select",SoundData("se_select_button.wav","COMMON",200),},
		{"Select_Change",SoundData("se_select_change.wav","COMMON",200),},
		{"Play_BGM",SoundData("bgm_play.mp3","PLAY",64), },
		{"Start_Count",SoundData("se_count.wav","PLAY",200),},
		{"Gate_Open",SoundData("se_gate_open.wav","PLAY",200),},
		{"Whip",SoundData("se_whip.wav","PLAY",200),},
		{"Footstep",SoundData("se_footstep.wav","PLAY",100),},
		{"Goal",SoundData("se_goal.wav","PLAY",128),},
		{"Wind",SoundData("se_wind.wav","PLAY",128),},
		{"Cheer",SoundData("se_cheer.wav","PLAY",200),},
		{"Cannot",SoundData("se_cannot.wav","PLAY",200),},
	};

	inline const std::map<std::string, SoundData>& GetSoundData()
	{
		return SOUND_LIST;
	}
}