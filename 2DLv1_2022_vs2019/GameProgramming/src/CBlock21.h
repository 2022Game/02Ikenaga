#pragma once

#include "CCharacter.h"
/*
* CBlock21
* �u���b�N�N���X
* �}�b�v�̃u���b�N�Ɏg�p����
*/
class CBlock21 : public CCharacter
{
public:
	//CBlock21(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBlock21(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update();
};