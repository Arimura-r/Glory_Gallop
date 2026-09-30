#pragma once
#include "Component.h"

class SphereRenderer :public Component
{
public:
	SphereRenderer(){ color = 0xffffff; }
	void Draw()override;
	void SetColor(int _color) { color = _color; }

private:
	int color;
	const float DEFAULT_SPHERE_RADIUS = 100.0f;
};