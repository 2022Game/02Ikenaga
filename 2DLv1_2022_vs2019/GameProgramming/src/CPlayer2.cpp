#include "CPlayer2.h"
#include "CApplication.h"
#include"CAttack.h"
#include"CAttack2.h"

#define TEXCOORD 30, 66, 100, 50	//テクスチャマッピング
#define TEXCRY 196, 216, 158, 128	//テクスチャマッピング
#define GRAVITY (TIPSIZE / 16.0f)	//重力加速度16
#define GRAVITY10 (TIPSIZE / 0.2f)
#define JUMPV0 (TIPSIZE / 1.8f)//ジャンプの初速
#define JUMPVO2 (TIPSIZE / -1.8f)
#define TEXCOORD2 150,189,340,293   //右向き2
#define TEXCOORD3 510,550,340,293   //右向き
#define TEXCOORD4 750,790,340,293   //右向きジャンプ
#define TEXCOORD5 790,750,340,293    //左向きジャンプ
#define TEXLEFT1 190,150,340,293	//右向き2
#define TEXLEFT2 550,510,340,293	//左向き2
#define TEXSLEEPY  860,925,226,190   //お休み
#define TEXSUPIDO 210,250,405,360 //スピードアップ
#define TEXSUOIDOUP 250,210,405,360//反対のスピードアップ
#define SUPIDOTEX  570,610,408,361 //スピードアップ2
#define SUOIDOUPTEX 610,570,408,361 //反対スピードアップ2
#define TEXJUMP 680,715,270,220  //スピードアップ中のジャンプ
#define JUMPTEX 715,680,270,220  //反対スピードアップ中のジャンプ
#define TYAKUTI 748,790,220,180 //着地
#define TEXTYAKUTI 790,748,220,180 //反対の着地

#define TEXPOINT 200,200,100,55
#define TEXATTACK 640,710,100,55   //攻撃モーション
#define TEXATTACK2 710,640,100,55   //反対攻撃モーション
#define MOSYONN 500,550,100,39
#define VELOCITY 2.0f	//移動速度
#define VELOCITY2 2.5f
#define SOKUDO 1.0f
#define HP 3 //HPの初期値は3
#define POINT 0
#define SE_JUMP "res\\jump.wav" //ジャンプの音声ファイル
#define ATTACK1  860,960,225,180   //攻撃のテクスチャ

//int CUi::mTime = 0;
//void CUi::Time(int time)
//{
//	 mTime=time;
//}

int CPlayer2::sTime = +100;

int CPlayer2::sPoint = 0;	//POINT

int  CPlayer2::Point()
{
	return sPoint;
}

int CPlayer2::sHp = 0;	//HP

int CPlayer2::Hp()
{
	return sHp;
}

