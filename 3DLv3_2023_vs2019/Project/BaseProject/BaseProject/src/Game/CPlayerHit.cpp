#include "CPlayerHit.h"
#include "CCharaBase.h"
#include "Maths.h"
#include "Easing.h"

#define SCALE_TIME  0.01f  // スケール値が最大値になるまでの時間
#define ANIM_TIME   0.13f  // 終了待ち中のアニメーションの時間
#define ANIM_SCALE  15.0f  // 終了待ち中のアニメーションで拡縮するスケール値
#define FADEOUT_TIME 0.0f  // 終了時のフェードアウト時間

// コンストラクタ
CPlayerHit::CPlayerHit(float height)
	: CBillBoardImage("Effect/hitEffect.png", ETag::eHit, ETaskPauseType::eGame)
	, mState(EState::Idle)
	, mElapsedTime(0.0f)
	, mBaseScale(1.0f)
	, mHeight(height)
{
	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CPlayerHit::~CPlayerHit()
{
	Kill();
}

// 状態の切り替え
void CPlayerHit::ChangeState(EState state)
{
	if (state == mState)return;
	mState = state;
}

// 持ち主の設定
void CPlayerHit::SetOwner(CCharaBase* owner)
{
	CBillBoardImage::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// ヒットエフェクト開始
void CPlayerHit::StartHit()
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

// ヒットエフェクト終了
void CPlayerHit::EndHit()
{
	if (mState != EState::Idle)
	{
		mElapsedTime = 0.0f;
		ChangeState(EState::End);
	}
}

// 自身のベーススケール値を算出
float CPlayerHit::CalcScale() const
{
	return mOwner->Scale().X() / mBaseScale;
}

// 待機中
void CPlayerHit::UpdateIdle()
{
}

// 開始
void CPlayerHit::UpdateStart()
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
void CPlayerHit::UpdateWait()
{
	float scale = CalcScale();

	// サインカーブでスケール値の増減値を算出
	float percent = mElapsedTime / ANIM_TIME;
	float alpha = sinf(M_PI * 0.5f * percent);
	scale += 1.0f * alpha;
	Scale(scale * 13.0f, scale * 13.0f, scale * 13.0f);

	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= ANIM_TIME)
	{
		//mElapsedTime -= ANIM_TIME;
		if (mState != EState::Idle)
		{
			mElapsedTime = 0.0f;
			ChangeState(EState::End);
		}
	}
}

// 終了
void CPlayerHit::UpdateEnd()
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
void CPlayerHit::Update()
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

	float scale = CalcScale();
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(mHeight);
	Position(center + pos * scale);
}