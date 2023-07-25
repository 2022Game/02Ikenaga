#include"CCollisionManager.h"


//�Փˏ���
//Collision(�R���C�_,�͈�)
//�R���C�_�̗D��x+�͈́`�D��x-�͈͂܂ł̃R���C�_�ƏՓ˔�������s����
void CCollisionManager::Collision(CCollider* c, int range) {
	//���݈ʒu��擪�ɂ���
	CCollider* task = (CCollider*)mHead.mpNext;
	//�͈͂߂œǂݔ�΂�
	while(task->mpNext)
	{
		if (task->mPriority <= c->mPriority + range)
		{
			break;
		}
		//���݈ʒu�����ɂ���
		task = (CCollider*)task->mpNext;
	}
	//�Ō�܂ł�����I������
	while (task->mpNext)
	{
		if (task->mPriority <= c->mPriority - range)
		{
			break;
		}
		//�e�̏Փˏ������Ăяo��
		if (c->mpParent != nullptr)
		{
			if (CanCollision(c, task))
			{
				c->mpParent->Collision(c, task);
			}
		}
		//�����̏Փˏ���
		//if (c->mpParent && c != task)c->mpParent->Collision(c, task);
		//���݈ʒu�����ɂ���
		task = (CCollider*)task->mpNext;
	}
}

//�Փˏ���
void CCollisionManager::Collision() {
	//���݈ʒu��擪�ɂ���
	CCollider* task = (CCollider*)mHead.mpNext;
	//�Ō�܂ł�����I������
	while (task->mpNext){
		//���݈ʒu�̎������߂�
		CCollider* next = (CCollider*)task->mpNext;
		//����0�ɂȂ�����I��
		while (next->mpNext){
			//�e�̏Փˏ������Ăяo��
			if (CanCollision(task, next))
			{
				if (task->mpParent != nullptr)
				{
					task->mpParent->Collision(task, next);
				}
				if (next->mpParent !=nullptr)
				{
					next->mpParent->Collision(next, task);
				}
			}
			//�����̏Փˏ���
			//if (task->mpParent)task->mpParent->Collision(task, next);
			//����̏Փˏ���
			//if (next->mpParent)next->mpParent->Collision(next, task);
			//�������߂�
			next = (CCollider*)next->mpNext;
		}
		//���݈ʒu�����ɂ���
		task = (CCollider*)task->mpNext;
	}
}

//�}�l�[�W���̃C���X�^���X
CCollisionManager* CCollisionManager::mpInstance = nullptr;
//�C���X�^���X�̎擾
CCollisionManager* CCollisionManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CCollisionManager();
	}
	return mpInstance;
}

bool CCollisionManager::CanCollision(CCollider* c1, CCollider* c2) const
{
	if (c1 == nullptr || c2 == nullptr) return false;

	if (c1 == c2) return false;

	if (c1->mpParent != nullptr && c1->mpParent->IsKill()) return false;

	if (c2->mpParent != nullptr && c2->mpParent->IsKill()) return false;

	return true;
}