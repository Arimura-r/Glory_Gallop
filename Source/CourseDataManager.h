#pragma once
#include "../Library/GameObject.h"
#include <vector>

//ToDo　かなり処理が複雑になっているため、処理を綺麗にする＆　クラスを分ける
//トラック(走るコース)についてのデータを管理する
class CourseDataManager :public GameObject
{
public:
	enum SegmentType
	{
		NONE = -1,
		HOME_STREATCH=1,//正面スタンド前の直線
		BACK_STREATCH,//向こう正面の直線
		FIRST_CORNER,//前半のコーナー
		SECOND_CORNER,//後半のコーナー
	};

	//スタート・ゴールライン関連の値
	struct LineData
	{
		VECTOR3 innerPos;//内側のライン上のスタートライン地点
		VECTOR3 lineVec;//スタートラインがどの方向に向くかのベクトル
	};

	CourseDataManager();
	~CourseDataManager();
	void Update()override;
	void Draw()override;

	VECTOR3 GetRightCornerCenter()const { return rightCornerCenter; }
	VECTOR3 GetLeftCornerCenter()const { return leftCornerCenter; }
	float GetInnerLine()const { return innerLine;}
	float GetOuterLine()const { return outerLine; }
	//コース幅を取得する
	float GetCourseWidth()const { return outerLine - innerLine; }
	float GetCourseAllDistance()const { return courseAllDistance; }
	//float GetStartLine()const { return startLine; }
	LineData GetStartLineData()const { return startData; }
	float GetStartHorseDistance()const { return startHorseDistance; }
	LineData GetGoalLineData()const { return goalData; }

	SegmentType GetCurrentSegmentType(VECTOR3 _pos)const;
	//残りのコーナー区間の長さを調べる
	float GetRemainingCornerDistance(VECTOR3 _pos, int& _currentSegmentIndex)const;

	/// <summary>
	/// コーナーで発生する距離ロスを計算する
	/// </summary>
	/// <param name="_pos">現在座標</param>
	/// <param name="_currentSegmentIndex">現在の区間</param>
	/// <param name="_plusInnerDistance">将来的に今の座標からどれだけ外に移動する予定か</param>
	/// <returns></returns>
	float GetCornerLossDistance(VECTOR3 _pos, int& _currentSegmentIndex,float _plusInnerDistance=0.0f)const;

	/// <summary>
	/// 各馬の残り距離を更新する
	/// </summary>
	/// <param name="_pos">自身の座標</param>
	/// <param name="_segmentNum">現在いる区間(これより前のものは通り過ぎている)</param>
	/// <returns></returns>
	float HorseRemainingDistanceUpdate(VECTOR3 _pos,int& _currentSegmentIndex)const;

	//その区間のどれだけの割合に到達しているかを調べる
	float GetSegmentRate(VECTOR3 _pos, SegmentType _type)const;
	//一番内側のラインからどれだけ離れているかを調べている
	float GetInnerToDistance(VECTOR3 _pos, SegmentType _type)const;

private:
	//コースを構成する各区間のデータ
	struct Segment
	{
		SegmentType type;//コースの形状
		float distance;//区間の距離

		//区間の種類は必ず指定する　区間の距離は指定しなければ後からCourseData側で自動で入力する
		Segment(SegmentType _type, float _distance = 0.0f)
		{
			type = _type;
			distance = _distance;
		}
	};

	//Data data;
	bool segmentDrawing;

	VECTOR3 rightCornerCenter;
	VECTOR3 leftCornerCenter;
	float innerLine;
	float outerLine;
	float straightDistance;//直線の長さ
	float cornerDistance;//コーナーの長さ
	float courseAllDistance;//コースの全長

	//デバッグ表示用の値
	//VECTOR3 lineRot;
	//float lineLen;
	LineData startData;
	float startHorseDistance;//馬同士の距離
	float startSegmentDistance;
	LineData goalData;
	float goalSegmentDistance;

	//各データのロード
	void DataLoad();
	//コースデータのロード
	void CourseDataLoad();
	//スタートデータのロード
	void StartDataLoad();
	//ゴールデータのロード
	void GoalDataLoad();

	//ラインの位置、向きを計算する
	LineData CalcLineData(float _segDistance,Segment _lineSegment);
	//スタート区間の距離を計算する
	//void CalcStartSegment(float _segDistance);
	//ゴール区間の距離を計算する
	//void CalcGoalSegment(float _segDistance);
	//コース・区間の距離を計算する(スタート・ゴール区間を除く)
	void CalcCourseDistance();

	//データを元にコースの形状を描画する
	void CourseDataDraw();
	//次の区間に入れるかどうかを調べ、入れるなら移動する
	bool SegmentUpdate(VECTOR3 _pos,int& _currentSegmentIndex)const;

	const std::string COURSE_DATA_FILE_PATH = "data\\csv\\course\\courseData.csv";
	const std::string START_DATA_FILE_PATH = "data\\csv\\course\\startData.csv";
	const std::string GOAL_DATA_FILE_PATH = "data\\csv\\course\\goalData.csv";

	std::vector<Segment>courseSegments;
	//{
	//	//Segment(HOME_STREATCH),//スタート後の直線
	//	//Segment(FIRST_CORNER),//1,2コーナー
	//	Segment(BACK_STREATCH),//向こう正面
	//	Segment(SECOND_CORNER),//3,4コーナー
	//	Segment(HOME_STREATCH),//最終直線
	//};
};