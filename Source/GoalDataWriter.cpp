#include "GoalDataWriter.h"
#include "CsvReader.h"

namespace
{
	const int DATA_NUM_MAX = 5;
}

GoalDataWriter::GoalDataWriter()
{
	OldDataDelete();
}

GoalDataWriter::~GoalDataWriter()
{
}

void GoalDataWriter::DataAdd(std::vector<std::string> _data)
{
	auto csv=new CsvReader();
	csv->Load(SAVE_FILE_PATH);

	auto allData = csv->GetAll();
	allData.push_back(_data);

	csv->SaveAll(SAVE_FILE_PATH,allData);

	delete csv;
}

void GoalDataWriter::DataAdd(std::vector<int> _data)
{
	std::vector<std::string> strData;

	for (auto& d : _data)
	{
		strData.push_back(std::to_string(d));
	}
	DataAdd(strData);
}

void GoalDataWriter::OldDataDelete()
{
	auto csv = new CsvReader();
	csv->Load(SAVE_FILE_PATH);
	
	//先頭の説明行を除き、データ数が一定以上なら処理を行う
	if ((int)csv->GetLines()>DATA_NUM_MAX+1)
	{
		auto allData = csv->GetAll();
		allData.erase(allData.begin() + 1);//先頭の次の行のデータを消す

		csv->SaveAll(SAVE_FILE_PATH, allData);
	}

	delete csv;
}