void CPlayer2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CPlayer2::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o, &x, &y))
		{
			//敵の衝突判定を実行
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			/*X(X() + x);
			Y(Y() + y);*/
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
				else
				{	//ジャンプでなければ泣く
					mState = EState::ECRY;
					if (mInvincible == 0)
					{
						mInvincible = 300;
						sPoint--;
					}
				}
			}
			else
			{	//ジャンプでなければ泣く
				mState = EState::ECRY;
				if (mInvincible == 0)
				{
					mInvincible = 300;
					sPoint--;
				}
			}
		}
		break;
	case ETag::EITEM:
		if (CRectangle::Collision(o, &x, &y))
		{
			//時計の衝突判定を実行
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
				else
				{	//ジャンプでなければ泣く
					mState = EState::EDOWN;
					if (mdown == 0)
					{
						mdown = 1;
						int mTime = CGame::mTime;
						CGame::mTime = mTime - sTime;
					}
				}
			}
			else
			{	//ジャンプでなければ泣く
				mState = EState::EDOWN;
				if (mdown == 0)
				{
					mdown = 1;
					int mTime = CGame::mTime;
					CGame::mTime = mTime-sTime;
				}
			}
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
					mState = EState::EDOWN;
					if (mdown == 0)
					{
						mdown = 1;
						int mTime = CGame::mTime;
						CGame::mTime = mTime - sTime;
					}
				}
			}
			else
			{	//ジャンプでなければ泣く
				mState = EState::EDOWN;
				if (mdown == 0)
				{
					mdown = 1;
					int mTime = CGame::mTime;
					CGame::mTime = mTime - sTime;
				}
			}
		}
				break;
	case ETag::EITEM2:
		if (CRectangle::Collision(o, &x, &y))
		{
			//靴の衝突判定を実行
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			//着地した時
			if (y == 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
				else
				{	//ジャンプでなければ泣く
					mState = EState::EUP;
					if (mUp == 0)
					{
						mUp = 100;
					}
				}
			}
			else
				{	//ジャンプでなければ泣く
					mState = EState::EUP;
					if (mUp == 0)
					{
						mUp = 100;
					}
				}
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
					mState = EState::EUP;
					if (mUp == 0)
					{
						mUp = 100;
					}
				}
			}
			else
			{	//ジャンプでなければ泣く
				mState = EState::EUP;
				if (mUp == 0)
				{
					mUp = 100;
				}
			}
		}
		case ETag::EITEM3:
			if (CRectangle::Collision(o, &x, &y))
			{
				//コインの衝突判定を実行
				o->Collision(o, m);
				X(X() + x);
				Y(Y() + y);
				//着地した時
				if (y != 0.0f)
				{
					//Y軸速度を0にする
					mVy = 0.0f;
					if (y > 0.0f)
					{
						mState = EState::EMOVE;
					}
					else
					{	//ジャンプでなければ泣く
						mState = EState::EHIT;
						if (mnothing == 0)
						{
							mnothing = 1;
							sPoint++;
						}
					}
				}
				else
				{	//ジャンプでなければ泣く
					mState = EState::EHIT;
					if (mnothing == 0)
					{
						mnothing = 1;
						sPoint++;
						//sHp--;
					}
				}
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
						mState = EState::EHIT;
						if (mnothing == 0)
						{
							mnothing = 1;
							sPoint++;
						}
					}
				}
				else
				{	//ジャンプでなければ泣く
					mState = EState::EHIT;
					if (mnothing == 0)
					{
						mnothing = 1;
						sPoint++;
					}
				}
			}
			break;
	case ETag::EGOAL:
			if (CRectangle::Collision(o, &x, &y))
		{
			//敵の衝突判定を実行
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					//mState = EState::ESTOP;
					mState = EState::EMOVE;
				}
			}
			if (x != 0.0f)
			{
				//Y軸速度を0にする
				mVx = 0.0f;
				if (x > 0.0f)
				{
					//mState = EState::ESTOP;
					mState = EState::EMOVE;
				}
			}
		}
			break;
	case ETag::EPLAYER:
		break;
	case ETag::EBLOCK5:
		if (CRectangle::Collision(o, &x, &y))
		{
			//敵の衝突判定を実行
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			/*X(X() + x);
			Y(Y() + y);*/
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
				else
				{	//ジャンプでなければ泣く
					//mState = EState::ECRY;
					//if (mInvincible == 0)
					//{
					//	mInvincible = 300;
					//	//sPoint--;
					//}
				}
			}
			else
			{	//ジャンプでなければ泣く
				//mState = EState::ECRY;
				//if (mInvincible == 0)
				//{
				//	mInvincible = 300;
				//	//sPoint--;
				//}
			}
		}
		break;
	case ETag::EBLOCK:
	case ETag::EBLOCK4:
	case ETag::EBLOCK3:
		//default:
				//float x, y;
				//switch (o->Tag())
				//{
				//if (mTag == ETag::EBLOCK3)
				//{
				//	if (mInput.Key('S'))
				//	{
				//			//Y座標にY軸速度を加える
				//			Y(Y() + mVy);
				//			mVy = 0.18f;
				//			//Y軸速度に重力を減算する
				//			mVy = -mVy;  //GRAVITY10;
				//	}
				//}
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EMOVE;
				}
			}
		}
		break;
		if (x != 0.0f)
		{
			//Y軸速度を0にする
			mVx = 0.0f;
			if (x > 0.0f)
			{
				mState = EState::EMOVE;
			}
		}
		break;
	}
}

CPlayer2::CPlayer2(float x, float y, float w, float h, CTexture* pt)
	: mInvincible(0), mVy(0.0f),mVx(0.0f)
	,mnothing(0),mUp(0),mdown(0),mpUi(nullptr)
{
	Set(x, y, w, h);
	Texture(pt, TYAKUTI);
	mTag = ETag::EPLAYER;
	sHp = HP;
	sPoint = POINT;
	//ジャンプ音ロード
	mSoundJump.Load(SE_JUMP);
}

