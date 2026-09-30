#include "Sky.h"

Sky::Sky()
{
	hModel = MV1LoadModel("data\\model\\stage\\sky.mv1");
	SetDrawOrder(1001);
	scale = VECTOR3(50, 50, 50);
}

Sky::~Sky()
{
}
