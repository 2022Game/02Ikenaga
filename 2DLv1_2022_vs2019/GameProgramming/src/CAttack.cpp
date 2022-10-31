#include "CAttack.h"
#include"CPlayer2.h"
#include "CApplication.h"

#define TEXCOORD 310.0f, 380.0f, 150.0f, 190.0f//テクスチャ座標

void CAttack::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

CAttack::CAttack(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EPLAYER;
	//Y軸速度の初期値を移動速度にする
	; mVx = SOKUDO;
}

void CAttack::Update()
{
	if (mState == EState::EMOVE)
		{
		float x =X() + W();
			if (x > 400.0f)
			{
				x = 0.0f;
			}
			/*if (y > 600)
			{
				y = 700.0f;
			}*/
			X(x);
		}
}