void CPlayer2::Update()
{
	//無敵時間中はカウントを減少する
	if (mInvincible > 0)
	{
		mInvincible--;
	}	
		if (mnothing > 0)
	{
		mnothing--;
	}
		if (mUp > 0)
		{
			mUp--;
		}
		if (mdown > 0)
		{
			mdown--;
		}
		if (mState != EState::EJUMP)
		{
			if (mState != EState::EUP)
			{
				if (mInput.Key('J'))
				{
					//ジャンプ音
					mSoundJump.Play(0.1f);
					mVy = JUMPV0;
					mState = EState::EJUMP;
				}
			}
		}
		if (mState== EState::EATTACK)
		{
			if (mInput.Key(VK_SHIFT))
			{
				CApplication::CharacterManager()->Add(
					new CAttack(X(), Y() + H() - 20.0f
						, 20.0f, 16.0f, 106, 20, 50, 92, CApplication::Texture3()));
			}
		}
	if (mInput.Key('A'))
	{
		mVx = -VELOCITY;
		//		float x = X() - 4.0f;
		X(X() + mVx);
		if (mState == EState::EUP)
		{
			mVx = -VELOCITY2;
			X(X() + mVx);
			Y(Y() + mVy);
			//mVy -= GRAVITY10;
		}
	}
	if (mInput.Key('D'))
	{
		mVx = VELOCITY;
		//float x = X() - 4.0f;
		X(X() + mVx);
			if (mState == EState::EUP)
			{
				mVx = VELOCITY2;
				X(X() + mVx);
				Y(Y() + mVy);
				//mVy -= GRAVITY10;
			}
	}
	//Y座標にY軸速度を加える
	Y(Y() + mVy);
	//Y軸速度に重力を減算する
	mVy -= GRAVITY;
	if (mState == EState::EHIT)
	{
		if (mnothing == 0)
		{
			mnothing = 1;
		}
	}
	if (mnothing > 0)
	{
		mState = EState::EHIT;
		Texture(Texture(), TEXPOINT);
	}
		if (mState == EState::EUP)
		{
			Texture(Texture(), TEXPOINT);
			if (mUp == 0)
			{
				mUp = 100;
			}
		}
		if (mUp > 0)
		{
			mState = EState::EUP;
		}

		if (mState == EState::EDOWN)
		{
			Texture(Texture(), TEXPOINT);
			if (mdown == 0)
			{
				mdown = 1;
			}
		}
		if (mdown > 0)
		{
			mState = EState::EDOWN;
		}
		if (mState == EState::ECRY)
	{
		Texture(Texture(), TEXSLEEPY);
		if (mInvincible == 0)
		{
			mInvincible = 300;
			mState = EState::ESTOP;
		}
	}
	if (mInvincible > 0)
	{
		Texture(Texture(), TEXSLEEPY);
	}
	else
	{
		const int PITCH = 32;//画像を切り替える間隔
		if ((int)X() % PITCH < PITCH / 2)
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き１を設定
				Texture(Texture(), TEXLEFT1);
				if (mState == EState::EJUMP)
				{
					Texture(Texture(), TEXTYAKUTI);
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), TEXCOORD5);
					}
				}
				if (mState != EState::EJUMP)
				{
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), JUMPTEX);
					}
				}
				if (mState == EState::EUP)
				{
					Texture(Texture(), TEXSUOIDOUP);
				}
					if (mInput.Key(VK_SHIFT))
					{
						CApplication::CharacterManager()->Add(
							new CAttack2(X(), Y() + H() - 20.0f
								, 20.0f, 16.0f, 106, 20, 50, 92, CApplication::Texture3()));
						Texture(Texture(), TEXATTACK2);
					}
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD3);
				if (mState == EState::EJUMP)
				{
					Texture(Texture(), TYAKUTI);
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), TEXCOORD4);
					}
				}
				if (mState != EState::EJUMP)
				{
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), TEXJUMP);
					}
				}
				if (mState == EState::EUP)
				{
					Texture(Texture(), TEXSUPIDO);
				}
				if (mInput.Key(VK_SHIFT))
				{
					CApplication::CharacterManager()->Add(
						new CAttack(X(), Y() + H() - 20.0f
							, 20.0f, 16.0f, 106, 20, 50, 92, CApplication::Texture3()));
					Texture(Texture(), TEXATTACK);
				}
			}
		}
		else
		{
			if (mVx < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXLEFT2);
				if (mState == EState::EJUMP)
				{
					Texture(Texture(), TEXTYAKUTI);
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), TEXCOORD5);
					}
				}
				if (mState != EState::EJUMP)
				{
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), JUMPTEX);
					}
				}
				if (mState == EState::EUP)
				{
					Texture(Texture(), SUOIDOUPTEX);
				}
				if (mInput.Key(VK_SHIFT))
				{
					CApplication::CharacterManager()->Add(
						new CAttack2(X(), Y() + H() - 20.0f
							, 20.0f, 16.0f, 106, 20, 50, 92, CApplication::Texture3()));
					Texture(Texture(), TEXATTACK2);
				}
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(), TEXCOORD2);
				if (mState == EState::EJUMP)
				{
					Texture(Texture(), TYAKUTI);
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), TEXCOORD4);
					}
				}
				if (mState != EState::EJUMP)
				{
					if (mInput.Key('J'))
					{
						mState = EState::EJUMP;
						Texture(Texture(), TEXJUMP);
					}
				}
				if (mState == EState::EUP)
				{
					Texture(Texture(), SUPIDOTEX);
				}
				if (mInput.Key(VK_SHIFT))
				{
					CApplication::CharacterManager()->Add(
						new CAttack(X(), Y() + H() - 20.0f
							, 20.0f, 16.0f, 106, 20, 50, 92, CApplication::Texture3()));
					Texture(Texture(), TEXATTACK);
				}
			}
		}
	}
}

