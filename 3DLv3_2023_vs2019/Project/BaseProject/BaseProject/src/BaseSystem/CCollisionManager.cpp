#include "CCollisionManager.h"
#include "CCollider.h"
#include "CObjectBase.h"

// コリジョンマネージャのインスタンス
CCollisionManager* CCollisionManager::mpInstance = nullptr;

// インスタンスを取得
CCollisionManager* CCollisionManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CCollisionManager();
	}
	return mpInstance;
}

// インスタンスの破棄
void CCollisionManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// コンストラクタ
CCollisionManager::CCollisionManager()
{
}

// デストラクタ
CCollisionManager::~CCollisionManager()
{
}

// 指定したコライダーをリストに追加
void CCollisionManager::Add(CCollider* col)
{
	// 既にリストに追加されていたら、追加しない
	auto it = std::find(mColliderList.begin(), mColliderList.end(), col);
	if (it != mColliderList.end()) return;

	// リストの最後尾に追加
	mColliderList.push_back(col);
}

// 指定したコライダーをリストから取り除く
void CCollisionManager::Remove(CCollider* col)
{
	mColliderList.remove(col);
}

// 指定した2つのコライダーの衝突処理を行う
void CCollisionManager::Collision(CCollider* col0, CCollider* col1)
{
	// 指定したコライダーが有効でなければ、衝突判定を行わない
	if (col0 == nullptr || col1 == nullptr) return;
	if (!col0->IsEnable() || !col1->IsEnable()) return;
	// 両コライダーに持ち主が存在しない場合は、衝突判定を行わない
	if (col0->Owner() == nullptr && col1->Owner() == nullptr) return;
	// 同じ持ち主のコライダーであれば、衝突判定を行わない
	if (col0->Owner() == col1->Owner())
	{
		return;
	}

	// 衝突判定を行うコライダーでなければ、衝突判定を行わない
	if (!col0->IsCollision(col1)) return;
	if (!col1->IsCollision(col0)) return;

	// 衝突判定を行う
	CHitInfo hit;
	bool collision = CCollider::Collision(col0, col1, &hit);
	// 衝突していなければ、衝突処理を行わない
	if (!collision) return;

	// 各コライダーの持ち主に衝突したことを伝える
	if (col0->Owner() != nullptr)
		col0->Owner()->Collision(col0, col1, hit);
	if (col1->Owner() != nullptr)
		col1->Owner()->Collision(col1, col0, hit);
}

// 指定したコライダーと他の全てのコライダーとの衝突処理を行う
void CCollisionManager::Collision(CCollider* col)
{
	// 指定したコライダーが有効でなければ、衝突判定は行わない
	if (col == nullptr) return;
	if (!col->IsEnable()) return;

	// コライダーリスト内の全てコライダーと衝突処理を行う
	for (auto& other : mColliderList)
	{
		// 2つのコライダーの衝突処理を行う
		Collision(col, other);
	}
}

// 全てのコライダーの衝突処理を行う
void CCollisionManager::CollisionAll()
{
	// リストの先頭から順番に衝突処理を行う
	auto it = mColliderList.begin();
	auto end = mColliderList.end();
	for (; it != end; ++it)
	{
		CCollider* current = *it;

		// currentの次のコライダーから順番に衝突処理を行う
		// （currentより前のコライダーとは既に衝突処理を行っているため）
		auto nextIt = it;
		nextIt++;
		for (; nextIt != end; ++nextIt)
		{
			CCollider* next = *nextIt;
			// 2つのコライダーの衝突処理を行う
			Collision(current, next);
		}
	}
}

// 全コライダーを描画
void CCollisionManager::Render()
{
	// リスト内の全てのコライダーを描画
	for (auto& col : mColliderList)
	{
		col->Render();
	}
}
