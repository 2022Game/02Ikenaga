#pragma once
#include "CCharacter.h"

class CEnemy4 : public CCharacter
{
public:
	//�G�̐���ݒ�
	static void Num(int num);
	//�G�̐����擾
	static int Num();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CEnemy4(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CEnemy4(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	static int sNum;	//�G�̐�
};
