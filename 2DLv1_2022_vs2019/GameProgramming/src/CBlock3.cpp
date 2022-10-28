#include "CBlock3.h"
#include "CApplication.h"

#define TEXCOORD 310.0f, 380.0f, 150.0f, 190.0f//テクスチャ座標

void CBlock3::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}


void CBlock3::Collision(CCharacter* m, CCharacter* o)
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
		    mVy = -mVy;
		}
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->State() == EState::EJUMP)
			{
				mState = EState::EMOVE;
			}
		}
		break;
	//case ETag::EBLOCK:
	//	break;
	/*case ETag::EPLAYER:
		break;*/
	}
}

CBlock3::CBlock3(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK3;
	//Y軸速度の初期値を移動速度にする
	mVy = SOKUDO;
}

void CBlock3::Update()
{
	switch (mState)
	{
	case EState::ECRY:
		//泣く画像を設定
		//Texture(Texture(), TEXCRY);
		break;
	case EState::EMOVE:
		//Y軸速度分、Y座標を更新する
		Y(Y() + mVy);
		const int PITCH = 32;//画像を切り替える間隔
		if ((int)Y() % PITCH < PITCH / 2)
		{
			if (mVy < 0.0f) //左へ移動
			{
				//左向き１を設定
				//Texture(Texture(), TEXLEFT1);
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD);
			}
		}
		else
		{
			if (mVy < 0.0f) //左へ移動
			{
				//左向き2を設定
				//Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2番目の画像を設定
				//Texture(Texture(), TEXCOORD2);
			}
		}
		break;
	}
}