#pragma once

//�v���C���[�̃X�e�[�^�X
struct CharaStatus
{
	int level;         //���x��
	int hp;            //HP
	int power;         //�U����
	int defense;       //�h���
	float mobility;    //�ړ���
	int SpecialAttack; //����U��
	float volume;      //�傫��
}; 

//�v���C���[�̍ő僌�x��
#define PLAYER_LEVEL_MAX 100

//�v���C���[�̃��x�����ƂɃX�e�[�^�X�̃e�[�u��
extern const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX];