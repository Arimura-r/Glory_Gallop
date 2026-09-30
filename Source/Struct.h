#pragma once
#include "../Library/myDxLib.h"

inline VECTOR3 operator *(const VECTOR3 v1, const VECTOR3 v2) { return VECTOR3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }