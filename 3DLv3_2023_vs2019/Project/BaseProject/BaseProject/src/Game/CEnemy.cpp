#include "CEnemy.h"
#include "CExpManager.h"

// コンストラクタ
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
{
	// HPゲージ作成
	mpHpGauge = new CHpGauge(true);
	mpHpGauge->SetCenterRatio(CVector2(0.5, 0.0f));
}

// デストラクタ
CEnemy::~CEnemy()
{
	// HPゲージを削除
	mpHpGauge->Kill();
}

// 小さいスライムの死亡処理
void CEnemy::SlimeDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::SlimeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大きいスライムの死亡処理
void CEnemy::SlimeBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::SlimeBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さいマッシュルームの死亡処理
void CEnemy::MushroomDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::MushroomSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大きいマッシュルームの死亡処理
void CEnemy::MushroomBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::MushroomBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 死亡処理(レベル50以下の敵用)
void CEnemy::Death()
{
	// 死亡時に経験値を生成する
	CExpManeger::SpawnExp(
		mCharaStatus.exp,
		Position()+CVector(0.0f,2.0f,0.0f)
	);
}

// 死亡処理(レベル90以下の敵用)
void CEnemy::Death2()
{
	// 死亡時に経験値を生成する
	CExpManeger::SpawnExp2(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}