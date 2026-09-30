#include "OddsManager.h"
#include "CsvReader.h"
#include "HorseManager.h"
#include "Horse.h"
#include "../Library/Random.h"
#include "ArrayNumberFunc.h"
#include <numeric>

namespace
{
	const std::string RACE_RESULT_DATA_FILE_PATH = "data/csv/goalData.csv";

	const float WIN_TICKET_PAY_OUT_RATE = 0.8f;//単勝馬券の払い戻し率
	const float PLACE_TICKET_PLACE_OUT_RATE = 0.8f;//複勝馬券の払い戻し率

	//const int FIRST_EVALUATE_NUM = 900;//1位だった際に人気指数をどれだけプラスするか
	//const int IN_THIRD_EVALUATE_NUM = 400;//2~3位だった際に人気指数をどれだけプラスするか
	//const int IN_FIFTH_EVALUATE_NUM = 300;//4~5位だった際に人気指数をどれだけプラスするか
	//const int IN_TENTH_EVALUATE_NUM = 50;//6~10位だった際に人気指数をどれだけプラスするか
	//const int LESS_TENTH_EVALUATE_NUM = 20;//11位以下だった際に人気指数をどれだけプラスするか

	const std::vector<int> RACE_RESULT_EVALUATE_NUM//レースの着順に応じて人気指数をどれだけプラスするか
	{ 10000,5000,4750,4000,3750,2000,1700,1400,1100,800,500,400,380,360,340,320,300,280 };
	const std::vector<float>RACE_RESULT_EVALUATE_BY_RACE_AGO//直近のレースほど人気への影響を大きくする　左ほど古く、右が前走
	{ 1.0f,1.2f,1.5f,2.0f,3.0f };

	const int CONSECTIVE_1st_ADD_EVALUATE_NUM = 11000;//連続で1着の際に加算する人気指数
	const int CONSECTIVE_OR_BETTER_3rd_ADD_EVALUATE_NUM = 3000;//連続で3着以内の際に加算する人気指数
	const int CONSECTIVE_OR_BETTER_5th_ADD_EVALUATE_NUM = 1000;//連続で5着以内の際に加算する人気指数
	const int CONSECTIVE_OR_WORTH_6th_SUB_EVALUATE_NUM = 50;//連続で6着以下の際に減算する人気指数
	const int CONSECTIVE_OR_WORTH_10th_SUB_EVALUATE_NUM = 50;//連続で10着以下の際に減算する人気指数

	const std::vector<float>CONSECTIVE_RATE{ 0.0f,2.0f,4.0f,7.0f,10.0f };//連続数に応じて掛ける人気指数の割合

	const float FAVORITE_FACTOR_RAND_RANGE = 0.1f;//ランダムの乱数幅

	//下2つからより大きい値になる方を使う
	const int VERY_BAD_CONDITION_SUB = 5000;//調子が悪い馬から減らす指数
	const float VERY_BAD_CONDITION_MULT = 0.6f;//調子が悪い馬から乗算する指数

	//下2つからより大きい値になる方を使う
	const int BAD_CONDITION_SUB = 2000;//調子が少し悪い馬から減らす指数
	const float BAD_CONDITION_MULT = 0.9f;//調子が少し悪い馬から乗算する指数

	//下2つからより小さい値になる方を使う
	const int GOOD_CONDITION_ADD = 4000;//調子が良い馬に加算する指数
	const float GOOD_CONDITION_MULT = 1.4f;//調子が良い馬から乗算する指数
}

OddsManager::OddsManager(HorseManager* _horse):horse(_horse)
{
	CalcFavoriteFactor();

	CalcFavoriteOrder();

	/*std::vector<float>a;

	for (int i = 0; i < 18; i++)
	{
		a.push_back(GetWinOdds(i));
	}
	int b = 3;*/
}

OddsManager::~OddsManager()
{
}

float OddsManager::GetWinOdds(int _horseNum) const
{
	//return ((sumFavoriteFactor - favoriteFactor[_horseNum]) / favoriteFactor[_horseNum]) * WIN_TICKET_PAY_OUT_RATE+1.0f;
	return sumFavoriteFactor * PLACE_TICKET_PLACE_OUT_RATE / favoriteFactor[_horseNum];
}

