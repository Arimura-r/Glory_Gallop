#include "Stage.h"

namespace
{
	const VECTOR3 STAGE_POSITION = VECTOR3(0, -100.0f, 0);
	const VECTOR3 STAGE_ROTATION = VECTOR3(0, DX_PI_F / 2, 0);
	const VECTOR3 STAGE_SCALE= VECTOR3(2, 2, 2);
}

Stage::Stage()
{
	hModel = MV1LoadModel("data\\model\\stage\\stage.mv1");
	position = STAGE_POSITION;
	rotation = STAGE_ROTATION;
	scale = STAGE_SCALE;
	
	SetDrawOrder(1000);

	new StageLine();
	new StageSeat();
	new StageWall();
}

Stage::~Stage()
{
	MV1DeleteModel(hModel);
	hModel = -1;
}

bool Stage::CollideLine(VECTOR3 _pos1, VECTOR3 _pos2, VECTOR3& _hitPos)
{
	MV1RefreshCollInfo(hModel);//ÉÇÉfÉãÇÃç¿ïWÇîΩâfÇ≥ÇπÇÈ
	MV1_COLL_RESULT_POLY ret = MV1CollCheck_Line(hModel, -1, _pos1, _pos2);

	//ìñÇΩÇ¡ÇƒÇ¢ÇÈÇ»ÇÁìñÇΩÇ¡ÇΩç¿ïWÇï‘Ç∑
	if (ret.HitFlag != 0)
	{
		_hitPos = ret.HitPosition;
		return true;
	}

	return false;
}

StageLine::StageLine()
{
	hModel = MV1LoadModel("data\\model\\stage\\stageLine.mv1");
	position = STAGE_POSITION;
	rotation = STAGE_ROTATION;
	scale = STAGE_SCALE;
	SetDrawOrder(999);
}

StageLine::~StageLine()
{
	MV1DeleteModel(hModel);
	hModel = -1;
}

StageSeat::StageSeat()
{
	hModel = MV1LoadModel("data\\model\\stage\\stageSeat.mv1");
	position = STAGE_POSITION;
	rotation = STAGE_ROTATION;
	scale = STAGE_SCALE;
	SetDrawOrder(999);
}

StageSeat::~StageSeat()
{
	MV1DeleteModel(hModel);
	hModel = -1;
}

StageWall::StageWall()
{
	hModel = MV1LoadModel("data\\model\\stage\\stageWall.mv1");
	position = STAGE_POSITION;
	rotation = STAGE_ROTATION;
	scale = STAGE_SCALE;
	SetDrawOrder(999);
}

StageWall::~StageWall()
{
	MV1DeleteModel(hModel);
	hModel = -1;
}
