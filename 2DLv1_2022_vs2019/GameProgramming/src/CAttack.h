#pragma once
#include "CCharacter.h"

class CAttack : public CCharacter
{
public:
	//�Փˏ���1
	void Collision();
	//�Փˏ���3
	//void Collision(CCharacter* m, CCharacter* o);
	//CAttack(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CAttack(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	//bool Collision(CRectangle* rect);
	void Update();
};