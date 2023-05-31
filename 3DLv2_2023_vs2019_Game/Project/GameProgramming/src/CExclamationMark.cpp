#include "CExclamationMark.h"
#include"CEffect.h"
#include "CCollisionManager.h"
#include"CPlayer.h"
#include "CEnemy.h"

#define OBJ1 "res\\!.obj"
#define MTL1 "res\\!.mtl"

CModel CExclamationMark::sModel;

//�ړ����x
#define VELOCITY CVector(0.0f,0.0f,0.1f)//9
#define VELOCITY2 0.11f

void CExclamationMark::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.2f, 0.2f,0.2f);
	//�O�p�`�̒��_�ݒ�
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//�O�p�`�̖@���ݒ�
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

CExclamationMark::CExclamationMark()
	:CCharacter3(1)
	,mark(0)
	//, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
{
	//���f���������Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ1, MTL1);
	}
	//���f���̃|�C���^
	mpModel = &sModel;
}

//�R���X�g���N�^
//CExclamationMark(�ʒu,��],�g�k)
CExclamationMark::CExclamationMark(const CVector& position, const CVector& rotation, const CVector& scale)
	:CExclamationMark()  //�f�t�H���g�R���X�g���N�^�����s����
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
void CExclamationMark::Update()
{
	/*mark++;
	if (mark > 1)
	{
		mEnabled = false;
		mark = 0;
	}*/
		//�v���C���[�̃|�C���^��0�ȊO�̎�
		//CPlayer* player = CPlayer::Instance();
		//if (player != nullptr)
		//{
		//	//�v���C���[�܂ł̃x�N�g�������߂�
		//	CVector vp = player->Position() - mPosition;
		//	//���x�N�g���Ƃ̓��ς����߂�
		//	float dx = vp.Dot(mMatrixRotate.VectorX());
		//	//��x�N�g���Ƃ̓��ς����߂�
		//	float dy = vp.Dot(mMatrixRotate.VectorY());
		//	float dz = vp.Dot(mMatrixRotate.VectorZ());
	CEnemy* enemy = CEnemy::Instance();
	if (enemy != nullptr)
	{
		CVector vp = enemy->Position() - mPosition;
		//���x�N�g���Ƃ̓��ς����߂�
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//��x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dz = vp.Dot(mMatrixRotate.VectorZ());

			//X���̃Y����2.0�ȉ�
		if (-1.0f < dx && dx < 1.0f)
		{
			mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
			//Y���̃Y����2.0�ȉ�
			if (-1.0f < dy && dy < 1.0f)
			{
				//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
				if (30.0f > dz && dz >= 0.0f)
				{
					//�e�𔭎˂��܂�
					/*CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
					bullet->Position(
						CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					bullet->Rotation(mRotation);
					bullet->Update();*/
				}
			}
		}
	}
	//HP��0�ȉ��̎� ���j
	//if (mHp <= 0)
	//{
	//	mHp--;
	//	//15�t���[�����ɃG�t�F�N�g
	//	if (mHp % 15 == 0)
	//	{
	//		//�G�t�F�N�g����
	//		new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//	}
	//	//���~������
	//	mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
	//	CTransform::Update();
	//	return;
	//}
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
		mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);  //���։�]
	}
	else if (dx < -margin)
	{
		//�ۑ�
		mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);  //�E�։�]
	}
	//�㉺�����։�]
	if (dy > margin)
	{
		//mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);  //��։�]
		mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	else if (dy < -margin)
	{
		//�ۑ�
		//mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);  //���։�]
	  mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	}
	//�@�̑O���ֈړ�����
	//mPosition = mPosition + mMatrixRotate.VectorZ() * VELOCITY2;
	CTransform::Update(); //�s��X�V
	//���悻3�b���ɖڕW�n�_���X�V
	int r = rand() % 180; //rand()�͐����̗�����Ԃ�
						  //% 180��180�Ŋ������]������߂�
	if (r == 0)
	{
		if (enemy == nullptr)
		{
			mPoint = enemy->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}

	/*if (r == 0)
	{
		if (player != nullptr)
		{
			mPoint = player->Position();
		}
		else
		{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}*/
	//�s����X�V
	//CTransform::Update();
	//�ʒu���ړ�
	//mPosition = mPosition + VELOCITY * mMatrixRotate;
}

void CExclamationMark::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CExclamationMark::Collision(CCollider* m, CCollider* o) {
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
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			//mPosition = mPosition + adjust;
		}
		break;
	}

	////�R���C�_��m��o���Փ˂��Ă��邩����
	//if (CCollider::CCollision(m, o)) {
	//	//�G�t�F�N�g����
	//	new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
	//	//mEnabled = false;
	//}
}

//�R���X�g���N�^
//CExclamationMark(���f��,�ʒu,��],�g�k)
CExclamationMark::CExclamationMark(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mark(0)
/*: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
, mCollider2(this, &mMatrix, CVector(0.0f,5.0f,20.0f),0.8f)
, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)*/
{
	//���f��,�ʒu,��],�g�k��ݒ肷��
	mpModel = model;  //���f���̐ݒ�
	mPosition = position;  //�ʒu�̐ݒ�
	mRotation = rotation;  //��]�̐ݒ�
	mScale = scale;  //�g�k�̐ݒ�
	//mColliderMesh1.Set(this, &mMatrix, mpModel);
}