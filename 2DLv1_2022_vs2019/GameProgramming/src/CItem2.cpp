#include "CItem2.h"
#include "CApplication.h"

#define TEXCOORD1  151,190,262,225       //靴、足(スピードアップ)//ポイント(コイン)

void CItem2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem2::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//折り返しに当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVx = -mVx;
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		break;
	}
}
void CItem2::Update()
{
	//通常の画像を設定
	Texture(Texture(), TEXCOORD1);
}

CItem2::CItem2(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD1);
	mTag = ETag::EITEM;
}