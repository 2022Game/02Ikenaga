#pragma once
#include"CCharacter.h"
#include"CUi.h"

class CItem : public CCharacter
{
public:
	//static int Point();
	//�Փˏ���1
	void Collision();
	//�Փˏ���3
	void Collision(CCharacter* m, CCharacter* o);
	//CItem(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CItem(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update();
	CUi* mpUi;
private:
	int mTime;
};