#include "CBuffAura.h"
#include "CCharaBase.h"
#include "Maths.h"
#include "CPlayer.h"

#define SCALE_TIME   0.1f  // スケール値が最大値になるまでの時間
#define ANIM_TIME    0.1f  // 終了待ち中のアニメーションの時間
#define ANIM_SCALE   1.5f  // 終了待ち中のアニメーションで拡縮するスケール値
#define FADEOUT_TIME 0.5f  // 終了時のフェードアウト時間

// コンストラクタ
CBuffAura::CBuffAura()
	: CBillBoardImage("Effect/Aura1.png", ETag::eAura, ETaskPauseType::eGame)
	, mState(EState::Idle)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
{
	SetAlpha(0.0f);
	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CBuffAura::~CBuffAura()
{
}

// 持ち主を設定
void CBuffAura::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// カラーを設定
void CBuffAura::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CBuffAura::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 状態の切り替え
void CBuffAura::ChangeState(EState state)
{
	if (state == mState)return;
	mState = state;
}

// バフオーラ開始
void CBuffAura::StartAura()
{
	if (mState == EState::Idle)
	{
		SetEnable(true);
		SetShow(true);
		ChangeState(EState::Start);
		Scale(0.0f, 0.0f, 0.0f);
		SetAlpha(1.0f);
	}
}

// バフオーラ終了
void CBuffAura::EndAura()
{
	if (mState != EState::Idle)
	{
		mElapsedTime = 0.0f;
		ChangeState(EState::End);
	}
}

// 自身のベーススケール値を算出
float CBuffAura::CalcScaleXZ() const
{
	return mOwner->Scale().X() / mBaseScale;
}

float CBuffAura::CalcScaleY() const
{
	return mOwner->Scale().Y() / mBaseScale * 0.5f;
}

// 待機中
void CBuffAura::UpdateIdle()
{
}

// 開始
void CBuffAura::UpdateStart()
{
	float scaleXZ = CalcScaleXZ();
	float scaleY = CalcScaleY();

	// 経過時間に合わせて大きくする
	if (mElapsedTime < SCALE_TIME)
	{
		float percent = mElapsedTime / SCALE_TIME;
		scaleXZ *= percent;
		scaleY *= percent;
		Scale(scaleXZ, scaleY, scaleXZ);

		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		Scale(scaleXZ, scaleY, scaleXZ);
		mElapsedTime -= SCALE_TIME;
		ChangeState(EState::Wait);
	}
}

// 終了待ち
void CBuffAura::UpdateWait()
{
	float scaleXZ = CalcScaleXZ();
	float scaleY = CalcScaleY();

	// サインカーブでスケール値の増減値を算出
	float percent = mElapsedTime / ANIM_TIME;
	float alpha = sinf(M_PI * 1.0f * percent);
	scaleXZ += ANIM_SCALE * alpha;
	scaleY += ANIM_SCALE * alpha;
	Scale(scaleXZ, scaleY, scaleXZ);

	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= ANIM_TIME)
	{
		mElapsedTime -= ANIM_TIME;
	}
}

// 終了
void CBuffAura::UpdateEnd()
{
	// フェードアウト中
	if (mElapsedTime < FADEOUT_TIME)
	{
		// 経過時間からアルファ値を求める
		float alpha = 1.0f - mElapsedTime / FADEOUT_TIME;
		SetAlpha(alpha);

		mElapsedTime += Time::DeltaTime();
	}
	// フェードアウト終了
	else
	{
		SetAlpha(0.0f);
		ChangeState(EState::Idle);
		mElapsedTime = 0.0f;
		SetEnable(false);
		SetShow(false);
	}
}

// 更新
void CBuffAura::Update()
{
	switch (mState)
	{
	case EState::Idle:
		UpdateIdle();
		break;
	case EState::Start:
		UpdateStart();
		break;
	case EState::Wait:
		UpdateWait();
		break;
	case EState::End:
		UpdateEnd();
		break;
	}

	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 回転するバフオーラの中心座標
	float scaleXZ = CalcScaleXZ();
	float scaleY = CalcScaleY();
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	//pos.X()* scaleXZ;
	pos.Y(11.0f * scaleY); 
	//pos.Z()* scaleXZ;
	Position(center + pos);
}