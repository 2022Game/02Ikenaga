#pragma once
#include "CCharacter.h"

class CAttack : public CCharacter
{
public:
	//�Փˏ���1
	void Collision();
	//�Փˏ���3
	void Collision(CCharacter* m, CCharacter* o);
	//CAttack(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CAttack(float x, float y, float w, float h, CTexture* pt);
	CAttack(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);
	CAttack();
	//Update�ł͉������Ȃ�
	void Update();
	bool Collision(CRectangle* rect);
};