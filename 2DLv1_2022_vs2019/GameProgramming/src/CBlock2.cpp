#include "CBlock2.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//テクスチャ座標

CBlock2::CBlock2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK2;
}