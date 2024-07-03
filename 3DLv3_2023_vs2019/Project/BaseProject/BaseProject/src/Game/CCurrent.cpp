#include "CCurrent.h"
#include "CLineEffect.h"
#include "CCharaBase.h"

// �R���X�g��
CCurrent::CCurrent(CObjectBase* owner, const CVector& pos, const CVector& dir)
	: mEffectAnimData(1, 11, true, 11, 0.03f)
{
	Position(pos);

	mpLineEffect = new CLineEffect(ETag::eCurrent);
	mpLineEffect->SetTexture("Current");
	mpLineEffect->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	mpLineEffect->SetColor(CColor::yellow);
	mpLineEffect->SetAnimData(&mEffectAnimData);

	CVector forward = dir;
	forward.Y(0.0f);
	forward.Normalize();
	CVector Pos = pos + forward * 40.0f;

	CVector startPos = pos;
	CVector endPos = Pos;
	int div = 3;
	float width = 5.0f;
	mpLineEffect->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		mpLineEffect->AddPoint(pos, width, width);
	}
}

// �f�X�g���N�^
CCurrent::~CCurrent()
{
	if (mpLineEffect != nullptr)
	{
		mpLineEffect->Kill();
	}
}

// �Փˏ���
void CCurrent::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eDamageCol)
	{
		// �L�����̃|�C���^�ɕϊ�
		CCharaBase* chara = dynamic_cast<CCharaBase*> (other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł���΁A
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				// �^����_���[�W���v�Z
				int damage = CalcDamage(1.5f, mOwner, chara);

				// �_���[�W��^����
				chara->TakeDamage(damage, mOwner);

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
	}
}

// �X�V
void CCurrent::Update()
{
}