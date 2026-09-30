#include "ArrayNumberFunc.h"

int ArrayNumberFunc::GetArrayBeginConsectiveNum(const std::vector<int>& _array)
{
    int ret = 0;
    int beforeNum = _array[0];
    for (int i = 1; i < _array.size(); i++)
    {
        if (_array[i] != beforeNum + 1)
            break;

        ret++;
        beforeNum++;
    }

    return ret;
}
