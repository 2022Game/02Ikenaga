#include "CGoal.h"

#define TEXCOORD 195.0f, 280.0f, 418.0f, 330.0f//テクスチャ座標

CGoal::CGoal(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EGOAL;
}