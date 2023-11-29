#include "CEnemy.h"
#include "CExpManager.h"

// コンストラクタ
CEnemy::CEnemy()
	:CXCharacter(ETag::eEnemy,ETaskPriority::eEnemy)
{

}



// デストラクタ
CEnemy::~CEnemy()
{

}

// 死亡処理
void CEnemy::Death()
{
	// 死亡時に経験値を生成する
	CExpManeger::SpawnExp(
		mCharaStatus.exp,
		Position()+CVector(0.0f,2.0f,0.0f)
	);
}