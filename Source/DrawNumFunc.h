#pragma once
#include "../Library/myDxLib.h"

namespace DrawNumFunc
{
	/// <summary>///
/// 数字画像を使用して描画をする関数
/// </summary>/// 
/// <param name="_x  _y">一番右に描画する文字の中央座標</param>
/// <param name="_num">描画したい数値</param>
/// <param name="_interval">文字同士の間隔</param>
/// <param name="_numImage">描画に使用する数字の画像のハンドル</param>
/// <param name="_width _height">画像の縦横の大きさ</param>
/// <param name="_exRate">画像の拡大率</param>
/// <param name="_angle">画像の傾き角度</param>
/// <param name="_defaultZeroNum">0を何桁まで初期表示させるか　3を入力した場合001と表示される</param>
	void DrawRotaNum(float _x, float _y, int _num, int _interval, int _numImage, int _width, int _height, float _exRate, float _angle, int _defaultZeroNum = 1);
}