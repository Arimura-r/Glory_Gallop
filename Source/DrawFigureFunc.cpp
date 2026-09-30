#include "DrawFigureFunc.h"
#include "ColorFunc.h"

void DrawFigureFunc::DrawBoxOutLine(VECTOR3 _pos1, VECTOR3 _pos2, int _color)
{
	VECTOR3 drawPos1 = _pos1;
	VECTOR3 drawPos2 = _pos2;

	/*drawPos2.x = _pos2.x;
	DrawLine3D(drawPos1, drawPos2, _color);

	drawPos2.x = _pos1.x;
	drawPos2.z = _pos2.z;
	DrawLine3D(drawPos1, drawPos2, _color);

	drawPos1 = _pos2;
	drawPos1.y = _pos1.y;
	DrawLine3D(drawPos1, drawPos2, _color);

	drawPos2.z = _pos1.z;
	drawPos2.x = _pos2.x;
	DrawLine3D(drawPos1, drawPos2, _color);*/
	DrawSquare(_pos1, _pos2, _pos1.y, _color);//上面の描画
	DrawSquare(_pos1, _pos2, _pos2.y, _color);//下面の描画

	//上面と下面を結ぶ線の描画
	drawPos2.x = _pos1.x;
	drawPos2.z = _pos1.z;
	DrawLine3D(drawPos1, drawPos2, _color);
	drawPos1.x = _pos2.x;
	drawPos2.x = _pos2.x;
	DrawLine3D(drawPos1, drawPos2, _color);
	drawPos1.z = _pos2.z;
	drawPos2.z = _pos2.z;
	DrawLine3D(drawPos1, drawPos2, _color);
	drawPos1.x = _pos1.x;
	drawPos2.x = _pos1.x;
	DrawLine3D(drawPos1, drawPos2, _color);
}

void DrawFigureFunc::DrawSquare(VECTOR3 _pos1, VECTOR3 _pos2, float _height, int _color)
{
	//最初は両方同じ位置に設定する
	VECTOR3 drawPos1 = _pos1;
	drawPos1.y = _height;
	VECTOR3 drawPos2 = drawPos1;

	//_pos1~__pos2.x,_pos1,zへの線の描画
	drawPos2.x = _pos2.x;
	DrawLine3D(drawPos1, drawPos2, _color);

	//_pos1~__pos1.x,_pos2,zへの線の描画
	drawPos2.x = _pos1.x;
	drawPos2.z = _pos2.z;
	DrawLine3D(drawPos1, drawPos2, _color);

	//_pos2~__pos1.x,_pos2,zへの線の描画
	drawPos1 = drawPos2;
	drawPos1.x = _pos2.x;
	DrawLine3D(drawPos1, drawPos2, _color);

	//_pos2~__pos2.x,_pos1,zへの線の描画
	drawPos2.z = _pos1.z;
	drawPos2.x = _pos2.x;
	DrawLine3D(drawPos1, drawPos2, _color);
}

void DrawFigureFunc::DrawArc3D(VECTOR3 _center, float _radius, float _startAngle, float _endAngle, int _div, int _color)
{
	//1分割ごとの直線の角度の変化量
	float divAngle = (_endAngle - _startAngle) / _div;
	
	//前の直線の終点(次の直線の始点)　_startAngleの地点で初期化する
	VECTOR3 previewLine = VECTOR3(_center.x + cosf(_startAngle) * _radius, _center.y, _center.z + sinf(_startAngle) * _radius);

	for (int i = 0; i < _div; i++)
	{
		float currentAngle = _startAngle + divAngle * (i + 1);
		VECTOR3 currentLine= VECTOR3(_center.x + cosf(currentAngle) * _radius, _center.y, _center.z + sinf(currentAngle) * _radius);

		DrawLine3D(previewLine, currentLine, _color);

		previewLine = currentLine;
	}
}

void DrawFigureFunc::DrawGauge(VECTOR2 _position, int _length, int _height, float _rate, int _color1, int _color2)
{
	for (int i = 0; i < _length; i++)
	{
		int color = _color1;

		if ((float)i / _length > _rate)
			color = _color2;

		DrawBox((int)_position.x + i, (int)_position.y, (int)_position.x + i+1, (int)_position.y + _height, color, true);
	}
}

void DrawFigureFunc::DrawMixGauge(VECTOR2 _position, int _length, int _height, float _rate1, float _rate2, int _color1, int _color2, int _color3)
{
	float minRate = _rate1;
	float maxRate = _rate2;
	int maxColor = _color2;
	if (_rate1 > _rate2)
	{
		maxRate = _rate1;
		minRate = _rate2;
		maxColor = _color1;
	}
	
	int color1Length = (int)(_length * minRate);//両方混ざっているところの長さ
	int color2Length = (int)(_length * maxRate)-color1Length;//どちらかのみのところの長さ
	int color3Length = _length - color2Length - color1Length;//残りの部分の長さ
	int mixColor = ColorFunc::ColorMix(_color1, _color2);//color1とcolor2の混ざったところの色

	DrawBoxF(_position, VECTOR2(color1Length, _height), mixColor, true);
	//DrawBox(_position.x, _position.y, _position.x+color1Length, _position.y+_height, mixColor, true);
	DrawBoxF(_position + VECTOR2(color1Length,0), VECTOR2(color2Length,_height), maxColor, true);
	//DrawBox(_position.x+ color1Length, _position.y, _position.x + color1Length+color2Length, _position.y+_height, maxColor, true);
	//DrawBox(_position.x + color1Length+color2Length, _position.y, _position.x + color1Length + color2Length+ color3Length, _position.y+_height, _color3, true);
	DrawBoxF(_position + VECTOR2(color1Length + color2Length,0), VECTOR2(color3Length, _height), _color3, true);
}

void DrawFigureFunc::DrawBoxF(VECTOR2 _position, VECTOR2 _size, unsigned int _color, bool _fillFlg)
{
	DrawBox((int)_position.x, (int)_position.y, (int)(_position.x + _size.x), (int)(_position.y + _size.y), _color, _fillFlg);
}

void DrawFigureFunc::DrawBoxF(VECTOR3 _position, VECTOR3 _size, unsigned int _color, bool _fillFlg)
{
	DrawBoxF(VECTOR2(_position.x, _position.y), VECTOR2(_size.x, _size.y), _color, _fillFlg);
}
