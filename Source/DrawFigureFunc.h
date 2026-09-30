#pragma once
#include "../Library/myDxLib.h"

namespace DrawFigureFunc
{
	//_pos1と_pos2を両端とした直方体の辺を描画する
	void DrawBoxOutLine(VECTOR3 _pos1, VECTOR3 _pos2, int _color = 0xffffff);

	//_pos1と_pos2を両端とした長方形を描画する _heightをY座標とし、VECTOR3内のyは無視する
	void DrawSquare(VECTOR3 _pos1, VECTOR3 _pos2, float _height, int _color = 0xffffff);//ToDo　X-Y平面、X-Z平面、Y-Z平面全てに対応させる

	/// <summary>
	/// 3D空間上に弧を描く(X-Z平面のみ対応)
	/// </summary>
	/// <param name="_center">弧の中心座標</param>
	/// <param name="_radius">弧の中心からの距離(半径)</param>
	/// <param name="_startAngle">弧の描画を始める角度</param>
	/// <param name="_endAngle">弧の描画を終了する角度</param>
	/// <param name="_div">弧を描く際の線の分割数</param>
	/// <param name="_color">弧の色</param>
	void DrawArc3D(VECTOR3 _center, float _radius, float _startAngle, float _endAngle, int _div, int _color);

	/// <summary>
	/// ゲージを描画する
	/// </summary>
	/// <param name="_position">描画する左上の座標</param>
	/// <param name="_length">ゲージの全長</param>
	/// <param name="_height">ゲージの幅</param>
	/// <param name="_rate">現在のゲージの溜まり具合</param>
	/// <param name="_color1">ゲージがあるところの色</param>
	/// <param name="_color2">ゲージがないところの色</param>
	void DrawGauge(VECTOR2 _position, int _length, int _height, float _rate, int _color1, int _color2);

	/// <summary>
	/// 2本重なったゲージを描画する
	/// </summary>
	/// <param name="_position">描画する左上の座標</param>
	/// <param name="_length">ゲージの全長</param>
	/// <param name="_height">ゲージの幅</param>
	/// <param name="_rate1">ゲージ1の溜まり具合(0~1)</param>
	/// <param name="_rate2">ゲージ2の溜まり具合(0~1)</param>
	/// <param name="_color1">ゲージ1があるところの色</param>
	/// <param name="_color2">ゲージ2があるところの色</param>
	/// <param name="_color3">ゲージがどちらもないところの色</param>
	void DrawMixGauge(VECTOR2 _position, int _length, int _height, float _rate1,float _rate2, int _color1, int _color2,int _color3);

	//DrawBoxをVECTORに対応させたもの
	void DrawBoxF(VECTOR2 _position, VECTOR2 _size, unsigned int _color, bool _fillFlg);
	void DrawBoxF(VECTOR3 _position, VECTOR3 _size, unsigned int _color, bool _fillFlg);
}