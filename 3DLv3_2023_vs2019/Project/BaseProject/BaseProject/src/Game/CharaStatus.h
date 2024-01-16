#pragma once

//�L�����̃X�e�[�^�X
struct CharaStatus
{
    int level;         // ���x��
	int exp;           // �o���l
	int hp;            // HP
	int power;         // �U����
	int defense;       // �h���
	float mobility;    // �ړ���
	int SpecialAttack; // ����U��
	float volume;      // �傫��
	float cameraHeight; // �J�����̍���

	// �X�e�[�^�X�̃��Z�b�g
	void Reset()
	{
		level = 1;
		exp = 0;
		power = 5;
		defense = 0;
		mobility = 1.0f;
		SpecialAttack = 2;
		volume = 1.02f;
		cameraHeight = 0.0f;
	}
}; 

// �v���C���[�̍ő僌�x��
#define PLAYER_LEVEL_MAX 100
// �v���C���[�̃��x�����ƂɃX�e�[�^�X�̃e�[�u��
extern const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX];

// �G�̍ő僌�x��
#define ENEMY__LEVEL_MAX 5
// �G�̃��x�����ƂɃX�e�[�^�X�̃e�[�u��
extern const CharaStatus ENEMY_STATUS[ENEMY__LEVEL_MAX];
extern const CharaStatus ENEMY2_STATUS[ENEMY__LEVEL_MAX];
extern const CharaStatus ENEMY3_STATUS[ENEMY__LEVEL_MAX];
