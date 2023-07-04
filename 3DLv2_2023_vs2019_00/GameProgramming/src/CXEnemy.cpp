#include "CXEnemy.h"
//#include "CCollider.h"

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Parent()->Tag())
	{
		//case ETag::EPL/YER && CCollider::ESPHERE:
	case EPLAYER://���R���C�_�̎�
		if (o->Tag() == CCollider::ETag::ESWORD)
		{
			//�R���C�_��m��o���Փ˂��Ă��邩����
			if (CCollider::Collision(o, m))
			{
				ChangeAnimation(11, false, 30);
			}
		}
		else if (o->Tag() == CCollider::ETag::EBODY && o->Tag() == CCollider::ETag::ESWORD)
		{
			if (CCollider::Collision(o, m))
			{
				ChangeAnimation(0, true, 60);
			}
		}
		break;
	}
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	// //��
	mColSphereBody.Matrix(&mpCombinedMatrix[1]);
	//��
	mColSphereHead.Matrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword0.Matrix(&mpCombinedMatrix[26]);
	//��
	mColSphereSword1.Matrix(&mpCombinedMatrix[26]);
	//��
	mColSphereSword2.Matrix(&mpCombinedMatrix[26]);
}

CXEnemy::CXEnemy()
	:mColSphereBody(this, nullptr, CVector(0.5, -1.0f, 0.0f), 1.0f, CCollider::ETag::EBODY)
	, mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f,CCollider::ETag::ESWORD)
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
{
}