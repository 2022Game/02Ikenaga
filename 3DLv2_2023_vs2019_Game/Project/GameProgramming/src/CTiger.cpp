#include "CTiger.h"
#include "CCollisionManager.h"
#include "CMeat.h"

CModel CTiger::sModel;

//�ړ����x
#define VELOCITY CVector(0.0f,0.0f,0.1f)
#define VELOCITY2 0.15f
#define VELOCITY3 CVector(0.0f,0.0f,0.2f)

void CTiger::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.01f, 0.01f, 0.01f);
}

CTiger::CTiger()
	:CCharacter3(1)
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
//CTiger(�ʒu,��],�g�k)
CTiger::CTiger(const CVector& position, const CVector& rotation, const CVector& scale)
	:CTiger()  //�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	CTransform::Update();  //�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 10.0f) * mMatrixRotate;
}

// �X�V����
void CTiger::Update()
{
	//���̃|�C���^��0�ȊO�̎�
	CMeat* meat = CMeat::Instance();
	if (meat != nullptr)
	{
		//�v���C���[�܂ł̃x�N�g�������߂�
		CVector vp = meat->Position() - mPosition;
		//���x�N�g���Ƃ̓��ς����߂�
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//��x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		if (dy < 0.5)
		{
			mPosition = mPosition + VELOCITY3 * mMatrixRotate;
		}
	}
	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;
	//�ۑ�
	//���x�N�g���Ƃ̓��ς����߂�
	float dx = vp.Dot(mMatrixRotate.VectorX());
	//��x�N�g���Ƃ̓��ς����߂�
	float dy = vp.Dot(mMatrixRotate.VectorY());
	const float margin = 0.1f;
	//���E�����։�]
	if (dx > margin)
	{
		//mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);  //���։�]
	}
	else if (dx < -margin)
	{
		//�ۑ�
		//mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);  //�E�։�]
	}
	//�㉺�����։�]
	if (dy > margin)
	{
		//mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);  //��։�]
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	else if (dy < -margin)
	{
		//�ۑ�
		//mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);  //���։�]
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	//�@�̑O���ֈړ�����
	//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	CTransform::Update(); //�s��X�V
	//���悻3�b���ɖڕW�n�_���X�V
	int r = rand() % 180; //rand()�͐����̗�����Ԃ�
						  //% 180��180�Ŋ������]������߂�

	if (r == 0)
	{
		if (meat != nullptr)
		{
			mPoint = meat->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}
	//�s����X�V
	CTransform::Update();
	//�ʒu���ړ�
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CTiger::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	mLine.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CTiger::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::CCollision(m, o))
		{
			
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
			CTransform::Update();
		}
		break;
	}
}

//�R���X�g���N�^
//CExclamationMark(���f��,�ʒu,��],�g�k)
CTiger::CTiger(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.5f), 1.0f)
	, mLine(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), CVector(0.0f, -1.0f, 0.0f))
{
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	//mColliderMesh1.Set(this, &mMatrix, mpModel);
}