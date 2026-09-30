#include "HorseCollisionManager.h"
#include "Horse.h"
#include "CourseDataManager.h"
#include "Stage.h"

HorseCollisionManager::HorseCollisionManager(CourseDataManager* _course, Stage* _stage)
{
	course = _course;
	stage = _stage;
}

HorseCollisionManager::~HorseCollisionManager()
{
}

void HorseCollisionManager::HorseToHorseCollisionUpdate(std::vector<Horse*>& _horses)
{
	int horseNum = (int)_horses.size();
	for (int i = 0; i < horseNum-1; i++)
	{
		for (int j = i+1; j < horseNum; j++)
		{
			Horse* col1 = _horses[i];
			Horse* col2 = _horses[j];

			//ToDo　√を使っていて重いのでSquareSizeのままで計算する
			float distance =VSize(col1->GetPosition() - col2->GetPosition());
			float radiusSum = col1->GetCollisionRadius() + col2->GetCollisionRadius();//2つの当たり判定の大きさの合計
			if (distance < radiusSum)
			{
				//ToDo　必要であれば各々に当たっているフラグを持たせtrueにする

				//押し返し
				VECTOR3 col1PushVec = VNorm(col1->GetPosition() - col2->GetPosition());
				VECTOR3 col2PushVec = VNorm(col2->GetPosition() - col1->GetPosition());
				
				col1->PositionPush(col1PushVec* ( (radiusSum-distance) / 2));
				col2->PositionPush(col2PushVec * ((radiusSum-distance) / 2));
				
				col1->HitOtherHorse();
				col2->HitOtherHorse();
			}
		}
	}
}

void HorseCollisionManager::HorseToCourseCollisionUpdate(std::vector<Horse*>& _horses)
{
	for (auto& h : _horses)
	{
		const VECTOR3& pos=h->GetPosition();

		if (pos.z < course->GetLeftCornerCenter().z)
		{
			//zがマイナス側のカーブに馬がいる
			
			VECTOR3 centerPos = course->GetLeftCornerCenter();
			centerPos.y = pos.y;//高さによる距離を無視する
			float centerDistance = VSize(pos - centerPos);
			if (centerDistance <course->GetInnerLine())
			{
				h->CourseOut();
				VECTOR3 posToCenterVec = VNorm(pos - centerPos);
				h->PositionPush(posToCenterVec * (course->GetInnerLine() - centerDistance));//押し返し
			}
			else if (centerDistance > course->GetOuterLine())
			{
				h->CourseOut();
				VECTOR3 posToCenterVec = VNorm(pos - centerPos);
				h->PositionPush(posToCenterVec * (course->GetOuterLine() - centerDistance));//押し返し
			}
		}
		else if (pos.z > course->GetRightCornerCenter().z)
		{
			//zがプラス側のカーブに馬がいる

			VECTOR3 centerPos = course->GetRightCornerCenter();
			centerPos.y = pos.y;//高さによる距離を無視する

			//内側の線より外、外側の線より内ならコース内
			float centerDistance = VSize(pos - centerPos);
			if (centerDistance < course->GetInnerLine() ) 
			{
				h->CourseOut();

				//現在座標からカーブの中心へのベクトル
				VECTOR3 posToCenterVec = VNorm(pos - centerPos);
				h->PositionPush(posToCenterVec * (course->GetInnerLine() - centerDistance));//押し返し
			}
			else if (centerDistance >course->GetOuterLine())
			{
				h->CourseOut();
				//現在座標からカーブの中心へのベクトル
				VECTOR3 posToCenterVec = VNorm(pos - centerPos);
				h->PositionPush(posToCenterVec * (course->GetOuterLine() - centerDistance));//押し返し
			}
		}
		else
		{
			//直線に馬がいる

			//ToDo　下側の直線だとバグる

			//馬の座標とコースの中心との距離を調べ、innnerより大きく、outerより小さいならコース内に入っている
			float centerDistance = pos.x - course->GetLeftCornerCenter().x;
			if (abs(centerDistance) < course->GetInnerLine())
			{
				h->CourseOut();
				//押し返し
				VECTOR3 pushVec = VECTOR3(course->GetInnerLine() - abs(centerDistance), 0, 0);
				if (centerDistance < 0)
					pushVec.x *= -1;
				h->PositionPush(pushVec);
			}
			else if (abs(centerDistance) > course->GetOuterLine())
			{
				h->CourseOut();
				//押し返し
				VECTOR3 pushVec = VECTOR3(course->GetOuterLine() - abs(centerDistance), 0, 0);
				if (centerDistance < 0)
					pushVec.x *= -1;
				h->PositionPush(pushVec);
			}
		}
	}
}

