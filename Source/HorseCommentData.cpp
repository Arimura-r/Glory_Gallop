#include "HorseCommentData.h"
#include "CsvReader.h"
#include "Horse.h"
#include "CsvReader.h"
#include "../Library/Random.h"

namespace
{
	const std::string DATA_FILE_PATH = "data/csv/commentData.csv";
}

HorseCommentData::HorseCommentData(std::vector<Horse*>& _horses)
{
	AllCommentLoad();
	AssignHorseComment(_horses);
}

HorseCommentData::~HorseCommentData()
{
}

void HorseCommentData::AllCommentLoad()
{
	commentIndexes.resize(ConditionParam::CONDITION_NUM);

	CsvReader* csv = new CsvReader();
	csv->Load(DATA_FILE_PATH);

	int commentIndex = 0;

	//先頭の行を飛ばし、全ての行を読み込む
	for (int l = 1; l < csv->GetLines(); l++)
	{
		//コメントの一覧に格納
		allComments.push_back(csv->GetString(l, 0));
		
		//コメントの横のIndexの分値を読み込み、対応する配列にコメントのインデックスを格納する
		for (int c = 1; c < csv->GetColums(l);c++)
		{
			commentIndexes[csv->GetInt(l, c)].push_back(commentIndex);
		}
		commentIndex++;
	}

	delete csv;
}

void HorseCommentData::AssignHorseComment(std::vector<Horse*>& _horses)
{
	for (const auto& h : _horses)
	{
		const auto& comments = commentIndexes[(int)h->GetParam().condition.condition];//表示出来うるコメント候補
		int index=Random::GetInt(0, (int)comments.size() - 1);//ランダムで1つ選ぶ
		horseComments.push_back(allComments[comments[index]]);
	}
}
