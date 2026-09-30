#include "BoxCollisionFunc.h"

VECTOR3 BoxCollisionFunc::GetMinPosition(VECTOR3 _pos1, VECTOR3 _pos2)
{
    return VECTOR3(min(_pos1.x, _pos2.x), min(_pos1.y, _pos2.y), min(_pos1.z, _pos2.z));
}

VECTOR3 BoxCollisionFunc::GetMaxPosition(VECTOR3 _pos1, VECTOR3 _pos2)
{
    return VECTOR3(max(_pos1.x, _pos2.x), max(_pos1.y, _pos2.y), max(_pos1.z, _pos2.z));
}

bool BoxCollisionFunc::IsInBox(VECTOR3 _pos, VECTOR3 _boxPos1, VECTOR3 _boxPos2)
{
    VECTOR3 boxMinPos = GetMinPosition(_boxPos1, _boxPos2);
    VECTOR3 boxMaxPos = GetMaxPosition(_boxPos1, _boxPos2);

    if (boxMinPos.x < _pos.x && _pos.x < boxMaxPos.x&&
        boxMinPos.y < _pos.y && _pos.y < boxMaxPos.y&&
        boxMinPos.z < _pos.z && _pos.z < boxMaxPos.z)
        return true;

    return false;
}
