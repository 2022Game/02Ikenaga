#include "CBlock.h"

#define TEXCOORD 168.0f, 188.0f, 190.0f, 160.0f//テクスチャ座標

CBlock::CBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}