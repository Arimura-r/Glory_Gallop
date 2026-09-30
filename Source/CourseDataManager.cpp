#include "CourseDataManager.h"
#include "CsvReader.h"
#include "../ImGui/imgui.h"
#include "DrawFigureFunc.h"

CourseDataManager::CourseDataManager()
{
	DataLoad();
	CalcCourseDistance();
	//startLine = 22000.0f;
	segmentDrawing = false;

	//leftCornerCenter = VECTOR3(32100, 2000, -30000);
	//rightCornerCenter = VECTOR3(32100, 2000, 30000);
	//innerLine = 25050.0f;
	//outerLine = 28000.0f;
	//lineRot = VECTOR3(0, 0, 0);
	//lineLen = 500.0f;
}

CourseDataManager::~CourseDataManager()
{
}

void CourseDataManager::Update()
{
	//ImGui::Begin("Course");
	//ImGui::Text("Purple=Straight,Pink=Corner");
	//ImGui::Checkbox("segmentDraw",&segmentDrawing);
	//ImGui::InputFloat3("LeftCornerCenterPos", &leftCornerCenter.x);
	//ImGui::InputFloat3("RightCornerCenterPos", &rightCornerCenter.x);
	//ImGui::SliderFloat3("LineRot", &lineRot.x,-DX_PI_F,DX_PI_F);
	//ImGui::InputFloat("LineLen", &lineLen);
	//ImGui::End();
}

void CourseDataManager::Draw()
{
	return;
	//デバッグ表示
	DrawSphere3D(leftCornerCenter, 100, 3, 0xff0000, 0xff0000, true);
	DrawSphere3D(rightCornerCenter, 100, 3, 0x00ffff, 0x00ffff, true);
	//MATRIX mat = MGetRotX(lineRot.x) * MGetRotZ(lineRot.z) * MGetRotY(lineRot.y);
	//DrawLine3D(leftCornerCenter, leftCornerCenter + VECTOR3(0,0,lineLen)*mat,0xff0000 );
	//DrawLine3D(rightCornerCenter, rightCornerCenter + VECTOR3(0, 0, lineLen) * mat, 0x00ffff);
	CourseDataDraw();
}

CourseDataManager::SegmentType CourseDataManager::GetCurrentSegmentType(VECTOR3 _pos) const
{
	SegmentType ret = NONE;

	if (_pos.z < leftCornerCenter.z)
		ret = SECOND_CORNER;
	else if (rightCornerCenter.z < _pos.z)
		ret = FIRST_CORNER;
	else
	{
		if (_pos.x < leftCornerCenter.x)
			ret = HOME_STREATCH;
		else
			ret = BACK_STREATCH;
	}
	return ret;
}

float CourseDataManager::GetRemainingCornerDistance(VECTOR3 _pos, int& _currentSegmentIndex) const
{
	float ret = 0.0f;
	for (int i = _currentSegmentIndex; i < courseSegments.size(); i++)
	{
		if(courseSegments[i].type==FIRST_CORNER|| courseSegments[i].type == SECOND_CORNER)
			ret += courseSegments[i].distance;
	}

	float rate = GetSegmentRate(_pos, courseSegments[_currentSegmentIndex].type);
	if (courseSegments[_currentSegmentIndex].type == FIRST_CORNER || courseSegments[_currentSegmentIndex].type == SECOND_CORNER)
	{
		ret -= rate * cornerDistance;
	}

	return ret;
}

float CourseDataManager::GetCornerLossDistance(VECTOR3 _pos, int& _currentSegmentIndex, float _plusInnerDistance) const
{
	float cornerRate = 0.0f;
	//残っているコーナーがどれだけの区間あるか調べる(半周=1.0)
	for (int i = _currentSegmentIndex; i < courseSegments.size(); i++)
	{
		if (courseSegments[i].type == FIRST_CORNER || courseSegments[i].type == SECOND_CORNER)
			cornerRate += 1.0f;
	}
	SegmentType type = courseSegments[_currentSegmentIndex].type;
	if (type == FIRST_CORNER || type == SECOND_CORNER)
	{
		cornerRate -= GetSegmentRate(_pos, type);
	}
	//最内ラインとの距離を調べる
	float toInnerDis = GetInnerToDistance(_pos, type) + _plusInnerDistance;

	float allLossDistance = (toInnerDis)*DX_PI_F * cornerRate;

	return allLossDistance;
}

