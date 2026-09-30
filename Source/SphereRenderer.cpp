#include "SphereRenderer.h"

void SphereRenderer::Draw()
{
	Transform transform = GetOwner()->GetTransform()->GetGloabalTransform();
	float maxScl = max(transform.scale.x, max(transform.scale.y, transform.scale.z));//Å‚à‘å‚«‚¢scale‚ğ“üè‚·‚é
	float radius = maxScl * DEFAULT_SPHERE_RADIUS;//‹…‚Ì”¼Œa‚ÍŠî–{’l*Scale‚ÌÅ‘å’l‚Å‹‚ß‚é

	//‹…‚Ì•`‰æ
	DrawSphere3D(transform.position, radius, 10, color, color, true);
}