void HorseCollisionManager::HorseToGroundCollisionUpdate(std::vector<Horse*>& _horses)
{
	for (auto& h : _horses)
	{
		VECTOR3 hitPos;
		if (stage->CollideLine(h->GetPosition() + VECTOR3(0, 200, 0), h->GetPosition() + VECTOR3(0, -2000, 0), hitPos))
			h->PositionPush(hitPos-h->GetPosition());
	}
}

void HorseCollisionManager::CheckAroundHorse(std::vector<Horse*>& _horses)
{
	for (auto& h : _horses)
	{
		float hitRadius = h->DEFUALT_COLLISION_RADIUS*2;
		VECTOR3 frontPos = h->GetPosition() + VECTOR3(0, 0, 1) * h->GetRotationMatrix() * hitRadius * 1.5f;
		VECTOR3 innerPos= h->GetPosition() + VECTOR3(1,0,0)*h->GetRotationMatrix() * hitRadius;
		VECTOR3 innerPos2 = h->GetPosition() + VECTOR3(1, 0, 0) * h->GetRotationMatrix() * hitRadius + VECTOR3(0, 0, 1) * h->GetRotationMatrix() * hitRadius*1.5f;
		VECTOR3 innerPos3 = h->GetPosition() + VECTOR3(1, 0, 0) * h->GetRotationMatrix() * hitRadius + VECTOR3(0, 0, -1) * h->GetRotationMatrix() * hitRadius * 1.5f;

		for (auto& col1 : _horses)
		{
			if (h == col1)
				continue;

			if (VSquareSize(frontPos - col1->GetPosition()) < hitRadius * hitRadius)
				h->FrontHorseAdd(col1);

			if (VSquareSize(innerPos - col1->GetPosition()) < hitRadius * hitRadius||
				VSquareSize(innerPos2 - col1->GetPosition()) < hitRadius * hitRadius||
				VSquareSize(innerPos3 - col1->GetPosition()) < hitRadius * hitRadius)
				h->InnerHorseAdd(col1);
		}

	}
}

//void HorseCollisionManager::VelocityCollisionUpdate(std::vector<Horse*>& _horses)
//{
//	int horseNum = (int)_horses.size();
//	for (int i = 0; i < horseNum; i++)
//	{
//		for (int j = 0; j < horseNum; j++)
//		{
//			if (i == j)
//				continue;
//
//			Horse* col1 = _horses[i];
//			Horse* col2 = _horses[j];
//			VECTOR3 col1NextPos = col1->GetPosition() + col1->GetVelocity();
//			float radiusSum = col1->GetCollisionRadius() + col2->GetCollisionRadius();//2つの当たり判定の大きさの合計
//			//ToDo　√を使っていて重いのでSquareSizeのままで計算する
//			float distance = VSize(col1NextPos - col2->GetPosition());
//
//			if (distance < col1->GetCollisionRadius() + col2->GetCollisionRadius())
//			{
//				//ToDo　必要であれば各々に当たっているフラグを持たせtrueにする
//				VECTOR3 velocity = col1->GetVelocity()-VNorm(col1NextPos - col2->GetPosition()) * (radiusSum - distance);
//				col1->CheckColVeclocity(velocity);
//			}
//		}
//	}
//}
