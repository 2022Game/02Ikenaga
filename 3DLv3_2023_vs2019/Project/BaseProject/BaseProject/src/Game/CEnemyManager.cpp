#include "CEnemyManager.h"
#include "Maths.h"
#include "CBee.h"
#include "CBeholder.h"
#include "CBoxer.h"
#include "CCactus.h"
#include "CChest.h"
#include "CDragon.h"
#include "CMushroom.h"
#include "CRay.h"
#include "CLich.h"
#include "CSlime.h"
#include "CSlime2.h"
#include "CTurtle.h"

#define RESPAWN_TIME 5.0f

CEnemyManager* CEnemyManager::spInstance = nullptr;

// CEnemyManager�̃C���X�^���X�𐶐�
CEnemyManager* CEnemyManager::Create()
{
	if (spInstance == nullptr)
	{
		spInstance = new CEnemyManager();
	}
	return spInstance;
}

// CEnemyManager�̃C���X�^���X�j��
void CEnemyManager::Delete()
{
	SAFE_DELETE(spInstance);
}

// �R���X�g���N
CEnemyManager::CEnemyManager()
	: CTask(ETaskPriority::eDefault, 0, ETaskPauseType::eGame, false, true)
{
}

// �f�X�g���N�^
CEnemyManager::~CEnemyManager()
{
}

// �w�肵���G�����X�g�ɒǉ�����
void CEnemyManager::Add(CEnemy* enemy)
{
	if (spInstance == nullptr) return;

	spInstance->mSpawnedEnemies.push_back(enemy);
}

// �w�肵���G�����X�g�����菜��
void CEnemyManager::Remove(CEnemy* enemy)
{
	if (spInstance == nullptr) return;

	spInstance->mSpawnedEnemies.remove(enemy);

	RespawnEnemy(enemy);
}

// �����_���Ȉʒu�Ɏw�肵���G�𐶐�
void CEnemyManager::SpawnEnemyRandomPos(EEnemyType type)
{
	if (spInstance == nullptr) return;

	CEnemy* enemy = SpawnEnemy(type);
	if (enemy == nullptr) return;

	CVector pos = enemy->GetRandomSpawnPos();
	enemy->Position(pos);

	CVector rot = CVector::zero;
	rot.Y(Math::Rand(0.0f, 360.0f));
	enemy->Rotation(rot);
}

// �w�肵���ʒu�ɓG�𐶐�
void CEnemyManager::SpawnEnemy(EEnemyType type, const CVector& pos, const CVector& rot)
{
	if (spInstance == nullptr) return;

	CEnemy* enemy = SpawnEnemy(type);
	if (enemy == nullptr) return;

	enemy->Position(pos);
	enemy->Rotation(rot);
}

// �w�肵���^�C�v�̓G�𐶐�
CEnemy* CEnemyManager::SpawnEnemy(EEnemyType type)
{
	switch (type)
	{
	case EEnemyType::eBee: return new CBee();
	case EEnemyType::eBeholder: return new CBeholder();
	case EEnemyType::eBoxer: return new CBoxer();
	case EEnemyType::eCactus: return new CCactus();
	case EEnemyType::eChest: return new CChest();
	case EEnemyType::eDragon: return new CDragon();
	case EEnemyType::eMushroom: return new CMushroom();
	case EEnemyType::eRay: return new CRay();
	case EEnemyType::eLich: return new CLich();
	case EEnemyType::eSlime: return new CSlime();
	case EEnemyType::eSlime2: return new CSlime2();
	case EEnemyType::eTurtle: return new CTurtle();
	}
	return nullptr;
}

// �w�肵���G�����X�|�[�����X�g�ɒǉ�
void CEnemyManager::RespawnEnemy(CEnemy* enemy)
{
	RespawnDate data;
	data.type = enemy->GetType();
	data.respawnTime = RESPAWN_TIME;
	spInstance->mRespawnEnemies.push_back(data);
}

// �X�V
void CEnemyManager::Update()
{
	auto itr = mRespawnEnemies.begin();
	auto itrEnd = mRespawnEnemies.end();
	while (itr != itrEnd)
	{
		RespawnDate& data = *itr;

		if (data.respawnTime > 0.0f)
		{	
			data.respawnTime -= Time::DeltaTime();
			itr++;
		}
		else
		{
			SpawnEnemyRandomPos(data.type);
			itr = mRespawnEnemies.erase(itr);
		}
	}
}
