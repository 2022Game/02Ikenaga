#pragma once
#include "CCharacter.h"
#include"CItem.h"
#include"CItem2.h"
#include"CItem3.h"
class CEnemy2 : public CCharacter
{
public:
	/*CItem* mItem3;
	 CItem2* mItem2;
	CItem* mItem;*/
	//char ch;
	//char item[3][]{
	//"CItem"; //�A�C�e����
	//"CItem2"; //�A�C�e����
	//"CItem3"; //�A�C�e����
	//};
	//int i;
	static int mItem3;
	static int mItem2;
	static int mItem;
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
private:
	static int sNum;	//�G�̐�
};