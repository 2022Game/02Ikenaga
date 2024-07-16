#include "CEnemy.h"
#include "CExpManager.h"
#include "CPortionManager.h"
#include "CGameEnemyUI.h"

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
	, mpSummoner(nullptr)
{
	mpGameUI = new CGameEnemyUI();
}

// ƒfƒXƒgƒ‰ƒNƒ^
CEnemy::~CEnemy()
{
	mpGameUI->Kill();

	if (mpSummoner != nullptr)
	{
		mpSummoner->DeathSummonEnemy(this);
	}
}

// ¬‚³‚¢ƒXƒ‰ƒCƒ€‚ÌŽ€–Sˆ—
void CEnemy::SlimeDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::SlimeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢ƒXƒ‰ƒCƒ€‚ÌŽ€–Sˆ—
void CEnemy::SlimeBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::SlimeBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢ƒ}ƒbƒVƒ…ƒ‹[ƒ€‚ÌŽ€–Sˆ—
void CEnemy::MushroomDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::MushroomSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢ƒ}ƒbƒVƒ…ƒ‹[ƒ€‚ÌŽ€–Sˆ—
void CEnemy::MushroomBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::MushroomBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢‹T‚ÌŽ€–Sˆ—
void CEnemy::TurtleDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::TurtleSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢‹T‚ÌŽ€–Sˆ—
void CEnemy::TurtleBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::TurtleBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢ƒGƒC‚ÌŽ€–Sˆ—
void CEnemy::RayDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::RaySpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢ƒGƒC‚ÌŽ€–Sˆ—
void CEnemy::RayBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::RayBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢–I‚ÌŽ€–Sˆ—
void CEnemy::BeeDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::BeeSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«–I‚ÌŽ€–Sˆ—
void CEnemy::BeeBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::BeeBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢ƒTƒ{ƒeƒ“‚ÌŽ€–Sˆ—
void CEnemy::CactusDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::CactusSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢ƒTƒ{ƒeƒ“‚ÌŽ€–Sˆ—
void CEnemy::CactusBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::CactusBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢ƒ`ƒFƒXƒgƒ‚ƒ“ƒXƒ^[‚ÌŽ€–Sˆ—
void CEnemy::ChestDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::ChestSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢ƒ`ƒFƒXƒgƒ‚ƒ“ƒXƒ^[‚ÌŽ€–Sˆ—
void CEnemy::ChestBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::ChestBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢‹…‘Ìƒ‚ƒ“ƒXƒ^[‚ÌŽ€–Sˆ—
void CEnemy::BeholderDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::BeholderSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢‹…‘Ìƒ‚ƒ“ƒXƒ^[‚ÌŽ€–Sˆ—
void CEnemy::BeholderBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::BeholderBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ¬‚³‚¢ƒ{ƒNƒT[‚ÌŽ€–Sˆ—
void CEnemy::BoxerDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::BoxerSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ‘å‚«‚¢ƒ{ƒNƒT[‚ÌŽ€–Sˆ—
void CEnemy::BoxerBigDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::BoxerBigSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);

	CPortionManager::RandomPortion(
		Position() + CVector(0.0f, 8.0f, 0.0f)
	);
}

// ƒ{ƒX(ƒhƒ‰ƒSƒ“)‚ÌŽ€–Sˆ—
void CEnemy::DragonDeath()
{
	// Ž€–SŽž‚ÉŒoŒ±’l‚ð¶¬‚·‚é
	CExpManeger::DragonSpawnExp(
		mCharaStatus.exp,
		Position() + CVector(0.0f, 2.0f, 0.0f)
	);
}

// Ž€–Sˆ—
void CEnemy::Death()
{
}

// “G‚ÌŽí—Þ‚ðŽæ“¾
EEnemyType CEnemy::GetType() const
{
	return mType;
}

// ¢Š«‚µ‚½“G‚ÌŽ€–Sˆ—
void CEnemy::DeathSummonEnemy(CEnemy* enemy)
{
}

// Ž©g‚ð¢Š«‚µ‚½“G‚ðÝ’è
void CEnemy::SetSummoner(CEnemy* summone)
{
	mpSummoner = summone;
}

// XV
void CEnemy::Update()
{
	// HPƒQ[ƒW‚ÉŒ»Ý‚ÌHP‚ðÝ’è
	mpGameUI->SetHp(mCharaStatus.hp);
	
	CHpGauge* hpGauge = mpGameUI->GetHpGauge();
	CVector gaugePos = Position() + mpGameUI->GetHpGaugeOffsetPos();
	hpGauge->SetWorldPos(gaugePos);
	
	CLevelUI* lvUI = mpGameUI->GetLv();
	CVector levelPos = Position() + mpGameUI->GetLvOffsetPos();
	lvUI->SetWorldPos(levelPos);

}
