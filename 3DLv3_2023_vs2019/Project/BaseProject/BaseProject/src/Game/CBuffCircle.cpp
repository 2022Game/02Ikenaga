#include "CBuffCircle.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Easing.h"

// サークルのスケール値の最大値
#define SCALE_MAX 2.0f
// サークルのスケール値が最大値になるまでの時間
#define SCALE_TIME 1.0f

// コンストラク
CBuffCircle::CBuffCircle()
	: mCount(0)
	, mPowerUp(false)
	, mElapsedPowerUpTime(0.0f)
	, mBaseScale(1.0f)
{
	mpBuffCircle = CResourceManager::Get<CModel>("BuffCircle");
	mpBuffCircle->SetupEffectSettings();
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

// バフサークル開始
void CBuffCircle::StartCircle()
{
	mCount += 1;
}

// 更新
void CBuffCircle::Update()
{
	mPowerUp = mOwner->IsPowerUp();
	mElapsedPowerUpTime = mOwner->GetElapsedPowerUpTime();
	if (mCount >= 1)
	{
		//mElapsedPowerUpTime += Time::DeltaTime();
		if (mElapsedPowerUpTime >= 0.0f)
		{
			CTask::SetShow(true);
		}
		if (mElapsedPowerUpTime >= 10.0f)
		{
			mElapsedPowerUpTime = 0.0f;
			mCount = 0;
			CTask::SetShow(false);
		}
	}
	
	// 持ち主のベーススケール値から、
	// 現在の拡大率を求めて、サークルにも反映
	float scale = mOwner->Scale().X() / mBaseScale;
	Scale(scale, scale, scale);

	//float mElapsedTime = 0.0f;
	//if (mElapsedTime < SCALE_TIME)
	//{
	//	// 経過時間に合わせて、徐々に星を大きくする
	//	float per = mElapsedTime / SCALE_TIME;
	//	if (per < 1.0f)
	//	{
	//		float scale = Easing::QuadOut(per, 1.0f, 1.0f, 1.0f);
	//		Scale(CVector::one * scale * SCALE_MAX);
	//	}

	//	mElapsedTime += Time::DeltaTime();
	//}

	float size = 0.0f;
	if (mElapsedPowerUpTime >= 0.0f)
	{
		if (mElapsedPowerUpTime >= 0.1f)
		{
			Scale(CVector::one * (scale + size));
			size += 0.1f;
			if (mElapsedPowerUpTime >= 0.2f)
			{
				Scale(CVector::one * (scale + size));
				size += 0.1f;
				if (mElapsedPowerUpTime >= 0.3f)
				{
					Scale(CVector::one * (scale + size));
					size += 0.1f;
					if (mElapsedPowerUpTime >= 0.4f)
					{
						Scale(CVector::one * (scale + size));
						size += 0.1f;
						if (mElapsedPowerUpTime >= 0.5f)
						{
							Scale(CVector::one * (scale + size));
							size -= 0.1f;
							if (mElapsedPowerUpTime >= 0.6f)
							{
								Scale(CVector::one * (scale + size));
								size -= 0.1f;
								if (mElapsedPowerUpTime >= 0.7f)
								{
									Scale(CVector::one * (scale + size));
									size -= 0.1f;
									if (mElapsedPowerUpTime >= 0.8f)
									{
										Scale(CVector::one * (scale + size));
										size -= 0.1f;
										if (mElapsedPowerUpTime >= 0.9f)
										{
											Scale(CVector::one * (scale + size));
											size -= 0.1f;
											if (mElapsedPowerUpTime >= 1.0f)
											{
												Scale(CVector::one * (scale + size));
												size += 0.1f;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	CDebugPrint::Print(" PUtime: %f\n", mElapsedPowerUpTime);

	// サークルの中心座標
	CVector center = mOwner->Position();
	CVector pos = CVector::zero;
	pos.Y(0.4f);
	Position(center + pos * scale);
}

// 描画
void CBuffCircle::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpBuffCircle->SetColor(mColor);
		mpBuffCircle->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpBuffCircle->Render(sm * (*mpAttachMtx));
	}
}