#pragma once
#include "CCharacter.h"
/*
* CBlock5
* �u���b�N�N���X
* �}�b�v�̃u���b�N�Ɏg�p����
*/
class CBlock5 : public CCharacter
{
public:
	void Collision(CCharacter* m, CCharacter* o);
	//CBlock5(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBlock5(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update() {}
};