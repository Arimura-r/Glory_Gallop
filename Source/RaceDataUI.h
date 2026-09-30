#pragma once
#include "UIBase.h"

class CourseDataManager;
class HorseManager;
class GameManager;

//レースの情報(レース名、頭数、馬場など)を表示するクラス
class RaceDataUI :public UIBase
{
public:
	RaceDataUI(VECTOR2 _position,CourseDataManager* _courseData,HorseManager* _horseManager,GameManager* _gameManager);
	~RaceDataUI();
	
	void DrawUI()override;

	void LoadRaceData();//別の箇所に移す可能性あり

private:

	//レースデータ　他の箇所でも使うことになれば定義を移動させる
	//路盤の種類
	enum SurfaceType
	{
		TURF=0,
		DIRT,
	};
	//路盤タイプを日本語名の文字列に変換する
	std::string SurfaceTypeConvStr(SurfaceType _surface);
	//周りの種類
	enum TurnDirection
	{
		STRAIGHT=0,//直線
		LEFT,//左回り
		RIGHT,//右回り
	};
	//回りの方向をを日本語名の文字列に変換する
	std::string TurnDirectionConvStr(TurnDirection _turnDir);
	//天気の種類
	enum Weather
	{
		SUNNY=0,//晴れ
		CLOUDY,//曇り
		DRIZZLE,//小雨
		RAINY,//雨
		SNOWY,//雪
	};
	//天気を日本語名の文字列に変換する
	std::string WeatherConvStr(Weather _weather);
	//馬場状態
	enum TrackCondition
	{
		FIRM = 0,//良
		GOOD,//稍重
		YIELDING,//重
		SOFT,//不良
	};
	//馬場状態を日本語名の文字列に変換する
	std::string TrackConditionConvStr(TrackCondition _condition);
	struct RaceData
	{
		std::string raceName;//レース名
		std::string raceGrade;//レースの格
		std::string raceTrack;//競馬場の名前
		SurfaceType surfaceType;
		TurnDirection turnDirection;
		Weather weather;
		TrackCondition trackCondition;
		float distance;//距離
		int horseNum;//出走頭数

		RaceData()
		{
			surfaceType = TURF;
			turnDirection = STRAIGHT;
			weather = SUNNY;
			trackCondition = FIRM;
			distance = 0.0f;
			horseNum = 0;
		}
	};

	RaceData data;

	CourseDataManager* courseData;
	HorseManager* horseManager;
	GameManager* gameManager;
};