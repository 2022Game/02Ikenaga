#include "CButton2.h"
#include "CButton.h"
#include "CCollisionManager.h"
#include"CPlayer.h"

CModel CButton2::sModel;

//�ړ����x
#define VELOCITY CVector(0.0f,0.0f,0.1f)
#define VELOCITYYX CVector(0.0f,-0.02f,0.0f)
#define VELOCITYYZ -0.5f

void CButton2::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.01f, 0.01f, 0.01f);
}

CButton2::CButton2()
	:CCharacter3(1)
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
//CButton2(�ʒu,��],�g�k)
CButton2::CButton2(const CVector& position, const CVector& rotation, const CVector& scale)
	:CButton2()  //�f�t�H���g�R���X�g���N�^�����s����
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
void CButton2::Update()
{
	SetHidden(true);
	CButton* button = CButton::Instance();
	if (button != nullptr)
	{
		//�v���C���[�܂ł̃x�N�g�������߂�
		CVector vp = button->Position() - mPosition;
		//���x�N�g���Ƃ̓��ς����߂�
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//��x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

		if (dy > 1.0f)
		{
			CTransform::Update();
			mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITYYZ;
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
	int r = rand() % 60; //rand()�͐����̗�����Ԃ�
						  //% 180��180�Ŋ������]������߂�
						  // 
	//�s����X�V
	CTransform::Update();
	//�ʒu���ړ�
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CButton2::Collision()
{
	mColliderMesh1.ChangePriority();
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CButton2::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ELINE: //�O�p�R���C�_�̎�
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
CButton2::CButton2(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
{
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}