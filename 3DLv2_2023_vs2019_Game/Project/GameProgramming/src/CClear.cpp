#include "CClear.h"
#include"CStageManager.h"
#include "CTask.h"
#include "CCollisionManager.h"

#define STCOUNT 1

int CClear::mSTcount = 1;

int CClear::STcount()
{
	return mSTcount;
}

CClear* CClear::Instance()
{
	return spInstance;
}

CClear* CClear::spInstance = nullptr;

CClear::CClear()
{
	spInstance = this;
}

void CClear::Collision()
{
	mColliderMesh1.ChangePriority();
	mCollider.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

void CClear::Collision(CCollider* m, CCollider* o) {
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		if (CCollider::CCollision(o, m))
		{
			CStageManager::Instance()->LoadStage(0);
			mSTcount = mSTcount += 1;
			if (mSTcount == 100)
			{
				//CStageManager::Instance()->LoadStage(1);
				mSTcount = mSTcount + 101;
			}
			else if (mSTcount == 200)
			{
				//CStageManager::Instance()->LoadStage(2);
				mSTcount = mSTcount + 201;
			}
			else if (mSTcount == 300)
			{
				//CStageManager::Instance()->LoadStage(3);
			}
		}
		break;
	case CCollider::ELINE: //�O�p�R���C�_�̎�
		if (CCollider::CCollision(m, o)) 
		{
			//CStageManager::Instance()->LoadStage(0);
		}
		break;
	}
}

CClear::CClear(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	:mCollider(this, &mMatrix, CVector(0.0f, 0.5f, 0.0f), 1.6f)
{
	mSTcount = STCOUNT;
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