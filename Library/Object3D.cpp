#include "Object3D.h"

Object3D::Object3D()
{
	hModel = -1;
	scale = VECTOR3(1, 1, 1);
}

Object3D::~Object3D()
{
	if (hModel > -1)
		MV1DeleteModel(hModel);
}

void Object3D::Update()
{
	NormarizeRotation();
}

void Object3D::Draw()
{
	if (hModel < 1)
		return;

	MV1SetMatrix(hModel, GetLocalMatrix());
	MV1DrawModel(hModel);
}

void Object3D::NormarizeRotation()
{
	rotation = VECTOR3(NormarizePi(rotation.x), NormarizePi(rotation.y), NormarizePi(rotation.z));
}

float Object3D::NormarizePi(float _num)
{
	if (_num < -DX_PI_F)
		_num += DX_PI_F*2;
	else if (_num > DX_PI_F)
		_num -= DX_PI_F*2;

	return _num;
}
