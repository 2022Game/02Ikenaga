#pragma once
#include "CUIBase.h"

class CImage;

// ���b�N���̕�
class CLockWall : public CUIBase
{
public:
	// �R���X�g���N
	CLockWall(float x, float y);

	// �f�X�g���N�^
	~CLockWall();

	// �X�V
	void Update();

private:
	CImage* mpLockWall;
	int mLevel;
};