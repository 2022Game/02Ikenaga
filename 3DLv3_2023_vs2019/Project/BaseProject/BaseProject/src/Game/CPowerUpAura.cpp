#include "CPowerUpAura.h"
#include "CCharaBase.h"
#include "CPlayer.h"
#include "Maths.h"

// コンストラクタ
CPowerUpAura::CPowerUpAura(const CVector& pos, const CVector& dir, float speed, float dist)
	: CBillBoardImage("Effect/Ball.png", ETag::eAura, ETaskPauseType::eGame)
	, mMoveDir(CVector::zero)
	, mMovedDist(0.0f)
	, mKillMoveDist(dist)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
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

// 自身のベーススケール値を算出
float CPowerUpAura::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// 更新
void CPowerUpAura::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 移動ベクトルを求める
	CVector move = mMoveSpeed * Time::DeltaTime();

	float moveDist = move.Length();
	if (mMovedDist + moveDist >= mKillMoveDist)
	{
		moveDist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * moveDist;
	}
	move.Y(0.1f);
	Position(Position() + move);

	mMovedDist += moveDist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}