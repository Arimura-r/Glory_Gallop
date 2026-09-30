#pragma once
#include "../Library/GameObject.h"

//3Dモデルの描画機能を持った基底クラス
class Object3D :public GameObject
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update()override;
	virtual void Draw()override;

	const VECTOR3& GetPosition()const { return position; }
	const VECTOR3& GetRotation()const { return rotation; }
	const VECTOR3& GetScale()const { return scale; }
	int GetModel()const { return hModel; }
	MATRIX GetRotationMatrix()const
	{
		MATRIX rotX = MGetRotX(rotation.x);
		MATRIX rotY = MGetRotY(rotation.y);
		MATRIX rotZ = MGetRotZ(rotation.z);

		MATRIX rot = rotZ * rotX * rotY;
		return rot;
	}
	MATRIX GetLocalMatrix()const
	{
		MATRIX mScale = MGetScale(scale);
		MATRIX mRot = GetRotationMatrix();
		MATRIX mTrans = MGetTranslate(position);
		return (mScale * mRot)* mTrans;
	}
	VECTOR3 GetForward()const { return VECTOR3(0, 0, 1) * GetRotationMatrix(); }

protected:
	//Rotationの値を-PI~PIの範囲に収める
	void NormarizeRotation();
	//渡された値を-PI~PIの範囲に収めて返す
	float NormarizePi(float _num);

	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	int hModel;
};