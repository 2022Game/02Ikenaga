#include "CHealAuar.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"

// コンストラク
CHealAura::CHealAura(float angle, float dist)
	: mCount(0)
	, mElapsedHealTime(0.0f)
	, mAngle(angle)
	, mDistance(dist)
	, mBaseScale(1.0f)
{
	mpHealAura = CResourceManager::Get<CModel>("Aura");
	mpHealAura->SetupEffectSettings();
}

// デストラクタ
CHealAura::~CHealAura()
{
}

// 持ち主を設定
void CHealAura::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// オーラ開始
void CHealAura::StartAura()
{
	mCount += 1;
}

// 更新
void CHealAura::Update()
{
	// 持ち主のベーススケール値から、
	// 現在の拡大率を求めて、回復オーラにも反映
	float scale = mOwner->Scale().X() / mBaseScale;
	Scale(scale, scale, scale);

	// 回転する回復オーラの中心座標
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(0.0f);
	Position(center + pos * scale);
	Rotate(0.0f, 0.3f, 0.0f);

	if (mCount >= 1)
	{
		mElapsedHealTime += Time::DeltaTime();
		if (mElapsedHealTime >= 0.0f)
		{
			CTask::SetShow(true);
		}
		if (mElapsedHealTime >= 1.5f)
		{
			CTask::SetShow(false);
			mElapsedHealTime = 0.0f;
			mCount = 0;
		}
	}
}

// 描画
void CHealAura::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpHealAura->SetColor(mColor);
		mpHealAura->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpHealAura->Render(sm * (*mpAttachMtx));
	}
}