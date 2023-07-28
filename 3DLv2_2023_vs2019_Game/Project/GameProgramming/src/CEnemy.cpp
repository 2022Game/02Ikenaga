#include "CEnemy.h"
#include "CCollisionManager.h"
#include"CPlayer.h"
#include"CCollider.h"

//�ړ����x
#define VELOCITY CVector(0.0f,0.0f,0.1f)//9
#define VELOCITY2 0.09f
#define VELOCITY3 0.02f
#define ROTATION_Y CVector(0.0f,-0.1f,0.0f) //��]���x

CModel CEnemy::sModel;
//
CEnemy* CEnemy::Instance()
{
	return spInstance;
}

CEnemy* CEnemy::spInstance = nullptr;

CEnemy::CEnemy()
	:CCharacter3(1)
{
	//���f���������Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		//sModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^
	mpModel = &sModel;
	spInstance = this;
}

// �X�V����
void CEnemy::Update()
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

		//X���̃Y����2.0�ȉ�
		if (-10.0f < dx && dx < 10.0f)
		{
		    mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);
			//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY3;
			//Y���̃Y����2.0�ȉ�
			if (-10.0f < dy && dy < 10.0f)
			{
				//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
				if (20.0f > dz  && dz > -5.0f)
				{
					mState = EState::EMOVE;
					if (mState == EState::EMOVE)
					{
						mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
					}
				}
				else //if (5.0f == dz || dz == -1.0f)
				{
					mState = EState::ESTOP;
					mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);
					mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
				}
			}
		}
	}
	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;
	//���x�N�g���Ƃ̓��ς����߂�
	float dx = vp.Dot(mMatrixRotate.VectorX());
	//��x�N�g���Ƃ̓��ς����߂�
	float dy = vp.Dot(mMatrixRotate.VectorY());
	const float margin = 0.1f;
	//���E�����։�]
	if (dx > margin)
	{
		mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);  //���։�]
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY3;
	}
	else if (dx < -margin)
	{
		mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);  //�E�։�]
	}
	//�㉺�����։�]
	if (dy > margin)
	{
		//mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);  //��։�]
		//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	else if (dx < -margin)
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
	if (r == 0)
	{
		if (player != nullptr)
		{
			mPoint = player->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}
	////�s����X�V
	CTransform::Update();
	////�ʒu���ړ�
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CEnemy::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CEnemy::Collision(CCollider * m, CCollider * o) {
	//����̃R���C�_�^�C�v�̔���
	switch (m->ELINE)
	{
	case CCollider::ELINE: //�O�p�R���C�_�̎�
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (o->Type() == CCollider::ETRIANGLE)
		{
			CVector adjust;//�����p�x�N�g��
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				mState = EState::ESTOP;
				//mState = EState::ESTOP;
				//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
				if (rand() % 60)
				{
					mState = EState::ESTOP;
				}
				else
				{
					mState = EState::ESTOP;
				}
				CTransform::Update();
			}
		}
		break;
	}
}

//�R���X�g���N�^
//CEnemy(���f��,�ʒu,��],�g�k)
CEnemy::CEnemy(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
:mCollider(this, &mMatrix, CVector(0.0f, 2.5f, 0.0f), 0.5f)
,mLine(this, &mMatrix, CVector(0.0f, 4.9f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
, mLine2(this, &mMatrix, CVector(1.0f, 2.5f, -0.5f), CVector(-1.0f, 2.5f, -0.5f))
, mLine3(this, &mMatrix, CVector(0.0f, 2.5f, 0.5f), CVector(0.0f, 2.5f, -0.5f))
{
	CTransform::Update(position, rotation, scale);
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	spInstance = this;
	mPoint = mPosition + CVector(0.0f, 0.0f, 10.0f) * mMatrixRotate;
}