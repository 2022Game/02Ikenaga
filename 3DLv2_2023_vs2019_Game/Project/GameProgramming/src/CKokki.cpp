#include "CKokki.h"
#include"CEffect.h"
#include "CCollisionManager.h"


void CKokki::Collision()
{
	////�R���C�_�̗D��x�ύX
	//mCollider1.ChangePriority();
	//mCollider2.ChangePriority();
	//mCollider3.ChangePriority();
	////�Փˏ��������s
	//CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

void CKokki::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::CCollision(m, o)) {
			//�G�t�F�N�g����
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//�Փ˂��Ă��鎞�͖����ɂ���
			//mEnabled=false;
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			//mPosition = mPosition + adjust;
		}
		break;
	}

	//�R���C�_��m��o���Փ˂��Ă��邩����
	//if (CCollider::CCollision(m, o)) {
	//	//�G�t�F�N�g����
	//	new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//	//mEnabled = false;
	//}
}

CKokki::CKokki(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
/*: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
, mCollider2(this, &mMatrix, CVector(0.0f, 5.0f, 20.0f), 0.8f)
, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)*/
{
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	mColliderMesh1.Set(this, &mMatrix, mpModel);
	//CTransform::Update();
}

//�X�V����
void CKokki::Update()
{
	// �s����X�V
	CTransform::Update();
}