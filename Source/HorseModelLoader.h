#pragma once
#include "../Library/GameObject.h"
#include <vector>
#include <unordered_map>

class HorseModelLoader :public GameObject
{
public:

	enum BodyColor
	{
		BAY_BLACK=0,
		BAY_WHITE,
		BLACK_BLACK,
		BLACK_WHITE,
		CHESTNUT_BLACK,
		CHESTNUT_WHITE,
		GRAY,
		BLACK,
		TAN_BLACK,
		TAN_WHITE,
		WHITE_BLACK,
		WHITE_WHITE,

		COLOR_NUM,
	};

	HorseModelLoader();
	~HorseModelLoader();

	int GetModel()const { return hModel; }
	//コピーしたハンドルを渡す
	int GetDupliModel(BodyColor _color)const;

private:
	const std::string MODEL_FILE_PATH = "data\\model\\character\\HorseModel.mv1";
	const std::string TEXTURE_FILE_PATH = "data\\model\\character\\texture\\";
	
	const std::vector<std::pair<BodyColor,std::string>> BODY_TEXTURE_NAME_LIST
	{
		{BAY_BLACK,"Horse_Body_BayBlack.png"},
		{BAY_WHITE,"Horse_Body_BayWhite.png"},
		{BLACK_BLACK,"Horse_Body_BlackBlack.png"},
		{BLACK_WHITE,"Horse_Body_BlackWhite.png" },
		{CHESTNUT_WHITE,"Horse_Body_ChestnutBlack.png"},
		{CHESTNUT_BLACK,"Horse_Body_ChestnutWhite.png"},
		{GRAY,"Horse_Body_Specular.png" },
		{BLACK,"Horse_Body_Specular2.png" },
		{TAN_BLACK,"Horse_Body_TanBlack.png"},
		{TAN_WHITE,"Horse_Body_TanWhite.png"},
		{WHITE_BLACK,"Horse_Body_WhiteBlack.png"},
		{WHITE_WHITE,"Horse_Body_WhiteWhite.png"},
	};

	int hModel;
	std::unordered_map<BodyColor,int> bodyTextureImages;

	void AllTextureLoad();
};