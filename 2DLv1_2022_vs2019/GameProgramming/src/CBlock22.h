#pragma once

#include "CCharacter.h"
/*
* CBlock22
* �u���b�N�N���X
* �}�b�v�̃u���b�N�Ɏg�p����
*/
class CBlock22 : public CCharacter
{
public:
	//CBlock22(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBlock22(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update();
};
