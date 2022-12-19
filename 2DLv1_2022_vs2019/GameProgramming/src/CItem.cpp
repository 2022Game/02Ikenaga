#include "CItem.h"
#include "CApplication.h"

#define TEXCOORD 40,90,263,220      //時計
//#define POINT 0
//#define AITEM1                     

//int CItem::sPoint = 0;	//POINT
//
//int  CItem::Point()
//{
//	return sPoint;
//}

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
	//case ETag::EITEM:
		//敵の衝突判定を実行
		o->Collision(o, m);
		X(X() + x);
		Y(Y() + y);
		if (x != 0.0f)
		{
			//X軸速度を0にする
			mVx = 0.0f;
			if (x > 0.0f)
			{
				mState = EState::EMOVE;
			}
			else
			{	//ジャンプでなければ泣く
				mState = EState::ECRY;
			}
		}
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if (o->Tag() == ETag::EPLAYER)
			{
				mEnabled = false;
				//mTime--;
			}
		}
		break;
	//default:
	//	if (CRectangle::Collision(o))
	//	{
	//		//mState = EState::ESTOP;
	//		mEnabled = false;
	//	}
	}
}
void CItem::Update()
{
	//通常の画像を設定
	Texture(Texture(), TEXCOORD);
	//Texture(Texture(), AITEM);
	//mTime--;
}

CItem::CItem(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
	//sPoint = POINT;
}