#include "CBuffCircle.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Easing.h"

#define SCALE_TIME   1.0f  // スケール値が最大値になるまでの時間
#define ANIM_TIME    3.0f  // 終了待ち中のアニメーションの時間
#define ANIM_SCALE  0.25f  // 終了待ち中のアニメーションで拡縮するスケール値
#define FADEOUT_TIME 2.0f  // 終了時のフェードアウト時間

// コンストラク
CBuffCircle::CBuffCircle()
	: mState(EState::Idle)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
{
	mpBuffCircle = CResourceManager::Get<CModel>("BuffCircle");
	mpBuffCircle->SetupEffectSettings();

	SetAlpha(0.0f);
	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CBuffCircle::~CBuffCircle()
{
}

// 持ち主を設定
void CBuffCircle::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// 状態の切り替え
void CBuffCircle::ChangeState(EState state)
{
	if (state == mState)return;
	mState = state;
}

// バフサークル開始
void CBuffCircle::StartCircle()
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

// バフサークル終了
void CBuffCircle::EndCircle()
{
	if(mState != EState::Idle)
	{
		mElapsedTime = 0.0f;
		ChangeState(EState::End);
	}
}

// 自身のベーススケール値を算出
float CBuffCircle::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// 待機中
void CBuffCircle::UpdateIdle()
{
}

// 開始
void CBuffCircle::UpdateStart()
{
	float scale = CalcScale();
	
	// 経過時間に合わせて大きくする
	if (mElapsedTime < SCALE_TIME)
	{
		float percent = mElapsedTime / SCALE_TIME;
		scale *= percent;
		Scale(scale, scale, scale);

		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		Scale(scale, scale, scale);
		mElapsedTime -= SCALE_TIME;
		ChangeState(EState::Wait);
	}
}

// 終了待ち
void CBuffCircle::UpdateWait()
{
	float scale = CalcScale();

	// サインカーブでスケール値の増減値を算出
	float percent = mElapsedTime / ANIM_TIME;
	float alpha = sinf(M_PI * 2.0f * percent);
	scale += ANIM_SCALE * alpha;
	Scale(scale, scale, scale);

	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= ANIM_TIME)
	{
		mElapsedTime -= ANIM_TIME;
	}
}

// 終了
void CBuffCircle::UpdateEnd()
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
void CBuffCircle::Update()
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
	case EState::End :
		UpdateEnd();
		break;
	}

	// サークルの中心座標
	float scale = CalcScale();
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	center.Y(0.0f);
	pos.Y(0.4f * scale);
	Position(center + pos);
}

// 描画
void CBuffCircle::Render()
{
	if (mpAttachMtx == nullptr)
	{
		CColor c = mColor;
		c.R(c.R() * c.A());
		c.G(c.G() * c.A());
		c.B(c.B() * c.A());
		mpBuffCircle->SetColor(c);
		mpBuffCircle->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpBuffCircle->Render(sm * (*mpAttachMtx));
	}
}