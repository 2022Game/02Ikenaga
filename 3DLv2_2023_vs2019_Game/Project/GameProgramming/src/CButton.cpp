#include "CButton.h"
#include"CStageManager.h"
#include "CTask.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define VELOCITYX -0.02

CButton* CButton::Instance()
{
	return spInstance;
}

CButton* CButton::spInstance = nullptr;

CButton::CButton()
{
	spInstance = this;
}

void CButton::Collision()
{
	mColliderMesh1.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mLine.ChangePriority();
}

void CButton::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->ETRIANGLE)
	{
	//case CCollider::ESPHERE: //���R���C�_�̎�
	//	if (CCollider::CCollision(o, m))
	//	{
	//	}
	//	break;
	case CCollider::ETRIANGLE:
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (o->Type() == CCollider::ETRIANGLE)
		{
			CVector adjust;//�����p�x�N�g��
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
				CTransform::Update();
			}
		}
		break;
	}
}

CButton::CButton(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	//:mCollider(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 0.5f)
	/*:mLine(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
	, mLine2(this, &mMatrix, CVector(0.5f, 0.5f, 0.0f), CVector(-0.5f, 0.5f, 0.0f))
	, mLine3(this, &mMatrix, CVector(0.0f, 0.5f, 0.5f), CVector(0.0f, 0.5f, -0.5f))*/
{
	spInstance = this;
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//�X�V����
void CButton::Update()
{
	//�v���C���[�̃|�C���^��0�ȊO�̎�
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//�v���C���[�܂ł̃x�N�g�������߂�
		CVector vp = player->Position() - mPosition;
		//���x�N�g���Ƃ̓��ς����߂�
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//��x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		if (dx < 0.1f && dy < 10.0f && dz < 0.5f && dz>-0.5f)
		{
			SetHidden(true);
			mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITYX;
		}
		//if  (dy < 0.1f && dz < -0.5f)//(dx < 0.5f && dy < 0.1f && dz == 10.1f)
		//{
		//	SetHidden(true);
		//	mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY;
		//}
		else
		{
			//SetHidden(false);
		}
	}
	// �s����X�V
	CTransform::Update();
}