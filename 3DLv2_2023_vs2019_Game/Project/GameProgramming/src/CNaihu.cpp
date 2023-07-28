#include "CNaihu.h"
#include "CCollisionManager.h"
#include"CPlayer.h"

CModel CNaihu::sModel;

void CNaihu::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.2f, 0.2f, 0.2f);
}

CNaihu::CNaihu()
	:CCharacter3(1)
	,mLife(10)
	//, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
{
	//���f���������Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		//sModel.Load(OBJ1, MTL1);
	}
	//���f���̃|�C���^
	mpModel = &sModel;
}

//�R���X�g���N�^
//CWater(�ʒu,��],�g�k)
CNaihu::CNaihu(const CVector& position, const CVector& rotation, const CVector& scale)
	:CNaihu()  //�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	CTransform::Update();  //�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	//mPoint = mPosition + CVector(0.0f, 0.0f, 0.9f) * mMatrixRotate;
}

// �X�V����
void CNaihu::Update()
{
	//�������Ԃ̔���
	if (mLife-- > 0) {
		CTransform::Update();
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.5f) * mMatrixRotate;
	}
	else {
		//�����ɂ���
		mEnabled = false;
	}
}

void CNaihu::Collision()
{
	mCollider.ChangePriority();
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CNaihu::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::CCollision(m, o)) {
			//�G�t�F�N�g����
			//new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//�Փ˂��Ă��鎞�͖����ɂ���
			//mEnabled=false;
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			//mPosition = mPosition + adjust;
			mEnabled = false;
		}
		break;
	}
}

//�R���X�g���N�^
//CNaihu(���f��,�ʒu,��],�g�k)
CNaihu::CNaihu(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mLife(10)
	, mCollider(this, &mMatrix, CVector(0.0f, 1.4f, 1.2f), 0.1)
{
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
}
