#pragma once
#include "CCharacter.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CItem11.h"
#include"CPlayer2.h"

class CEnemy3 : public CCharacter
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
	//CEnemy3(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CEnemy3(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	CItem3 *mItem3;
	CItem2 *mItem2;
	CItem *mItem;
	CItem11* mItem11;
	static int sNum;	//�G�̐�
};
