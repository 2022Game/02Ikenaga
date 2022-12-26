#include "CBlock5.h"
#include"CApplication.h"
#include"CEnemy4.h"

#define TEXCOORD 315.0f, 290.0f, 289.0f, 319.0f//テクスチャ座標

CBlock5::CBlock5(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK5;
}

void CBlock5::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::EBLOCK:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVx = -mVx;
			//-mVx <=mVx;
		}
		break;
	case ETag::EBLOCK5:
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			mEnabled = false;
			//敵を生成して、キャラクタマネージャに追加
			CApplication::CharacterManager()->Add(
				new CEnemy4(X(),
					Y(),
					TIPSIZE, TIPSIZE, CApplication::Texture8()));
			if (o->State() == EState::EJUMP)
			{
				//mEnabled = false;
			}
		}
		break;
	}
}