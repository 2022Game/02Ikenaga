#pragma once
#include"CCharacter.h"
#include"CUi.h"

class CItem11 : public CCharacter
{
public:
	static int mItem11;
	//static int Point();
	//�Փˏ���1
	void Collision();
	//�Փˏ���3
	void Collision(CCharacter* m, CCharacter* o);
	//CItem11(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CItem11(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update();
	//void Render();
	//CUi* mpUi;
private:
	//static int mTime;
	//int mTime;
};
