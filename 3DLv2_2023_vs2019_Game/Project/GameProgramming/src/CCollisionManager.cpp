#include"CCollisionManager.h"


//衝突処理
//Collision(コライダ,範囲)
//コライダの優先度+範囲～優先度-範囲までのコライダと衝突判定を実行する
void CCollisionManager::Collision(CCollider* c, int range) {
	//現在位置を先頭にする
	CCollider* task = (CCollider*)mHead.mpNext;
	//範囲めで読み飛ばし
	while(task->mpNext)
	{
		if (task->mPriority <= c->mPriority + range)
		{
			break;
		}
		//現在位置を次にする
		task = (CCollider*)task->mpNext;
	}
	//最後まできたら終了する
	while (task->mpNext)
	{
		if (task->mPriority <= c->mPriority - range)
		{
			break;
		}
		//親の衝突処理を呼び出す
		if (c->mpParent != nullptr)
		{
			if (CanCollision(c, task))
			{
				c->mpParent->Collision(c, task);
			}
		}
		//自分の衝突処理
		//if (c->mpParent && c != task)c->mpParent->Collision(c, task);
		//現在位置を次にする
		task = (CCollider*)task->mpNext;
	}
}

//衝突処理
void CCollisionManager::Collision() {
	//現在位置を先頭にする
	CCollider* task = (CCollider*)mHead.mpNext;
	//最後まできたら終了する
	while (task->mpNext){
		//現在位置の次を求める
		CCollider* next = (CCollider*)task->mpNext;
		//次が0になったら終了
		while (next->mpNext){
			//親の衝突処理を呼び出す
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
			//自分の衝突処理
			//if (task->mpParent)task->mpParent->Collision(task, next);
			//相手の衝突処理
			//if (next->mpParent)next->mpParent->Collision(next, task);
			//次を求める
			next = (CCollider*)next->mpNext;
		}
		//現在位置を次にする
		task = (CCollider*)task->mpNext;
	}
}

//マネージャのインスタンス
CCollisionManager* CCollisionManager::mpInstance = nullptr;
//インスタンスの取得
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