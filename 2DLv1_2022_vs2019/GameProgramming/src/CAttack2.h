#pragma once
#include "CCharacter.h"

class CAttack2 : public CCharacter
{
public:
	//�Փˏ���1
	void Collision();
	//�Փˏ���3
	void Collision(CCharacter* m, CCharacter* o);
	//CAttack(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	//CAttack2(float x, float y, float w, float h, CTexture* pt);
	CAttack2(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);
	CAttack2();
	//Update�ł͉������Ȃ�
	void Update();
	bool Collision(CRectangle* rect);
};
