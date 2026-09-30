#pragma once
#include "../Library/GameObject.h"
#include <vector>

class HorseManager;

class OddsManager :public GameObject
{
public:
	OddsManager(HorseManager* _horse);
	~OddsManager();

	//該当馬の単勝オッズを得る
	float GetWinOdds(int _horseNum)const;
	//該当馬の複勝オッズを得る
	float GetPlaceOdds(int _horseNum)const;
	//全馬の人気順を得る　[0]なら1人気の馬番を得る
	const std::vector<int>& GetFavoriteOrder()const { return favoriteOrder; }
	//指定した馬が何番人気かを得る
	int GetHorseFavoriteOrder(int _horseNum)const;

private:
	//人気指数を計算から求める
	void CalcFavoriteFactor();
	//人気順を計算する
	void CalcFavoriteOrder();

	//近走成績を人気指数に加える
	void FavoriteFactorAddRaceResult(std::vector<int>& _favorites);
	//レース結果を元に人気指数をどれだけプラスするかどうかの評価式
	int EvaluateRaceResult(int _raceResult);
	//連続で〇着に走ったなどの情報を元にどれだけ人気指数を加算するかを調べる
	int EvaluateConsectiveResult(std::vector<int>& _results);
	//馬の調子を人気指数に加える
	void FavoriteFactorAddHorseCondition(std::vector<int>& _favorites);
	//人気指数に馬ごとの人気のしやすさを加える
	void CalcHorseFavoriteRate(std::vector<int>& _favorites);
	//人気指数に乱数を混ぜる
	void RandFavoriteFactor(std::vector<int>& _favorites);

	HorseManager* horse;
	std::vector<int> favoriteFactor;//各馬ごとに人気のしやすさの指数　これを元に各馬券券種のオッズを決定する
	std::vector<int> favoriteOrder;//各馬の人気順　[0]なら1人気の馬番を得る
	int sumFavoriteFactor;//人気指数の全体の合計値
};