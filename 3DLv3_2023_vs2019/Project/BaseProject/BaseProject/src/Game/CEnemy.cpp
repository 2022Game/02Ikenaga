#include "CEnemy.h"
#include "CExpManager.h"
#include "CPortionManager.h"

// コンストラクタ
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
	, mpSummoner(nullptr)
{
	// HPゲージ作成
	mpHpGauge = new CHpGauge(true);
	mpHpGauge->SetCenterRatio(CVector2(0.5f, 0.0f));
}

// デストラクタ
CEnemy::~CEnemy()
{
	// HPゲージを削除
	mpHpGauge->Kill();

	if (mpSummoner != nullptr)
	{
		mpSummoner->DeathSummonEnemy(this);
	}
}

// 小さいスライムの死亡処理
void CEnemy::SlimeDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::SlimeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
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

// 小さい亀の死亡処理
void CEnemy::TurtleDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::TurtleSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大きい亀の死亡処理
void CEnemy::TurtleBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::TurtleBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さいエイの死亡処理
void CEnemy::RayDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::RaySpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大きいエイの死亡処理
void CEnemy::RayBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::RayBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さい蜂の死亡処理
void CEnemy::BeeDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::BeeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大き蜂の死亡処理
void CEnemy::BeeBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::BeeBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さいサボテンの死亡処理
void CEnemy::CactusDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::CactusSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大きいサボテンの死亡処理
void CEnemy::CactusBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::CactusBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さいチェストモンスターの死亡処理
void CEnemy::ChestDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::ChestSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 大きいチェストモンスターの死亡処理
void CEnemy::ChestBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::ChestBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さい球体モンスターの死亡処理
void CEnemy::BeholderDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::BeholderSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// 大きい球体モンスターの死亡処理
void CEnemy::BeholderBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::BeholderBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 小さいボクサーの死亡処理
void CEnemy::BoxerDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::BoxerSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// 大きいボクサーの死亡処理
void CEnemy::BoxerBigDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::BoxerBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// ボス(ドラゴン)の死亡処理
void CEnemy::DragonDeath()
{
	// 死亡時に経験値を生成する
	CExpManeger::DragonSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// 死亡処理
void CEnemy::Death()
{
}

// 敵の種類を取得
EEnemyType CEnemy::GetType() const
{
	return mType;
}

// 召喚した敵の死亡処理
void CEnemy::DeathSummonEnemy(CEnemy* enemy)
{
}

// 自身を召喚した敵を設定
void CEnemy::SetSummoner(CEnemy* summone)
{
	mpSummoner = summone;
}