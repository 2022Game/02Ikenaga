#pragma once

//�G�l�~�[(�X���C��)�̃X�e�[�^�X
struct CharaStatus2
{
	int level;         //���x��
	int hp;            //HP
	int power;         //�U����
};

//�G�l�~�[(�X���C��)�̍ő僌�x��
#define ENEMY__LEVEL_MAX 5

//�G�l�~�[�̃��x�����ƂɃX�e�[�^�X�̃e�[�u��
extern const CharaStatus2 ENEMY_STATUS[ENEMY__LEVEL_MAX];