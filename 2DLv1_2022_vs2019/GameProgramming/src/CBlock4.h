#pragma once
#include "CCharacter.h"
/*
* CBlock4
* �u���b�N�N���X
* �}�b�v�̃u���b�N�Ɏg�p����
*/
class CBlock4 : public CCharacter
{
public:
	//CBlock4(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBlock4(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update() {}
};
