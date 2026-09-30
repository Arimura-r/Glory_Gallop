#include "DrawNumFunc.h"

void DrawNumFunc::DrawRotaNum(float _x, float _y, int _num, int _interval, int _numImage, int _width, int _height, float _exRate, float _angle, int _defaultZeroNum)
{
	int tmp = _num;
	int dig = 0;
	int commaNum = 0;
	do
	{
		DrawRectRotaGraphF(_x - (_interval * dig) - commaNum * 8, _y, tmp % 10 * _width, 0, _width, _height, _exRate, _angle, _numImage, true, false);
		tmp = tmp / 10;
		dig++;
	} while (tmp > 0 || dig < _defaultZeroNum);
}