float OddsManager::GetPlaceOdds(int _horseNum) const
{
	int horseNum = (int)horse->GetHorses().size();

	int myFavFactor = favoriteFactor[_horseNum];//自分の人気指数

	//自分以外で1人気の馬の人気指数を求める
	int firstFavHorseNumber = favoriteOrder[0];
	if (firstFavHorseNumber == _horseNum)
		firstFavHorseNumber = favoriteOrder[1];
	int firstFavFactor = favoriteFactor[firstFavHorseNumber];//自分以外で1人気の人気指数

	//自分以外で2人気の馬の人気指数を求める
	int secondFavHorseNumber = favoriteOrder[1];
	if (secondFavHorseNumber == _horseNum|| secondFavHorseNumber == firstFavHorseNumber)
		secondFavHorseNumber = favoriteOrder[2];
	int secondFavFactor = favoriteFactor[secondFavHorseNumber];//自分以外で2人気の人気指数

	int sumHitFavoriteFactor = myFavFactor + firstFavFactor;//自分と自分以外で1人気の馬の人気指数

	//出走頭数が7頭以下なら2頭までしか当たらないようにする
	int hitHorseNum = 3;
	if (horseNum < 8)
		hitHorseNum = 2;
	else//8頭以上なら自分以外で2人気の馬も指数の合計に加える
		sumHitFavoriteFactor += secondFavFactor;

	return (myFavFactor + (sumFavoriteFactor - sumHitFavoriteFactor) / (float)hitHorseNum) * PLACE_TICKET_PLACE_OUT_RATE / myFavFactor;
}

int OddsManager::GetHorseFavoriteOrder(int _horseNum) const
{
	for (int i=0;i<favoriteOrder.size();i++)
	{
		if (favoriteOrder[i] == _horseNum)
			return i+1;
	}

	return -1;
}

void OddsManager::CalcFavoriteFactor()
{
	favoriteFactor.resize(horse->GetHorses().size());//頭数分の配列を先に確保する
	FavoriteFactorAddRaceResult(favoriteFactor);
	FavoriteFactorAddHorseCondition(favoriteFactor);
	CalcHorseFavoriteRate(favoriteFactor);
	RandFavoriteFactor(favoriteFactor);

	for (int& n : favoriteFactor)
	{
		n = max(1, n);//最低値を1にする
		sumFavoriteFactor += n;//合計値を求める
	}
}

void OddsManager::CalcFavoriteOrder()
{
	//そこに大きい順に要素番号を並べる
	favoriteOrder.resize(favoriteFactor.size());
	std::iota(favoriteOrder.begin(), favoriteOrder.end(), 0);

	for (int i = 0; i < favoriteOrder.size() - 1; i++)
	{
		for (int j = i + 1; j < favoriteOrder.size(); j++)
		{
			if (favoriteFactor[favoriteOrder[i]] < favoriteFactor[favoriteOrder[j]])
			{
				int tmp = favoriteOrder[i];
				favoriteOrder[i] = favoriteOrder[j];
				favoriteOrder[j] = tmp;
			}
		}
	}
}

void OddsManager::FavoriteFactorAddRaceResult(std::vector<int>& _favorites)
{
	CsvReader* csv = new CsvReader();
	csv->Load(RACE_RESULT_DATA_FILE_PATH);

	//ToDo　連続で好走している場合は追加で加算を行う

	//1行目を飛ばし、最後まで調べる　馬ごとに順に調べたいため、列ではなく行を先に調べる
	int l = 1;
	for (int c = 0; c < csv->GetColums(1); c++)
	{
		std::vector<int> results;
		for (l = 1; l < csv->GetLines(); l++)
		{
			results.push_back(csv->GetInt(l, c));
			_favorites[c] += (int)(EvaluateRaceResult(csv->GetInt(l, c)) * RACE_RESULT_EVALUATE_BY_RACE_AGO[l-1]);
		}
		std::reverse(results.begin(), results.end());//5走前から順に並んでいるので逆順にして渡す
		_favorites[c] += EvaluateConsectiveResult(results);//連続好走などの条件を調べる
	}

	delete csv;
}

int OddsManager::EvaluateRaceResult(int _raceResult)
{
	int ret = RACE_RESULT_EVALUATE_NUM[_raceResult];

	//1位なら
	/*if (_raceResult == 0)
		ret += FIRST_EVALUATE_NUM;
	else if (_raceResult < 3)
		ret += IN_THIRD_EVALUATE_NUM;
	else if (_raceResult < 5)
		ret += IN_FIFTH_EVALUATE_NUM;
	else if (_raceResult < 10)
		ret += IN_TENTH_EVALUATE_NUM;
	else
		ret += +LESS_TENTH_EVALUATE_NUM;*/

	return ret;
}

