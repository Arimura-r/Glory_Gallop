#pragma once
#include  <vector>
#include <string>
#include "../Library/myDxLib.h"

class CsvReader
{
public:
	CsvReader();
	~CsvReader();
	/// <summary>
	/// データを読み込む
	/// </summary>
	/// <param name="_filename"></param>
	/// <returns></returns>
	bool Load(std::string _filename);
	/// <summary>
	/// 全データを一括で書き込む（上書き）
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="all"></param>
	void SaveAll(std::string _filename,const std::vector<std::vector<std::string>>& all) const;
	/// <summary>
	/// 一ラインを書き換える
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="targetLine"></param>
	/// <param name="newLine"></param>
	void SaveLine(std::string _filename, int targetLine, const std::vector<std::string>& newLine) const;
	/// <summary>
	/// セル一箇所だけ書き換える
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="line"></param>
	/// <param name="column"></param>
	/// <param name="newValue"></param>
	void SaveCell(std::string _filename, int line, int column, const std::string& newValue)const;
	
	/// <summary>
	/// 全てのセルを1種類の文字で埋める
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="_all"></param>
	void ResetAll(std::string _filename, const std::string& _allText);
	
	//ライン数を取得
	size_t GetLines();
	//行数を取得
	size_t GetColums(int line);
	/// <summary>
	/// セルの値をint型で取得
	/// </summary>
	/// <param name="line">行</param>
	/// <param name="column">列</param>
	/// <returns></returns>
	int GetInt(int line, int column);
	float GetFloat(int line, int column);
	std::string GetString(int line, int column);
	/// <summary>
	/// セルの値をVECTOR3型で取得　指定したセルから2個右までをFloatでx,y,zで取得する
	/// </summary>
	VECTOR3 GetVECTOR3(int line, int column);
	std::vector < std::vector < std::string>> GetAll() const { return record; }

	//文字コード変換
	std::string UTF8ToShiftJis(std::string data);

private:
	std::vector<std::vector<std::string>> record;
	
};