#pragma once
#include "CCharacter.h"
/*
* CBlock3
* �u���b�N�N���X
* �}�b�v�̃u���b�N�Ɏg�p����
*/
class CBlock3 : public CCharacter
{
public:
	//�Փˏ���1
	void Collision();
	//�Փˏ���3
	void Collision(CCharacter* m, CCharacter* o);
	//CBlock(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBlock3(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update();
};