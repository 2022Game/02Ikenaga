#pragma once
#include "CUIBase.h"

class CImage;

// ���b�N���̕�2
class CLockWall2 : public CUIBase
{
public:
	// �R���X�g���N
	CLockWall2(float x, float y);

	// �f�X�g���N�^
	~CLockWall2();

	// �X�V
	void Update();

private:
	CImage* mpLockWall;
	int mLevel;
};