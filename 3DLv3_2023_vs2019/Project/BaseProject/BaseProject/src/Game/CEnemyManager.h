#pragma once
#include <list>
#include "CTask.h"
#include "CEnemy.h"

// 敵の管理クラス
class CEnemyManager : public CTask
{
public:
	// CEnemyManagerのインスタンスを生成
	static CEnemyManager* Create();
	// CEnemyManagerのインスタンス破棄
	static void Delete();

	// 指定した敵をリストに追加する
	static void Add(CEnemy* enemy);
	// 指定した敵をリストから取り除く
	static void Remove(CEnemy* enemy);

	// ランダムな位置に指定した敵を生成
	static void SpawnEnemyRandomPos(EEnemyType type);
	// 指定した位置に敵を生成
	static void SpawnEnemy(EEnemyType type, const CVector& pos, const CVector& rot);

	// 更新
	void Update()override;

private:
	// コンストラク
	CEnemyManager();

	// デストラクタ
	~CEnemyManager();

	// 指定したタイプの敵を生成
	static CEnemy* SpawnEnemy(EEnemyType type);
	// 指定した敵をリスポーンリストに追加
	static void RespawnEnemy(CEnemy* enemy);

	static CEnemyManager* spInstance;

	// 生成済みの数をリスト
	std::list<CEnemy*> mSpawnedEnemies;

	struct RespawnDate
	{
		EEnemyType type;
		float respawnTime;
	};
	std::list<RespawnDate> mRespawnEnemies;
};