float CourseDataManager::HorseRemainingDistanceUpdate(VECTOR3 _pos, int& _currentSegmentIndex) const
{
	//現在区間に更新があるかの確認
	SegmentUpdate(_pos, _currentSegmentIndex);

	//残り区間のみ距離を加算
	float distance = 0.0f;
	for (int i =_currentSegmentIndex; i < courseSegments.size(); i++)
	{
		distance += courseSegments[i].distance;
	}

	float rate=GetSegmentRate(_pos, courseSegments[_currentSegmentIndex].type);

	if (courseSegments[_currentSegmentIndex].type == HOME_STREATCH || courseSegments[_currentSegmentIndex].type == BACK_STREATCH)
	{
		//スタート時のみの例外処理　ToDo例外処理にならないようにしたい
		if (_currentSegmentIndex == 0)
		{
			distance -= rate * (straightDistance - startSegmentDistance);
		}
		else
		{
			distance -= rate * straightDistance;
		}

	}
	else
	{
		distance -= rate * cornerDistance;
	}

#if false
	//どこの区間にいるのかを調べ、現在区間の経過割合に応じて距離を減らす
	if (_pos.z > rightCornerCenter.z)
	{
		//右コーナー(1,2コーナー)
		//distance -= straightDistance;//最初の直線は既に通っているのでその分を減らす
		float angle = atan2f(rightCornerCenter.x - _pos.x, rightCornerCenter.z - _pos.z);

		//途中で角度が負の値になると計算しづらいため、計算しやすい値に直す
		if (angle < 0)
			angle += DX_PI_F * 2;
		angle -= DX_PI_F/2;

		float segmentRate = angle / DX_PI_F;

		distance -= segmentRate * cornerDistance;
	}
	else
	{
		if (_pos.z < leftCornerCenter.z)
		{
			//左コーナー(3,4コーナー)
			float angle = atan2f(leftCornerCenter.x - _pos.x, leftCornerCenter.z - _pos.z);
			//計算しやすい値に直す
			angle += DX_PI_F / 2;

			float segmentRate = angle / DX_PI_F;

			distance -= segmentRate * cornerDistance;
			
		}
		else if (_pos.x < leftCornerCenter.x)//コースの中央より上か下か
		{
			//スタンド前直線
			//スタート時のみ計算方法を変更する
			if (_currentSegmentIndex == 0)
				distance -= _pos.z - data.startLine;
			else
				distance -= _pos.z - leftCornerCenter.z;
		}
		else
		{
			//向こう正面の直線
			//distance -= straightDistance + cornerDistance;
			distance -= rightCornerCenter.z-_pos.z;
		}
	}
#endif
	return distance;
}

void CourseDataManager::DataLoad()
{
	CourseDataLoad();
	StartDataLoad();
	GoalDataLoad();
}

void CourseDataManager::CourseDataLoad()
{
	CsvReader* csv = new CsvReader();
	csv->Load(COURSE_DATA_FILE_PATH);
	
	//各種コースに関するデータを得る
	leftCornerCenter = csv->GetVECTOR3(1,0);
	rightCornerCenter = csv->GetVECTOR3(1, 3);
	innerLine = csv->GetFloat(1, 6);
	outerLine = csv->GetFloat(1, 7);

	//コースの形状(区間)に関するデータを得る
	for (int c = 1; c < csv->GetColums(2); c++)
	{
		int courseType = csv->GetInt(2, c);
		//値が正の値でないなら正しい値ではないため抜ける
		if (courseType <1)
			break;

		courseSegments.push_back(Segment((SegmentType)courseType));
	}

	delete csv;

	straightDistance = rightCornerCenter.z - leftCornerCenter.z;//直線の距離
	cornerDistance = DX_PI_F * innerLine;//コーナーの距離
}

