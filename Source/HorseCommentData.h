#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Horse;

//各競走馬に対するコメントを管理するクラス
class HorseCommentData :public GameObject
{
public:
	HorseCommentData(std::vector<Horse*>& _horses);
	~HorseCommentData();

	const std::vector<std::string>& GetHorseComments()const{return horseComments;}

private:
	void AllCommentLoad();//全てのコメントを読み込む
	void AssignHorseComment(std::vector<Horse*>& _horses);//各競走馬に対するコメントを決定する

	std::vector<std::string> horseComments;//各競走馬に対するコメント

	std::vector<std::vector<int>> commentIndexes;//各コンディションごとに表示出来るコメントのインデックスの配列
	std::vector<std::string> allComments;//全てのコメント一覧
};