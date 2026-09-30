#pragma once
#include "../Library/myDxLib.h"

namespace ColorFunc
{
	//2D用のカラーコードを3D用のCOLOR_Fに変換する
	COLOR_F ColorCodeConvCOLOR_F(unsigned int _color,float _alpha=1.0f);
	//3D用のCOLOR_Fを2D用のカラーコードに変換する
	unsigned int COLOR_FConvColorCode(COLOR_F _color);

	//2つの色を混ぜる
	unsigned int ColorMix(unsigned int _color1, unsigned int _color2);
	COLOR_F ColorMix(COLOR_F _color1, COLOR_F _color2);
}