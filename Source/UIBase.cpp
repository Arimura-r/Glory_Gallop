#include "UIBase.h"

UIBase::UIBase(VECTOR2 _position):UIBase(_position,nullptr)
{
}

UIBase::UIBase(VECTOR2 _position, Horse* _target, bool _active)
{
	position = _position;
	target = _target;
	active = _active;
}

UIBase::~UIBase()
{
}
