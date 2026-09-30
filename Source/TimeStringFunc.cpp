#include "TimeStringFunc.h"

std::string TimeStringFunc::GetTimeString(float _time, char _intervalChar, int _decimalPlaces, bool _drawZero)
{
	int decimalNum = (int)((_time - (int)_time) * pow(10, _decimalPlaces));//¬”ˆÈ‰º‚Ì”’l
	int second = ((int)_time) % 60;//•b”
	int minute = (((int)_time) / 60) % 60;//•ª”
	int hour = (((int)_time) / 60) / 60;//ŠÔ

	std::string ret;
	char str[200];

	if (_drawZero || hour > 0)
	{
		sprintfDx(str, "%d%c", hour, _intervalChar);
		ret += str;
	}
	if (_drawZero || minute > 0)
	{
		sprintfDx(str, "%d%c", minute, _intervalChar);
		ret += str;
	}
	sprintfDx(str, "%d.", second, _intervalChar);
	ret += str;
	if (_decimalPlaces > 0)
	{
		sprintfDx(str, "%d", decimalNum);
		ret += str;
	}

	return ret;
}
