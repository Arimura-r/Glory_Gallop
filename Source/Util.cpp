#include "Util.h"

std::string Util::GetRandName(const std::string& _baseName, const int& _randNum)
{
	int r = GetRand(_randNum - 1);
	return _baseName + std::to_string(r);
}
