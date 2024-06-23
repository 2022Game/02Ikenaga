#include "CShieldRotate.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "Maths.h"

// コンストラク
CShieldRotate::CShieldRotate(float angle,float dist)
	: mStateStep(0)
	, mDefenseUp(false)
	, mElapsedDefenseUpTime(0.0f)
	, mAngle(angle)
	, mDistance(dist)
	, mBaseScale(1.0f)
{
	mpShieldRotate = CResourceManager::Get<CModel>("ShieldRotate");
	mpShieldRotate->SetColor(CColor(0.15f, 0.75f, 0.75f));
	mpShieldRotate->SetBlend(EBlend::eAdd);
}

// デストラクタ
CShieldRotate::~CShieldRotate()
{
}

// 持ち主を設定
void CShieldRotate::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
	mBaseScale = owner->Scale().X();
}

// 更新
void CShieldRotate::Update()
{
	mDefenseUp = mOwner->IsDefenseUp();
	mElapsedDefenseUpTime = mOwner->GetElapsedDefenseUpTime();

	// 持ち主のベーススケール値から、
	// 現在の拡大率を求めて、盾にも反映
	float scale = mOwner->Scale().X() / mBaseScale;
	Scale(scale, scale, scale);

	// 回転する盾の中心座標
	CVector center = mOwner->Position();
	// 中心座標から現在の回転で座標を求める(円運動)
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Y(5.0f);
	Position(center + pos * scale);

	// 現在位置から中心座標までのベクトルを求めて、
	// そちらの方向へ向けやる
	CVector dif = center - Position();
	dif.Y(0.0f);
	dif.Normalize();
	Rotation(CQuaternion::LookRotation(dif));

	// 毎フレーム回転角度を加算して、中心座標を中心に回転させる
	mAngle += 90.0f * Time::DeltaTime();
	if (mAngle >= 360.0f)mAngle -= 360.0f;

	if (mDefenseUp == true)
	{
		switch (mStateStep)
		{
		case 0:
			if (mElapsedDefenseUpTime <= 15.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 1:
			if (mElapsedDefenseUpTime >= 15.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 2:
			if (mElapsedDefenseUpTime >= 16.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 3:
			if (mElapsedDefenseUpTime >= 16.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 4:
			if (mElapsedDefenseUpTime >= 17.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 5:
			if (mElapsedDefenseUpTime >= 17.5f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 6:
			if (mElapsedDefenseUpTime >= 18.0f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 7:
			if (mElapsedDefenseUpTime >= 18.2f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 8:
			if (mElapsedDefenseUpTime >= 18.4f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 9:
			if (mElapsedDefenseUpTime >= 18.6f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 10:
			if (mElapsedDefenseUpTime >= 18.8f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 11:
			if (mElapsedDefenseUpTime >= 19.0f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 12:
			if (mElapsedDefenseUpTime >= 19.2f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 13:
			if (mElapsedDefenseUpTime >= 19.4f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 14:
			if (mElapsedDefenseUpTime >= 19.6f)
			{
				CTask::SetShow(true);
				mStateStep++;
			}
			break;
		case 15:
			if (mElapsedDefenseUpTime >= 19.8f)
			{
				CTask::SetShow(false);
				mStateStep++;
			}
			break;
		case 16:
			if (mElapsedDefenseUpTime >= 19.9f)
			{
				CTask::SetShow(true);
				mStateStep = 0;
			}
			break;
		}
	}
	else
	{
		CTask::SetShow(false);
	}
}

// 描画
void CShieldRotate::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpShieldRotate->Render(Matrix());
		mpShieldRotate->SetColor(mColor);
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpShieldRotate->Render(sm * (*mpAttachMtx));
	}
}