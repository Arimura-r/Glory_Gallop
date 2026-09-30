#pragma once
#include "../Library/GameObject.h"

//ゲームスタート時のデータを記録するクラス
//タイトル→プレイシーンのデータの受け渡しなどで利用する
class GameStartData :public GameObject
{
public:
	struct Data
	{
		bool canPlayerControll;//プレイヤーを操作するかどうか
		int money;//所持金

		Data(bool _canPlayerControll=true,int _money=10000)
		{
			canPlayerControll = _canPlayerControll;
			money = _money;
		}
	};

	GameStartData();
	~GameStartData();

	void SetData(Data& _data);
	Data GetData()const { return data; }

private:
	Data data;
};