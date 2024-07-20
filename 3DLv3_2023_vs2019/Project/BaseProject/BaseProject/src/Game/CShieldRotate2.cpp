#include "CShieldRotate2.h"
#include "CCollisionManager.h"
#include "CCharaBase.h"
#include "Maths.h"

// コンストラク
CShieldRotate2::CShieldRotate2(float angle, float dist, float height)
	: mDefenseUp(false)
	, mElapsedDefenseUpTime(0.0f)
	, mAngle(angle)
	, mDistance(dist)
	, mHeight(height)
{
	mpShieldRotate2 = CResourceManager::Get<CModel>("ShieldRotate");
	mpShieldRotate2->SetColor(CColor(0.15f, 0.75f, 0.75f));
	mpShieldRotate2->SetBlend(EBlend::eAdd);
}

// デストラクタ
CShieldRotate2::~CShieldRotate2()
{
}

// 持ち主を設定
void CShieldRotate2::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
}

// 更新
void CShieldRotate2::Update()
{
	mDefenseUp = mOwner->IsDefenseUp();
	mElapsedDefenseUpTime = mOwner->GetElapsedDefenseUpTime();

	// 回転する盾の中心座標
	CVector center = mOwner->Position();
	// 中心座標から現在の回転で座標を求める(円運動)
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Y(mHeight);
	Position(center + pos);

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
		CTask::SetShow(true);
	}	
	else
	{
		CTask::SetShow(false);
	}
}

// 描画
void CShieldRotate2::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpShieldRotate2->Render(Matrix());
		mpShieldRotate2->SetColor(mColor);
	}
	else
	{
		CMatrix sm;
		sm.Scale(100.0f, 100.0f, 100.0f);
		mpShieldRotate2->Render(sm * (*mpAttachMtx));
	}
}