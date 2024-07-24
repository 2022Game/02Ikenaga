#pragma once
#include <list>
#include "CTask.h"
#include "CEnemy.h"

// �G�̊Ǘ��N���X
class CEnemyManager : public CTask
{
public:
	// CEnemyManager�̃C���X�^���X�𐶐�
	static CEnemyManager* Create();
	// CEnemyManager�̃C���X�^���X�j��
	static void Delete();

	// �w�肵���G�����X�g�ɒǉ�����
	static void Add(CEnemy* enemy);
	// �w�肵���G�����X�g�����菜��
	static void Remove(CEnemy* enemy);

	// �����_���Ȉʒu�Ɏw�肵���G�𐶐�
	static void SpawnEnemyRandomPos(EEnemyType type);
	// �w�肵���ʒu�ɓG�𐶐�
	static void SpawnEnemy(EEnemyType type, const CVector& pos, const CVector& rot);

	// �X�V
	void Update()override;

private:
	// �R���X�g���N
	CEnemyManager();

	// �f�X�g���N�^
	~CEnemyManager();

	// �w�肵���^�C�v�̓G�𐶐�
	static CEnemy* SpawnEnemy(EEnemyType type);
	// �w�肵���G�����X�|�[�����X�g�ɒǉ�
	static void RespawnEnemy(CEnemy* enemy);

	static CEnemyManager* spInstance;

	// �����ς݂̐������X�g
	std::list<CEnemy*> mSpawnedEnemies;

	struct RespawnDate
	{
		EEnemyType type;
		float respawnTime;
	};
	std::list<RespawnDate> mRespawnEnemies;
};