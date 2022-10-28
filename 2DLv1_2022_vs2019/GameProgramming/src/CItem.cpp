#include "CItem.h"
#include "CApplication.h"

#define TEXCOORD 40,90,263,220      //時計(時間が減る)
//#define AITEM1                      //ポイント(コイン)

void CItem::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CItem::Collision(CCharacter* m, CCharacter* o)
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
void CItem::Update()
{
	//通常の画像を設定
	Texture(Texture(), TEXCOORD);
	//Texture(Texture(), AITEM);
}

CItem::CItem(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
}