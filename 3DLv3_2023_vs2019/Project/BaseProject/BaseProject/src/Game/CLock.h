#pragma once
#include "CUIBase.h"

class CImage;

// ���b�N
class CLock : public CUIBase
{
public:
	// �R���X�g���N
	CLock(float x, float y);

	// �f�X�g���N�^
	~CLock();

	// �X�V
	void Update();

private:
	CImage* mpLock;
	int mLevel;
};