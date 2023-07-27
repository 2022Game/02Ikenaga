#include "CButton.h"
#include"CStageManager.h"
#include "CCollisionManager.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CSceneBase.h"
#include "CMeat.h"

#define VELOCITYX -0.02
#define ROTATION_YY CVector(0.0f,0.02f,0.0f)
#define ROTATION_YZ CVector(0.0f,-0.02f,0.0f)

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
}

void CButton::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	//case CCollider::ESPHERE: //���R���C�_�̎�
	//	if (CCollider::CCollision(o, m))
	//	{
	//	}
	//	break;
	case CCollider::ELINE:
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (o->Type() == CCollider::ELINE)
		{
			CVector adjust;//�����p�x�N�g��
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				SetHidden(true);
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

		if (dx < 0.1f && dy < 1.0f && dz < 0.5f && dz>-0.5f)
		{
			CTransform::Update();
			mPosition = mPosition + ROTATION_YY * mMatrixRotate;
		}
		else if (dy > 6.0f)
		{
			CTransform::Update();
			mPosition = mPosition + ROTATION_YZ * mMatrixRotate;
		}
	}
	// �s����X�V
	CTransform::Update();
}