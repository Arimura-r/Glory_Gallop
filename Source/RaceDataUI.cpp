#include "RaceDataUI.h"
#include "CsvReader.h"
#include "CourseDataManager.h"
#include "HorseManager.h"
#include "GameManager.h"
#include "BeforeRaceState.h"
#include <assert.h>

namespace
{
	const std::string RACE_DATA_FILE_PATH= "data/csv/raceData.csv";
	const int RACE_NAME_FONT_SIZE = 64;
	const int RACE_DATA_FONT_SIZE = 32;
	const float RACE_DATA_OFFSET_Y = 100.0f;

	const VECTOR2 BACK_BOX_SIZE = VECTOR2(900,200);//背景の四角の大きさ
	const int BACK_BOX_ALPHA = 128;
}

RaceDataUI::RaceDataUI(VECTOR2 _position, CourseDataManager* _courseData, HorseManager* _horseManager, GameManager* _gameManager):UIBase(_position),
courseData(_courseData),horseManager(_horseManager),gameManager(_gameManager)
{
	LoadRaceData();
}

RaceDataUI::~RaceDataUI()
{
}

void RaceDataUI::DrawUI()
{
	if (gameManager->GetBeforeRaceState()->GetCurrentBeforeRaceSelect() != BeforeRaceState::RACE_DATA)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_BOX_ALPHA);
	//テキストの背景に黒四角を置く
	DrawBox((int)position.x, (int)position.y, (int)position.x + (int)BACK_BOX_SIZE.x, (int)position.y + (int)BACK_BOX_SIZE.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetFontSize(RACE_NAME_FONT_SIZE);
	DrawFormatStringF(position.x, position.y, 0xffffff, "%s", data.raceName.c_str());

	std::string surface = SurfaceTypeConvStr(data.surfaceType);
	std::string turnDir = TurnDirectionConvStr(data.turnDirection);
	std::string weather = WeatherConvStr(data.weather);
	std::string condition = TrackConditionConvStr(data.trackCondition);
	std::string distance = std::to_string((int)data.distance/100) + "m";
	std::string horseNum = std::to_string(data.horseNum) + "頭";

	SetFontSize(RACE_DATA_FONT_SIZE);
	DrawFormatStringF(position.x, position.y + RACE_DATA_OFFSET_Y, 0xffffff,
		"%s %s %s%s %s %s %s %s ", data.raceGrade.c_str(), data.raceTrack.c_str(), surface.c_str(), distance.c_str(), turnDir.c_str(), weather.c_str(), condition.c_str(), horseNum.c_str());
}

void RaceDataUI::LoadRaceData()
{
	CsvReader* csv = new CsvReader();
	csv->Load(RACE_DATA_FILE_PATH);

	data.raceName = csv->GetString(1, 0);
	data.raceGrade = csv->GetString(1, 1);
	data.raceTrack = csv->GetString(1, 2);
	data.surfaceType = (SurfaceType)csv->GetInt(1, 3);
	data.turnDirection= (TurnDirection)csv->GetInt(1, 4);
	data.weather = (Weather)csv->GetInt(1, 5);
	data.trackCondition = (TrackCondition)csv->GetInt(1, 6);

	delete csv;

	data.distance = courseData->GetCourseAllDistance();
	data.horseNum = (int)horseManager->GetHorses().size();
}

std::string RaceDataUI::SurfaceTypeConvStr(SurfaceType _surface)
{
	std::string ret;
	switch (_surface)
	{
	case TURF:
		ret = "芝";
		break;
	case DIRT:
		ret = "ダート";
		break;
	default:
		assert(false && "路盤タイプに異常な値が入力されています");
		break;
	}
	return ret;
}

std::string RaceDataUI::TurnDirectionConvStr(TurnDirection _turnDir)
{
	std::string ret;
	switch (_turnDir)
	{
	case STRAIGHT:
		ret = "直線";
		break;
	case LEFT:
		ret = "左回り";
		break;
	case RIGHT:
		ret = "右回り";
		break;
	default:
		assert(false && "周りの方向に異常な値が入力されています");
		break;
	}
	return ret;
}

std::string RaceDataUI::WeatherConvStr(Weather _weather)
{
	std::string ret;
	switch (_weather)
	{
	case SUNNY:
		ret = "晴れ";
		break;
	case CLOUDY:
		ret = "曇り";
		break;
	case DRIZZLE:
		ret = "小雨";
		break;
	case RAINY:
		ret = "雨";
		break;
	case SNOWY:
		ret = "雪";
		break;
	default:
		assert(false && "天気に異常な値が入力されています");
		break;
	}
	return ret;
}

std::string RaceDataUI::TrackConditionConvStr(TrackCondition _condition)
{
	std::string ret;
	switch (_condition)
	{
	case FIRM:
		ret = "良";
		break;
	case GOOD:
		ret = "稍重";
		break;
	case YIELDING:
		ret = "重";
		break;
	case SOFT:
		ret = "不良";
		break;
	default:
		assert(false && "馬場状態に異常な値が入力されています");
		break;
	}
	return ret;
}
