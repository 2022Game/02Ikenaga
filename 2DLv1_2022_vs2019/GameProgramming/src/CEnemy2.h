#pragma once
#include "CCharacter.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
#include"CPlayer2.h"
class CEnemy2 : public CCharacter
{
public:
	static int mTime;
	CItem3 *mItem3;
	CItem2 *mItem2;
	CItem *mItem;
	//�G�̐���ݒ�
	static void Num(int num);
	//�G�̐����擾
	static int Num();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CEnemy2(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CEnemy2(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
	//void Render();
private:
	static int sNum;	//�G�̐�
};