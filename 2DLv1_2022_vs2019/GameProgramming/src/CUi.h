#pragma once
#include "CFont.h"
/*
* CUi
* ���[�U�[�C���^�t�F�[�X�N���X
*/
class CUi
{
public:
	//�Q�[���N���A�\��
	void Clear();
	//�Q�[���I�[�o�[�\��
	void Over();
	//�X�^�[�g�\��
	void Start();
	//Enemy(�G�̐�)
	void Goal(int goal);
	void Enemy(int enemy);
	CUi();
	void Hp(int hp);
	void Point(int point);
	//Time(����)
    void Time(int time);
	void Pulltime(int pulltime);
	void Render();
private:
	int mGoal;
	int mEnemy;	//�G�̐�
	int mHp;
	int mPoint;  //�|�C���g
    int mTime;	//�o�ߎ���
	int mPulltime;
	CFont mFont;
	//CFont mFont1;
};
