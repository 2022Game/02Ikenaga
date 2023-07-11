#include "CClear.h"
#include "CCollisionManager.h"
#include"CPlayer.h"
#include"CStageManager.h"

#define HP 1

int CClear::sHp = 0;

int CClear::Hp()
{
	return sHp;
}

CClear* CClear::Instance()
{
	return spInstance;
}

CClear* CClear::spInstance = nullptr;

CClear::CClear()
{

}

void CClear::Collision()
{
	mColliderMesh1.ChangePriority();
	mCollider.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

CClear::CClear(const CVector& position, const CVector& rotation, const CVector& scale)
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	//CTransform::Update();  //�s��̍X�V
}

void CClear::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::CCollision(m, o))
		{
			sHp = 0;
			CStageManager::Instance()->LoadStage(1);
		}
		break;
	case CCollider::ELINE: //�O�p�R���C�_�̎�
		if (CCollider::CCollision(m, o)) 
		{
			sHp = 0;
		}
		break;
	}
}

CClear::CClear(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mCollider(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 1.6f)
{
	sHp = HP;
	spInstance = this;
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//�X�V����
void CClear::Update()
{
	// �s����X�V
	CTransform::Update();
}