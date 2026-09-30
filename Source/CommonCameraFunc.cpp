#include "CommonCameraFunc.h"
#include "../Library/Time.h"

VECTOR3 CommonCameraFunc::MoveBitweenPosition(VECTOR3 _currentPos, VECTOR3 _goalPos, float _moveSpeed)
{
	if (VSquareSize(_goalPos - _currentPos) > _moveSpeed* _moveSpeed * Time::DeltaTime() * Time::DeltaTime())
	{
		VECTOR3 positionVec = VNorm(_goalPos - _currentPos);
		return _currentPos + positionVec * _moveSpeed * Time::DeltaTime();
	}

	return _goalPos;
}

VECTOR3 CommonCameraFunc::ZoomIn(VECTOR3 _cameraPos, VECTOR3 _cameraTarget, float _distance)
{
	float targetDistance = VSize(_cameraPos - _cameraTarget);

	if (targetDistance > _distance)
	{
		VECTOR3 positionToTargetVec = VNorm(_cameraTarget - _cameraPos);
		_cameraPos += positionToTargetVec * (targetDistance - _distance);
	}
	return _cameraPos;
}