void CourseDataManager::StartDataLoad()
{
	CsvReader* csv = new CsvReader();
	csv->Load(START_DATA_FILE_PATH);
	
	startSegmentDistance = csv->GetFloat(1, 0);//各区間の中でどれだけの距離のところにスタートラインがあるかを得る
	startHorseDistance = csv->GetFloat(1, 1);

	delete csv;

	startData=CalcLineData(startSegmentDistance,courseSegments[0]);
	//CalcStartSegment(startLineSegDis);
}

void CourseDataManager::GoalDataLoad()
{
	CsvReader* csv = new CsvReader();
	csv->Load(GOAL_DATA_FILE_PATH);

	goalSegmentDistance = csv->GetFloat(1, 0);//各区間の中でどれだけの距離のところにゴールラインがあるかを得る

	delete csv;

	goalData=CalcLineData(goalSegmentDistance,courseSegments.rbegin()[0]);
	//CalcGoalSegment(goalLineSegDis);
}

CourseDataManager::LineData CourseDataManager::CalcLineData(float _segDistance, Segment _lineSegment)
{
	LineData line;
	switch (_lineSegment.type)
	{
	case HOME_STREATCH:
		line.innerPos = leftCornerCenter;
		line.innerPos.x -= innerLine;
		line.innerPos.y = 0.0f;
		line.innerPos.z += _segDistance;
		line.lineVec = VECTOR3(-1, 0, 0);
		break;
	case BACK_STREATCH:
		line.innerPos = rightCornerCenter;
		line.innerPos.x += innerLine;
		line.innerPos.y = 0.0f;
		line.innerPos.z -= _segDistance;
		line.lineVec = VECTOR3(1, 0, 0);
		break;
		//ToDo　コーナーでもラインの向きを出来るようにする→コーナースタート・ゴールが出来るようになる
	default:
		break;
	}
	return line;
}

//void CourseDataManager::CalcStartSegment(float _segDistance)
//{
//	switch (courseSegments[0].type)
//	{
//	case HOME_STREATCH:
//	case BACK_STREATCH:
//		courseSegments[0].distance = straightDistance - _segDistance;
//		break;
//	case FIRST_CORNER:
//	case SECOND_CORNER:
//		courseSegments[0].distance = cornerDistance - _segDistance;
//		break;
//	default:
//		break;
//	}
//}
//
//void CourseDataManager::CalcGoalSegment(float _segDistance)
//{
//	(*courseSegments.rbegin()).distance = _segDistance;
//}

void CourseDataManager::CalcCourseDistance()
{
	//courseSegments[0].distance = startLine - leftCornerCenter.z;
	//(*courseSegments.rbegin()).distance = 20000.0f - leftCornerCenter.z;

	//スタートとゴール区間は先に値を決めておく
	switch (courseSegments[0].type)
	{
	case HOME_STREATCH:
	case BACK_STREATCH:
		courseSegments[0].distance = straightDistance - startSegmentDistance;
		break;
	case FIRST_CORNER:
	case SECOND_CORNER:
		courseSegments[0].distance = cornerDistance - startSegmentDistance;
		break;
	default:
		break;
	}
	(*courseSegments.rbegin()).distance = goalSegmentDistance;

	//他の区間は特定の数値が指定されていなければ上で計算した値をそのまま入れる
	float sumDistance = 0.0f;
	for (auto& s : courseSegments)
	{
		sumDistance += s.distance;
		if (s.distance > 0)
			continue;

		if (s.type == HOME_STREATCH || s.type == BACK_STREATCH)
			s.distance = straightDistance;
		else if (s.type == FIRST_CORNER || s.type == SECOND_CORNER)
			s.distance = cornerDistance;
		else
			assert(false && "異常なコースの区間が存在します");

		sumDistance += s.distance;
	}

	courseAllDistance = sumDistance;
}

