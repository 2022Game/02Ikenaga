#include "CHealCircle.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CStarAura.h"
#include "Maths.h"

// コンストラク
CHealCircle::CHealCircle()
	: mCount(0)
	, mElapsedHealTime(0.0f)
	, mBaseScale(1.0f)
{
	mpHealCircle = CResourceManager::Get<CModel>("HealCircle");
	mpHealCircle->SetupEffectSettings();
}

// デストラクタ
CHealCircle::~CHealCircle()
{
}

// 持ち主を設定
void CHealCircle::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// 回復サークル開始
void CHealCircle::StartCircle()
{
	mCount += 1;
}

// 削除
void CHealCircle::Delete()
{
	if (mCount >= 0)
	{
		Kill();
	}
}

// 更新
void CHealCircle::Update()
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
			CStarAura* star = new CStarAura
			(
				center,
				VectorY(),
				10.0f,
				20.0f
			);
			star->SetColor(CColor(0.0f, 1.0f, 0.0f));
			center.X(center.X() + Math::Rand(-40.0f, 40.0f));
			center.Y(center.Y() + Math::Rand(-30.0f, 30.0f));
			center.Z(center.Z() + Math::Rand(-40.0f, 40.0f));
			star->Position(center);
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
void CHealCircle::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpHealCircle->SetColor(mColor);
		mpHealCircle->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpHealCircle->Render(sm * (*mpAttachMtx));
	}
}