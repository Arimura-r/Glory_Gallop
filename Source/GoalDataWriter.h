#pragma once
#include "../Library/GameObject.h"
#include <vector>

//ゴールした際の情報をcsvにセーブするクラス(パラメータ調整のための結果データを抽出する)
class GoalDataWriter :public GameObject
{
public:
	GoalDataWriter();
	~GoalDataWriter();

	//データを新しく書き込む
	void DataAdd(std::vector<std::string> _data);
	void DataAdd(std::vector<int> _data);

	//データ量が一定以上になった時、先頭のデータを削除する
	void OldDataDelete();

	const std::string SAVE_FILE_PATH = "data\\csv\\goalData.csv";
};