void CourseDataManager::CourseDataDraw()
{
	DrawLine3D(leftCornerCenter - VECTOR3(outerLine, 0, 0), rightCornerCenter - VECTOR3(outerLine, 0, 0), 0x00ff00);//上外側直線
	DrawLine3D(leftCornerCenter - VECTOR3(innerLine, 0, 0), rightCornerCenter - VECTOR3(innerLine, 0, 0), 0x00ff00);//上内側直線
	DrawLine3D(leftCornerCenter + VECTOR3(outerLine, 0, 0), rightCornerCenter + VECTOR3(outerLine, 0, 0), 0x00ff00);//下外側直線
	DrawLine3D(leftCornerCenter + VECTOR3(innerLine, 0, 0), rightCornerCenter + VECTOR3(innerLine, 0, 0), 0x00ff00);//下内側直線
	DrawFigureFunc::DrawArc3D(leftCornerCenter, innerLine, -DX_PI_F, 0,64, 0x00ff00);//左内側コーナー
	DrawFigureFunc::DrawArc3D(leftCornerCenter, outerLine, -DX_PI_F, 0, 64, 0x00ff00);//左外側コーナー
	DrawFigureFunc::DrawArc3D(rightCornerCenter, innerLine, 0, DX_PI_F, 64, 0x00ff00);//右内側コーナー
	DrawFigureFunc::DrawArc3D(rightCornerCenter, outerLine, 0, DX_PI_F, 64, 0x00ff00);//右外側コーナー
}

bool CourseDataManager::SegmentUpdate(VECTOR3 _pos, int& _currentSegmentIndex)const
{
	bool ret = false;
	if (_currentSegmentIndex >= courseSegments.size()-1)
		return false;

	//次の区間の種類によって区間の遷移条件を変える
	switch (courseSegments[_currentSegmentIndex+1].type)
	{
	case HOME_STREATCH:
		if (leftCornerCenter.z < _pos.z && _pos.z < rightCornerCenter.z
			&& _pos.x < rightCornerCenter.x)
			ret = true;
		break;
	case BACK_STREATCH:
		if (leftCornerCenter.z < _pos.z && _pos.z < rightCornerCenter.z
			&& _pos.x > rightCornerCenter.x)
			ret = true;
		break;
	case FIRST_CORNER:
		if (_pos.z > rightCornerCenter.z)
			ret = true;
		break;
	case SECOND_CORNER:
		if (_pos.z < leftCornerCenter.z)
			ret = true;
		break;
	default:
		break;
	}
	if (ret)
		_currentSegmentIndex++;

	return ret;
}

float CourseDataManager::GetSegmentRate(VECTOR3 _pos, SegmentType _type)const
{
	float angle = 0.0f;
	float ret = 0.0f;
	switch (_type)
	{
	case CourseDataManager::HOME_STREATCH:
		ret=(_pos.z - leftCornerCenter.z) / straightDistance;
		break;
	case CourseDataManager::BACK_STREATCH:
		ret=(rightCornerCenter.z - _pos.z) / straightDistance;
		break;
	case CourseDataManager::FIRST_CORNER:
		angle = atan2f(rightCornerCenter.x - _pos.x, rightCornerCenter.z - _pos.z);

		//途中で角度が負の値になると計算しづらいため、計算しやすい値に直す
		if (angle < 0)
			angle += DX_PI_F * 2;
		angle -= DX_PI_F / 2;

		ret = angle / DX_PI_F;
		break;
	case CourseDataManager::SECOND_CORNER:
		//左コーナー(3,4コーナー)
		angle = atan2f(leftCornerCenter.x - _pos.x, leftCornerCenter.z - _pos.z);
		//計算しやすい値に直す
		angle += DX_PI_F / 2;

		ret = angle / DX_PI_F;
		break;
	default:
		assert(false && "異常なデータ値の区間が存在します");
		break;
	}

	return ret;
}

float CourseDataManager::GetInnerToDistance(VECTOR3 _pos, SegmentType _type) const
{
	float ret = 0.0f;
	switch (_type)
	{
	case CourseDataManager::NONE:
		break;
	case CourseDataManager::HOME_STREATCH:
	case CourseDataManager::BACK_STREATCH:
		ret = abs(_pos.x - rightCornerCenter.x);
		break;
	case CourseDataManager::FIRST_CORNER:
		ret = VSize(_pos - rightCornerCenter);
		break;
	case CourseDataManager::SECOND_CORNER:
		ret = VSize(_pos - leftCornerCenter);
		break;
	default:
		break;
	}

	return ret - innerLine;
}
