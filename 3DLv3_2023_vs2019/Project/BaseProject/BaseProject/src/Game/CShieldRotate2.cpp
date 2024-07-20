#include "CShieldRotate2.h"
#include "CCollisionManager.h"
#include "CCharaBase.h"
#include "Maths.h"

// �R���X�g���N
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

// �f�X�g���N�^
CShieldRotate2::~CShieldRotate2()
{
}

// �������ݒ�
void CShieldRotate2::SetOwner(CCharaBase* owner)
{
	CWeapon::SetOwner(owner);
}

// �X�V
void CShieldRotate2::Update()
{
	mDefenseUp = mOwner->IsDefenseUp();
	mElapsedDefenseUpTime = mOwner->GetElapsedDefenseUpTime();

	// ��]���鏂�̒��S���W
	CVector center = mOwner->Position();
	// ���S���W���猻�݂̉�]�ō��W�����߂�(�~�^��)
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * mDistance);
	pos.Y(mHeight);
	Position(center + pos);

	// ���݈ʒu���璆�S���W�܂ł̃x�N�g�������߂āA
	// ������̕����֌������
	CVector dif = center - Position();
	dif.Y(0.0f);
	dif.Normalize();
	Rotation(CQuaternion::LookRotation(dif));

	// ���t���[����]�p�x�����Z���āA���S���W�𒆐S�ɉ�]������
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

// �`��
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