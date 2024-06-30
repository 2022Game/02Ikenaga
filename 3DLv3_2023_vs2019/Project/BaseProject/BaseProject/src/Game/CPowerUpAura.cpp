#include "CPowerUpAura.h"
#include "CCharaBase.h"
#include "CPlayer.h"
#include "Maths.h"

// コンストラクタ
CPowerUpAura::CPowerUpAura(float angle, float width, const CVector& dire, float speed, float dist)
	: CBillBoardImage("Effect/Buff.png", ETag::eAura, ETaskPauseType::eGame)
	, mPowerUp(false)
	, mElapsedPowerUpTime(0.0f)
	, mAngle(angle)
	, mDistance(width)
	, mMoveDir(CVector::zero)
	, mMovedDist(0.0f)
	, mKillMoveDist(dist)
	, mBaseScale(1.0f)
{
	mMoveSpeed = dire.Normalized() * speed;
}

// デストラクタ
CPowerUpAura::~CPowerUpAura()
{
}

// 持ち主を設定
void CPowerUpAura::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// カラーを設定
void CPowerUpAura::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CPowerUpAura::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// バフオーラ開始
void CPowerUpAura::StartAura()
{
}

// バフオーラ終了
void CPowerUpAura::EndAura()
{
}

// 更新
void CPowerUpAura::Update()
{
	mPowerUp = mOwner->IsPowerUp();
	mElapsedPowerUpTime = mOwner->GetElapsedPowerUpTime();

	// 基底クラスの更新処理
	CBillBoardImage::Update();

	if (mPowerUp == true)
	{

		// 持ち主のベーススケール値から、
		// 現在の拡大率を求めて、オーラにも反映
		float scale = mOwner->Scale().X() / mBaseScale;
		float size = 7.0f;
		Scale(scale + size, scale + size, scale + size);

		// 回転するオーラの中心座標
		CVector center = mOwner->Position();
		// 中心座標から現在の回転で座標を求める(円運動)
		CVector pos = CVector::zero;
		center.X(center.X() + Math::Rand(-20.0f, 20.0f));
		center.Y(center.Y() + Math::Rand(5.0f, 5.0f));
		center.Z(center.Z() + Math::Rand(-20.0f, 20.0f));
		pos.Y(mMovedDist);
		Position(center + pos * scale);

		// 移動ベクトルを求める
		CVector move = mMoveSpeed * Time::DeltaTime();

		float moveDist = move.Length();
		if (mMovedDist + moveDist >= mKillMoveDist)
		{
			moveDist = mKillMoveDist - mMovedDist;
			move = move.Normalized() * moveDist;
		}

		if (mMovedDist < mKillMoveDist)
		{
			Position(Position() + move);
		}

		mMovedDist += moveDist;
		if (mMovedDist >= mKillMoveDist)
		{
			Kill();
		}
	}
	else
	{
		SetShow(false);
	}
}