int OddsManager::EvaluateConsectiveResult(std::vector<int>& _results)
{
	int ret = 0;

	//前走から連勝で評価UP
	if (_results[0] == 0)
	{
		std::vector<int> hitIndexes;//条件に当てはまっているインデックスをリストにする
		for (int i=0;i<_results.size();i++)
		{
			if (_results[i] == 0)
				hitIndexes.push_back(i);
		}
		ret += (int)(CONSECTIVE_RATE[ArrayNumberFunc::GetArrayBeginConsectiveNum(hitIndexes)] * CONSECTIVE_1st_ADD_EVALUATE_NUM);//連続で1着になった回数分人気指数を掛けて足す
	}
	//前走から連続3着内で評価UP
	if (_results[0] < 3)
	{
		std::vector<int> hitIndexes;//条件に当てはまっているインデックスをリストにする
		for (int i = 0; i < _results.size(); i++)
		{
			if (_results[i] <3)
				hitIndexes.push_back(i);
		}
		ret += (int)(CONSECTIVE_RATE[ArrayNumberFunc::GetArrayBeginConsectiveNum(hitIndexes)] * CONSECTIVE_OR_BETTER_3rd_ADD_EVALUATE_NUM);//連続で3着以内になった回数分人気指数を掛けて足す
	}
	//前走から連続5着以内で評価UP
	if (_results[0] < 5)
	{
		std::vector<int> hitIndexes;//条件に当てはまっているインデックスをリストにする
		for (int i = 0; i < _results.size(); i++)
		{
			if (_results[i] < 5)
				hitIndexes.push_back(i);
		}
		ret += (int)(CONSECTIVE_RATE[ArrayNumberFunc::GetArrayBeginConsectiveNum(hitIndexes)] * CONSECTIVE_OR_BETTER_5th_ADD_EVALUATE_NUM);//連続で5着以内になった回数分人気指数を掛けて足す
	}
	//前走から連続6~9着で評価DOWN
	if (_results[0] > 4)
	{
		std::vector<int> hitIndexes;//条件に当てはまっているインデックスをリストにする
		for (int i = 0; i < _results.size(); i++)
		{
			if (_results[i] > 4)
				hitIndexes.push_back(i);
		}
		ret -= (int)(CONSECTIVE_RATE[ArrayNumberFunc::GetArrayBeginConsectiveNum(hitIndexes)] * CONSECTIVE_OR_WORTH_6th_SUB_EVALUATE_NUM);//連続で10着以下になった回数分人気指数を掛けて引く
	}
	//前走から連続10~で評価DOWN
	if (_results[0] > 9)
	{
		std::vector<int> hitIndexes;//条件に当てはまっているインデックスをリストにする
		for (int i = 0; i < _results.size(); i++)
		{
			if (_results[i] > 9)
				hitIndexes.push_back(i);
		}
		ret -= (int)(CONSECTIVE_RATE[ArrayNumberFunc::GetArrayBeginConsectiveNum(hitIndexes)] * CONSECTIVE_OR_WORTH_10th_SUB_EVALUATE_NUM);//連続で10着以下になった回数分人気指数を掛けて引く
	}

	return ret;
}

void OddsManager::FavoriteFactorAddHorseCondition(std::vector<int>& _favorites)
{
	for (int i = 0; i < _favorites.size(); i++)
	{
		int& fav = _favorites[i];

		switch (horse->GetHorses()[i]->GetParam().condition.condition)
		{
		case ConditionParam::VERY_BAD:
			fav = (int)max(fav - VERY_BAD_CONDITION_SUB, fav * VERY_BAD_CONDITION_MULT);
			break;
		case ConditionParam::BAD:
			fav = (int)max(fav - BAD_CONDITION_SUB, fav * BAD_CONDITION_MULT);
			break;
		case ConditionParam::GOOD:
			fav = (int)min(fav + GOOD_CONDITION_ADD, fav * GOOD_CONDITION_MULT);
			break;
		}
	}
}

void OddsManager::CalcHorseFavoriteRate(std::vector<int>& _favorites)
{
	const auto& h = horse->GetHorses();

	for (int i = 0; i < _favorites.size(); i++)
	{
		_favorites[i] = (int)(_favorites[i] * h[i]->GetParam().favoriteRate);
	}
}

void OddsManager::RandFavoriteFactor(std::vector<int>& _favorites)
{
	for (auto& f : _favorites)
	{
		f = (int)(f * Random::GetFloat(1.0f, FAVORITE_FACTOR_RAND_RANGE));
	}
}
